# Islandora Events

`islandora_events` is the Drupal-native replacement for the legacy
ActiveMQ/Alpaca stack. It handles derivative generation, durable queueing, and
operator-visible job state through Symfony Messenger and the `sm_ledger`
module. Service-specific indexing logic can live in submodules, with
`islandora_events_fcrepo` owning direct Fedora/fcrepo indexing and
`islandora_events_blazegraph` owning direct Blazegraph indexing.

For most sites, the point is not "use Messenger because Symfony has it". The
point is that Drupal now owns the queue, the worker, the retry model, and the
operator-visible state that used to be split across ActiveMQ, Alpaca, and
downstream callback behavior.

Container-managed Messenger workers require the `islandora/drupal` image at
version 6.4.0 or newer. If you are enabling the s6-supervised worker services
in `docker-compose.yml`, confirm the Drupal image tag meets that minimum before
switching `DRUPAL_SM_WORKERS_MODE` to `container`.

## What it replaces

The old stack depended on:

- ActiveMQ for transport
- Alpaca for routing and connector orchestration
- Java and Camel changes for many connector customizations

`islandora_events` keeps the same high-level repository workflows, but moves
the transport, worker lifecycle, retry visibility, and operational state into
Drupal.

## Distributed architecture translation

Use this table when you are translating Kafka, RabbitMQ, or general
distributed-systems concepts into the Drupal and Islandora implementation.

| Distributed concept | Drupal / `islandora_events` equivalent |
| --- | --- |
| Broker topic | SM transport such as `islandora_derivatives`, `islandora_index_fedora`, or `failed` |
| Consumer group offset | SQL transport claim state through `claimed_at` and `claim_token`; some scanner cursors use Drupal `state` |
| Partition | One transport queue or table per concern; parallel workers rely on transport claiming instead of shared-memory threading |
| Message ID | Transport row `id` plus a business dedupe key |
| Producer | Drupal hooks and services such as `DerivativeQueueService` and `IndexEventService` |
| Consumer | Long-lived `drush sm:consume ...` worker process supervised by `systemd`, `supervisor`, `s6`, Kubernetes, or similar |
| Dead-letter topic | The `failed` transport and its corresponding failure handling |
| Consumer group registry | `sm_workers` worker-definition registry |
| Connector configuration | `WorkerDefinitionProviderInterface` implementations and module transport routing config |
| Schema registry | Symfony Messenger serializer plus PHP message classes |
| Offset commit / ack | Transport `ack()` after successful handling; SQL transport deletes the claimed row after claim-token validation |

Two translation caveats matter:

- Drupal workers are long-lived PHP CLI processes, not threads.
- `islandora_events` now relies on `drupal/sm` transport implementations
  directly instead of shipping its own custom transport.

## Core concepts

- **Derivative runners** define how a derivative queue is executed.
- **Index targets** define how an entity event is sent to an indexing backend.
- **Ledger records** provide the durable operator-facing history for queued,
  running, failed, and completed work.
- **Circuit breakers** protect downstream HTTP integrations from repeated
  failures.

!!! islandora "Lobster trap"
    `index_targets` is the indexing configuration model. If an integration
    needs a new target, add or install a Drupal service for that target rather
    than relying on legacy queue-name compatibility settings.

## Choose the right workflow

Use the path that matches your role:

- **Operator, no code**: configure a new HTTP derivative endpoint in
  `derivative_runners`, point an Islandora action at that queue, and run the
  existing derivative worker.
- **Operator, Fedora/fcrepo**: enable `islandora_events_fcrepo`, set
  `index_targets.fedora.endpoint` to the Fedora REST base URL, and run the
  Fedora indexing worker.
- **Operator, new indexing integration**: install the module that provides the
  target service, enable the target in configuration, and run either the
  shared custom-index worker or the dedicated worker shipped by that module.
- **Developer, PHP**: implement `IndexTargetInterface` and tag the service as
  `islandora_events.index_target` to add a new indexing target.
- **Integrator, PHP plus transport**: add a custom message class, transport
  routing, and worker definition when a workflow needs its own queue and worker
  topology instead of the shared derivative or custom-index transports.

## Configure an HTTP derivative microservice

Use this when a derivative service is reachable over HTTP and does not need a
local command runner.

1. Open **Configuration** >> **Web services** >> *Islandora Events settings*
   (`/admin/config/services/islandora-events/settings`).
2. Expand **Derivative queue runners**.
3. Add a queue entry under `derivative_runners` keyed by the queue name used by
   your Islandora action.
4. Set:
    - `execution_mode: http`
    - `endpoint: http://your-service:port/`
    - `timeout: 300` or another appropriate value
5. Save configuration.
6. Confirm the Islandora action entity uses the same queue name.
7. Start or restart the derivative worker:

```bash
drush sm:consume islandora_derivatives --time-limit=3600
```

Example:

```yaml
derivative_runners:
  islandora-connector-myservice:
    execution_mode: http
    endpoint: 'http://myservice:8080/'
    timeout: 300
```

Matching action shape:

```yaml
queue: islandora-connector-myservice
event: Generate Derivative
```

Validation flow:

1. Trigger the action from Drupal.
2. Confirm a ledger row is created.
3. Confirm the derivative worker processes it.
4. Confirm the breaker appears in **Configuration** >> **Web services** >>
   *SM Workers Circuit Breakers* after first use.

!!! islandora "Config-only path"
    This derivative HTTP workflow is config-driven. Adding the endpoint does
    not require PHP code unless you also need a new execution mode, message
    type, or dedicated transport.

## Enable a command-mode derivative runner

Use this when the derivative workflow must run a local binary instead of making
an HTTP request.

1. Add or update the queue entry in `derivative_runners`.
2. Set `execution_mode: command`.
3. Set `command` and, if applicable, `config_path`.
4. In `settings.php`, explicitly allow command execution and allowlist the
   binary.
5. Restart the derivative worker.

Example `settings.php`:

```php
$settings['islandora_events_derivative_command'] = [
  'enabled' => TRUE,
  'allowed_binaries' => [
    '/usr/bin/scyllaridae',
  ],
  'allow_insecure_args' => FALSE,
];
```

Example runner:

```yaml
derivative_runners:
  islandora-connector-myservice:
    execution_mode: command
    command: '/usr/bin/scyllaridae'
    config_path: '/opt/scyllaridae/myservice/scyllaridae.yml'
    timeout: 300
```

!!! note "Command mode is privileged"
    Command-mode execution is disabled unless `settings.php` enables it. This
    is intentional and should be treated like any other local process-execution
    permission.

## Add a new index target

Adding a new index target is a code change, not just a config change.

1. Create a class implementing `IndexTargetInterface`.
2. Tag it as `islandora_events.index_target` in your module's service
   definition.
3. Add target-specific configuration under `index_targets`.
4. Clear caches so Drupal rebuilds the tagged service container.
5. In the common case, no additional transport setup is required. Custom
   targets dispatch through `islandora_index_custom`.
6. Only if the target needs its own dedicated worker transport, add:
    - a new `IndexEventMessage` subclass
    - an `sm.routing.yml` route for that subclass
    - a matching `sm.transports.yml` transport
    - a dedicated worker command for that transport

The `islandora_events_blazegraph` and `islandora_events_fcrepo` submodules are
examples of the dedicated transport pattern.

## Configure the direct Blazegraph target

Use this when you want Drupal to write SPARQL updates directly to Blazegraph
without a separate Alpaca triplestore indexer service.

1. Enable `islandora_events_blazegraph`.
   Run `composer install` or `composer update` first if the submodule was just
   added to the codebase, so the required PHP RDF library is available.
2. Open **Configuration** >> **Web services** >> *Islandora Events settings*
   (`/admin/config/services/islandora-events/settings`).
3. In the **Blazegraph indexing target** section, enable the target.
4. Set the endpoint to the Blazegraph SPARQL update URL, for example
   `http://blazegraph:8080/bigdata/namespace/islandora/sparql`.
5. Optional: set a named graph URI if your repository writes repository
   triples into a non-default graph.
6. Start or restart the Blazegraph indexing worker:

```bash
drush sm:consume islandora_index_blazegraph --time-limit=3600
```

Useful operator command:

```bash
drush islandora-events-blazegraph:index-record 123
```

## Configure the direct Fedora/fcrepo target

Use this when you want Drupal to index directly into Fedora without a separate
Milliner service. Revision-backed Drupal updates also create Fedora mementos in
this path, so Fedora versioning no longer depends on Alpaca or Milliner.

1. Enable `islandora_events_fcrepo`.
2. Open **Configuration** >> **Web services** >> *Islandora Events settings*
   (`/admin/config/services/islandora-events/settings`).
3. In the **Fedora/fcrepo indexing target** section, enable the target.
4. Set the endpoint to the Fedora REST base URL, for example
   `http://fcrepo:8080/fcrepo/rest`.
5. Start or restart the Fedora indexing worker:

```bash
drush sm:consume islandora_index_fedora --time-limit=3600
```

Useful operator command:

```bash
drush islandora-events-fcrepo:index-record 123
```

## Start workers

When using the container-managed worker model, set
`DRUPAL_SM_WORKERS_MODE=container` in the Drupal service environment. Leave it
as `external` when workers run in a separate container or host.

Start the worker that matches the transport you configured:

```bash
drush sm:consume islandora_derivatives --time-limit=3600
drush sm:consume islandora_index_fedora --time-limit=3600
drush sm:consume islandora_index_blazegraph --time-limit=3600
drush sm:consume islandora_index_custom --time-limit=3600
```

If you add a dedicated custom transport, start a worker for that transport as
well.

When optional scheduler-driven submodules are enabled, keep their workers
separate from the main request-triggered transports. Typical examples are:

- `scheduler_islandora_events_backfill`
- `islandora_backfill`
- `scheduler_islandora_events_mergepdf`
- `islandora_mergepdf`

Do not rely on web requests to drain queues. Run workers under a process
manager, container supervisor, or orchestration platform.

Use `drush sm-workers:list` to inspect the canonical worker commands provided
by the enabled module set. Those commands can then be run under `systemd`,
`supervisor`, `s6`, Kubernetes, or another process manager.

## Worker topology and scaling

Start with one worker per core transport and scale by transport, not by one
undifferentiated worker pool.

Recommended tuning order:

1. Add derivative workers first.
2. Tune Fedora indexing workers separately.
3. Tune Blazegraph indexing workers separately.
4. Keep scheduler and reconciliation work isolated from normal ingest.

Use `drush islandora-events:capacity-report --window-minutes=15` before
changing topology. In general:

- if queue wait stays low and throughput meets your target, keep the current
  worker placement
- if queue depth and queue wait stay elevated, add transport-specific workers
  first
- if the database becomes the limiting factor, re-evaluate the transport
  backend before changing the ledger model

`sm_ledger` remains the durable operator projection even if transport
implementation changes later.

## Transport notes

`islandora_events` is transport-aware but no longer transport-owned.

- The worker model, message handlers, and `sm_workers` definitions work with
  any transport that `drupal/sm` exposes.
- The default Islandora transports in this repository now use `drupal-sql://`.
- If you switch to Redis, ActiveMQ, or another transport, the worker and
  handler layers should not need redesign. The transport DSNs and deployment
  topology are the main moving parts.
- The durable correctness guarantees live in the ledger and handler layers:
  enqueue-time deduplication, `findRecentByDedupeKey()` checks, and
  `isQueuedForProcessing()` guards.

## Operational limits

The current design is appropriate for normal Islandora deployments and moderate
parallelism. For larger fleets, keep these constraints in mind:

- Backfill scans are now guarded by a distributed lock so multiple web heads do
  not run the same scanner at once.
- `processNativeQueue()` still filters by queue name after fetching candidate
  records from the ledger. That is acceptable today, but at much larger scale a
  first-class queue field or another queryable selector would be better.
- Queue depth metrics can show when workers are falling behind, but autoscaling
  or admission control must still be provided by the deployment platform.
- Very large SQL-backed transports may eventually need sharding or a different
  transport backend rather than more PHP workers alone.

## Potential upstream work

Most of the removed custom transport behavior should stay removed.

- Transport-level UPSERT deduplication is not a good upstream fit. It depended
  on message-specific business keys and duplicated responsibilities that belong
  in the application and ledger layers.
- Claim-token stamps were only necessary because of that transport-level
  deduplication and should not be reintroduced.
- Custom dead-letter row moves are also unnecessary because `drupal/sm`
  already has failure-transport handling.

The one improvement that does look like a reasonable upstream contribution is
better concurrent claiming for SQL-backed transports, especially support for
`SKIP LOCKED` where the database supports it. That is a transport-runtime
improvement that could benefit any Drupal project using concurrent Messenger
workers.

## Observe work and failures

Use these places first:

- **Configuration** >> **Web services** >> *SM Ledger* for durable job state
- **Reports** >> *SM Ledger* for operational views
- **Configuration** >> **Web services** >> *SM Workers Circuit Breakers*
  for downstream HTTP breaker state

Useful commands:

```bash
drush islandora-events:capacity-report --window-minutes=15
drush sm:failed:show
drush sm:failed:retry
```

If `islandora_events_metrics` is enabled, use its Prometheus-style endpoint for
fleet-level dashboards and alerting. If `islandora_events_otel` is enabled, use
trace context recorded with ledger metadata to correlate worker activity with
external telemetry systems.

## Failure triage

Use this order when a job is failing:

1. Open the ledger record in Drupal.
2. Confirm status, retry count, timing, and `last_error`.
3. Note the source entity, action or plugin, and correlation key.
4. Check the matching worker logs for the same time window.
5. Check downstream service logs when the failure came from HTTP execution,
   Fedora, Blazegraph, or another remote dependency.
6. Requeue only after confirming the failure was transient or corrected.

Use the ledger to answer "what failed?" Use logs and external telemetry to
answer "why did it fail?"

Typical derivative failure sources:

- HTTP timeouts
- connection refusals
- authentication failures
- invalid derivative payloads
- destination write-back failures
- local command denials because privileged command execution is disabled or the
  binary is not allowlisted

Typical indexing failure sources:

- Fedora endpoint failures
- Blazegraph endpoint failures
- serialization or payload issues
- disabled or misconfigured targets

Typical worker/runtime failure sources:

- repeated worker exits
- database connectivity problems
- queue growth with no matching worker throughput
- redelivery or retry loops

## Validation checklist

Use this checklist when validating a deployment or major topology change:

1. Confirm `islandora_events` is enabled and `drush sm:stats` shows the
   expected transports.
2. Start the derivative and indexing workers.
3. Create or update content that should emit derivative and indexing work.
4. Confirm ledger records move through `queued`, `in_progress`, and either
   `completed` or the expected retry/failure states.
5. Capture a baseline capacity snapshot with
   `drush islandora-events:capacity-report --window-minutes=15`.
6. Force one downstream failure and confirm retry metadata, breaker behavior,
   and logs all line up.
7. Requeue a failed or abandoned record and confirm it is processed correctly.

## Deployment paths

The root `docker-compose.yml` supports two operational paths:

- **Path A: hybrid**. Keep the legacy HTTP microservices, but run Symfony
  Messenger workers in the Drupal container by setting
  `DRUPAL_SM_WORKERS_MODE=container`.
- **Path B: simplified**. Move Drupal onto in-container Messenger workers and
  remove the legacy broker and HTTP derivative microservices after migration.

For Path B, the legacy `DRUPAL_DEFAULT_BROKER_URL` setting is not used and
should be removed to avoid implying an ActiveMQ dependency in Drupal.

After validating Path B in your environment, the legacy stack components that
can be removed are:

- `activemq`
- `alpaca`
- `milliner`
- `crayfits`
- `homarus`
- `houdini`
- `hypercube`
- `mergepdf`
- `fits`
- `activemq-data`
- `ACTIVEMQ_PASSWORD`
- `ACTIVEMQ_WEB_ADMIN_PASSWORD`
- `ALPACA_JMS_PASSWORD`

`fcrepo` does not require ActiveMQ for its REST API. If you remove
`activemq`, ensure `fcrepo` depends only on its database or relies on its own
startup retry behavior.

## Migrate a legacy HTTP/Alpaca action

This walkthrough shows how to migrate an existing derivative action from the
legacy ActiveMQ/Alpaca path to `islandora_events`.

### Scenario: Generate a thumbnail

In the legacy stack, the "Generate a thumbnail image" action emitted a
JSON-LD event onto the `islandora-connector-houdini` queue, Alpaca consumed
that event, added JWT credentials, forwarded it to Houdini over HTTP, and
Houdini posted the result back to Drupal.

Typical legacy action shape:

- Action: Generate a Thumbnail
- Queue: `islandora-connector-houdini`
- Event: Generate Derivative
- Destination URI: `fedora://...`

### Step 1: verify prerequisites

Confirm `islandora_events` is enabled and the Messenger transports exist:

```bash
drush pm:list --status=enabled | grep islandora_events
drush sm:stats
```

You should see `islandora_events` enabled and the `islandora_derivatives`
transport listed.

### Step 2: choose the execution path

For Path A, keep the existing HTTP derivative service. `islandora_events`
dispatches onto the `islandora_derivatives` transport, the derivative handler
loads the configured action, and `HttpDerivativeExecutionStrategy` calls the
existing endpoint directly. Alpaca is no longer required for that route.

For Path B, replace the HTTP callback flow with command execution inside the
Drupal container. The required binary must exist in the image and be explicitly
allowlisted in Drupal settings.

### Step 3: allowlist the command runner for Path B

Example `settings.php` configuration:

```php
$settings['islandora_events_derivative_command'] = [
  'enabled' => TRUE,
  'allowed_binaries' => [
    '/usr/bin/convert',
  ],
  'allow_insecure_args' => FALSE,
];
```

Adjust the allowlisted binary paths to match the derivative you are migrating.

### Step 4: verify the action entity still exists

The queueing path still loads the Drupal action entity by machine name. Check
that the action survives the migration unchanged:

```bash
drush ev "print_r(\Drupal::entityTypeManager()->getStorage('action')->load('generate_a_thumbnail_image'));"
```

As long as the action entity remains present, `islandora_events` can enqueue
and execute it without the old broker bridge.

### Step 5: disable the legacy Alpaca route

Remove or comment out the Alpaca route that consumes the old queue, such as
`islandora-connector-houdini`. Do not leave both stacks active for the same
action or they will race to process the same derivative requests.

### Step 6: test end to end

Requeue work for a known entity and inspect the ledger:

```bash
drush islandora-events:process-derivatives --limit=1 --dry-run
drush islandora-events:capacity-report --window-minutes=15
drush ev "
  \$records = \Drupal::database()->select('sm_ledger_event_record', 'r')
    ->fields('r', ['id', 'status', 'action_plugin_id', 'target_system', 'created'])
    ->orderBy('r.id', 'DESC')->range(0, 10)->execute()->fetchAll();
  print_r(\$records);
"
```

### Step 7: remove the HTTP microservice for Path B

Once the derivative succeeds through the command runner, remove the now-unused
HTTP microservice from `docker-compose.yml` and any associated JWT or service
configuration that only supported that legacy route.

### Rollback

You can roll back by re-enabling the Alpaca route, but only after disabling the
`islandora_events` path for the same action. The ledger records remain valid;
the main risk is duplicate execution if both systems consume the same work.

## See also

- [Alpaca Tips](alpaca-tips.md) (deprecated, migration reference)
- [Installing ActiveMQ and Alpaca](../installation/manual/installing-alpaca.md)
  (deprecated, migration reference)
- [Alpaca Technical Stack](../alpaca/alpaca-technical-stack.md)
  (deprecated, migration reference)

# Customizations

The following sections describe optional customizations you can make to further modify your Islandora site.

## Blocking Connections

### Blocking IP Addresses

#### iptables

If your server uses iptables, you can block a range of IP addresses using the following command

`iptables -I DOCKER-USER -s XXX.XXX.XXX.0/24 -j DROP`

## Traefik customizations

### TLS Settings

Traefik allows you to modify security settings by doing things like setting a minimum TLS version or specifying cipher suites. Isle Site Template includes Traefik configuration files in the `traefik/dynamic` directory.

These configuration files control routing rules and TLS settings. You can modify them to customize your Traefik setup.

More information is available in the [Traefik documentation](https://doc.traefik.io/traefik/https/tls/#tls-options)

### Switching between HTTP and HTTPS

The template provides make commands to easily switch between different modes:

- `make traefik-http` - Switch to HTTP mode (default for local development)
- `make traefik-https-mkcert` - Switch to HTTPS using mkcert self-signed certificates
- `make traefik-https-letsencrypt` - Switch to HTTPS using Let's Encrypt ACME for production

After switching modes, restart Traefik with `make down-traefik up`

### Hiding Fedora from the public

By default, your Fedora repository will be exposed to the public at `fcrepo.${DOMAIN}`. If you don't want the public to be able to access your repository, you can modify the Traefik routing configuration in the `traefik/dynamic` directory to disable external access while keeping internal access for other services.

If you do this, you can remove the fcrepo subdomain from your DNS records.

Finally, ensure that Drupal is configured to access Fedora using the internal Docker network hostname:

```yaml
DRUPAL_DEFAULT_FCREPO_URL: "http://fcrepo:8080/fcrepo/rest/"
```

### Exposing ActiveMQ, Blazegraph, and Solr

By default in production environments, the admin UIs for ActiveMQ, Blazegraph, and Solr are not network accessible. These services are blocked from network access as a security precaution.

In order to access the admin UIs for these services you have two options: SSH Port Forwarding or Modifying Traefik's Dynamic Templates

#### SSH Port Forwarding

The most secure way to access these services' admin UI is by using SSH Port Forwarding. This approach requires no configuration changes, and ensures access over the network is secure.

##### Set environment variables

First, in your command line terminal, set the environment variable `SSH_DESTINATION` to the domain or IP you use to SSH into your machine. If you use a specific username to login, include that. Paste this with your updated value into your terminal:

```
SSH_DESTINATION=you@your.isle.site
```

You'll also need to use an available port on your local machine. If port 8080 is available on your local machine, you can map that port to the remote service. If 8080 isn't available on your machine, use another available port (e.g. 8888) and set `LOCAL_PORT` accordingly in your terminal.

```
LOCAL_PORT=8080
```

Now specify the docker service and the port you need to access by setting the `SERVICE` and `SERVICE_PORT` environment variables. We'll use these in the port forwarding command below. Below are the pair values for ActiveMQ, Blazegraph, and Solr.

###### ActiveMQ

```
SERVICE=activemq
SERVICE_PORT=8161
```

If you're connecting to ActiveMQ you need to know the value in your compose project secret in `./secrets/ACTIVEMQ_WEB_ADMIN_PASSWORD` to login via the browser. Running the two commands below should print that password in your terminal. You'll want to copy that value for later.

```
SECRET_PATH=$(ssh $SSH_DESTINATION "docker inspect \$(docker ps -q --filter 'name=activemq') --format '{{range .Mounts}}{{.Source}}{{\"\n\"}}{{end}}' | grep ACTIVEMQ_WEB_ADMIN_PASSWORD")
ssh $SSH_DESTINATION "cat $SECRET_PATH; echo"
```

###### Blazegraph

```
SERVICE=blazegraph
SERVICE_PORT=8080
```

###### Solr

```
SERVICE=solr
SERVICE_PORT=8983
```

##### Start a port forwarding session

Now that you've set the proper environment variables in your command line terminal, you're ready to setup port forwarding.

```
IP=$(ssh $SSH_DESTINATION "docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' \$(docker ps -q --filter 'name=$SERVICE')")
ssh $SSH_DESTINATION -L $LOCAL_PORT:$IP:$SERVICE_PORT
```

Then open [http://localhost:8080](http://localhost:8080) and you'll be viewing the service's admin UI. For ActiveMQ you can enter `admin` for the username and the value printed in the terminal for the password.

The service will be available as long as the SSH session remains active. When you're done, you can close your browser tab that is accessing the service port, and then exit out of your SSH session and it will close the port forwarding session, too.

#### Modifying Traefik's Dynamic Templates

If SSH Port Forwarding is not an option for you, ISLE Site Template also is setup to allow accessing these services' admin UI with custom domains.

ISLE Site Template uses [Traefik's Go Templating](https://doc.traefik.io/traefik/reference/routing-configuration/other-providers/file/#go-templating) in the [./conf/traefik/dynamic](https://github.com/Islandora-Devops/isle-site-template/blob/main/conf/traefik/) directory to manage exposing different services on the host network.

ActiveMQ, Blazegraph, and Solr are disabled by default in production since they are wrapped in a conditional that checks whether `DEVELOPMENT_ENVIRONMENT=true` in your docker compose project's `.env` file.

```
{{- if (eq (env "DEVELOPMENT_ENVIRONMENT") "true") }}
...
{{- end }}
```

To expose ActiveMQ, Blazegraph, and/or Solr, you can alter the respective service's traefik config in the [./conf/traefik/dynamic](https://github.com/Islandora-Devops/isle-site-template/blob/main/conf/traefik/) directory. Removing the `{{- if (eq (env "DEVELOPMENT_ENVIRONMENT") "true") }}` conditional (and its closing `{{- end }}`) would be enough to expose the respective service to the network, assumming the respective DNS is configured (i.e. there are valid DNS records for `activemq.${DOMAIN}`, `blazegraph.${DOMAIN}`, `solr.${DOMAIN}` that resolve to your ISLE deployment).

This change can be a disruptive operation since changing the config will likely require bringing the traefik service down, and subsequently offline, in order to ensure the config has been set in traefik. That action will also take your site offline during the restart. If the config change is not valid, there is a risk traefik will not come online and would result in a site outage. So it's best practice to make these traefik config changes on your local machine, test to ensure they are working, and deploy the updated config to your production environment.

Given this change is a code deployment, and not a live edit in a production environment, you will want to explore additional options to secure the service. e.g. using Traefik's [basicAuth](https://doc.traefik.io/traefik/reference/routing-configuration/http/middlewares/basicauth/) or [IPAllowList](https://doc.traefik.io/traefik/reference/routing-configuration/http/middlewares/ipallowlist/) so once your change is in place, your services are still protected.

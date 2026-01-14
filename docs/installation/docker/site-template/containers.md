# Working in Your Containers

When using Docker, we often have to run commands inside the Docker containers. For example, you can run the same Drush commands you would run in a non-Docker environment, but you must run those commands inside the Drupal container.

## Running Commands in the Containers

If you want to work inside a container, you can use Docker Compose to run bash from inside the container. You can be dropped into a bash shell inside the Drupal container by running

`docker compose exec -it drupal with-contenv bash`

Or you can run your command from the host machine by running

`docker compose exec drupal YOUR COMMAND`

For example, if you want to perform a Drush cache rebuild you could run

`docker compose exec drupal drush cr`

If your commands rely on bash expression like wildcard expandsions or referencing variables defined inside the container you'll need to wrap you command in `bash -lc ""`

```
docker compose exec drupal bash -lc "ls **/*.php"
```


!!! Note "Container Names"
    The container name `drupal` is the 'service' name as set in your docker-compose.yml.

    You can also access other containers in this way, by replacing `drupal` with their service name. For example, `mariadb`, `solr`, `fcrepo`, etc.

For more information, see the documentation on [docker compose exec](https://docs.docker.com/reference/cli/docker/compose/exec/)

### Container Variables

In some of the commands throughout the ISLE Site Template documentation you will see references to variables like `${DRUPAL_DEFAULT_SITE_URL}`. These are variables that are provided in the containers by the s6-overlay in [Islandora Buildkit](https://github.com/Islandora-Devops/isle-buildkit).

If you want to see their values, you can run

```
docker compose exec drupal echo ${DRUPAL_DEFAULT_SITE_URL}'
```

!!! Note "with-contenv"
    Adding `with-contenv` to your commands gives you access to the s6-overlay environment variables. If you don't need to use these variables, the commands above can be simplified to

    `docker compose exec drupal bash`

    and

    `docker compose exec -T drupal bash -lc 'YOUR COMMAND'`


## Helpful Commands

### Checking logs

You can read logs for a container with:

```
docker compose logs service_name
```

For example, to read nginx logs for Drupal, use `docker compose logs drupal`.

If you don't know what you're looking for exactly, you can turn on the fire hose and look through all logs by dropping the service name and simply using:

```
docker compose logs
```

### Reindex Solr

You can reindex Solr through the Drupal admin page, or via drush commands by queuing for reindex:

```
docker compose exec drupal drush search-api-reindex
```

then triggering the reindex:

```
docker compose exec drupal drush search-api-index
```

### Reindex Fedora

!!! note
    This requires the [Views Bulk Operations module](https://www.drupal.org/project/views_bulk_operations)


You can reindex all of your Drupal data into Fedora, by running the following commands:

```
docker compose exec drupal \
  drush vbo-exec non_fedora_files emit_file_event  \
    --configuration="queue=islandora-indexing-fcrepo-file-external&event=Update"
```

```
docker compose exec drupal \
  drush vbo-exec all_taxonomy_terms emit_term_event \ 
    --configuration="queue=islandora-indexing-fcrepo-content&event=Update"
```

```
docker compose exec drupal \
  drush vbo-exec content emit_node_event \
    --configuration="queue=islandora-indexing-fcrepo-content&event=Update"
```

```
docker compose exec drupal \
  drush vbo-exec media emit_media_event \
    --configuration="queue=islandora-indexing-fcrepo-media&event=Update"
```

### Reindex Blazegraph

!!! note
    This requires the [Views Bulk Operations module](https://www.drupal.org/project/views_bulk_operations)


You can reindex all of your Drupal data into Blazegraph, by running the following commands:

```
docker compose exec drupal \
  drush vbo-exec all_taxonomy_terms emit_term_event \
    --configuration="queue=islandora-indexing-triplestore-index&event=Update"
```

```
docker compose exec drupal \
  drush vbo-exec content emit_node_event \
    --configuration="queue=islandora-indexing-triplestore-index&event=Update"
```

```
docker compose exec drupal \
  drush vbo-exec media emit_media_event \
    --configuration="queue=islandora-indexing-triplestore-index&event=Update"
```

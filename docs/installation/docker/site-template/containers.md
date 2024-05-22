# Working in Your Containers

When using Docker, we often have to run commands inside the Docker containers. For example, you can run the same Drush commands you would run in a non-Docker environment, but you must run those commands inside the Drupal container.

## Running Commands in the Containers

If you want to work inside a container, you can use Docker Compose to run bash from inside the container. You can get a bash shell inside the Drupal container by running

`docker compose exec drupal-dev with-contenv bash`

Or you can run your command from the host machine by running

`docker compose exec -T drupal-dev with-contenv bash -lc 'YOUR COMMAND'`

For example, if you want to perform a Drush cache rebuild you could run

`docker compose exec -T drupal-dev with-contenv bash -lc 'drush cr'`

!!! Note "Container Names"
    The container name drupal-dev is the ‘services’ name as set in your docker-compose.yml. To access the drupal container in production you would replace drupal-dev with drupal-prod.

    You can also access other containers in this way, by replacing drupal-dev with their service name. For example, mariadb-dev or solr-prod.

For more information, see the documentation on [docker compose exec](https://docs.docker.com/reference/cli/docker/compose/exec/)

### Container Variables

In some of the commands throughout the ISLE Site Template documentation you will see references to variables like `${DRUPAL_DEFAULT_SITE_URL}`. These are variables that are provided in the containers by the s6-overlay in [Islandora Buildkit](https://github.com/Islandora-Devops/isle-buildkit).

If you want to see their values, you can run 

```
docker compose exec -T drupal-dev with-contenv bash -lc 'echo ${DRUPAL_DEFAULT_SITE_URL}'
```

!!! Note "with-contenv"
    Adding `with-contenv` to your commands gives you access to the s6-overlay environment variables. If you don’t need to use these variables, the commands above can be simplified to
    
    `docker compose exec drupal-dev bash`

    and

    `docker compose exec -T drupal-dev -lc 'YOUR COMMAND'`
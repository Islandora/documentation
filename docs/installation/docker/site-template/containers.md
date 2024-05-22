# Working in Your Containers

## Running Commands in the Containers

Many of the following commands require running inside of your containers. You can get a bash shell inside the container by running

`docker compose exec drupal-dev with-contenv bash`

Or you can run a command from the host machine by running

`docker compose exec -T drupal-dev with-contenv bash -lc 'YOUR COMMAND'`

For example, if you want to run a drush cache rebuild you could run

`docker compose exec -T drupal-dev with-contenv bash -lc 'drush cr'`

!!! Note "Container Names"
    The container name drupal-dev is the ‘services’ name as set in your docker-compose.yml. To access the drupal container in production you would replace drupal-dev with drupal-prod.

    You can also access other containers in this way, by replacing drupal-dev with their service name, like mariadb-dev.

For more information see the documentation on [docker compose exec](https://docs.docker.com/reference/cli/docker/compose/exec/)

### Container Variables

In some of the commands throughout the ISLE Site Template documentation you will see references to variables like `${DRUPAL_DEFAULT_SITE_URL}`. These are variables that are provided in the containers by the s6-overlay in [Islandora Buildkit](https://github.com/Islandora-Devops/isle-buildkit).

If you want to see their values you can run 

```
docker compose exec -T drupal-dev with-contenv bash -lc 'echo ${DRUPAL_DEFAULT_SITE_URL}'
```

!!! Note "with-contenv"
    Adding `with-contenv` to your commands gives you access to the s6-overlay environment variables. If you don’t need to use these variables, the commands above can be simplified to
    
    `docker compose exec drupal-dev bash`

    and

    `docker compose exec -T drupal-dev -lc 'YOUR COMMAND'`
# Updating

The following sections describe how to keep your Islandora install up to date with current versions of Drupal modules and Docker images.

## Updating Isle Buildkit (Islandora Docker Images)

Updating to a new version of Isle Buildkit is done by setting the ISLANDORA_TAG variable in your .env file. Once you have updated the .env file, you need to pull the new Islandora images, and rebuild your custom Drupal image from the specified Drupal image.

For example, to upgrade from Buildkit 2.0.0 to 3.0.0 you would do the following steps:

1. Change your .env file to say ISLANDORA_TAG=3.0.0

2. Stop your Docker containers

    `docker compose --profile dev down`

3. Pull the new Docker images (except the Drupal image)

    `docker compose --profile dev pull --ignore-buildable --ignore-pull-failures`
    
4. Build your custom Drupal image

    `docker compose --profile dev build`
    
5. Start your containers from the new images

    `docker compose --profile dev up -d`

Once you have upgraded your images, you may need to perform extra steps for Solr and Mariadb, depending on whether these have new versions in the new images. 

!!! note "Production Sites"

    Upgrading a production site works the same way, just replace `dev` with `prod` in the above instructions.

### Solr

You may need to regenerate your Solr configs if Solr has been updated to a new version, or when the Search API Solr Drupal module has been updated. If you visit /admin/config/search/search-api/server/default_solr_server on your Islandora site it will tell you if the configs need to be updated.

To generate new configs perform the following steps:

1. Remove existing solr configs

    `docker compose exec -T solr-dev with-contenv bash -lc 'rm -r server/solr/default/*'`
    
2. Recreate solr configs for new solr versions

    `docker compose exec -T drupal-dev with-contenv bash -lc "for_all_sites create_solr_core_with_default_config"`
    
3. Reindex Solr through the admin page or via Drush 

### MariaDB

After updating MariaDB, you may need to run [mariadb-upgrade](https://mariadb.com/kb/en/mariadb-upgrade/) inside your MariaDB container, to update your system tables. This should be safe to run any time, but it is a good idea to back up your database first, just in case.

You can run this with

`docker compose exec mariadb-dev mariadb-upgrade`

## Updating Traefik

Traefik is not updated with the other buildkit images. It is recommended that you periodically update Traefik by changing the image version in your docker-compose.yml file to the [current version in use by Isle Site Template](https://github.com/Islandora-Devops/isle-site-template/blob/main/docker-compose.yml)

## Updating Drupal Modules

Drupal updates are performed through composer on your development site. Once the modules have been added/removed/updated, your `composer.json` and `composer.lock` files can be checked into your git repository and you can rebuild your production Drupal container with the new files.

### Development

Composer commands need to [run in your Drupal container](/documentation/installation/docker/site-template/containers.md). For example:

​​`docker compose exec drupal-dev composer update -W`

Or 

`docker compose exec drupal-dev composer require 'drupal/islandora:^2.11'`

Running database updates is also done in the container like this:

`docker compose exec drupal-dev drush updb`

!!! note

    You should backup your database before running database updates

If you are enabling or uninstalling modules, you will also need to export your Drupal configuration.

Once you have finished your composer changes you can commit and push your repository with the new `composer.json` and `composer.lock` changes.

### Production

First you will `git pull` to get the `composer.json` and `composer.lock` changes you made in development.

Next you build your Drupal image again, which will install the modules specified in those files:

`docker compose --profile prod build`

Then you stop and start your containers to get the new image:

`docker compose --profile prod down`

`docker compose --profile prod up -d`

And if necessary, run database updates:

`docker compose exec drupal-prod drush updb`

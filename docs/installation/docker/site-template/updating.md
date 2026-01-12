# Updating

The following sections describe how to keep your Islandora install up to date with current versions of Drupal modules and Docker images.

## Updating Isle Buildkit (Islandora Docker Images)

Updating to a new version of Isle Buildkit is done by setting the ISLANDORA_TAG variable in your .env file. Once you have updated the .env file, you need to pull the new Islandora images, and rebuild your custom Drupal image from the specified Drupal image.

For example, to upgrade from Buildkit 2.0.0 to 3.0.0 you would do the following steps:

1. Change your .env file to say ISLANDORA_TAG=3.0.0

2. Stop your Docker containers

    `docker compose down`

3. Pull the new Docker images (except the Drupal image)

    `docker compose pull --ignore-buildable --ignore-pull-failures`

4. Build your custom Drupal image

    `docker compose build` or `make build`

5. Start your containers from the new images

    `docker compose up -d` or `make up`

Once you have upgraded your images, you may need to perform extra steps for Solr and MariaDB, depending on whether these have new versions in the new images.

### Solr

You may need to regenerate your Solr configs if Solr has been updated to a new version, or when the Search API Solr Drupal module has been updated. If you visit /admin/config/search/search-api/server/default_solr_server on your Islandora site it will tell you if the configs need to be updated.

To generate new configs perform the following steps:

1. Remove existing solr configs

    `docker compose exec -T solr with-contenv bash -lc 'rm -r server/solr/default/*'`

2. Restart the Solr container

    `docker compose restart solr`

3. Recreate solr configs for new solr versions

    `docker compose exec -T drupal with-contenv bash -lc "for_all_sites create_solr_core_with_default_config"`

4. Reindex Solr through the admin page or via Drush 

### MariaDB

After updating MariaDB, you may need to run [mariadb-upgrade](https://mariadb.com/kb/en/mariadb-upgrade/) inside your MariaDB container, to update your system tables. This should be safe to run any time, but it is a good idea to back up your database first, just in case.

You can run this with

`docker compose exec mariadb mariadb-upgrade`

## Updating Traefik

Traefik is not updated with the other buildkit images. It is recommended that you periodically update Traefik by changing the image version in your docker-compose.yml file to the [current version in use by Isle Site Template](https://github.com/Islandora-Devops/isle-site-template/blob/main/docker-compose.yml)

## Updating Drupal Modules

Drupal updates are performed through composer in your Drupal container. Once the modules have been added/removed/updated, your `composer.json` and `composer.lock` files can be checked into your git repository and you can rebuild your Drupal container with the new files.

### Local Development

Composer commands need to [run in your Drupal container](../docker-prereq.md#containers). For example:

`docker compose exec drupal composer update -W`

Or

`docker compose exec drupal composer require 'drupal/islandora:^2.11'`

Running database updates is also done in the container like this:

`docker compose exec drupal drush updb`

!!! note

    You should backup your database before running database updates

If you are enabling or uninstalling modules, you will also need to export your Drupal configuration.

Once you have finished your composer changes you can commit and push your repository with the new `composer.json` and `composer.lock` changes.

### Deploying to Production

First you will `git pull` to get the `composer.json` and `composer.lock` changes you made in development.

Next you build your Drupal image again, which will install the modules specified in those files:

`docker compose build` or `make build`

Then you stop and start your containers to get the new image:

`docker compose down`

`docker compose up -d` or `make up`

And if necessary, run database updates:

`docker compose exec drupal drush updb`

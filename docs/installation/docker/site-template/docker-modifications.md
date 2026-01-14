# Docker Modifications

ISLE Site Template provides you with a `docker-compose.yml` file that allows you to get an Islandora site running quickly, but it makes some assumptions about how the site will run, and which containers you will use. Once you have your site running you may want to make some modifications to the default setup that the Site Template uses.

## Adding / Editing Environment Variables

Islandora Buildkit provides several environment variables that can be modified when creating containers.

Please see the README for the different buildkit images to see what is available:

- [ActiveMQ](https://github.com/Islandora-Devops/isle-buildkit/tree/main/activemq)
- [Alpaca](https://github.com/Islandora-Devops/isle-buildkit/tree/main/alpaca)
- [Blazegraph](https://github.com/Islandora-Devops/isle-buildkit/tree/main/blazegraph)
- [Cantaloupe](https://github.com/Islandora-Devops/isle-buildkit/tree/main/cantaloupe)
- [Code Server](https://github.com/Islandora-Devops/isle-buildkit/tree/main/code-server)
- [Crayfits](https://github.com/Islandora-Devops/isle-buildkit/tree/main/crayfits)
- [Drupal](https://github.com/Islandora-Devops/isle-buildkit/tree/main/drupal)
- [Fedora](https://github.com/Islandora-Devops/isle-buildkit/tree/main/fcrepo6)
- [Fits](https://github.com/Islandora-Devops/isle-buildkit/tree/main/fits)
- [Homarus](https://github.com/Islandora-Devops/isle-buildkit/tree/main/homarus)
- [Houdini](https://github.com/Islandora-Devops/isle-buildkit/tree/main/houdini)
- [Hypercube](https://github.com/Islandora-Devops/isle-buildkit/tree/main/hypercube)
- [MariaDB](https://github.com/Islandora-Devops/isle-buildkit/tree/main/mariadb)
- [Milliner](https://github.com/Islandora-Devops/isle-buildkit/tree/main/milliner)
- [Solr](https://github.com/Islandora-Devops/isle-buildkit/tree/main/solr)

You can add these environment variables to your docker-compose.yml in order to change their values. For example, if you want to increase the PHP memory limit in your Drupal container, you can do so like this:

```yaml
services:
  drupal:
    environment:
      PHP_MEMORY_LIMIT: 1G
```

Or use a `docker-compose.override.yml` file to keep your customizations separate from the main configuration.


## Removing Services

You may not want to use all the images that are included in the Site Template's `docker-compose.yml`. You can remove containers by deleting their service definitions in the docker-compose.yml file.

For example, to remove Fedora, you would delete the `fcrepo` service.

Depending on the container you are removing, you may need to delete references to it as well. For example, some containers are referenced by others in the `depends_on` field. You will need to also delete these references.

If you are removing a container which is referenced by Drupal, ensure that you update Drupal as well (e.g. if removing Fedora, ensure your Media's files are not writing to the Fedora filesystem).

After doing `docker compose down`, run `docker compose up -d --remove-orphans` to remove the containers you removed from the docker-compose.yml file. 

## Hiding Fedora From the Public

By default, your Fedora repo will be available to the public at `fcrepo.${DOMAIN}`. If you do not want to expose your Fedora, you can stop this URL from working by disabling it via Traefik in your `docker-compose.yml` or `docker-compose.override.yml`.

You can modify the Traefik routing configuration in the `traefik/dynamic` directory to disable external access to Fedora while keeping it accessible internally to other services.

If you have done this, you can also remove the DNS records that point this URL to your server.

Finally, ensure that Drupal is configured to access Fedora using the internal Docker network hostname:

```yaml
DRUPAL_DEFAULT_FCREPO_URL: "http://fcrepo:8080/fcrepo/rest/"
```

This is typically the default configuration, allowing Drupal to access Fedora internally even when external access is disabled.

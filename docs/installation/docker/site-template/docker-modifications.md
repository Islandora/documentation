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

You can add these environment variables to your docker-compose.yml in order to change their values. For example, if you want to increase the PHP memory limit in your production Drupal container, you can do so like this:

```
    drupal-prod:
        <<: [*prod, *drupal]
        Environment:
            PHP_MEMORY_LIMIT: 1G
```


## Removing Services

You may not want to use all the images that are included in the Site Template’s `docker-compose.yml`. You can remove containers by deleting their sections in the docker-compose.yml file.

For example, to remove Fedora, you would delete the services called fcrepo-dev and fcrepo-prod.

Depending on the container you are removing, you may need to delete references to it as well. For example, some containers are referenced by others in the `depends_on` field. You will need to also delete these references, so if you delete the `fedora-dev` service, you will need to remove the rule that `traefik-dev` depends on it.

If you are removing a container which is referenced by Drupal, ensure that you update Drupal as well (e.g. if removing Fedora, ensure your Media's files are not writing to the Fedora filesystem).

After doing `docker compose down`, run `docker compose up -d --remove-orphans` to remove the containers you removed from the docker-compose.yml file. 

## Hiding Fedora From the Public

By default, your Fedora repo will be available to the public at `fcrepo.${DOMAIN}`. If you do not want to expose your Fedora, you can stop this URL from working by disabling it via Traefik in your `docker-compose.yml`. To do this, you need to add the `traefik-disable` label to `fcrepo-prod` like this,

```
    fcrepo-prod:
        <<: [*prod, *fcrepo]
        environment:
            <<: [*fcrepo-environment]
            FCREPO_ALLOW_EXTERNAL_DRUPAL: "https://${DOMAIN}/"
        labels:
            <<: [*traefik-disable, *fcrepo-labels]
```

If you have done this, you can also remove the DNS records that point this URL to your production server.

Finally, you will have to change the URL that Drupal uses to access the Fedora repo. This can be found in your `docker-compose.yml'` in the `environment` section for `drupal-prod`, and should be changed to:

```
DRUPAL_DEFAULT_FCREPO_URL: "http://fcrepo:8080/fcrepo/rest/"
```

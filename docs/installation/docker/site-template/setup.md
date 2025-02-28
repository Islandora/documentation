# Initial Setup

Instructions for creating your site can be found in the [project's README file](https://github.com/Islandora-Devops/isle-site-template)

This page lists a few things to note about the process, but is not meant to be a replacement for the instructions in the README file.

## Custom Drupal Image

ISLE Site Template uses a custom Drupal image that you build on top of the provided Islandora Drupal image. This means you will not be running the islandora/drupal image directly, but the provided Dockerfile will use it to build your image.

!!! Note "Note for those coming from ISLE-DC"

    In ISLE-DC, we only use a custom image in production, but in the ISLE Site Template, we use it for both.

Building your custom Drupal image is done by running

`docker compose --profile dev build` for your development image

or

`docker compose --profile prod build` for your production image

This builds the docker image based on the Dockerfile in the `drupal` directory, which uses your composer files to pull the Drupal modules it needs into the image. Because the Dockerfile and composer files are part of the git repository, you can build your Drupal image locally, or on your production server.

This documentation assumes you will be building your production image on the production server. If you do it this way, it is not necessary to push your image to a container registry. Instead you just pull your git repository anytime you make changes to your composer files, and run `docker compose build` again.

!!! Note "Using a Container Registry"
    If you want to build your production images somewhere other than on your production server, you can do so. The .env file allows you to set your image repository URL, which will allow you to push / pull your Drupal image to / from your container registry. If you do this, you can then run `docker compose pull` instead of `docker compose build` on your production server, to pull the already built image to that server.

    For more information please see the documentation on [docker compose pull](https://docs.docker.com/reference/cli/docker/compose/pull/) and [docker compose build](https://docs.docker.com/reference/cli/docker/compose/build/)

## Adding a Staging Site

The process for setting up a staging site is the same as production, but you will need to use a different URL. Since the URL is set in the .env file, which is checked into your git repository, you may wish to use docker-compose.override.yml for this. In docker-compose.override.yml you will need to override anywhere the `DOMAIN` variable is used, for example:

```
services:
    cantaloupe-prod:
        labels:
            traefik.http.routers.cantaloupe_http.rule: &traefik-host-cantaloupe-prod Host(`staging-url.com`) && PathPrefix(`/cantaloupe`)
            traefik.http.routers.cantaloupe_https.rule: *traefik-host-cantaloupe-prod
    drupal-prod:
        environment:
            DRUPAL_DEFAULT_CANTALOUPE_URL: "https://staging-url.com/cantaloupe/iiif/2"
            DRUPAL_DEFAULT_SITE_URL: "staging-url.com"
            DRUPAL_DRUSH_URI: "https://staging-url.com"
        labels:
            traefik.http.routers.drupal_http.rule: &traefik-host-drupal-prod Host(`staging-url.com`)
            traefik.http.routers.drupal_https.rule: *traefik-host-drupal-prod
    fcrepo-prod:
        environment:
            FCREPO_ALLOW_EXTERNAL_DRUPAL: "https://staging-url.com"
        labels:
            traefik.http.routers.fcrepo_http.rule: &traefik-host-fcrepo-prod Host(`fcrepo.staging-url.com`)
            traefik.http.routers.fcrepo_https.rule: *traefik-host-fcrepo-prod
    traefik-prod:
        networks:
            default:
                aliases:
                    # Allow services to connect on the same name/port as the outside.
                    - "staging-url.com" # Drupal is at the root domain.
                    - "fcrepo.staging-url.com"
```

!!! note "Restricting Access to Staging Servers"

    Using letsencrypt to generate your certs requires port 80 to be accessible on your server. If you would like to keep your site private by limiting access to certain IP addresses, you can still firewall port 443, but you will have to leave port 80 open. Alternatively, you can use [Traefik's IPAllowList middleware](https://doc.traefik.io/traefik/middlewares/http/ipallowlist/) to restrict access to outisde IPs.

## Adding Demo Content

If you are spinning up a new site for testing, you can add some demo content to your site by running
```
[ -d "islandora_workbench" ] || (git clone https://github.com/mjordan/islandora_workbench)

[ -d "islandora_workbench/islandora_demo_objects" ] || git clone https://github.com/Islandora-Devops/islandora_demo_objects.git islandora_workbench/islandora_demo_objects

cd islandora_workbench && docker build --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) -t workbench-docker .; cd ..

perl -i -pe 's#^host.*#host: "https://islandora.dev"#g' islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

perl -i -pe 's#^input_dir.*#input_dir: "islandora_demo_objects"#g' islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

perl -i -pe 's#^input_csv.*#input_csv: "create_islandora_objects.csv"#g' islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

grep secure_ssl_only islandora_workbench/islandora_demo_objects/create_islandora_objects.yml || echo 'secure_ssl_only: false' >> islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

cd islandora_workbench && docker run -it --rm --network="host" -v .:/workbench --name my-running-workbench workbench-docker bash -lc "./workbench --config islandora_demo_objects/create_islandora_objects.yml"; cd ..
```

## Custom Themes & Modules

You may wish to copy themes and modules into your project directly, instead of using Composer to manage them. For example, if you are creating your own theme instead of using a contributed one.

Isle Site Template provides directories at `drupal/rootfs/var/www/drupal/web/modules/custom` and `drupal/rootfs/var/www/drupal/web/themes/custom` for you to add your custom themes and modules.

These directories are mounted in development, so any changes to them will be shared between your host machine and your Drupal container.

In production, these themes and modules will be included when the Drupal image is built.

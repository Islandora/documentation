# Initial Setup

Instructions for creating your site can be found in the [project's README file](https://github.com/Islandora-Devops/isle-site-template)

This page lists a few things to note about the process, but is not meant to be a replacement for the instructions in the README file.

## Quick Start

Getting started is now simplified with the `make` commands:

1. Clone your repository (created from the template)
2. Run `make up` to initialize and start all services

The `make up` command handles:
- Preparing your host machine
- Creating the `.env` file from `sample.env` if needed
- Generating necessary secrets and certificates
- Building the custom Drupal Docker image
- Starting all services with smart port allocation

By default, your site will be available at `http://islandora.traefik.me` (which resolves to 127.0.0.1).

## Custom Drupal Image

ISLE Site Template uses a custom Drupal image that you build on top of the provided Islandora Drupal image. This means you will not be running the islandora/drupal image directly, but the provided Dockerfile will use it to build your image.

!!! Note "Note for those coming from ISLE-DC"

    In ISLE-DC, we only use a custom image in production, but in the ISLE Site Template, we use it for both development and production environments.

Building your custom Drupal image is done by running:

`make build` or `docker compose build`

This builds the docker image based on the Dockerfile in the `drupal` directory, which uses your composer files to pull the Drupal modules it needs into the image. Because the Dockerfile and composer files are part of the git repository, you can build your Drupal image locally, or on your production server.

This documentation assumes you will be building your production image on the production server. If you do it this way, it is not necessary to push your image to a container registry. Instead you just pull your git repository anytime you make changes to your composer files, and run `docker compose build` again.

!!! Note "Using a Container Registry"
    If you want to build your production images somewhere other than on your production server, you can do so. The .env file allows you to set your image repository URL, which will allow you to push / pull your Drupal image to / from your container registry. If you do this, you can then run `docker compose pull` instead of `docker compose build` on your production server, to pull the already built image to that server.

    For more information please see the documentation on [docker compose pull](https://docs.docker.com/reference/cli/docker/compose/pull/) and [docker compose build](https://docs.docker.com/reference/cli/docker/compose/build/)

## Adding a Staging Site

The process for setting up a staging site is the same as for your production environment. The simplest approach is to override the `DOMAIN` environment variable on your staging server by editing its `.env` file to use a different URL (e.g., `staging.example.com`).

If you need more fine-grained control, you can create a `docker-compose.override.yml` file with site-specific customizations. However, with the new unified docker-compose setup, most staging configurations can be handled by simply changing environment variables in the `.env` file.

!!! note "Restricting Access to Staging Servers"

    Using letsencrypt to generate your certs requires port 80 to be accessible on your server. If you would like to keep your site private by limiting access to certain IP addresses, you can still firewall port 443, but you will have to leave port 80 open. Alternatively, you can use [Traefik's IPAllowList middleware](https://doc.traefik.io/traefik/middlewares/http/ipallowlist/) to restrict access to outside IPs.

## Adding Demo Content

If you are spinning up a new site for testing, you can add some demo content to your site by running
```
[ -d "islandora_workbench" ] || (git clone https://github.com/mjordan/islandora_workbench)

[ -d "islandora_workbench/islandora_demo_objects" ] || git clone https://github.com/Islandora-Devops/islandora_demo_objects.git islandora_workbench/islandora_demo_objects

docker build \
  --build-arg USER_ID=$(id -u) \
  --build-arg GROUP_ID=$(id -g) \
  -t workbench-docker:latest \
  islandora_workbench

perl -i -pe 's#^host.*#host: "http://islandora.traefik.me"#g' islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

perl -i -pe 's#^input_dir.*#input_dir: "islandora_demo_objects"#g' islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

perl -i -pe 's#^input_csv.*#input_csv: "create_islandora_objects.csv"#g' islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

grep secure_ssl_only islandora_workbench/islandora_demo_objects/create_islandora_objects.yml || echo 'secure_ssl_only: false' >> islandora_workbench/islandora_demo_objects/create_islandora_objects.yml

pushd islandora_workbench
docker run -it --rm \
  --network="host" \
  -v .:/workbench \
  --name my-running-workbench \
  workbench-docker:latest \
  bash -lc "./workbench --config islandora_demo_objects/create_islandora_objects.yml"
popd
```

## Custom Themes & Modules

You may wish to copy themes and modules into your project directly, instead of using Composer to manage them. For example, if you are creating your own theme instead of using a contributed one.

Isle Site Template provides directories at `drupal/rootfs/var/www/drupal/web/modules/custom` and `drupal/rootfs/var/www/drupal/web/themes/custom` for you to add your custom themes and modules.

These directories are mounted in development, so any changes to them will be shared between your host machine and your Drupal container.

In production, these themes and modules will be included when the Drupal image is built.

# Installing a Development or Demo Server

When developing locally, your Drupal site resides in your `isle-dc/codebase` folder and is bind-mounted into your
Drupal container.  This lets you update code using the IDE of your choice on your host machine, and the
changes are automatically reflected on the Drupal container.

## Installing ISLE Docker Compose

Use Git to install the ISLE Docker Compose tool:

`git clone https://github.com/islandora-devops/isle-dc`

Tagged versions are available [here](https://github.com/Islandora-Devops/isle-dc/tags).

## Getting Started

If you don't already have a Drupal site, you'll be given a basic setup using Drupal 10 and the
[Islandora Starter Site](https://github.com/Islandora-Devops/islandora-starter-site).

If you do already have a Drupal site, use git to clone it into place as the `codebase` folder.

```
cd /path/to/isle-dc
git clone https://github.com/your_org/your_repo codebase
```

Now you'll need to tell `isle-dc` to look for it by setting the `ENVIRONMENT` variable in
your `.env` file. If you don't have one, copy over `sample.env` and name it `.env`. Then
set

```
ENVIRONMENT=starter
```

You should also change the `COMPOSE_PROJECT_NAME` variable. This determines the name of the
Docker containers and volumes that are created when you run `make starter`. If you leave this as the default
you will need to be careful not to overwrite the containers with another install of `isle-dc`
later.
```
COMPOSE_PROJECT_NAME=isle-dc
```

If your site includes exported configuration from `drush config:export`, then you'll also
need to set

```
INSTALL_EXISTING_CONFIG=true
DRUPAL_INSTALL_PROFILE=minimal
```

Once you are ready, run

```bash
make starter
```

to install the Drupal site in your `codebase` folder and spin up all the other containers with it.

Enjoy your Islandora instance!  Check out the [basic usage documentation](docker-basic-usage.md) to see
all the endpoints that are available and how to do things like start and stop Islandora. Your passwords,
including the Drupal admin password, can be found in the `secrets/live` directory after you run `make starter`.

## Demo Content

To populate your site with some demo content, you can run `make demo_content`. This will import some sample objects into your Islandora site.

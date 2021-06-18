# Installing a Development Server

When developing locally, your Drupal site resides in your `isle-dc/codebase` folder and is bind-mounted into your
Drupal container.  This lets you update code using the IDE of your choice on your host machine, and the
changes are automatically reflected on the Drupal container. 

## Getting Started

If you don't already have a Drupal site, you'll be given a basic setup using Drupal 9 and the
[Islandora install profile](https://github.com/islandora-devops/islandora_profile).

If you do already have a Drupal site, use git to clone it into place as the `codebase` folder.

```
cd /path/to/isle-dc
git clone https://github.com/your_org/your_repo codebase
```

Now you'll need to tell `isle-dc` to look for it by setting the `ENVIRONMENT` variable in
your `.env` file. If you don't have one, copy over `sample.env` and name it `.env`. Then
set

```
ENVIRONMENT=local
```
 
If your site includes exported configuration from `drush config:export`, then you'll also
need to set 

```
INSTALL_EXISTING_CONFIG=true
DRUPAL_INSTALL_PROFILE=minimal
```

Once you are ready, run

```bash
make local
```

to install the Drupal site in your `codebase` folder and spin up all the other containers with it.

## Kicking the Tires

All services will be available at the [same locations as the demo environment](../docker-demo#kicking-the-tires),
however your Drupal site is not baked into the container and instead should be available in your `codebase` folder.
You should see what looks like an installed Drupal site, with a `settings.php` file and contrib modules, etc...  

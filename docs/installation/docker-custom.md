# Installing a Production/Staging Server 

If you are deploying Islandora on a server that is publicly accessible, there's extra precautions you should take for
performance and security reasons. If you follow these steps, you'll see how we can use our `codebase` folder to build
a custom Drupal container, and bake code into the container instead of bind-mounting it in. We'll also cover how to
store passwords as secrets and set up SSL/TLS.
 
## Getting Started

If you don't already have a Drupal site, you'll be given a basic setup using Drupal 9 and the
[Islandora install profile](https://github.com/islandora-devops/islandora_profile).

If you do already have a Drupal site, use git to clone it into place as the `codebase` folder.

```
cd /path/to/isle-dc
git clone https://github.com/your_org/your_repo codebase
```

If your site includes exported configuration from `drush config:export`, then you'll need
to configure `isle-dc` to use it. Copy over the sample configuration file to use as a starting
point and name it `.env`.

```
cp sample.env .env
```

Then set these two configuration values in your `.env` file.

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


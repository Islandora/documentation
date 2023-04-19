# Maintaining Your Drupal Site

Drupal has a dedicated security team, and regularly produces updates to address security issues
that are discovered.  You should always keep your Drupal site up to date so that you are
protected against known vulnerabilities. Updating Drupal can be intimidating, but we have
step-by-step instructions to help you do it safely.

## Note on Production Sites

Please note that maintaining Drupal should be done on your development
site. For production sites, you should build an image from the `codebase` folder on your development
machine, and run that image in production. For more information on this, please see the [production install 
instructions](https://islandora.github.io/documentation/installation/docker-custom/).

## Running commands

Once you have a `codebase` folder, how do you maintain it and keep it up to date?  The quick answer is
"the same way you maintain any Drupal site with Composer and Drush"... with one small caveat. You most
likely do not have PHP or Composer on your machine, and even if you do, you want to make sure you're
using the exact same version that your Docker container is using.  So to ensure all the versions of things line
up, we use Docker to execute Drush and Composer from the Drupal container.  The general template for
running a command in your Drupal container looks like this:

```
docker-compose exec -T drupal with-contenv bash -lc 'YOUR COMMAND'
```

You can also just shell into the Drupal container and run commands as well,
just be aware that if you shut down your container for any reason, you'll lose your
bash history.  If you want to shell in to run commands, drop the `-T` and `-lc 'YOUR COMMAND'`
bits.

```
docker-compose exec drupal with-contenv bash
```

## Updating your Drupal Site

Use Composer to update your site's modules and their dependencies. The working directory
of the Drupal container is the Drupal root (a.k.a. `codebase`), so you don't need to `cd`
into any other directory before running the command.  The following command will update
all modules and their dependencies that are not pinned to specific versions.

```
docker-compose exec -T drupal with-contenv bash -lc "su nginx -s /bin/bash -c 'composer update -W'"

```
Note that we run this command as the nginx user. By default, commands are run as root, which 
can cause some ownership issues when running Composer. By running this as nginx, we ensure
that new files are owned by the nginx user.

### Permission Issue

When running Composer commands you may come across the following error
```
[ErrorException]
file_put_contents(/var/www/drupal/web/sites/default/settings.php): failed to open stream: Operation not permitted
```
This means that Composer is not able to write to your settings.php file. If you run into this
error, giving write permission to the nginx user should fix it.

## Drupal Database Updates

After getting the newest code, you'll want to use Drush to update the Drupal database
and run any other update hooks that have been introduced.  However, _YOU SHOULD BACK UP
YOUR DATABASE BEFORE GOING ANY FURTHER_. You never know when something will go wrong and
you don't want to be stuck with an unusable database and no plan B.

```
make drupal-database-dump DEST=/path/to/dump.sql
```

Now you can safely update the Drupal database with Drush via

```
docker-compose exec -T drupal with-contenv bash -lc 'drush updb'
```

If for any reason, something goes wrong, you can Restore the Drupal database at any time by running

```
make drupal-database-import SRC=/path/to/dump.sql
```

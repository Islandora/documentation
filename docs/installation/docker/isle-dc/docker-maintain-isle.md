# Maintaining Your ISLE Infrastructure

!!! warning
    isle-dc is deprecated in favor of [isle-site-template](/documentation/installation/docker/site-template/site-template/).

You will regularly be updating your Drupal site as security patches and module updates are released.
Less often, you will also need to update the rest of your Islandora installation.  ISLE makes this easy.
In fact, it was specifically designed to streamline this process.

Since Islandora is not a single piece of software, but instead many pieces of software working together
in concert, maintaining all of it is a daunting task.  There's nginx, tomcat, karaf, etc... Then there's
everything needed for the authentication layer and JWT keys.  Plus there's all the microservices.  You can
see that all this adds up to a significant maintenance burden.

Now imagine if all that time and effort spent on security updates and getting the newest versions could
be boiled down to a handful of simple commands.  That's exactly what ISLE does!

## Updating ISLE

!!! Updating your isle-dc repository
    Before we update ISLE we should pull the most recent version of the isle-dc repository. This ensures that our `docker-compose.yml` file has any new additions that the new version of ISLE will need.
    
    You may also want to check your `.env` file against the `sample.env` found in the repository to see if there are new variables you wish to make use of.

Updating ISLE is easy.  When a new release is made available, you update the `TAG` variable in your
`.env` file to the latest version. Suppose you are on ISLE 1.0.0, and ISLE 1.1.0 has been released.
Then we would set

```
TAG=1.1.0
```

Before we update our `docker-compose.yml` file we want to stop the running containers with

```
make down
```

We'll then generate a new `docker-compose.yml` file that includes the new tag with

```
make -B docker-compose.yml
```

After that, we pull the new containers with

```
make pull
```

And finally we deploy the updated containers by running

```
make up
```

You can check that everything is running at the version you've specified with

```
docker ps -a
```

The version that's running can be confirmed by looking at the `IMAGE` column in the output.

### Major Version Updates

Major version updates may require a bit more work after the new containers are up and running, if the new images are 
running higher major versions of software. For example, if there has been an update to PHP or Solr.

#### Solr

When upgrading to a new major version of Solr, you need to regenerate a new set of config files. You will need to do this on both 
your development site, and your production site.

First, remove the old solr data. This will remove everything that has been indexed, so we will have to reindex our site in a later step.

```
docker compose exec -T solr with-contenv bash -lc 'rm -r server/solr/ISLANDORA'
```

Next, recreate our solr core with a new set of config files.

```
make solr-cores
```

Finally, we reindex our site.

```
make reindex-solr
```

#### Drupal

Drupal updates will be handled by your composer.json file, but you may require a certain version of ISLE in order to have the correct 
version of PHP. Once you have the correct PHP version installed, you can update Drupal to a new major version in the same way you 
normally update modules. It may be helpful to reference the Starter Site's composer.json file at 
https://github.com/Islandora-Devops/islandora-starter-site/blob/main/composer.json

Once you have done this on your development site, the process for your production site is the same as any other Drupal updates.

#### Mariadb

After updating Mariadb, you may need to run [mariadb-upgrade](https://mariadb.com/kb/en/mariadb-upgrade/) inside your mariadb container, to update your system tables. 
This should be safe to run any time, but it is a good idea to back up your database first, just in case.

You can run this from your isle directory with
```
docker compose exec mariadb mariadb-upgrade
```

### Specific Update Notes

#### Version 1.x to 2.x

Upgrading ISLE from 1.x to the next major version requires the TAG be set to 2.0.5 or higher. Once you create your containers 
you will need to follow the above instructions for Mariadb, Solr, and Drupal.

ISLE 2.0 bumps PHP up to version 8.1, which allows you to upgrade to Drupal 10.

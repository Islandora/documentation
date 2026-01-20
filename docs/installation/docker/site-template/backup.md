# Backups & Transferring Data Between Sites

The following instructions describe how to back up and restore an Islandora site. This is also used to transfer data between a development and production site, or to sync a staging site with a production site.

Container names are based on the service names defined in your `docker-compose.yml` file. For example, the Drupal container is typically named `drupal`, the MariaDB container is `mariadb`, etc. The following instructions use these standard service names.

Before attempting the following commands, you should familiarize yourself with [running commands inside a container](/documentation/installation/docker/site-template/containers.md) and with [docker compose cp](https://docs.docker.com/reference/cli/docker/compose/cp/)


## Drupal Configuration

The typical use case of this is to export your Drupal configuration files out of your development site's Drupal database and onto the host machine. This process lets you check your configuration into your git repository so you can import it into your production site.


### Export

To export your config run:

```
docker compose exec drupal drush config:export -y
```

Then commit and push your git repo so the new config files are included.


### Import

To import your config to a production or staging site you first need to `git pull` the config files you pushed from your development site, then rebuild and restart your custom Drupal container.

```
git pull
docker compose build
docker compose down drupal
docker compose up -d
```

Once this is done, your config files will be included in the container. You can then import them by running

```
docker compose exec drupal drush config:import -y
```


## Drupal Database

The Drupal database contains all your Drupal settings and content. If you want to move configuration settings from development to production you should use the Drupal Configuration instructions above.

These instructions can also be used to move your content from production to staging or development.


### Back Up

Backing up the Drupal database involves doing a database dump in the drupal container:

```
docker compose exec drupal \
    bash -lc "drush sql-dump -y \
    --skip-tables-list=cache,cache_*,watchdog \
    --structure-tables-list=cache,cache_*,watchdog \
    --result-file=/tmp/drupal.sql"
```

and then copying that dump from the container to the host machine:

```
docker compose cp drupal:/tmp/drupal.sql [path/on/host/drupal.sql]
```


### Restore

Restoring from a database dump requires copying the dump file into the drupal container:

```
docker compose cp [path-to/drupal.sql] drupal:/tmp/drupal.sql
```

Then replacing the database with the dump:

```
docker compose exec drupal drush sqlq --debug --file /tmp/drupal.sql'
```

And finally, rebuilding the Drupal cache:

```
docker compose exec drupal drush cr
```


## Drupal Public Files

Drupal's public files contain any files used on static pages. This is also where your Islandora derivatives are stored, which includes FITS, thumbnails, etc.

These instructions can also be used to move your content from production to staging or development.


### Back Up

Drupal public files can be compressed to a tgz file in the Drupal container:

```
docker compose exec drupal \
  drush archive:dump \
  --files \
  --destination=/tmp/files.tar.gz
```

Then copied to the host machine:

```
docker compose cp drupal:/tmp/files.tar.gz [path/on/host/files.tar.gz]
```


### Restore

Drupal files can be restored from a tgz file by copying them into the Drupal container:

```
docker compose cp files.tar.gz drupal:/tmp/files.tar.gz
```

Then placed in the correct directory inside the Drupal volume:

```
docker compose exec drupal tar zxvf /tmp/files.tar.gz -C /var/www/drupal/web/sites/default/files
docker compose exec drupal chown -R nginx:nginx /var/www/drupal/web/sites/default/files
docker compose exec drupal rm /tmp/files.tar.gz
```

!!! note
    This will overwrite existing files if they have the same filename, but does not remove existing files otherwise. If you want to make sure that the public files directory does not contain anything but the newly imported files, you will want to empty the directory before copying the new files in.

    ```
    docker compose exec drupal with-contenv bash -lc 'rm -r /var/www/drupal/web/sites/default/files/*'
    ```


## Drupal Private Files

These instructions can also be used to move your content from production to staging or development.


### Back Up

Drupal private files can be compressed to a tgz file in the Drupal container:

```
docker compose exec drupal tar zcvf /tmp/private-files.tgz -C /var/www/drupal/private .
```

Then copied to the host machine:

```
docker compose cp drupal:/tmp/private-files.tgz [path/on/host/private-files.tgz]
```


### Restore

Drupal files can be restored from a tgz file by copying them into the Drupal container:

```
docker compose cp private-files.tgz drupal:/tmp/private-files.tgz
```

Then placed in the correct directory inside the Drupal volume:

```
docker compose exec drupal tar zxvf /tmp/private-files.tgz -C /var/www/drupal/private
docker compose exec drupal chown -R nginx:nginx /var/www/drupal/private
docker compose exec drupal rm /tmp/private-files.tgz
```

!!! note
    This will overwrite existing files if they have the same filename, but does not remove existing files otherwise. If you want to make sure that the private files directory does not contain anything but the newly imported files, you will want to empty the directory before copying the new files in.

    ```
    docker compose exec drupal with-contenv bash -lc 'rm -r /var/www/drupal/private/*'
    ```


## Fedora

Fedora 6 uses a file structure called [OCFL](https://ocfl.io/) to store files and metadata. The Fedora database is built based on this OCFL file structure, so we don't actually back up our fedora database. Instead, we back up the files and let Fedora rebuild the database based on them.

!!! note
    These instructions involve moving the entirety of your Fedora data. For small sites this is fine, but you may find this becomes cumbersome as your site grows. You may wish to bind your oclf-root directory as a volume to eliminate the need to run the docker compose cp commands. 

### Back Up

To back up Fedora we create a .tgz file from the ocfl-root directory in the Fedora container:

```
docker compose exec fcrepo tar zcvf /tmp/fcrepo-export.tgz -C /data/home/data/ocfl-root/ .
```

Then we copy that file to the host machine:

```
docker compose cp fcrepo:/tmp/fcrepo-export.tgz [path/on/host/fcrepo-export.tgz]
```


### Restore

To restore our fedora database we need to copy the backup into the Fedora container:

```
docker compose cp [path-to/fcrepo-export.tgz] fcrepo:/tmp/fcrepo-export.tgz
```

Empty the existing ocfl-root directory:
```
docker compose exec fcrepo bash -lc 'rm -r /data/home/data/ocfl-root/*'
```

Put our backup files in the ocfl-root directory:

```
docker compose exec fcrepo tar zxvf /tmp/fcrepo-export.tgz -C /data/home/data/ocfl-root/
docker compose exec fcrepo chown -R tomcat:tomcat /data/home/data/ocfl-root/
docker compose exec fcrepo rm /tmp/fcrepo-export.tgz
```

Drop the existing Fedora database:

```
docker compose exec mariadb mariadb -e "drop database fcrepo;"
```

Restart Fedora to reinitialize the database from the ocfl-root directory:

```
docker compose restart fcrepo
```

!!! note

    It may take a while for the database to be restored. In the meantime, you may see error messages that say Fedora is not connected.

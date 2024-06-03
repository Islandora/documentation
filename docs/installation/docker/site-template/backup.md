# Backups & Transferring Data Between Sites

The following instructions describe how to back up and restore an islandora site. This is also used to transfer data between a development and production site, or to sync a staging site with a production site.

Note that containers are named with a suffix of -dev or -prod depending on your environment. For example, backing up your development site’s Drupal database requires running the command on the drupal-dev container, but backing up the production site’s database requires running the command on the drupal-prod container. The following instructions are mostly written for development sites, but the same commands will work on production containers.

Before attempting the following commands, you should familiarize yourself with [running commands inside a container](/documentation/installation/docker/site-template/containers.md) and with [docker compose cp](https://docs.docker.com/reference/cli/docker/compose/cp/)


## Drupal Configuration

The typical use case of this is to export your drupal configuration files out of your development site’s Drupal database and onto the host machine. This process lets you check your configuration into your git repository so you can import it into your production site.


### Export

To export your development config run:

```
docker compose exec -T drupal-dev drush -l default config:export -y
```

Then commit and push your git repo so the new config files are included.


### Import

To import your config to a production or staging site you first need to `git pull` the config files you pushed from your development site, then rebuild and restart your custom Drupal container.

```
git pull
docker compose --profile prod build
docker compose --profile prod down
docker compose --profile prod up -d
```

Once this is done, your config files will be included in the container. You can then import them by running

```
docker compose exec -T drupal-prod drush -l default config:import -y
```


## Drupal Database

The Drupal database contains all your Drupal settings and content. If you want to move configuration settings from development to production you should use the Drupal Configuration instructions above.

These instructions can also be used to move your content from production to staging or development.


### Back Up

Backing up the Drupal database involves doing a database dump in the drupal container:

```
docker compose exec -T drupal-dev with-contenv bash -lc 'mysqldump -u ${DRUPAL_DEFAULT_DB_USER} -p${DRUPAL_DEFAULT_DB_PASSWORD} -h mariadb ${DRUPAL_DEFAULT_DB_NAME} > /tmp/dump.sql'
```

and then copying that dump from the container to the host machine:

```
docker compose cp drupal-dev:/tmp/dump.sql [path/on/host/dump.sql]
```


### Restore

Restoring from a database dump requires copying the dump file into the drupal container:

```
docker compose cp [path-to/dump.sql] drupal-dev:/tmp/dump.sql
```

Then replacing the database with the dump:

```
docker compose exec -T drupal-dev with-contenv bash -lc 'chown root:root /tmp/dump.sql && mysql -u ${DRUPAL_DEFAULT_DB_USER} -p${DRUPAL_DEFAULT_DB_PASSWORD} -h mariadb ${DRUPAL_DEFAULT_DB_NAME} < /tmp/dump.sql'
```

And finally, rebuilding the Drupal cache:

```
docker compose exec drupal-dev drush cr
```


## Drupal Public Files

Drupal's public files contain any files used on static pages. This is also where your Islandora derivatives are stored, which includes FITS, thumbnails, etc.

These instructions can also be used to move your content from production to staging or development.


### Back Up

Drupal public files can be compressed to a tgz file in the Drupal container:

```
docker compose exec -T drupal-dev with-contenv bash -lc 'tar zcvf /tmp/public-files.tgz -C /var/www/drupal/web/sites/default/files .'
```

Then copied to the host machine:

```
docker compose cp drupal-dev:/tmp/public-files.tgz  [path/on/host/public-files.tgz]
```


### Restore

Drupal files can be restored from a tgz file by copying them into the Drupal container:

```
docker compose cp public-files.tgz drupal-dev:/tmp/public-files.tgz
```

Then placed in the correct directory inside the Drupal volume:

```
docker compose exec -T drupal-dev with-contenv bash -lc 'tar zxvf /tmp/public-files.tgz -C /var/www/drupal/web/sites/default/files && chown -R nginx:nginx /var/www/drupal/web/sites/default/files && rm /tmp/public-files.tgz'
```

!!! note
    This will overwrite existing files if they have the same filename, but does not remove existing files otherwise. If you want to make sure that the public files directory does not contain anything but the newly imported files, you will want to empty the directory before copying the new files in.
    
    ```
    docker compose exec -T drupal-dev with-contenv bash -lc 'rm -r /var/www/drupal/web/sites/default/files/*'
    ```


## Drupal Private Files

These instructions can also be used to move your content from production to staging or development.



### Back Up

Drupal public files can be compressed to a tgz file in the Drupal container:

```
docker compose exec -T drupal-dev with-contenv bash -lc 'tar zcvf /tmp/private-files.tgz -C /var/www/drupal/private .'
```

Then copied to the host machine:

```
docker compose cp drupal-prod:/tmp/private-files.tgz [path/on/host/private-files.tgz]
```


### Restore

Drupal files can be restored from a tgz file by copying them into the Drupal container:

```
docker compose cp private-files.tgz drupal-dev:/tmp/private-files.tgz
```

Then placed in the correct directory inside the Drupal volume:

```
docker compose exec -T drupal-dev with-contenv bash -lc 'tar zxvf /tmp/private-files.tgz -C /var/www/drupal/private && chown -R nginx:nginx /var/www/drupal/private && rm /tmp/private-files.tgz'
```

!!! note
    This will overwrite existing files if they have the same filename, but does not remove existing files otherwise. If you want to make sure that the private files directory does not contain anything but the newly imported files, you will want to empty the directory before copying the new files in.
    
    ```
    docker compose exec -T drupal-dev with-contenv bash -lc 'rm -r /var/www/drupal/private/*'
    ```


## Fedora

Fedora 6 uses a file structure called [OCFL](https://ocfl.io/) to store files and metadata. The Fedora database is built based on this OCFL file structure, so we don't actually back up our fedora database. Instead, we back up the files and let Fedora rebuild the database based on them.

!!! note
    These instructions involve moving the entirety of your Fedora data. For small sites this is fine, but you may find this becomes cumbersome as your site grows. You may wish to bind your oclf-root directory as a volume to eliminate the need to run the docker compose cp commands. 

### Back Up

To back up Fedora we create a .tgz file from the ocfl-root directory in the Fedora container:

```
docker compose exec -T fcrepo-dev with-contenv bash -lc 'tar zcvf /tmp/fcrepo-export.tgz -C /data/home/data/ocfl-root/ .'
```

Then we copy that file to the host machine:

```
docker compose cp fcrepo-dev:/tmp/fcrepo-export.tgz [path/on/host/fcrepo-export.tgz]
```


### Restore

To restore our fedora database we need to copy the backup into the Fedora container:

```
docker compose cp [path-to/fcrepo-export.tgz] fcrepo-dev:/tmp/fcrepo-export.tgz
```

Empty the existing ocfl-root directory:
```
docker compose exec -T fcrepo-dev bash -lc 'rm -r /data/home/data/ocfl-root/*'
```

Put our backup files in the ocfl-root directory:

```
docker compose exec -T fcrepo-dev with-contenv bash -lc 'tar zxvf /tmp/fcrepo-export.tgz -C /data/home/data/ocfl-root/ && chown -R tomcat:tomcat /data/home/data/ocfl-root/ && rm /tmp/fcrepo-export.tgz'
```

Drop the existing Fedora database:

```
docker compose exec -T mariadb-dev bash -lc 'mysql -e "drop database fcrepo;"'
```

Restart Fedora to reinitialize the database from the ocfl-root directory:

```
docker compose --profile dev restart fcrepo-dev
```

!!! note

    It may take a while for the database to be restored. In the meantime, you may see error messages that say Fedora is not connected.
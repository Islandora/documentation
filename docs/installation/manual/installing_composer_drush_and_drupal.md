# Installing Composer, Drush, and Drupal

!!! warning "Needs Maintenance"
    The manual installation documentation is in need of attention. We are aware that some components no longer work as documented here. If you are interested in helping us improve the documentation, please see [Contributing](../../../contributing/CONTRIBUTING).

## In this section, we will install:

- [Composer](https://getcomposer.org/) at its current latest version, the package manager that will allow us to install PHP applications
- Either the [Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/), or the [Drupal recommended-project](https://www.drupal.org/docs/develop/using-composer/starting-a-site-using-drupal-composer-project-templates#s-drupalrecommended-project), which will install, among other things:
    - [Drush 10](https://www.drush.org/) at its latest version, the command-line PHP application for running tasks in Drupal
    - [Drupal 9](https://www.drupal.org/) at its latest version, the content management system Islandora uses for content modelling and front-end display

## Install Composer

### Download and install Composer 2.x

Composer provides PHP code that we can use to install it. After downloading and running the installer, we’re going to move the generated executable to a place in `$PATH`, removing its extension:

```bash
curl "https://getcomposer.org/installer" > composer-install.php
chmod +x composer-install.php
php composer-install.php
sudo mv composer.phar /usr/local/bin/composer
```


## Download and Scaffold Drupal

At this point, you have the option of using the [Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/), with its pre-selected modules
and configurations which function "out of the box," or build a clean stock Drupal via the Drupal Recommended Project and install
Islandora modules as you desire.

### Option 1: Create a project using the Islandora Starter Site

Navigate to the folder where you want to put your Islandora project (in our case `/var/www`), and
create the Islandora Starter Site:

```bash
cd /var/www
composer create-project islandora/islandora-starter-site
```

This will install all PHP dependencies, including Drush, and scaffold the site.

Drush is not accessible via `$PATH`, but is available using the command `composer exec -- drush` 

### Option 2: Create a basic Drupal Recommended Project

Navigate to the folder where you want to put your Drupal project (in our case `/var/www`), and
create the Drupal Recommended Project:

```bash
cd /var/www
composer create-project drupal/recommended-project my-project
```


## Make the new webroot accessible in Apache

Before we can proceed with the actual site installation, we’re going to need to make our new Drupal installation the default web-accessible location Apache serves up. This will include an appropriate `ports.conf` file, and replacing the default enabled site.

!!! notice
    Out of the box, these files will contain support for SSL, which we will not be setting up in this guide (and therefore removing with these overwritten configurations), but which are **absolutely indispensible** to a production site. This guide does not recommend any particular SSL certificate authority or installation method, but you may find [DigitalOcean's tutorial](https://www.digitalocean.com/community/tutorials/how-to-install-an-ssl-certificate-from-a-commercial-certificate-authority) helpful.

`/etc/apache2/ports.conf | root:root/644`
```
Listen 80
```

Remove everything but the "Listen 80" line. You can leave the comments in if you want.

`/etc/apache2/sites-enabled/000-default.conf | root:root/777`
```xml
<VirtualHost *:80>
  ServerName SERVER_NAME
  DocumentRoot "/opt/drupal/web"
  <Directory "/opt/drupal/web">
    Options Indexes FollowSymLinks MultiViews
    AllowOverride all
    Require all granted
  </Directory>
  # Ensure some logging is in place.
  ErrorLog "/var/log/apache2/localhost_error.log"
  CustomLog "/var/log/apache2/localhost_access.log" combined
</VirtualHost>
```
- `SERVER_NAME`: `localhost`
    - For a development environment hosted on your own machine or a VM, `localhost` should suffice. Realistically, this should be the domain or IP address the server will be accessed at.

Restart the Apache 2 service to apply these changes:

```bash
sudo systemctl restart apache2
```
## Prepare the PostgreSQL database

PostgreSQL roles are directly tied to users. We’re going to ensure a user is in place, create a role for them in PostgreSQL, and create a database for them that we can use to install Drupal.

```bash
# Run psql as the postgres user, the only user currently with any PostgreSQL
# access.
sudo -u postgres psql
# Then, run these commands within psql itself:
create database DRUPAL_DB encoding 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8' TEMPLATE template0;
create user DRUPAL_DB_USER with encrypted password 'DRUPAL_DB_PASSWORD';
grant all privileges on database DRUPAL_DB to DRUPAL_DB_USER;
# Then, quit psql.
\q
```
- `DRUPAL_DB`: `drupal9`
    - This will be used as the core database that Drupal is installed into
- `DRUPAL_DB_USER`: `drupal`
    - Specifically, this is the user that will connect to the PostgreSQL database being created, not the user that will be logging into Drupal
- `DRUPAL_DB_PASSWORD`: `drupal`
    - This should be a secure password; it’s recommended to use a password generator to create this such as the one provided by [random.org](https://www.random.org/passwords/)


## Install Drupal using Drush

The Drupal installation process can be done through the GUI in a series of form steps, or can be done quickly using Drush's `site-install` command. It can be invoked with the full list of parameters (such as `--db-url` and `--site-name`), but if parameters are missing, they will be asked of you interactiveley.

### Option 1: Site install the Starter Site with existing configs

Follow the instructions in the [README of the Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/#usage).
The steps are not reproduced here to remove redundancy. When this installation is done, you'll have a starter site ready-to-go. Once you set up the external services in the next sections, you'll need to configure Drupal to know where they are.

### Option 2: Site install the basic Drupal Recommended Project

```bash
cd /var/www/drupal
drush -y site-install standard --db-url="pgsql://DRUPAL_DB_USER:DRUPAL_DB_PASSWORD@127.0.0.1:5432/DRUPAL_DB" --site-name="SITE_NAME" --account-name=DRUPAL_LOGIN --account-pass=DRUPAL_PASS
```
This uses the same parameters from the above step, as well as:

- `SITE_NAME`: Islandora 2.0
    - This is arbitrary, and is simply used to title the site on the home page
- `DRUPAL_LOGIN`: `islandora`
    - The Drupal administrative username to use
- `DRUPAL_PASS`: `islandora`
    - The password to use for the Drupal administrative user

Congratulations, you have a Drupal site! It currently isn’t really configured to do anything, but we’ll get those portions set up in the coming sections.

# Installing Composer, Drush, and Drupal

## In this section, we will install:

- [Composer](https://getcomposer.org/) at its current latest version, the package manager that will allow us to install PHP applications
- The Islandora fork of the composer installer from [drupal-composer/drupal-project](https://github.com/Islandora/drupal-project), which will install, among other things:
    - [Drush 10](https://www.drush.org/) at its latest version, the command-line PHP application for running tasks in Drupal
    - [Drupal 9](https://www.drupal.org/) at its latest version, the content management system Islandora uses for content modelling and front-end display

## Composer 2.x

### Download and install Composer

Composer provides PHP code that we can use to install it. After downloading and running the installer, we’re going to move the generated executable to a place in `$PATH`, removing its extension:

```bash
curl "https://getcomposer.org/installer" > composer-install.php
chmod +x composer-install.php
php composer-install.php
sudo mv composer.phar /usr/local/bin/composer
```

## Drush 10 and Drupal 9

### Clone `drupal-project` and install it via Composer

Before we can fully install Drupal, we’re going to need to clone `drupal-project` and provision it using Composer. We’re going to install it into the `/opt` directory:

```bash
# Start by giving Drupal somewhere to live. The Drupal project is installed to
# an existing, empty folder.
sudo mkdir /opt/drupal
sudo chown www-data:www-data /opt/drupal
sudo chmod 775 /opt/drupal
# Change the ownership of default Apache directory so Composer can access it
sudo chown -R www-data:www-data /var/www/
# Clone drupal-project and build it in our newly-created folder.
git clone https://github.com/drupal-composer/drupal-project.git
cd drupal-project
# Expect this to take a little while, as this is grabbing the entire
# requirements set for Drupal.
sudo -u www-data composer create-project drupal-composer/drupal-project:9.x-dev /opt/drupal --no-interaction
```

### Make Drush accessible in `$PATH`

While it’s not required for Drush to be accessible in `$PATH`, not needing to type out the full path to it every time we need to use it is going to be incredibly convenient for our purposes. The rest of this guide will assume that we can simply run Drush from the command line when necessary without having to reference the full path.

```bash
sudo ln -s /opt/drupal/vendor/drush/drush/drush /usr/local/bin/drush
```

### Make the new webroot accessible in Apache

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

### Prepare the PostgreSQL database

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

### Run the Drupal installer with Drush

The standard Drupal installation method involves navigating to your site’s front page and navigating through a series of form steps, but we can fast-track this using Drush’s `site-install` command.

```bash
# Rather than defining the root directory in our Drush command, we're going to
# do this from the site root context.
cd /opt/drupal/web
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

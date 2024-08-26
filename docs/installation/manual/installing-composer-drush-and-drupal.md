# Installing Composer, Drush, and Drupal

!!! warning "Needs Maintenance"
    The manual installation documentation is in need of attention. We are aware that some components no longer work as documented here. If you are interested in helping us improve the documentation, please see [Contributing](../../contributing/CONTRIBUTING.md).

## In this section, we will install:

- [cURL](https://curl.se/) is used by composer to efficiently download libraries
- [Composer](https://getcomposer.org/) at its current latest version, the package manager that will allow us to install PHP applications
- Either the [Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/), or the [Drupal recommended-project](https://www.drupal.org/docs/develop/using-composer/starting-a-site-using-drupal-composer-project-templates#s-drupalrecommended-project), which will install, among other things:
    - [Drush](https://www.drush.org/) at its latest version, the command-line PHP application for running tasks in Drupal
    - [Drupal](https://www.drupal.org/) at its latest version, the content management system Islandora uses for content modelling and front-end display

## Install cURL

cURL may already be installed. Check by running `curl --version`. If it isn't, install it:

```bash
sudo apt install curl
```

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

On a default Ubuntu install the `/var/www` directory is owned by root, but we want the webserver to control this space, so we'll give it ownership:

```bash
sudo chown -R www-data: /var/www
```


### Option 1: Create a project using the Islandora Starter Site

Navigate to the folder where you want to put your Islandora project (in our case `/var/www/html`). You can give your site any name you like, but for these instructions we will simply call it "drupal":

```bash
cd /var/www/html
sudo -u www-data mkdir drupal
sudo -u www-data composer create-project islandora/islandora-starter-site drupal
```

This will install all PHP dependencies, including Drush, and scaffold the site.

Drush is not accessible via `$PATH`, but is available using the command `composer exec -- drush` 

### Option 2: Create a basic Drupal Recommended Project

Navigate to the folder where you want to put your Drupal project (in our case `/var/www/html`), and
create the Drupal Recommended Project:

```bash
cd /var/www/html
sudo -u www-data composer create-project drupal/recommended-project drupal
```


## Make the new webroot accessible in Apache

Before we can proceed with the actual site installation, we’re going to need to make our new Drupal installation the default web-accessible location Apache serves up. This will include an appropriate `ports.conf` file, and replacing the default enabled site.

!!! notice
    Out of the box, these files will contain support for SSL, which we will not be setting up in this guide (and therefore removing with these overwritten configurations), but which are **absolutely indispensable** to a production site. This guide does not recommend any particular SSL certificate authority or installation method, but you may find [DigitalOcean's tutorial](https://www.digitalocean.com/community/tutorials/how-to-install-an-ssl-certificate-from-a-commercial-certificate-authority) helpful.

`/etc/apache2/ports.conf | root:root/644`
```
Listen 80
```

Remove everything but the "Listen 80" line. You can leave the comments in if you want.

Create a drupal virtual host:
`/etc/apache2/sites-available/islandora.conf | root:root/644`
```xml
<VirtualHost *:80>
  ServerName SERVER_NAME
  DocumentRoot "/var/www/html/drupal/web"
  <Directory "/var/www/html/drupal/web">
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

Set permissions and enable the virtual host:

```bash
sudo systemctl restart apache2
sudo a2ensite islandora.conf
sudo systemctl reload apache2
```
## Prepare the MySQL database

We're going to create a user in MySQL for this Drupal site. Then create a database that we can use to install Drupal.

The following values can (and in the case of the password, *should*) be changed to local values.

- `DRUPAL_DATABASE_NAME`: This will be used as the core database that Drupal is installed into
- `MYSQL_USER_FOR_DRUPAL`: Specifically, this is the user that will connect to the MySQL database being created, not the user that will be logging into Drupal
- `MYSQL_PASSWORD_FOR_DRUPAL`:  This should be a secure password; it’s recommended to use a password generator to create this such as the one provided by [random.org](https://www.random.org/passwords/)

```bash
sudo mysql -u root
CREATE DATABASE [DRUPAL_DATABASE_NAME];
CREATE USER '[MYSQL_USER_FOR_DRUPAL]'@'localhost' IDENTIFIED BY '[MYSQL_PASSWORD_FOR_DRUPAL]';
GRANT ALL PRIVILEGES ON [DRUPAL_DATABASE_NAME].* TO '[MYSQL_USER_FOR_DRUPAL]'@'localhost';
exit
```

## Install Drupal using Drush

The Drupal installation process can be done through the [GUI](../../user-documentation/glossary.md#gui) in a series of form steps, or can be done quickly using Drush's `site-install` command. It can be invoked with the full list of parameters (such as `--db-url` and `--site-name`), but if parameters are missing, they will be asked of you interactively.

### Option 1: Site install the Starter Site with existing configs

Follow the instructions in the [README of the Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/#usage).
The steps are not reproduced here to remove redundancy. But specifically,

1. Configure the database connection information (see the section above) and fedora flysystem in `/var/www/html/drupal/web/sites/default/settings.php`.
2. Install the site using `sudo -u www-data composer exec -- drush site:install --existing-config`.

When this installation is done, you'll have a starter site ready-to-go. Once you set up the external services in the next sections, you'll need to configure Drupal to know where they are.

### Option 2: Site install the basic Drupal Recommended Project

```bash
cd /var/www/drupal
sudo -u www-data drush -y site-install standard --db-url="mysql://MYSQL_USER_FOR_DRUPAL:MYSQL_PASSWORD_FOR_DRUPAL@127.0.0.1:3306/DRUPAL_DATABASE_NAME" --site-name="SITE_NAME" --account-name=DRUPAL_LOGIN --account-pass=DRUPAL_PASS
```
This uses the same parameters from the above step, as well as:

- `SITE_NAME`: Islandora 2.0
    - This is arbitrary, and is simply used to title the site on the home page
- `DRUPAL_LOGIN`: `islandora`
    - The Drupal administrative username to use
- `DRUPAL_PASS`: `islandora`
    - The password to use for the Drupal administrative user

Congratulations, you have a Drupal site! It currently isn’t really configured to do anything, but we’ll get those portions set up in the coming sections.

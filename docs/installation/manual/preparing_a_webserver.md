# Preparing a LAPP Server

## In this section, we will install:

- [Apache 2](https://httpd.apache.org/), the webserver that will deliver webpages to end users
- [PHP 7](https://www.php.net/), the runtime code interpreter that Drupal will use to generate webpages and other services via apache, as well as that Drush and Composer will use to run tasks from the command line
- Several modules for PHP 7 which are required to run the PHP code that Drupal and other applications will be executing
- [PostgreSQL 10](https://www.postgresql.org/), the database that Drupal will use for storage (as well as other applications down the line)

## Apache 2

### Install Apache 2

Apache can typically be installed and configured outright by your operating system’s package manager:

```bash
sudo apt-get -y install apache2 apache2-utils
```

This will install:

- A `systemd` service that will ensure Apache can be stopped and started, and will run when the machine is powered on
- A set of Apache configurations in `/etc/apache2`, including the basic configuration, ports configuration, enabled mods, and enabled sites
- An Apache webroot in `/var/www/html`, configured to be the provided server on port `:80` in `/etc/apache2/sites-enabled/000-default.conf`; we’ll make changes and additions to this file later
- A user and group, `www-data`, which we will use to read/write web documents.

### Enable Apache Mods

We’re going to enable a couple of Apache mods that Drupal highly recommends installing, and which are de-facto considered required by Islandora:

```bash
sudo a2enmod ssl
sudo a2enmod rewrite
sudo systemctl restart apache2
```

### Add the Current User to the `www-data` Group

Since the user we are currently logged in as is going to work quite a bit inside the Drupal directory, we want to give it group permissions to anything the `www-data` group has access to.

**N.B.** This code block uses **backticks**, not single quotes; this is an important distinction as backticks have special meaning in `bash`.

```bash
sudo usermod -a -G www-data `whoami`
# Immediately log back in to apply the new group.
sudo su `whoami`
```

## PHP 7.2

### Install PHP 7.2

PHP can generally be easily installed using your operating system’s package manager, though whether or not the version you’ll be given is up to date depends entirely on whether or not that package manager is kept up-to-date. We’re going to enable both PHP 7.2, as well as the myriad modules we require, simultaneously:

```bash
sudo apt-get -y install php7.2 php7.2-cli php7.2-common php7.2-curl php7.2-dev php7.2-gd php7.2-imap php7.2-json php7.2-mbstring php7.2-opcache php7.2-xml php7.2-yaml php7.2-zip libapache2-mod-php7.2 php-pgsql php-redis php-xdebug unzip
```

This will install a series of PHP configurations and mods in `/etc/php/7.2`, including:

- A `mods-available` folder (from which everything is typically enabled by default)
- A configuration for PHP when run from Apache in the `apache2` folder
- A configuration for PHP when run from the command line - including when run via Drush - in the `cli` folder
- `unzip`, which is important for PHP’s zip module to function correctly despite it not being a direct dependency of the module. We will also need to unzip some things later, so this is convenient to have in place early in the installation process.

## PostgreSQL 10

### Install PostgreSQL 10

PostgreSQL can generally be easily installed using your operating system’s package manager. It is typically sensible to install the version the system recognizes as up-to-date; Ubuntu 18.04 sees this as version 10. We’re simply going to install the database software:

```bash
sudo apt-get -y install postgresql
```

This will install:

- A user at the system level named `postgres`; this will be the only user, by default, that has permission to run the `psql` binary and have access to Postgres configurations
- A binary executable at `/usr/bin/psql`, which anyone - even `root` - will get kicked out of the moment they run it, since only the `postgres` user has permission to run any Postgres commands
- A series of configurations that live in `/etc/postgresql/10/main` which can be used to modify how PostgreSQL works.

### Configure Postgresql 10 For Use With Drupal

A modification needs to be made to the PostgreSQL configuration in order for Drupal to properly install and function. This change can be made to the main configuration file at `/etc/postgresql/10/main/postgresql.conf`:

**Before**:
> 558 | #bytea_output = ‘hex’                      # hex, escape 

**After**:
> 558 | bytea_output = ‘escape’

The `postgresql` service should be restarted to accept the new configuration:

```bash
sudo systemctl restart postgresql
```

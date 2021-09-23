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

Since the user we are currently logged in as is going to work quite a bit inside the Drupal directory, we want to give it group permissions to anything the `www-data` group has access to. When we run `composer`, `www-data` will also be caching data in our own home directory, so we want this group modification to go in both directions.

**N.B.** This code block uses **backticks**, not single quotes; this is an important distinction as backticks have special meaning in `bash`.

**Note** If doing this in the terminal, replace "whoami" with your username and remove the backticks

```bash
sudo usermod -a -G www-data `whoami`
sudo usermod -a -G `whoami` www-data
# Immediately log back in to apply the new group.
sudo su `whoami`
```

## PHP 7.4

### Install PHP 7.4

Islandora defaults, a module which will install Islandora at the end, requires PHP 7.4. If you're running Debian 11 you should be able to install PHP 7.4 from the apt packages directly:

```bash
sudo apt-get -y install php7.4 php7.4-cli php7.4-common php7.4-curl php7.4-dev php7.4-gd php7.4-imap php7.4-json php7.4-mbstring php7.4-opcache php7.4-xml php7.4-yaml php7.4-zip libapache2-mod-php7.4 php-pgsql php-redis php-xdebug unzip
```

If you're running Debian 10, the repository for the PHP 7.4 packages needs to be installed first:

```bash
sudo apt-get -y install lsb-release apt-transport-https ca-certificates
sudo wget -O /etc/apt/trusted.gpg.d/php.gpg https://packages.sury.org/php/apt.gpg
echo "deb https://packages.sury.org/php/ $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/php.list
sudo apt-get update
sudo apt-get -y install php7.4 php7.4-cli php7.4-common php7.4-curl php7.4-dev php7.4-gd php7.4-imap php7.4-json php7.4-mbstring php7.4-opcache php7.4-xml php7.4-yaml php7.4-zip libapache2-mod-php7.4 php-pgsql php-redis php-xdebug unzip
```

This will install a series of PHP configurations and mods in `/etc/php/7.4`, including:

- A `mods-available` folder (from which everything is typically enabled by default)
- A configuration for PHP when run from Apache in the `apache2` folder
- A configuration for PHP when run from the command line - including when run via Drush - in the `cli` folder
- `unzip`, which is important for PHP’s zip module to function correctly despite it not being a direct dependency of the module. We will also need to unzip some things later, so this is convenient to have in place early in the installation process.

## PostgreSQL 11

### Install PostgreSQL 11

PostgreSQL can generally be easily installed using your operating system’s package manager. It is typically sensible to install the version the system recognizes as up-to-date. We’re simply going to install the database software:

```bash
sudo apt-get -y install postgresql
```

This will install:

- A user at the system level named `postgres`; this will be the only user, by default, that has permission to run the `psql` binary and have access to Postgres configurations
- A binary executable at `/usr/bin/psql`, which anyone - even `root` - will get kicked out of the moment they run it, since only the `postgres` user has permission to run any Postgres commands
- A series of configurations that live in `/etc/postgresql/11/main` which can be used to modify how PostgreSQL works.

### Configure Postgresql 11 For Use With Drupal

A modification needs to be made to the PostgreSQL configuration in order for Drupal to properly install and function. This change can be made to the main configuration file at `/etc/postgresql/11/main/postgresql.conf`:

**Before**:
> 558 | #bytea_output = ‘hex’                      # hex, escape 

**After**:
> 558 | bytea_output = ‘escape’

(Remove the "# hex, escape" comment and change the value from "hex" to "escape")

The `postgresql` service should be restarted to accept the new configuration:

```bash
sudo systemctl restart postgresql
```

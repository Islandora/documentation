# Preparing a LAPP Server

!!! warning "Needs Maintenance"
    The manual installation documentation is in need of attention. We are aware that some components no longer work as documented here. If you are interested in helping us improve the documentation, please see [Contributing](../../../contributing/CONTRIBUTING).

## In this section, we will install:

- [Java/OpenJDK](https://openjdk.org/) is the Java runtime environment used by multiple components: Solr, Cantaloupe, Alpaca, Fedora, and Blazegraph.
- [Apache 2](https://httpd.apache.org/), the webserver that will deliver webpages to end users
- [PHP 8](https://www.php.net/), the runtime code interpreter that Drupal will use to generate webpages and other services via apache, as well as that Drush and Composer will use to run tasks from the command line
- Several modules for PHP 8 which are required to run the PHP code that Drupal and other applications will be executing
- [MySQL](https://www.mysql.com/), the database that Drupal will use for storage (as well as other applications down the line)

## Installing OpenJDK 11

Tomcat runs in a Java runtime environment, so we'll need one to continue. In our case, OpenJDK 11 is open-source, free to use, and can fairly simply be installed using `apt-get`:

```bash
sudo apt-get -y install openjdk-11-jdk openjdk-11-jre
```

The installation of OpenJDK via `apt-get` establishes it as the de-facto Java runtime environment to be used on the system, so no further configuration is required.

The resultant location of the java JRE binary (and therefore, the correct value of `JAVA_HOME` when it’s referenced) will vary based on the specifics of the machine it’s being installed on; that being said, you can find its exact location using `update-alternatives`:

```bash
update-alternatives --list java
```
Take a note of this path as we will need it later.

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

## PHP 8.x

!! note "Installing Alternate Versions"
   Although the instructions below will install PHP 8.3, the instructions should work for future versions by replacing the `8.3` with whatever version you are attempting to install.

### Install PHP 8.x

If you're running Ununtu 20.04+ you should be able to install PHP 8 from the apt packages directly, although the `ondrej/php` repository provides additional libraries:

```bash
sudo apt update
sudo add-apt-repository ppa:ondrej/php
sudo apt update
sudo apt install php8.3 libapache2-mod-php unzip
sudo apt install php8.3-{cli,common,curl,gd,imap,intl,mysql,opcache,redis,xdebug,xml,yaml,zip}
```

Restart Apache to make the changes active:

```bash
sudo systemctl restart apache2
```

Installation directories created: 

- `/etc/php/8.3` (this is where you can edit PHP settings, such as timeouts, as needed for your site)
- `/usr/bin/php8.3`


## MySQL

### Install

```bash
sudo apt install mysql-server
```

There are a few ways to check the MySQL status:

```bash
sudo service mysql status  # press "q" to exit
sudo ss -tap | grep mysql
sudo service mysql restart
sudo journalctl -u mysql   # helps troubleshooting
```
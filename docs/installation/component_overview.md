# Component Overview

A functioning Islandora Stack is made up of dozens of components working in synchronization with each other to store information in your repository, manage that information, and disseminate it intelligently to users. Whether running an installation using the provided Ansible playbook or installing the stack manually, it may be helpful to have a brief overview of all the components we're going to need, in the order we're going to install them, as well as a brief introduction to each component's installation and configuration process.

This list includes four different kinds of components:

- Components which are hard-required (such as Drupal and the Islandora module)
- Components for which defaults are provided but which can be swapped out (such as the software managing databases, or the repository's storage system)
- Components that can't easily be swapped out but are not necessarily required (such as using Solr as the site's internal search engine)
- Components which do not have official alternatives and are not necessarily required, but will likely exist on the vast majority of Islandora installations (such as Alpaca and Crayfish)

## The Webserver Stack - Apache, PHP, and MySQL/PostgreSQL

Combined together, Apache, PHP, and MySQL/PostgreSQL comprise a LAMP or LAPP server used to provide end-user-facing components - namely, the website.

**Apache** is the webserver that will serve up webpages to the public. It will also manage some internal funcionality provided by Crayfish, and will expose Cantaloupe to the public. We’ll be making changes to the VirtualHost entry, enabling some modules, and modifying the ports configuration. The VirtualHost entry will eventually be modified when we need to expose other services like Cantaloupe to the public.

**PHP** is the runtime interpreter for all the code Drupal and Crayfish need to be processed. By default, installing PHP 7.2 will give us a command-line interpreter, as well as an interpreter for Apache. We’re going to install several PHP modules required and/or useful for the components that make use of PHP.

**MySQL** and **PostgreSQL** are database management systems that we will use to store information for many different components like Drupal and Fedora. By default, the Ansible playbook installs MySQL, though this can be switched to PostgreSQL. The manual installation guide recommends and walks through installing and using PostgreSQL.

## The Front-Facing CDM - Composer, Drush, and Drupal

Composer will be used to install both Drupal and Drush simultaneously using Islandora's fork of the [drupal-project](https://github.com/Islandora/drupal-project) repository.

**Composer** is an installer and dependency manager for PHP projects. We're going to need it to install components for any PHP code we need to make use of, including Drupal and Crayfish.

**Drush** and **Drupal** are installed simultaneously using [drupal-project](https://github.com/Islandora/drupal-project). Drupal will serve up webpages and manage Islandora content, and Drush will help us get some things done from the command-line.

## The Web Application Server - Tomcat and Cantaloupe

Several applets will be deployed via their `.war` files into Tomcat, including Fedora and Cantaloupe.

**Tomcat** serves up webpages and other kinds of content much like Apache, but is specifically designed to deploy Java applications as opposed to running PHP code.

**Cantaloupe** is an image tileserver that Islandora will connect to and use to serve up extremely large images in a way that doesn't have an adverse effect on the overall system.

## The Back-End File Management Repository - Fedora, Syn, and Blazegraph

Fedora will be installed in its own section, rather than as part of the Tomcat installation, as the installation process is rather involved and requires some authorization pieces to be set up in order to connect them back to Drupal and other components.

**Fedora** is the default backend repository that Islandora content will be synchronized with and stored in. A great deal of configuration will be required to get it up and running, including ensuring a database is created and accessible.

**Syn** is the authorization piece that allows Fedora to connect to other components.

**Blazegraph** will store representative graph data about the repository that can be queried using SPARQL. Some configuration will also be required to link it back to Fedora, as well as to ensure it is being properly indexed.

## The Search Engine - Solr and search_api_solr

The installation of Solr itself is rather straightforward, but a configuration will have to be generated and applied from the Drupal side.

**Solr** will be installed as a standalone application. Nothing of particular importance needs to happen here; the configuration will be applied when `search_api_solr` is installed.

**search_api_solr** is the Drupal module that implements the Solr API for Drupal-side searches. After installing and configuring the module, the `drush solr-gsc` command will be used to generate Solr configs, and these configs will be moved to the Solr configuration location.

## The Asynchronous Background Services - Crayfish

**Crayfish** is a series of microservices that perform different asynchronous tasks kicked off by Islandora. It contains a series of submodules that will be installed via Composer. Later, these configured components will be connected to Alpaca.

## The Broker Connecting Everything - Karaf and Alpaca

**Karaf**’s job is similar to Tomcat, except where Tomcat is a web-accessible endpoint for Java applets, Karaf is simply meant to be a container for system-level applets to communicate via its OSGI. Alpaca is one such applet; it will broker messages between Fedora and Drupal, and between Drupal and various derivative generation applications.

**Alpaca** contains Karaf services to manage moving information between Islandora, Fedora, and Blazegraph as well as kicking off derivative services in Crayfish. These will be configured to broker between Drupal and Fedora using an ActiveMQ queue.

## Finalized Drupal Configurations

**Drupal configuration** exists as a series of .yaml files that can either be created in a feature, or exported from Drupal using the `content_sync` module. It can also be manually entered in via the UI. We're going to place configuration in a few different ways; Some content will be synchronized onto the site, and some core configurations from the main Islandora module will need to be run in order to facilitate ingest.

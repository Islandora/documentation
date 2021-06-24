# Install Islandora on Docker (ISLE)

## Overview

This page describes using ISLE-DC to launch Islandora as a suite of Docker Containers (ISLE). This is an alternative to the [Islandora Ansible Playbook](playbook.md), which creates the Islandora stack on a single virtual machine. 

### What is ISLE?

ISLE, or ISLandora Enterprise, is a community initiative to ease the installation and maintenance of Islandora by using Docker. It was an initiative of the Islandora Collaboration Group, first using Islandora 7.x, and later with Islandora 8.x. The code for the ISLE 8 (sometimes just ISLE) is now under the purview of the Islandora Foundation. ISLE may also refer to the [ISLE system for Islandora 7](https://islandora-collaboration-group.github.io/ISLE/).

### Why use Docker?

[Docker](https://www.docker.com/) is a way to separate out the "state" of your site (i.e. all the content, files, and configurations that you've entered) from the underlying software that runs it (e.g. webserver, database engine, etc). This allows for easier upgrades, faster development, and more flexible deployment.

### Where is ISLE?

ISLE is a suite of Docker containers that run the various components of Islandora: drupal, fedora, solr, alpaca, crayfish, matomo, etc. The individual containers are created (and automatically pushed to [Docker Hub](https://hub.docker.com/u/islandora)) by [ISLE BuildKit](https://github.com/Islandora-Devops/isle-buildkit).

### How do I install Islandora using ISLE?

To deploy the containers provided from Dockerhub using `docker-compose`, use [isle-dc](https://github.com/Islandora-Devops/isle-dc). The following instructions detail how to install all requirements and use `isle-dc`.

## Requirements

- Docker 19.x+
- GNU Make 4.0+
- Git 2.0+
- At least 8GB of RAM (ideally 16GB)
- An administrator account
- (Mac OS) Apple Developer Tools
- (Windows) The following setup has been tested:
    - Windows 10
    - [Windows Subsystem for Linux v. 2 (WSL 2)](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
    - Ubuntu 20.04 running on WSL 2
    - GNU make, run `sudo apt update` and `sudo apt install make` to install
    - Docker Desktop for Windows, using the WSL 2 based engine (Settings > General) and with the WSL integration active for Ubuntu (Settings > Resources > WSL integration) 

!!! Note "What are we missing?"
    Are you, or your computer, new to spinning up development sandboxes? Do you have wisdom about installing make on Windows? We suspect these requirements may be incomplete and your experience would be appreciated at [Issue #1640](https://github.com/Islandora/documentation/issues/1640).

### Installing Docker

To see if you have Docker installed, type `docker --version` in a terminal.

If you need to install Docker, we recommend using the application [Docker Desktop](https://www.docker.com/products/docker-desktop). It provides a GUI for managing Docker container in Windows and MacOS, along with the Docker engine and suite of command-line tools. Linux users don't get a desktop client, but can download the Engine and command-line tools from that same link.

[Download Docker](https://www.docker.com/products/docker-desktop)

!!! Warning "Memory, Processors, and Swap Requirements"
    To run ISLE on Docker Desktop, you must increase the resources allocated to the software. See Docker docs on [setting resources on Windows](https://docs.docker.com/docker-for-windows/#resources) (see note on how to allocate/restrict memory when using WSL 2) or [setting resources on Mac](https://docs.docker.com/docker-for-mac/#resources).

    **CPUs (processors)**: The CPUs allowed to Docker Desktop are still shared with the host machine, so increasing this to the maximum value should allow both the Docker containers and your host machine to run simultaneously.

    **Memory (RAM)**: This memory is completely dedicated to Docker while Docker Desktop is running, so do not allocate more than you can spare and still run your host machine. Windows users may not require as much memory for Docker as Mac users. Current suggestions for memory allocated to Docker Desktop are below, but please edit this document if you have new information.

    - Sandbox (`make demo`): 4GB
    - Development (`make local`): 8GB
    - Production or production-like development: 16GB

    **Swap**: Swap space is space borrowed from your hard disk drive to serve as makeshift RAM as needed. If you cannot provide as much RAM as you would like, increase this as is reasonable given your free disk space.

### Getting isle-dc

```bash
git clone https://github.com/islandora-devops/isle-dc
cd isle-dc
```

## Deploying Islandora 

From within the `isle-dc` directory, you will have `make` comands at your disposal for building and maintaining your repository. Configuration is contained in a `.env` file.

!!! Tip "Don't have a .env file?"
    If you don't have a `.env` file, copy the `sample.env` file and rename it to `.env`.

!!! Warning "Are you using a domain you own? *Read This First!*"
    By default, your site will be available at `islandora.traefik.me`, which resolves to `127.0.0.1`. If you want to make your site available using a domain you own, change the `DOMAIN` setting in your `.env` _before_ using any of the methods below. Since full URIs are used in Fedora and Blazegraph, changing your domain after the fact will require reindexing. See the [useful make commands](#useful-make-commands) section for commands to reindex your RDF metadata.   

### Demo

`make demo` will spin up a repository using ISLE that is exactly like future.islandora.ca. If you want to kick the tires and see what Islandora can do, use this.

!!! Warning "Demonstration Purposes Only!"
    As the name implies, `make demo` is for demonstration purposes only. If you want to add new modules and enable them, you will need to do so using `docker-compose exec drupal with-contenv bash -lc 'YOUR COMMAND'`. Both `drush` and `composer` can be used in this manner. 

### Local

`make local` creates an Islandora instance with a Drupal site from your computer.  This lets you use tools from your machine, such as an IDE or `composer` on your Drupal site, and have the results reflected automatically in your repository.

If you provide a Drupal site in the `codebase` folder before running the command, it will be used to build your Drupal site. You can use either an entire Drupal site using git, or at a minimum, just a `composer.json` file. If your Drupal site has had its configuration exported using `drush`, you'll need to configure `isle-dc` to use it by editing your `.env` file and setting

```
INSTALL_EXISTING_CONFIG=true
DRUPAL_INSTALL_PROFILE=minimal
```

If you already have a Drupal site but don't know how to export it,
log into your server, navigate to the Drupal root, and run the following commands:

- `drush config:export`
- `git init`
- `git add -A .`
- `git commit -m "First export of site"`

Then you can `git push` your site to Github and `git clone` it down whenever you want. For
example, from within your `isle-dc` directory:
    
```
git clone https://github.com/your_org/your_repo.git codebase
make local
```

If you do not provide a `codebase` folder before running `make local`, you will be given a vanilla Drupal 9 installation with the `islandora` module enabled and only the iminimal configuration required to run.  It's not for the faint of heart, but if you want to avoid using `islandora_defaults`, you can build up your entire repository this way.

### Troubleshooting 

## Evaluating your installation

### Visit your site

Direct a browser to [https://islandora.traefik.me/](https://islandora.traefik.me/) or whatever you set `DOMAIN` to in your `.env` file. Here are the default locations for all of the various services that are available:

| Service     | Url                                                                                            |
| :---------- | :--------------------------------------------------------------------------------------------- |
| Drupal      | [https://islandora.traefik.me](https://islandora.traefik.me)                                   |
| Traefik     | [https://islandora.traefik.me:8080](https://islandora.traefik.me:8080)                         |
| Fedora      | [https://islandora.traefik.me:8081/fcrepo/rest](https://islandora.traefik.me:8081/fcrepo/rest) |
| Blazegraph  | [https://islandora.traefik.me:8082/bigdata](https://islandora.traefik.me:8082/bigdata)         |
| Activemq    | [https://islandora.traefik.me:8161](https://islandora.traefik.me:8161)                         |
| Solr        | [https://islandora.traefik.me:8983](https://islandora.traefik.me:8983)                         |
| Cantaloupe  | [https://islandora.traefik.me/cantaloupe](https://islandora.traefik.me/cantaloupe)             |
| Matomo      | [https://islandora.traefik.me/matomo/](https://islandora.traefik.me/matomo/)                   |
| Code Editor | [https://islandora.traefik.me:8443/](https://islandora.traefik.me:8443/)                       |

If your containers are still "spinning up", you will see a white screen with the words "Bad Gateway". 

!["Bad Gateway" white screen while still loading](../assets/docker_bad_gateway_still_loading.png)

This often lasts 2-5 minutes, and should be shorter for subsequent launches. If it takes more than a few minutes, check to make sure that none of your containers have failed to launch using `docker ps -a`. When all containers are ready, you should see a basic Drupal login screen.

![Drupal login screen](../assets/docker_drupal_login_screen.png)

By default, the credentials to login are

* username: **admin**
* password: **password**

If one of your containers fails to launch, you'll need to check its logs using `docker-compose logs -tf SERVICENAME`.  For example, to see Drupal's logs, use `docker-compose logs -tf drupal`.  Usually, when you see `confd using 'env' backend`, the service has booted up successfully.  If you want to see all logs, just run `docker-compose logs`. 

### Shutting down your Islandora site

To shut down the containers without destroying your site, use `docker-compose down`. To also destroy your "state" (i.e. your content, your database, your files), use `docker-compose down -v`.

### Bringing your Islandora site back up

Use `docker-compose up -d` to bring a site back up. You should _not_ run `make local` or `make demo` again unless you used `-v` when shutting down the site.

## Useful Make Commands

There's a lot of other useful utilities available to you from within the `isle-dc` folder.

### Rebuild docker-compose.yml

`make -B docker-compose.yml` will rebuild your `docker-compose.yml` file based on configuration in your `.env` file.  `make demo` and `make local` will automatically do this for you, but if you change configuration at a later point in time, you'll need to run this command to see your changes reflected.

### Config Export

`make config-export` will export your site's configuration to your site's config sync directory (usually `config/sync` inside your Drupal root folder).

### Config Import

`make config-import` will import site's configuration from your site's config sync directory (usually `config/sync` inside your Drupal root folder).

### Database Dump

`make drupal-database-dump DEST=/your/path/dump.sql` will dump your Drupal database and place the file at `DEST`.

### Database Import

`make drupal-database-import SRC=/your/path/dump.sql` will import your Drupal database from the file at `SRC`.

### Export Public Files

`make drupal-public-files-dump DEST=/your/path/public_files.tgz` will export your public filesystem and place it as a single zipped tarball at `DEST`. 

### Import Public Files

`make drupal-public-files-import SRC=/your/path/public_files.tgz` will import your public filesystem from a single zipped tarball at `SRC`. 

### Fcrepo Export

`make fcrepo-export DEST=/your/path/fcrepo-export.tgz` will export your Fedora repository and place it as a single zipped tarball at `DEST`

### Fcrepo Import

`make fcrepo-import SRC=/your/path/fcrepo-export.tgz` will import your Fedora repository from a single zipped tarball at `SRC`

### Reindex Fedora Metadata
  
`make reindex-fcrepo-metadata` will reindex RDF metadata from Drupal into Fedora.

### Reindex Solr
`make reindex-solr` will rebuild rebuild Solr search index for your repository. 

### Reindex the Triplestore

`make reindex-triplestore` will reindex RDF metadata from Drupal into Blazegraph. 

## Developing with ISLE 

If you used `make local` then the drupal root folder is in a new directory in the isle-dc folder named `codebase`. This is live and editable in whatever development environment you would like.

### Embedded Editor

If you 

- Used `make demo`
- Are using a custom Drupal image
- Don't want to bother with setting up a development environment right now

You can use an embedded VSCode that's available at port 8443. To enable it

- Set `INCLUDE_CODE_SERVER_SERVICE=true` in your `.env` file
- make -B docker-compose.yml
- docker-compose up -d  

### Testing a Pull Request

Islandora modules in the `codebase/web/contrib/modules` folder are already set up with `git` and the `origin` remote is the canonical Islandora repository. You can follow the command-line instructions for testing pull requests available on Github. When finished, don't forget to `git checkout main` (or the default branch if not named main) so you can pull new code.

### Editing non-Drupal code

Editing code for Crayfish microservices or Alpaca isn't exposed yet as bind mounts.  For the time being, until a better solution arrives, we've found Visual Studio Code's Docker Extensions work great for developing directly on the running images.

## Updating ISLE

!!! Warning "Before you do anything, *READ THIS*"
    You should always, __always__ make backups before doing updates.  You can backup your fedora, drupal database, and drupal public filesystem using `isle-dc`.  You can also export your config, commit your Drupal site, and push it somewhere safe. See the [useful make commands](#useful-make-commands) for commands to do all these things.
 
### Updating your containers

- Edit your .env file and set the `TAG` property to the version you want to update to.  `TAG` can be either a named version (e.g. 1.0.0-alpha-2) or a commit hash from [isle-buildkit](https://github.com/Islandora-Devops/isle-buildkit).
- `make -B docker-compose.yml`
- `make pull`
- `docker-compose up -d`

### Updating Drupal

See our [updating Drupal](technical-documentation/updating_drupal/) page for more information on updating Drupal core as well as installing/updating new modules. If you used `make local`, you can run those instructions directly.  If you used `make demo` or are running a custom Drupal image, wrap the command in `docker-compose exec drupal with-contenv bash -lc 'COMMAND'`.  For example, `drush cr` becomes `docker-compose exec drupal with-contenv bash -lc 'drush cr'`.

### Updating isle-dc

Tagged releases of isle-dc are [available on Github](https://github.com/Islandora-Devops/isle-dc).

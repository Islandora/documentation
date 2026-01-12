# Prerequisites

## Software involved in ISLE

ISLE uses the following software in order to help you set up and run your site. If you are new to any of the following software, that's OK, but learning more about each piece will make it much easier to modify ISLE to suit your particular needs, and maintain your Islandora site.

The following pieces of software are not unique to Islandora, and there are many great resources online to learn more about them. Below is a brief introduction to each piece and how they fit into your Islandora site. There are also many great videos on the [Islandora Foundation's YouTube page](https://www.youtube.com/@islandorafoundation9224/videos) if you would like to learn more about how they are used within the context of Islandora.

### Git

Git is a version control system that is used to store the code for Islandora. When you first download ISLE you will be downloading it using Git. Git is also useful for managing your site's Drupal codebase, once you have a site up and running. 

More about using Git for Drupal sites can be found in the [Drupal documentation](https://www.drupal.org/docs/develop/git/setting-up-git-for-drupal).

### Docker

Docker allows us to run our Islandora sites in virtualized environments, called containers. Each container runs one part of the Islandora software, and it takes several containers to run a complete Islandora site. 

The three main components of Docker are Images, Containers, and Volumes. 

#### Images

Images are what our containers are built from. Images are read-only, and allow us to quickly create containers based on them.

The Islandora images are provided by [Islandora Buildkit](https://github.com/Islandora-Devops/isle-buildkit).

#### Containers

Containers are where the Islandora software actually runs. ISLE gives us tools to easily download the Islandora images and start our containers from them.

#### Volumes

Volumes are where our persistent storage lives. Because containers are often rebuilt from images, any data stored in them is easily lost. Volumes allow us to store specific parts of our containers that we don't want to lose. For example, our Drupal database will live in a volume so that it is not lost when a container is shut down.

### Docker Compose

Docker Compose is a tool to simplify the process of running multiple Docker containers for a single project. It uses a file called docker-compose.yml to store the settings for your project's containers. ISLE gives us tools to create this docker-compose.yml file, so we can use it to manage the containers for our site.

### GNU Make

Make allows us to define commands that simplify installing and maintaining our Islandora site. For a complete list of available commands see the Makefile included with ISLE Site Template.

### Composer

Composer is a dependency manager for PHP, and is the recommended way to install and update Drupal modules. Composer is installed in the Islandora Drupal container and should be run within the container to manage your Drupal site. More information on using Composer with Drupal is available in the [Drupal documentation](https://www.drupal.org/docs/develop/using-composer/manage-dependencies).

### Drupal

Drupal is a Content Management System used to create websites. During the setup of your Islandora site, a Drupal site is created for you in your Drupal container, by the commands in the Makefile. 

### Drush

Drush is a command line tool for managing your Drupal site. It comes installed in your Drupal container and allows you to perform Drupal actions from the command line inside your container.

## Requirements for using ISLE with Docker Compose

- Docker 19.x+
- Docker Compose version 2.x+
- GNU Make 4.0+
- Git 2.0+
- At least 8GB of RAM (ideally 16GB)
- An administrator account your machine (a.k.a. the host machine)
- (Mac OS) Apple Developer Tools
- (Windows) The following setup has been tested:
    - Windows 10
    - [Windows Subsystem for Linux v. 2 (WSL 2)](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
    - Ubuntu 20.04 running on WSL 2
    - GNU make, run `sudo apt update` and `sudo apt install make` to install
    - Docker Desktop for Windows, using the WSL 2 based engine (Settings > General) and with the WSL integration active for Ubuntu (Settings > Resources > WSL integration)

!!! Note "What are we missing?"
    Are you, or your computer, new to spinning up development sandboxes? Do you have wisdom about installing make on Windows? We suspect these requirements may be incomplete and your experience would be appreciated at [Issue #1640](https://github.com/Islandora/documentation/issues/1640).

## Installing Docker

To see if you have Docker installed, type `docker --version` in a terminal.

If you need to install Docker, we recommend using the application [Docker Desktop](https://www.docker.com/products/docker-desktop). It provides a GUI for managing Docker container in Windows and MacOS, along with the Docker engine and suite of command-line tools. Linux users don't get a desktop client, but can install the Engine and command-line tools the instructions [here](https://docs.docker.com/engine/install/).

!!! Warning "Memory, Processors, and Swap Requirements"
    To run ISLE on Docker Desktop, you must increase the resources allocated to the software. See Docker docs on [setting resources on Windows](https://docs.docker.com/docker-for-windows/#resources) (see note on how to allocate/restrict memory when using WSL 2) or [setting resources on Mac](https://docs.docker.com/docker-for-mac/#resources).

    **CPUs (processors)**: The CPUs allowed to Docker Desktop are still shared with the host machine, so increasing this to the maximum value should allow both the Docker containers and your host machine to run simultaneously.

    **Memory (RAM)**: This memory is completely dedicated to Docker while Docker Desktop is running, so do not allocate more than you can spare and still run your host machine. Windows users may not require as much memory for Docker as Mac users. Current suggestions for memory allocated to Docker Desktop are below, but please edit this document if you have new information.

    - Sandbox or development environment: 8GB
    - Production or production-like development: 16GB

    **Swap**: Swap space is space borrowed from your hard disk drive to serve as makeshift RAM as needed. If you cannot provide as much RAM as you would like, increase this as is reasonable given your free disk space.

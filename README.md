# Islandora [![Build Status](https://travis-ci.org/Islandora-CLAW/CLAW.png?branch=7.x-2.x)](https://travis-ci.org/Islandora-CLAW/CLAW)

## Introduction

[Islandora](http://islandora.ca) and [Fedora 4](http://fedorarepository.org/) Integration Project! 

This is where the Islandora and Fedora 4 development will happen. If you would like to get involved in the community around this project, please check out the Islandora Foundation [Fedora 4 Interest Group](https://github.com/Islandora/Islandora-Fedora4-Interest-Group).

For more information about the project, please see the [project documentation](http://islandora-labs.github.io/islandora/).

### Repository layout

* [camel](https://github.com/Islandora-Labs/islandora/tree/7.x-2.x/camel) - Middleware layer based on Apache Camel that synchronizes a Fedora 4 JCR with a Drupal CMS.
* [docs](https://github.com/Islandora-Labs/islandora/tree/7.x-2.x/docs) - Documentation
* [drupal](https://github.com/Islandora-Labs/islandora/tree/7.x-2.x/drupal) - Drupal modules
* [install](https://github.com/Islandora-Labs/islandora/tree/7.x-2.x/install) - Vagrant development environment


## Requirements

1. [VirtualBox](https://www.virtualbox.org/)
2. [Vagrant](http://www.vagrantup.com)
3. [git](https://git-scm.com/)

## Variables

### System Resources

By default the virtual machine that is built uses 2GB of RAM. Your host machine will need to be able to support the additional memory use. You can override the CPU and RAM allocation by creating `ISLANDORA_VAGRANT_CPUS` and `ISLANDORA_VAGRANT_MEMORY` environment variables and setting the values. For example, on an Ubuntu host you could add to `~/.bashrc`:

```bash
export ISLANDORA_VAGRANT_CPUS=4
export ISLANDORA_VAGRANT_MEMORY=4096
```

### Hostname and Description

If you use a DNS or host file management plugin with Vagrant, you may want to set a specific hostname for the virtual machine. You can do that with the `ISLANDORA_VAGRANT_HOSTNAME` variable.  The `ISLANDORA_VAGRANT_VIRTUALBOXDESCRIPTION` variables can be used to track the VM build. For example:

```bash
export ISLANDORA_VAGRANT_HOSTNAME="islandora-deux"
export ISLANDORA_VAGRANT_VIRTUALBOXDESCRIPTION="Islandora CLAW"
```

## Use

1. `git clone https://github.com/Islandora-CLAW/CLAW`
2. `cd CLAW`
3. `vagrant up`


## Maintainers/Sponsors

* UPEI
* discoverygarden inc.
* LYRASIS
* McMaster University
* University of Limerick
* York University
* University of Manitoba
* Simon Fraser University
* PALS
* American Philosophical Society
* common media inc.

Current maintainers:

* [Nick Ruest](https://github.com/ruebot)
* [Daniel Lamb](https://github.com/daniel-dgi/)

## Development

If you would like to contribute, please get involved with the [Islandora Fedora 4 Interest Group](https://github.com/Islandora/Islandora-Fedora4-Interest-Group). We love to hear from you!

If you would like to contribute code to the project, you need to be covered by an Islandora Foundation [Contributor License Agreement](http://islandora.ca/sites/default/files/islandora_cla.pdf) or [Corporate Contributor Licencse Agreement](http://islandora.ca/sites/default/files/islandora_ccla.pdf). Please see the [Contributors](http://islandora.ca/resources/contributors) pages on Islandora.ca for more information.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

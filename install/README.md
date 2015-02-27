## Introduction

The is a development environment virtual machine for the Islandora and Fedora 4 project. It should work on any operating system that supports VirtualBox and Vagrant.

N.B. This virtual machine **should not** be used in production.

## Requirements

1. [VirtualBox](https://www.virtualbox.org/)
2. [Vagrant](http://www.vagrantup.com/)

## Use

1. `git clone https://github.com/islandora-labs/islandora`
2. `cd islandora/install`
3. `vagrant up`

Optionally, if you want to install the vim configuration for Java development, use `VIM=true vagrant up`.  It practically doubles the amount of time to spin up the VM, so only do so if you’re sure you want to develop directly on the VM using vim.  The script merely checks to see if the shell variable has been set, so it’s value can be anything.  That is, `VIM=sandwiches vagrant up` will still trigger the vim configuration install.  So if you don’t want it, don’t set it.

## Connect

You can connect to the machine via the browser at [http://localhost:8000](http://localhost:8000).

The default Drupal login details are:
  
  * username: admin
  * password: islandora

MySQL:
  
  * username: root
  * password: islandora

The Fedora 4 REST API can be accessed at [http://localhost:8080/fcrepo/rest](http://localhost:8080/fcrepo/rest).  It currently has authentication disabled.

Tomcat Manager:
  
  * username: islandora
  * password: islandora

You can connect to the machine via ssh: `ssh -p 2222 vagrant@localhost`

The default VM login details are:
  
  * username: vagrant
  * password: vagrant

## Environment

- Ubuntu 14.04
- Drupal 7.34
- MySQL 5.5.41
- Apache 2.26
- Tomcat 7.0.52
- Solr 4.10.3
- Camel 2.14.1
- Fedora 4.1.0
- Fedora Camel Component 4.1.0
- Fuseki 1.1.1
- Sync 0.0.0
- Islandora 7.x-2.x
- PHP 5.5.9 
- Java 7 (OpenJDK)

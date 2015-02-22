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

## Connect

You can connect to the machine via the browser at [http://localhost:8000](http://localhost:8000).

The default Drupal login details are:
  - username: admin
  - password: islandora

You can connect to the machine via ssh: `ssh -p 2222 vagrant@localhost`

The default VM login details are:
  - username: vagrant
  - password: vagrant

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
- Islandora 7.x-2.x
- PHP 5.5.9 
- Java 7 (OpenJDK)

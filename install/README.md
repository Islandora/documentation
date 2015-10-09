## Introduction

The is a development environment virtual machine for the Islandora and Fedora 4 project. It should work on any operating system that supports VirtualBox and Vagrant.

N.B. This virtual machine **should not** be used in production.

## Requirements

1. [VirtualBox](https://www.virtualbox.org/)
2. [Vagrant](http://www.vagrantup.com/)

## Use

VirtualBox:

1. `git clone https://github.com/islandora-labs/islandora`
2. `cd islandora/install`
3. `vagrant up`

DigitalOcean:

1. `git clone https://github.com/islandora-labs/islandora`
2. `cd islandora/install`
3. `vagrant plugin install vagrant-digitalocean`
4. Set the following environment variables:
  * `DIGITALOCEAN_TOKEN` -- Your DigitalOcean API token
  * `DIGITALOCEAN_KEYNAME` -- Your DigitalOcean ssh key name
  * `DIGITALOCEAN_KEYPATH` -- Path to your ssh keys that you've setup with DigitalOcean
5. `vagrant up --provider=digital_ocean`

Amazon:

1. `git clone https://github.com/islandora-labs/islandora`
2. `cd islandora/install`
3. `vagrant plugin install vagrant-aws`
4. Set the following environment variables:
  * `AWS_KEY`
  * `AWS_SECRET`
  * `AWS_KEYNAME`
  * `AWS_KEYPATH`
5. `vagrant up --provider=aws`

N.B. You may not be able to connect to your AWS instance depending on VPC settings.

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
- Drupal 7.37
- MySQL 5.5.41
- Apache 2.26
- Tomcat 7.0.52
- Solr 4.10.3
- Camel 2.14.1
- Fedora 4.3.0
- Fedora Camel Component 4.2.0
- BlazeGraph 1.5.1
- Karaf 3.0.4
- Sync 0.0.0
- Islandora 7.x-2.x
- PHP 5.5.9 
- Java 8 (Oracle)

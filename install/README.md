## Introduction

The is a development environment virtual machine for the Islandora and Fedora 4 project. It should work on any operating system that supports VirtualBox and Vagrant.

N.B. This virtual machine **should not** be used in production.

## Requirements

1. [VirtualBox](https://www.virtualbox.org/)
2. [Vagrant](http://www.vagrantup.com/)
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

VirtualBox:

1. `git clone https://github.com/Islandora-CLAW/CLAW`
2. `cd CLAW`
2. `git submodule update --init --recursive`
3. `cd install`
4. `vagrant up`

DigitalOcean:

1. `git clone https://github.com/Islandora-CLAW/CLAW`
2. `cd CLAW`
3. `git submodule update --init --recursive`
4. `cd install`
5. `vagrant plugin install vagrant-digitalocean`
6. Set the following environment variables:
  * `DIGITALOCEAN_TOKEN` -- Your DigitalOcean API token
  * `DIGITALOCEAN_KEYNAME` -- Your DigitalOcean ssh key name
  * `DIGITALOCEAN_KEYPATH` -- Path to your ssh keys that you've setup with DigitalOcean
7. `vagrant up --provider=digital_ocean`

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

- Ubuntu 16.04.1
- Drupal 8.1.10
- MySQL 5.7.15
- Apache 2.4.18
- Tomcat 7.0.68.0
- Solr 4.10.3
- Camel 2.15.1
- Fedora 4.6.0
- Fedora Camel Component 4.4.0
- BlazeGraph 1.5.1
- Karaf 4.0.5
- Alpaca 0.0.1-SNAPSHOT
- Islandora 8.x-1.x
- Loris HEAD
- PHP 7.0.8
- Java 8 (Oracle)

## Windows Troubleshooting

If you receive errors involving `\r` (end of line) you have two options:

1. Clone down the current development branch using `--single-branch`.

  ```
  git clone --single-branch --branch sprint-002 git@github.com:Islandora-CLAW/CLAW.git <optional directory name>
  ```
  A benifit to this approach is that files created or edited on a Windows environment will be pushed back to your fork with appropriate `LF` endings.

2. Modify your global `.gitconfig` file to disable the Windows behavior of `autocrlf` entirely.

  Edit the global `.gitconfig` file, find the line:
  ```
  autocrlf = true
  ```
  and change it to
  ```
  autocrlf = false
  ```
  Remove and clone again. This will prevent Windows git clients from automatically replacing Unix line endings LF with Windows line endings CRLF.

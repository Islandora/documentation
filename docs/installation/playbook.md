# Islandora Playbook

The Islandora Playbook ([Islandora-Devops/islandora-playbook](https://github.com/Islandora-Devops/islandora-playbook)) is a tool for installing the Islandora stack on a single virtual machine. It can be used both as a [Vagrant](https://www.vagrantup.com/) project to create a local development environment, or as an [Ansible](https://www.ansible.com/community) playbook to provision a local or remote server. It can set up a Drupal based either on Islandora Starter Site, or on the Install Profile Demo.

## Basic Usage (Vagrant):

Install requirements (below), then:
```bash
$ git clone -b dev https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
```
First, create the islandora base box:
```bash
$ ISLANDORA_BUILD_BASE=true vagrant up
$ vagrant package --output islandora_base
$ vagrant destroy
```
Second, build a VM using the islandora base box:
```bash
$ vagrant up
```

When used this way, you can trash your working site and get a fresh Islandora relatively quickly, with `vagrant destroy` (you will be asked to confirm, as this will delete all your changes and your content), and then `vagrant up`. 

## Requirements (Vagrant)

To create a local VM, download and install the following.

1. [Virtual Box](https://www.virtualbox.org/)
2. [Vagrant](https://www.vagrantup.com/) (version 2.0 or higher required)
3. [Git](https://git-scm.com/)
4. [OpenSSL](https://www.openssl.org/)
5. [Ansible](https://www.ansible.com/community) (Tested on version 2.11+, versions back to 2.9 should work.)


### Installing Git and Ansible on MacOS

OpenSSL is already pre-installed on MacOS. Git can be installed using XCode's command line tools (see below). Python and Pip can either be installed via the downloaded installer direct from the site or via Homebrew (not shown below). Ansible is best installed using [Homebrew](https://brew.sh/) (see below).

```bash
# Use xcode-select to install command line components, including git
$ xcode-select --install
# Install homebrew
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
# Use homebrew to install ansible
$ brew install ansible
```

## Installing a local development environment (Vagrant)

You will use `vagrant up` twice, first to create an Islandora base box, and then again to provision that base box into a full Islanodra. This method uses Vagrant, VirtualBox, and Ansible.

!!! warn Make sure that no [required ports](#port-clashes-for-local-environments) are currently in use.

### Clone the playbook

```bash
$ git clone -b dev https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
```

### Create the Base Box

The Playbook will create a "base box" that includes core software (PHP, Apache, etc). Since these do not need to be updated too often, this can be packaged as a Vagrant base box that will be used to quickly provision the Islandora part.

Notes:
- If building a CentOS box, you also need to install the vbguest additions with `vagrant plugin install vagrant-vbguest`.
- If this is not your first time spinning up the Islandora Playbook from this directory, you may want to clear cached ansible roles in roles/external" `rm -rf roles/external`

First, create the islandora base box by overriding the Vagrant variable `ISLANDORA_BUILD_BASE` (which defaults to `false` in the `Vagrantfile`):
```bash
$ ISLANDORA_BUILD_BASE=true vagrant up
```

Once it is complete (without errors), store it as a Vagrant base box. It will appear as `islandora_base` in the current directory.
```bash
$ vagrant package --output islandora_base
```

Then, get rid of the existing VM since we've saved what we need:
```bash
$ vagrant destroy
     default: Are you sure you want to destroy the 'default' VM? [y/N] y
==> default: Forcing shutdown of VM...
==> default: Destroying VM and associated drives...

```

### Provision the Islandora code

Then, to install Islandora (including Drupal and its configuration, Crayfish, Alpaca, etc) on top of the new base box:
```bash
vagrant up
```

Access your site at `http://localhost:8000`.

### Rebuilding your Islandora development box

You can quickly create a new "out of the box" Islandora VM by just re-doing the last step (provisioning Islandora) while using your existing base box. In doing this, you will destroy your existing VM, and all of its configuration and data.  This will not upgrade PHP, apache, or other back-end services (see below, "Refreshing your base box.")

```bash
vagrant destroy
```
You will be asked to confirm, and Vagrant will inform you it is destroying the VM and associated drives. 
```bash
vagrant up
```
This will create a new development environment on the existing islandora_base box.

### Refreshing your base box. 

If you want to use the Playbook to rebuild the base box (e.g. if this playbook now includes updated versions of PHP or Apache), then you will need to take some extra steps to ensure that Vagrant sees your new base box. 

```bash
vagrant destroy  # Delete your existing VM
vagrant box remove islandora_base  # clear Vagrant's cached version of islandora_base
ISLANDORA_BUILD_BASE=true vagrant up # and proceed to package the box, as above.
```

This is because once you use the base box once, it is stored in the vagrant box list under your home directory. While subsequent builds of `ISLANDORA_BUILD_BASE=true vagrant up` will build a new box and you can package it, but that file does not update the cached box and consequently you still get the old base box when you try build a VM. See [`vagrant box` documentation](https://developer.hashicorp.com/vagrant/docs/cli/box) for more useful commands for managing base boxes.

## Deploying to a remote environment

The `ansible-playbook` command using `playbook.yml` can provision a remote environment. First, you will need to create a new folder in `inventory/` with the details of your remote deployment. This includes:

- Configuring the SSH parameters so Ansible can log in
- Changing usernames and passwords to something more sensible than the default
- Changing IP addresses to use the remote machine's IP
- Changing Apache to serve at port 80 (as opposed to 8000, which we use for development purposes)

Before beginning, you may want to:

- Create a non-root user on the remote machine. Ansible will need to "escalate" (i.e. `sudo`) as this user. See Ansible Docs on [Understanding privilege escalation: become](https://docs.ansible.com/ansible/latest/playbook_guide/playbooks_privilege_escalation.html)
- Ensure all ports except the Drupal port and ports used by Ansible are behind a firewall.

We're going to build up this new remote environment configuration from the default provided Vagrant configuration. To start, take the inventory for the `vagrant` development environment and make a copy of it. Be sure to give it an appropriate name. Here we're using `production`.

```bash
$ git clone https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
$ cp -r inventory/vagrant inventory/production
```

Then you can update the following entries in the following files using your own information. If an entry does not exist in a file,
just add it.  Ansible will then use the value you provide instead of relying on its defaults.

We're using `changeme` to represent passwords and assume the server will be available at `example.org`, but you'll want to provide
your own values.

#### group_vars/all/passwords.yml

```yml
# Drupal
drupal_db_password: changeme
drupal_account_pass: changeme

# MySQL/Postgres
islandora_db_root_password: changeme

# Tomcat
islandora_tomcat_password: changeme

# Syn
islandora_syn_token: changeme

# Cantaloupe
cantaloupe_admin_password: changeme

# Fedora
fcrepo_db_password: changeme
```

#### group_vars/webserver/apache.yml

This is where we specify that the webserver is listening on the default port 80, instead of the development machine port 8000.
```yml
apache_listen_port: 80
```

#### group_vars/webserver/general.yml

You will have to add the matomo line.

```yml
matomo_site_url: http://example.org
```

#### hosts

You'll need to put particulars for logging into your server in the `inventory/production/hosts` file . This example is set up to login as the `ansible` user (to avoid trying to run Composer as `root`) and uses
an SSH key. You'll need to get the details for logging into your remote server from your hosting provider (AWS, Digital Ocean, etc...)
or your systems administrator if you're running the server in-house. See
[this page](https://docs.ansible.com/ansible/latest/user_guide/intro_inventory.html#connecting-to-hosts-behavioral-inventory-parameters)
for more details about what you can put into a
host file
[//]: # (How to avoid getting asked for a sudo password? Use -K in the command below? Configuring passwordless sudo seemed to not work. no idea how vagrant does it.)

```
default ansible_host=example.org ansible_port=22 ansible_user=ansible ansible_ssh_private_key_file='/home/username/.ssh/id_rsa'
```

### Running the remote installer

First, you'll want to install the ansible roles that are needed for the version of Islandora you are trying to install.  This can be done
with

```bash
$ ansible-galaxy install -r requirements.yml
```

Then, if you are on Ubuntu 22.04, run the following commands:

```bash
$ ansible-playbook -i inventory/production playbook.yml -e "islandora_distro=ubuntu/jammy64" -e "islandora_profile=starter" -e "islandora_build_base_box=true"
$ ansible-playbook -i inventory/production playbook.yml -e "islandora_distro=ubuntu/jammy64" -e "islandora_profile=starter" -e "islandora_build_base_box=false"
```

## Troubleshooting

### Out of date playbooks

Ansible caches the code used to provision the environment, so if you've already installed once you may not be getting the latest version
of things even if you've `git pull`'d the latest playbook.  The code is stored in `roles/external`, so if you want to clear it out you can
remove these before attempting to provision an environment

```bash
$ rm -rf roles/external
```

### Port clashes for local environments

When provisioning using a local environment, you should be aware of any ports that are already in use by your computer that are also going to be
used by Vagrant, as these may clash and cause problems during and after provisioning. These include:

- 8000 (Apache)
- 8080 (Tomcat)
- 3306 (MySQL)
- 5432 (PostgreSQL)
- 8983 (Solr)
- 8161 (ActiveMQ)
- 8081 (API-X)

If there are port clashes for any of these, you will need to either find and replace them in the configuration .yml files under
`inventory/vagrant/group_vars`, or provide new values for the different playbooks that support changing the ports (for example, `postgresql_databases`
supports adding a `port` property which is currently simply unused). You will also need to replace the port forwarding values in `Vagrantfile`.

Additionally, Ansible attempts to use port 2200 for SSH. If this port is already in use, your local environment cannot be provisioned. To
change this, set a new value for `ansible_port` in `inventory/vagrant/hosts`.

### Help

If you run into any issues installing the environment, do not hesitate to email the [mailing list](mailto:islandora@googlegroups.com) to
ask for help.  If you think you've stumbled across a bug in the installer, please create an issue in the
[Islandora issue queue](http://github.com/Islandora/documentation/issues) and give it an `ansible` tag.


# Islandora Playbook

!!! note "Still Alive"
    Reports of the playbook's demise have been exaggerated. It is still in use and being maintained. Local VMs (VirtualBox) cannot be run on M1 series Apple silicon at this time, but you can use the remote deployment option to deploy Islandora on low-cost virtual hosting.  

The Islandora Playbook ([Islandora-Devops/islandora-playbook](https://github.com/Islandora-Devops/islandora-playbook)) is a tool for installing the Islandora stack on a single virtual machine. It can be used both as a [Vagrant](https://www.vagrantup.com/) project to create a local development environment, or as an [Ansible](https://www.ansible.com/community) playbook to provision a local or remote server. It can set up a Drupal based either on Islandora Starter Site, or on the Install Profile Demo. 

## Basic Usage (local, Islandora Starter Site):

Install requirements, then:
```bash
$ git clone -b dev https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
$ vagrant up
```
## Basic Usage (local, Install Profile Demo):

Install requirements, then:
```bash
$ git clone -b dev https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
$ export ISLANDORA_INSTALL_PROFILE=demo
$ vagrant up
```

## Requirements

To create a local VM, download and install the following.

1. [Virtual Box](https://www.virtualbox.org/)
2. [Vagrant](https://www.vagrantup.com/) (version 2.0 or higher required)
3. [Git](https://git-scm.com/)
4. [OpenSSL](https://www.openssl.org/)
5. [Ansible](https://www.ansible.com/community) (Tested on version 2.11+, versions back to 2.9 should work.)


#### Installing Git and Ansible on MacOS

OpenSSL is already pre-installed on MacOS. Git can be installed using XCode's command line tools (see below). Python and Pip can either be installed via the downloaded installer direct from the site or via Homebrew (not shown below). Ansible is best installed using [Homebrew](https://brew.sh/) (see below).

```bash
# Use xcode-select to install command line components, including git
$ xcode-select --install
# Install homebrew
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
# Use homebrew to install ansible
$ brew install ansible
```

## Installing a local development environment

Clone the `islandora-playbook` and use `vagrant up` to automatically provision a local environment. This method uses Vagrant, VirtualBox, and Ansible. Before provisioning a local environment, you should likely double check that no [required ports](#port-clashes-for-local-environments) are currently in use.

### Clone the playbook

```bash
$ git clone -b dev https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
```

### Spin up with Vagrant

Before using `vagrant up`:

- If building a CentOS box, you also need to install the vbguest additions with `vagrant plugin install vagrant-vbguest`.
- If this is not your first time spinning up Islandora from this directory, and you want to get the latest code, you may want to clear cached ansible roles in roles/external `rm -rf roles/external`

Then, to spin up the machine:

```bash
vagrant up
```

## Deploying to a remote environment

A remote environment can be provisioned by providing SSH credentials to `islandora-playbook` and using the `ansible-galaxy` installer instead of Vagrant. Some preparation of configuration entries in the `inventory` also need to be changed to be aware of the particulars of your remote environment; this includes:

- Changing usernames and passwords to something more sensible than the default
- Changing IP addresses to use the remote machine's actual IP
- Changing Apache to serve at port 80 (as opposed to 8000, which we use for development purposes)

We're going to build up this new remote environment configuration from the default provided Vagrant configuration. To start, take the inventory for the vagrant development environment and make a copy of it. Be sure to give it an appropriate name. Here we're using `example`.

```bash
$ git clone https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
$ cp -r inventory/vagrant inventory/example
```

Then you can update the following entries in the following files using your own information. If an entry does not exist in a file,
just add it.  Ansible will then use the value you provide instead of relying on its defaults.

We're using `changeme` to represent passwords and assume the server will be available at `example.org`, but you'll want to provide
your own values.

#### group_vars/all/passwords.yml

```yml
drupal_db_password: changeme
drupal_account_pass: changeme
islandora_db_root_password: changeme
islandora_tomcat_password: changeme
islandora_syn_token: changeme
cantaloupe_admin_password: changeme
```

#### group_vars/crayfish.yml

```yml
crayfish_gemini_fedora_base_url: http://example.org:8080/fcrepo/rest
crayfish_houdini_fedora_base_url: http://example.org:8080/fcrepo/rest
crayfish_hypercube_fedora_base_url: http://example.org:8080/fcrepo/rest
crayfish_milliner_fedora_base_url: http://example.org:8080/fcrepo/rest
crayfish_milliner_drupal_base_url: http://example.org
crayfish_milliner_gemini_base_url: http://example.org/gemini
crayfish_homarus_fedora_base_url: http://example.org:8080/fcrepo/rest
crayfish_recast_fedora_base_url: http://example.org:8080/fcrepo/rest
crayfish_recast_drupal_base_url: http://example.org
crayfish_recast_gemini_base_url: http://example.org/gemini
```

#### group_vars/karaf.yml

For Alpaca, only the `token.value` and various URLs are of particular importance, but the entire configuration chunk is provided here for convenience.

```yml
alpaca_settings:
  - pid: ca.islandora.alpaca.http.client
    settings:
      token.value: changeme
  - pid: org.fcrepo.camel.indexing.triplestore
    settings:
      input.stream: activemq:topic:fedora
      triplestore.reindex.stream: activemq:queue:triplestore.reindex
      triplestore.baseUrl: http://example.org:8080/bigdata/namespace/islandora/sparql
  - pid: ca.islandora.alpaca.indexing.triplestore
    settings:
      error.maxRedeliveries: 10
      index.stream: activemq:queue:islandora-indexing-triplestore-index
      delete.stream: activemq:queue:islandora-indexing-triplestore-delete
      triplestore.baseUrl: http://example.org:8080/bigdata/namespace/islandora/sparql
  - pid: ca.islandora.alpaca.indexing.fcrepo
    settings:
      error.maxRedeliveries: 5
      node.stream: activemq:queue:islandora-indexing-fcrepo-content
      node.delete.stream: activemq:queue:islandora-indexing-fcrepo-delete
      media.stream: activemq:queue:islandora-indexing-fcrepo-media
      file.stream: activemq:queue:islandora-indexing-fcrepo-file
      file.delete.stream: activemq:queue:islandora-indexing-fcrepo-file-delete
      milliner.baseUrl: http://example.org/milliner/
      gemini.baseUrl: http://example.org/gemini/

alpaca_blueprint_settings:
  - pid: ca.islandora.alpaca.connector.houdini
    in_stream: activemq:queue:islandora-connector-houdini
    derivative_service_url: http://example.org/houdini/convert
    error_max_redeliveries: 5
    camel_context_id: IslandoraConnectorHoudini
  - pid: ca.islandora.alpaca.connector.homarus
    in_stream: activemq:queue:islandora-connector-homarus
    derivative_service_url: http://example.org/homarus/convert
    error_max_redeliveries: 5
    camel_context_id: IslandoraConnectorHomarus
```

#### group_vars/tomcat.yml

```yml
fcrepo_allowed_external_content:
  - http://example.org/
cantaloupe_HttpResolver_BasicLookupStrategy_url_prefix: http://example.org/
```

#### group_vars/webserver/apache.yml

This is where we specify that the webserver is listening on the default port 80, instead of the development machine port 8000.
```yml
apache_listen_port: 80
```

#### group_vars/webserver/drupal.yml

```yml
drupal_trusted_hosts:
  - ^localhost$
  - example.org
fedora_base_url: "http://example.org:8080/fcrepo/rest/"
```

#### group_vars/webserver/general.yml

```yml
openseadragon_iiiv_server: http://example.org:8080/cantaloupe/iiif/2
matomo_site_url: http://example.org
```

#### hosts

You'll need the SSH particulars for logging into your server in the `inventory/vagrant/hosts` file .  This example is set up to login as `root` using
an SSH key. You'll need to get the details for logging into your remote server from your hosting provider (AWS, Digital Ocean, etc...)
or your systems administrator if you're running the server in-house. See
[this page](https://docs.ansible.com/ansible/latest/user_guide/intro_inventory.html#connecting-to-hosts-behavioral-inventory-parameters)
for more details about what you can put into a
host file

```
default ansible_host=example.org ansible_port=22 ansible_user=root ansible_ssh_private_key_file='/home/username/.ssh/id_rsa'
```

### Running the remote installer

First, you'll want to get the ansible roles that are needed for the version of Islandora you are trying to install.  This can be done
with

```bash
$ ansible-galaxy install -r requirements.yml
```

Then, depending on the operating system installed on the remote environment, you can use the following command for Ubuntu 16.04

```bash
$ ansible-playbook -i inventory/production playbook.yml -e "islandora_distro=ubuntu/xenial64"
```

or for CentOS 7

```bash
$ ansible-playbook -i inventory/production playbook.yml -e "islandora_distro=centos/7"
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

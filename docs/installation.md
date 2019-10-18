Islandora 8 is installed through an Ansible Playbook called [claw-playbook](https://github.com/Islandora-Devops/claw-playbook).

## Requirements

Download and install the following:

1. [Virtual Box](https://www.virtualbox.org/)
1. [Vagrant](https://www.vagrantup.com/) (version 2.0 or required)

Use your package manager of choice to get [Git](https://git-scm.com/) and [Ansible](https://www.ansible.com/community) if
you don't have them already.

For example, if you're using Ubuntu and `apt`

```
$ sudo apt-get install software-properties-common
$ sudo apt-add-repository ppa:ansible/ansible
$ sudo apt-get update
$ sudo apt-get install git ansible
```

If you want to provision a CENTOS 7 environment, you'll also need to install the [vbguest](https://github.com/dotless-de/vagrant-vbguest)
plugin for Vagrant

```bash
$ vagrant plugin install vagrant-vbguest
```

## Installing a local development environment

Once you've installed all the requirements, you can spin up a local development environment with
```bash
$ git clone https://github.com/Islandora-Devops/claw-playbook
$ cd claw-playbook
$ vagrant up
```

By default, this provisions an Ubuntu 18.04 environment.  If you would prefer to use CENTOS 7 instead, set the `ISLANDORA_DISTRO`
environment variable to `centos/7`. To prevent having to do this every time you open a new shell, add the following command to
your `.bashrc` file

```bash
$ export ISLANDORA_DISTRO="centos/7"
```

## Installing a remote environment

If you want to provision a remote server using the playbook, there's a handful of configuration entries you need to update to include your
usernames/passwords and IP addresses. You'll also want Apache to serve at port 80 as opposed to 8000, which we use for development
purposes.  To start, take the inventory for the vagrant development environment and copy it. Be sure to
give it an appropriate name. Here we're using `example`.

```bash
$ git clone https://github.com/Islandora-Devops/claw-playbook
$ cd claw-playbook
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
Unfortunately, you have to copy/paste this whole chunk into the yml, even though you're only updating the URLs and
the `token.value` entry.

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
Here's where you set the port to 80 instead of 8000.
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
You'll need the ssh particulars for logging into your server in the hosts file.  This example is set up to login as `root` using
an ssh key. You'll need to get the details for logging into your remote server from your hosting provider (AWS, Digital Ocean, etc...)
or your systems administrator if you're running the server in-house. See
[this page](https://docs.ansible.com/ansible/latest/user_guide/intro_inventory.html#connecting-to-hosts-behavioral-inventory-parameters)
for more details about what you can put into a
host file

```
default ansible_ssh_host=example.org ansible_ssh_user=root ansible_ssh_private_key_file='/home/username/.ssh/id_rsa'
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

or for CENTOS 7

```bash
$ ansible-playbook -i inventory/production playbook.yml -e "islandora_distro=centos/7"
```

## Troubleshooting

Ansible caches the code used to provision the environment, so if you've already installed once you may not be getting the latest version
of things even if you've `git pull`'d the latest playbook.  The code is stored in `roles/external`, so if you want to clear it out you can
run

```bash
$ rm -rf roles/external
```

If you run into any issues installing the environment, do not hesitate to email the [mailing list](mailto:islandora@googlegroups.com) to
ask for help.  If you think you've stumbled across a bug in the installer, please create an issue in the
[Islandora 8 issue queue](http://github.com/Islandora-CLAW/CLAW/issues) and give it an `ansible` tag.

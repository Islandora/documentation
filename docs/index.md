# Overview

[Islandora](https://islandora.ca) is an open-source framework that provides the necessary tools to turn a [Drupal](https://www.drupal.org) website into a fully-functional Digital Assets Management System.

## About

Islandora:

[//]: # (We should probably replace as many of the links in this section as possible with links to within this documentation, and make it clear which are internal links and which link out.) 
- **Is native Drupal** - With Islandora, you can create preservation-ready digital resources using Drupal nodes, media, files, and taxonomy terms. A suite of customized Drupal resource types and configurations that illustrate Islandora's capabilities are packaged as the [Islandora Defaults module](https://github.com/Islandora/islandora_defaults).
- **Integrates with Fedora** - Drupal resources can be stored in Duraspace's [Fedora 5.x Repository](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home) as binary assets and RDF metadata.
- **Uses microservices** - Islandora provides an architecture for messaging and integration with any number of microservices, that provide services outside of the Drupal framework. Islandora's [Crayfish](https://github.com/Islandora/crayfish) suite of microservices provides functionality for synchronizing resources into Fedora and automatically generating derivative files.
- **Can handle messages at scale** - Islandora uses [Alpaca](https://github.com/Islandora/Alpaca), an integration middleware based on Apache Camel, to scalably handle messaging and queueing for microservices such as Crayfish.
[//]: # (Allowing bulk uploads to be processed without affecting the server... would be the ideal right? but right now they're on the same server. Is just saying "scalability" ok?)
- **Offers digital preservation features** - Using a robust storage layer for preservation (Fedora), and generating technical metadata with FITS, are digital preservation tools provided out of the box in the default implementation. Community members have created additional features for doing [digital preservation using Islandora](https://github.com/mjordan/digital_preservation_using_islandora), which have not yet been contributed to "Core Islandora". 
- **Exposes data to harvesters** - Metadata about resources is available as linked data through the [JSON-LD serialization module](https://github.com/Islandora/jsonld), and can be made available through Drupal, Fedora, or a triplestore. Islandora also offers a [full configuration for exposing OAI-PMH](https://github.com/Islandora/islandora_defaults/tree/8.x-1.x/modules/islandora_oaipmh) to metadata harvesters, and [IIIF support](https://github.com/Islandora/islandora/tree/8.x-1.x/modules/islandora_iiif) for images.
- **Offers flexibility** - As Islandora content is Drupal content, migrations and batch editing can be done through Drupal's built-in migrate framework and controlled vocabularies created using Drupal taxonomies. Contributed Drupal modules such as [Solr Search API](https://www.drupal.org/project/search_api_solr) enable in-site search, and [Matomo Analytics](https://www.drupal.org/project/matomo) provides usage metrics for site analytics.
- **Is a community** - A [dedicated, active community of users and developers](https://groups.google.com/forum/#!forum/islandora) is working to push new features, collaborate on improvements, design custom solutions, and create extended functionality. Some of these for Islandora 8 take the form of [Recipes](https://github.com/Islandora-Labs/Islandora-Cookbook).

## Demo

Visit [http://future.islandora.ca/](http://future.islandora.ca/).

## Quickstart

These instructions use Vagrant and the [Islandora Ansible playbook](https://github.com/Islandora-Devops/islandora-playbook) to spin up a local development machine, which includes the [Islandora Default site configuration](https://github.com/Islandora/islandora_defaults). See the [Automatic Provisioning](installation/playbook/) page for full instructions.
[//]: # (Move this link to contributing/hacking-on-islandora.md once that's de-CLAWed.)


### Requirements
- [Virtual Box](https://www.virtualbox.org/)
- [Vagrant](https://www.vagrantup.com/) (version 2.0 or higher required)
- [Git](https://git-scm.com/)
- [OpenSSL](https://www.openssl.org/)
- [Ansible](https://www.ansible.com/community) (up to, and not past, 2.8.7)

### Instructions

Ubuntu 18.04 or Mac OS:
```console
$ git clone https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
$ vagrant up
```
CentOS 7:
```console
$ git clone https://github.com/Islandora-Devops/islandora-playbook
$ cd islandora-playbook
$ vagrant plugin install vagrant-vbguest
$ ISLANDORA_DISTRO="centos/7" vagrant up
```

Then, navigate to [http://localhost:8000](http://localhost:8000) and log in with username `admin` and password `islandora`.


!!! note "Documentation for previous versions"
    Documentation for Islandora 6 and 7 is on the [Lyrasis documentation wiki](https://wiki.lyrasis.org/display/ISLANDORA/Start).


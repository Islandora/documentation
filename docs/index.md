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

## Try Islandora

### Online Sandbox

To try Islandora without installing anything, visit [http://future.islandora.ca/](http://future.islandora.ca/). Anyone can log in to this sandbox as an administrator (credentials are below and on the front page) and explore the interface! However, this site is refreshed periodically so your changes will not be permanent. This site uses Islandora Defaults, a way of setting up Islandora for demonstration purposes. This is not the only way that Islandora can be made to work! This sandbox includes, on top of Islandora Defaults, some sample content and configuration (such as views and blocks, and other Drupal modules like Views Bulk Edit) to increase its usefulness as a sandbox. 

- username: Test
- password: islandora

### Virtual Machine Image (.ova file)

To try Islandora locally, you can download and run the latest [community sandbox Virtual Machine image](https://islandora.ca/try). This requires installing VirtualBox or another virtualization provider. This also uses Islandora Defaults, and similarly to the online sandbox includes sample content and other configured Views, Blocks, and Drupal modules.

- username: admin
- password: islandora

### Vagrant and Islandora Base Box

The latest release of Islandora Defaults is uploaded as the [Islandora 8 Base Box](https://app.vagrantup.com/islandora/boxes/8) on Vagrant Cloud. If you have Vagrant and Git installed as well as VirtualBox, you can spin up the latest release of Islandora Defaults quickly by using the [Islandora Playbook](https://github.com/Islandora-Devops/islandora-playbook). On the `dev` branch, the Vagrantfile has the ISLANDORA_DISTRO variable default to 'islandora/8'. Like this, `vagrant up` will download the latest Islandora Base Box and will skip ansible provisioning. Same credentials as above, with further access to services documented in its README file.

```bash
git clone https://github.com/Islandora-Devops/islandora-playbook
cd islandora-playbook
git checkout dev
vagrant up
```

### Islandora Playbook via Ansible

To use the Islandora Playbook as an Ansible Playbook, either set ISLANDORA_DISTRO to `ubuntu/bionic64` or `centos/7` (by editing the Vagrantfile or setting a shell variable), or, use Ansible to run it against an external linux server. Either way, be prepared to wait a while as Ansible installs Drupal via drupal-project and Composer. See [Installation](installation/playbook) and [Hacking on Islandora](contributing/hacking-on-islandora) for more details. 


## Join the Community

The [Islandora community](https://islandora.ca/index.php/community) is an active group of users, managers, librarians, documenters, and developers from GLAM (and beyond!) institutions worldwide. We welcome discussion and contribution through various mailing lists, channels, interest groups, and calls. The Islandora community operates under the [Islandora Code Of Conduct](https://islandora.ca/codeofconduct). See our Contributing Guidelines for more information.


!!! note "Documentation for previous versions"
    Documentation for Islandora 6 and 7 is on the [Lyrasis documentation wiki](https://wiki.lyrasis.org/display/ISLANDORA/Start).


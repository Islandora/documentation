# About

## This is Islandora

[Islandora](https://islandora.ca) is an open-source framework that provides the necessary tools to use a [Drupal](https://www.drupal.org) website as a fully-functional Digital Assets Management System.

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
- **Is a community** - A [dedicated, active community of users and developers](https://groups.google.com/forum/#!forum/islandora) is working to push new features, collaborate on improvements, design custom solutions, and create extended functionality. Some of these for Islandora take the form of [Recipes](https://github.com/Islandora-Labs/Islandora-Cookbook).

## Try Islandora

### Online Sandbox

Try Islandora without installing anything at [sandbox.islandora.ca](https://sandbox.islandora.ca/).

Login credentials for the sandbox can be found [here](https://github.com/Islandora/documentation/wiki/Sandbox.Islandora.ca-online-credentials).

Anyone can log in to this sandbox as an administrator and explore the interface! However, this site is refreshed periodically so your changes will not be permanent. This site uses Islandora Defaults, a way of setting up Islandora for demonstration purposes. This is not the only way that Islandora can be made to work! This sandbox includes, on top of Islandora Defaults, some sample content and configuration (such as views and blocks, and other Drupal modules like Views Bulk Edit) to increase its usefulness as a sandbox.

### Ansible Playbook

Islandora can be installed via an an [Ansible Playbook](https://github.com/Islandora-Devops/islandora-playbook), which provisions the full Islandora stack. It can be used to create a local Islandora (requiring Vagrant and Virtualbox) or can be used to provision a remote Linux server. The provisioning process involves many steps where software is downloaded and installed, so it can take a while. There is an option to get a basic ("standard") site, or to install a suite of demo configurations known as the Demo Install Profile. See [Installation - Ansible Playbook](installation/playbook) for more details.

### Docker

Islandora sites can also be created using Docker. This can be done using the ISLE-DC project, which launches the Islandora Docker images created by [Isle Buildkit](https://github.com/Islandora-Devops/isle-buildkit). Like in the Ansible Playbook, there is an option to use a pre-built demo site, or build it completely from scratch. (There is also an option to build the demo site on command, which takes a bit more time, or to use ISLE-DC to build up the environment to support a Drupal site that you have already exported.)  See [Installation - Docker ISLE](installation/docker-introduction) for more details

## Join the Community

The [Islandora community](https://www.islandora.ca/community) is an active group of users, managers, librarians, documenters, and developers from galleries, libraries, archives, museums, and other institutions worldwide. We welcome discussion and contribution through various mailing lists, channels, interest groups, and calls. The Islandora community operates under the [Islandora Code Of Conduct](https://www.islandora.ca/code-of-conduct). See our Contributing Guidelines for more information.


!!! note "Documentation for previous versions"
    Documentation for Islandora 6 and 7 is on the [Lyrasis documentation wiki](https://wiki.lyrasis.org/display/ISLANDORA/Start).

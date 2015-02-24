# About Islandora

Islandora is an open-source software framework designed to help institutions and organizations and their audiences collaboratively manage, and discover digital assets using a best-practices framework.  Islandora was originally developed by the University of Prince Edward Island's Robertson Library, but is now implemented and contributed to by an ever-growing international community.

Islandora consists of:

  * Sync - Event driven middleware based on Apache Camel that synchronizes a Fedora 4 JCR with a Drupal CMS.
  * Islandora - Fedora 4 Repository module
  * Install - The is a development environment virtual machine for the Islandora and Fedora 4 project. It should work on any operating system that supports VirtualBox and Vagrant.

## About this guide

The [Technical Design documentation](technical-design/technical_documentation.md) will help you:

  * Understand the Islandora 7.x-2.x design rationale
  * Importance of using an integration framework
  * Using camel
  * Inversion of control and camel
  * Camel and scripting languages
  * Islandora Sync
  * Islandora (Drupal)
  * Solr and Triple store indexing

### Sync

The [Camel section](camel/sync/README.md) provides and overiew of the camel middleware.

### Islandora (Drupal module)

The [Drupal section](drupal/islandora/README.md) provides an overview of the Drupal modules in the Islandora environment.

### Installation (Vagrant)

The [installation section](install/README.md) will show you how to create a virtual development environment.

## Contributing

If you would like to contribute, please get involved with the [Islandora Fedora 4 Interest Group](https://github.com/Islandora/Islandora-Fedora4-Interest-Group). We love to hear from you!

## Sponsors

* LYRASIS
* York University
* McMaster University
* University of Prince Edward Island
* University of Manitoba
* University of Limerick

## Maintainers

* [Nick Ruest](https://github.com/ruebot)
* [Daniel Lamb](https://github.com/daniel-dgi/)

## Licensing

Islandora is licensed under the GNU General Public License, Version 3. See [LICENSE](https://github.com/Islandora-Labs/islandora/blob/7.x-2.x/LICENSE) for the full license text.

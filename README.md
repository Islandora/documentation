# ![Islandora 8](https://camo.githubusercontent.com/738dd7cbd90a3ef06b9bb55a4cf5ed385a048fd4/687474703a2f2f69736c616e646f72612e63612f73697465732f64656661756c742f66696c65732f696d616765732f6c6f6273746572434c41572e706e67) Islandora 8 Documentation

## About this repository (github.com/Islandora/documentation)

This repository contains the user, developer, and administrator documentation for the [Islandora project](https://islandora.ca/), for versions 8 and above. This documentation is available to read at https://islandora.github.io/documentation/, where it is automatically deployed on commit from the source code at https://github.com/Islandora/documentation. Documentation for Islandora 7.x and previous versions is available at https://wiki.lyrasis.org/display/ISLANDORA/. 

This repository's issue queue is the [Islandora-wide issue queue](https://github.com/Islandora/documentation/issues) for Islandora versions 8 and above. Issues may be applicable to repositories in all Islandora-associated github organizations, including [Islandora](https://github.com/Islandora), [Islandora DevOps](https://github.com/Islandora-Devops), [Islandora Labs](https://github.com/Islandora-Labs), and [Islandora Interest Groups](https://github.com/islandora-interest-groups). Issues for Islandora 7.x and previous versions is available at https://jira.lyrasis.org/projects/ISLANDORA/issues.


## What is Islandora 8?

Islandora 8 is the next generation of Islandora. It pairs [Drupal 8](https://www.drupal.org/8) with [Fedora 5](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home)

For more details, please check out the following resources:

* [Documentation](https://islandora.github.io/documentation/)
* [Contributing](https://github.com/Islandora/documentation/blob/master/CONTRIBUTING.md)

* [Weekly Tech Calls](https://github.com/Islandora/documentation/wiki#islandora-8-tech-calls)
  * [2015](https://github.com/Islandora/documentation/wiki/2015)
  * [2016](https://github.com/Islandora/documentation/wiki/2016)
  * [2017](https://github.com/Islandora/documentation/wiki/2017)
  * [2018](https://github.com/Islandora/documentation/wiki/2018)
  * [2019](https://github.com/Islandora/documentation/wiki/2019)
  * [2020](https://github.com/Islandora/documentation/wiki/2020)


## Repository Structure

This repository pulls in additional documentation from the following repositories, which is reflected in the repository tree.

* [Alpaca](https://github.com/islandora/Alpaca): Event driven middleware based on Apache Camel that synchronizes a Fedora with Drupal.
* [docs](https://github.com/Islandora/documentation/tree/master/docs): Documentation!
* [carapace](https://github.com/islandora/carapace/): A starter theme for an Islandora 8 repository. 
* [chullo](https://github.com/islandora/chullo/): PHP client for Fedora 4 built using Guzzle and EasyRdf.
* [islandora-playbook](https://github.com/Islandora-Devops/islandora-playbook): Ansible installer.
* [controlled_access_terms](https://github.com/islandora/controlled_access_terms/): Drupal module for subjects and agents. 
* [Crayfish](https://github.com/islandora/Crayfish): A collection of Islandora 8 microservices.
* [Crayfish-Commons](https://github.com/Islandora/Crayfish-Commons): A library housing shared code for Crayfish microservices
* [drupal-project](https://github.com/Islandora/drupal-project): Composer template for Islandora 8 Drupal
* [islandora](https://github.com/Islandora/islandora/tree/8.x-1.x): Islandora 8 Drupal core module
* [islandora_defaults](https://github.com/Islandora/islandora_defaults): Starter configuration for an Islandora 8 repository 
* [jsonld](https://github.com/islandora/jsonld): JSON-LD Serializer for Drupal 8
* [migrate_islandora_csv](https://github.com/Islandora/migrate_islandora_csv): Tutorial and example migration into Islandora 8 using a CSV file.
* [migrate_7x_claw](https://github.com/Islandora-Devops/migrate_7x_claw): Starter migration for Islandora 7 -> 8.
* [openseadragon](https://github.com/islandora-claw/openseadragon): Drupal 8 Field Formatter for OpenSeadragon
* [Syn](https://github.com/islandora/Syn): Tomcat valve for JWT Authentication


## Installation
Islandora 8 provides an Ansible [islandora-playbook](https://github.com/Islandora-Devops/islandora-playbook) to fully deploy the stack to a vagrant, bare-metal server or multi server environments.

## Sponsors

* UPEI
* discoverygarden inc.
* LYRASIS
* McMaster University
* University of Limerick
* York University
* University of Manitoba
* Simon Fraser University
* PALS
* American Philosophical Society
* Common Media Inc.

## Maintainers

* [Melissa Anez](https://github.com/manez/)
* [Daniel Lamb](https://github.com/dannylamb/)
* [Jared Whiklo](https://github.com/whikloj)

## Development

If you would like to contribute, please get involved by attending our weekly [Tech Call](https://github.com/Islandora/documentation/wiki#islandora-8-tech-calls). We love to hear from you!

If you would like to contribute code to the project, you need to be covered by an Islandora Foundation [Contributor License Agreement](http://islandora.ca/sites/default/files/islandora_cla.pdf) or [Corporate Contributor License Agreement](http://islandora.ca/sites/default/files/islandora_ccla.pdf). Please see the [Contributors](http://islandora.ca/resources/contributors) pages on Islandora.ca for more information.

We recommend using the [islandora-playbook](https://github.com/Islandora-Devops/islandora-playbook) to get started.  If you want to pull down the submodules for development, don't forget to run `git submodule update --init --recursive` after cloning.

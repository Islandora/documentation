# ![Islandora CLAW](https://camo.githubusercontent.com/738dd7cbd90a3ef06b9bb55a4cf5ed385a048fd4/687474703a2f2f69736c616e646f72612e63612f73697465732f64656661756c742f66696c65732f696d616765732f6c6f6273746572434c41572e706e67) Islandora CLAW

## Introduction

Islandora CLAW is the next generation of Islandora. Still in development, this major upgrade will be compatible with [Fedora 4](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home).

For more details, please check out the following resources:

* [Weekly Tech Calls](https://github.com/Islandora-CLAW/CLAW/wiki#islandora-claw-tech-calls)
  * [2015](https://github.com/Islandora-CLAW/CLAW/wiki/2015)
  * [2016](https://github.com/Islandora-CLAW/CLAW/wiki/2016)
  * [2017](https://github.com/Islandora-CLAW/CLAW/wiki/2017)
* [Contributing](https://github.com/Islandora-CLAW/CLAW/blob/master/CONTRIBUTING.md)

## Repository Structure

* [Alpaca](https://github.com/islandora-claw/Alpaca): Event driven middleware based on Apache Camel that synchronizes a Fedora 4 with Drupal.
* [docs](https://github.com/Islandora-CLAW/CLAW/tree/master/docs): Documentation!
* [chullo](https://github.com/islandora-claw/chullo/): PHP client for Fedora 4 built using Guzzle and EasyRdf
* [Crayfish](https://github.com/islandora-claw/Crayfish): A collection of Islandora CLAW microservices
* [Crayfish-Commons](https://github.com/Islandora-CLAW/Crayfish-Commons): A library housing shared code for Crayfish microservices
* [drupal-project](https://github.com/Islandora-CLAW/drupal-project): Composer template for Islandora CLAW Drupal
* [claw_install](https://github.com/Islandora-CLAW/claw_vagrant): Bleeding edge development environment
* [islandora](https://github.com/Islandora-CLAW/islandora): Islandora CLAW Drupal core module
* [islandora_collection](https://github.com/Islandora-CLAW/islandora_collection): Islandora CLAW Drupal collection module
* [islandora_image](https://github.com/Islandora-CLAW/islandora_image): Islandora CLAW Drupal image module
* [jsonld](https://github.com/islandora-claw/jsonld): JSON-LD Serializer for Drupal 8 and Islandora CLAW
* [Syn](https://github.com/islandora-claw/Syn): Tomcat valve for JWT Authentication

**Note**: the [Ansible](https://github.com/Islandora-CLAW/claw-ansible) and [Docker](https://github.com/Islandora-CLAW?utf8=%E2%9C%93&query=docker) repositories were early experimental provisioning tools for the CLAW stack, and are currently **not** supported. For context see [this](https://github.com/Islandora-CLAW/CLAW/issues/182). Development is focused on [claw_vagrant](https://github.com/islandora-claw/claw_vagrant), which is the recommended starting point for working with CLAW.

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

* [Nick Ruest](https://github.com/ruebot)
* [Daniel Lamb](https://github.com/dannylamb/)
* [Jared Whiklo](https://github.com/whikloj)
* [Diego Pino](https://github.com/DiegoPino)

## Development

If you would like to contribute, please get involved by attending our weekly [Tech Call](https://github.com/Islandora-CLAW/CLAW/wiki). We love to hear from you!

If you would like to contribute code to the project, you need to be covered by an Islandora Foundation [Contributor License Agreement](http://islandora.ca/sites/default/files/islandora_cla.pdf) or [Corporate Contributor Licencse Agreement](http://islandora.ca/sites/default/files/islandora_ccla.pdf). Please see the [Contributors](http://islandora.ca/resources/contributors) pages on Islandora.ca for more information.

If you want to pull down the submodules for development, don't forget to run `git submodule update --init --recursive` after cloning.

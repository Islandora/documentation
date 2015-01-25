# Islandora-Sync 

## Introduction

Event driven middleware based on Apache Camel that synchronizes a Fedora 4 JCR with a Drupal CMS.  

## Requirements

* Java 7
* Maven 3

## Compilation

`mvn install`

## Deployment

After successful compilation, copy the resulting jar from the target directory to the deploy directory of your karaf installation.  If you are actively developing, it is advised to install through maven via `osgi:install -s mvn:org.islandora/islandora-sync/VERSION` in the karaf shell.  Be sure to put in the appropriate version number from the pom file.  Afterwards, you can have karaf automatically redeploy anytime you successfully run `mvn install` by issuing the following karaf command: `dev:watch --dev:watch mvn:org.islandora/islandora-sync/VERSION`.

## Maintainers/Sponsors

Current maintainers:

* [Nick Ruest](https://github.com/ruebot)
* [Daniel Lamb](https://github.com/daniel-dgi/)

## Development

If you would like to contribute, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)


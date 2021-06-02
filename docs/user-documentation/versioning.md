# Versioning
As a user of an Islandora repository, you may be wondering - Is this content being versioned? Could I restore from a previous version if I needed to? Can I see a list of versions for an object?
The answer to these questions is two-fold, and largely yes. The architecture of Islandora provides users with a Drupal implementation and a Fedora implementation which are connected in Islandora.

!!! note "Islandora Software Versioning" 
    Looking for information about versions of Islandora itself? The latest Islandora follows [semantic versioning](https://semver.org/). Previously, Islandora's versions were tied to the version of Drupal and numbered in order of release, such as [Islandora 6.x-13.1](https://wiki.lyrasis.org/display/ISLANDORA6131/Islandora) or [Islandora 7.x-1.13](https://wiki.lyrasis.org/display/ISLANDORA/Start). [More information](../technical-documentation/versioning).

## Drupal Revisioning
Drupal provides a concept of revisions which allows you to track the differences between multiple versions of your content and revert to older ones. The list of revisions for a node are available at [http://localhost:8000/node/1/revisions](http://localhost:8000/node/1/revisions). There are [Drupal docs](https://www.drupal.org/docs/8/administering-a-drupal-8-site/node-revisions) on revisioning. Media objects are also versioned in Drupal but there is not a UI component for this - [see related issue](https://github.com/Islandora/documentation/issues/1035).

## Fedora and Memento
Fedora implements the [Memento](http://mementoweb.org/about/) specification for versioning resources, which is a time-based HTTP framework. Fedora provides [documentation](https://wiki.lyrasis.org/display/FEDORA5x/Versioning) as well as an [API implementation](https://wiki.lyrasis.org/display/FEDORA5x/RESTful+HTTP+API+-+Versioning).


## Basic Data Flow
1. A node or media object is created or updated in Drupal.
2. When an entity is revisionable and it isn't the initial creation, it [adds a flag](https://github.com/Islandora/islandora/blob/8.x-1.x/src/EventGenerator/EventGenerator.php#L109) to the event object that gets passed to Alpaca.
3. The [islandora-indexing-fcrepo module](https://github.com/Islandora/Alpaca/tree/dev/islandora-indexing-fcrepo) of Alpaca looks for that flag and fires a call to the [versioning endpoint](https://github.com/Islandora/Crayfish/blob/dev/Milliner/src/app.php#L52) of [Milliner](https://github.com/Islandora/Crayfish/tree/dev/Milliner).
4. Milliner uses the [Chullo library](https://github.com/Islandora/chullo/blob/dev/src/FedoraApi.php#L320) to [create a version](https://github.com/Islandora/Crayfish/blob/dev/Milliner/src/Service/MillinerService.php#L551) in Fedora.

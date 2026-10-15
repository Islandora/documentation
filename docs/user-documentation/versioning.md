# Versioning
As a user of an Islandora repository, you may be wondering - Is this content being versioned? Could I restore from a previous version if I needed to? Can I see a list of versions for an object?
The answer to these questions is two-fold, and largely yes. The architecture of Islandora provides users with a Drupal implementation and a Fedora implementation which are connected in Islandora.

!!! note "Islandora Software Versioning"
    Looking for information about versions of Islandora itself? The latest Islandora follows [semantic versioning](https://semver.org/). Previously, Islandora's versions were tied to the version of Drupal and numbered in order of release, such as [Islandora 6.x-13.1](https://wiki.lyrasis.org/display/ISLANDORA6131/Islandora) or [Islandora 7.x-1.13](https://wiki.lyrasis.org/display/ISLANDORA/Start). [More information](../technical-documentation/versioning.md).

## Drupal Revisioning
Drupal provides a concept of revisions which allows you to track the differences between multiple versions of your content and revert to older ones. The list of revisions for a node, media, or taxonomy term are available at the entity's page, with `/revisions` appended to the URL. There are [Drupal docs](https://www.drupal.org/docs/8/administering-a-drupal-8-site/node-revisions) on revisioning.

## Fedora and Memento
Fedora implements the [Memento](http://mementoweb.org/about/) specification for versioning resources, which is a time-based HTTP framework. Fedora provides [documentation](https://wiki.lyrasis.org/display/FEDORA5x/Versioning) as well as an [API implementation](https://wiki.lyrasis.org/display/FEDORA5x/RESTful+HTTP+API+-+Versioning).

## Basic Data Flow
1. A node or media object is created or updated in Drupal.
2. When a revisionable entity is updated with a new Drupal revision, `islandora_events_fcrepo` records that the Fedora indexing run should also create a version snapshot.
3. The Fedora indexing worker updates the live Fedora resource directly from Drupal-managed JSON or JSON-LD.
4. The same Drupal worker then creates a Fedora memento for that updated resource, so Fedora versioning happens without Alpaca or Milliner.

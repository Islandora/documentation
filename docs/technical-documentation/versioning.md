# Versioning Policy

Islandora 8 uses semantic versioning, except for Drupal modules/themes.

## [Semantic Versioning](http://semver.org/)

**Major version** . **Minor version** . **Patch**

- **Major version**; Major changes, and breaks the API
- **Minor version**; New features, and does not break the API
- **Patch**; Bug fixes, and never breaks backward compatibility

Examples:

* `1.2.3 => 1.2.4` - Just a bug fix, should be a drop-in replacement.
* `1.2.3 => 1.3.0` - Adds in new features, should be a drop-in replacement to get new functionality.
* `1.2.3 => 2.0.0` - Major changes, may require a migration or changes to your set-up.

The following Islandora 8 components use semantic versioning:

* [Alpaca](https://github.com/Islandora/Alpaca)
* [Chullo](https://github.com/Islandora/Chullo)
* [Crayfish](https://github.com/Islandora/Crayfish)
* [Crayfish Commons](https://github.com/Islandora/Crayfish-Commons)
* [Syn](https://github.com/Islandora/Syn)

## [Drupal Contrib Versioning](https://www.drupal.org/docs/8/choosing-a-drupal-version/what-do-version-numbers-mean-on-contributed-modules-and-themes)

**Core Compatibility** - **Major** . **PatchLevel[-Extra]**

Examples: 

* `8.x-1.0 => 8.x-1.1` - Just a bug fix, should be a drop-in replacement.
* `8.x-1.0 => 8.x-2.0` - Major changes, could require a migration or changes to your set-up.

The `8.x-` part will not change to `9.x-` until Drupal 9.

The following Islandora 8 components use Drupal Contrib versioning:

* [carapace](https://github.com/Islandora/carapace)
* [controlled\_access\_terms](https://github.com/Islandora/controlled_access_terms)
* [drupal-project](https://github.com/Islandora/drupal-project)
* [islandora](https://github.com/Islandora/islandora/tree/8.x-1.x)
* [islandora_defaults](https://github.com/Islandora/islandora_defaults)
* [jsonld](https://github.com/Islandora/jsonld)
* [openseadragon](https://github.com/Islandora/openseadragon)

This list does *not* include sub-modules which share versions with their parents.

ie. 
[Islandora Audio](https://github.com/Islandora/islandora/tree/8.x-1.x/modules/islandora_audio), [Islandora IIIF](https://github.com/Islandora/islandora/tree/8.x-1.x/modules/islandora_iiif) and [Islandora Breadcrumbs](https://github.com/Islandora/islandora/tree/8.x-1.x/modules/islandora_breadcrumbs) are all sub-modules of Islandora and shares it's version.

# Versioning Policy

Islandora uses semantic versioning for all non-Legacy code in the [github.com/Islandora](https://github.com/Islandora) organization. This allows us to be compatible with Composer and with Drupal's [release naming conventions](https://www.drupal.org/node/1015226) for contributed modules.

## Semantic Versioning

[Semantic Versioning](https://semver.org/) is a common versioning standard. Versions have the form **Major version** . **Minor version** . **Patch**

- **Major version**; Major changes, and breaks the API
- **Minor version**; New features, and does not break the API
- **Patch**; Bug fixes, and never breaks backward compatibility

### Examples

* `1.2.3 => 1.2.4` - Just a bug fix, should be a drop-in replacement.
* `1.2.3 => 1.3.0` - Adds in new features, should be a drop-in replacement to get new functionality.
* `1.2.3 => 2.0.0` - Major changes, may require a migration or changes to your set-up.

### Repositories under semantic versioning

The following Islandora components use semantic versioning:

* [Alpaca](https://github.com/Islandora/Alpaca)
* [Chullo](https://github.com/Islandora/Chullo)
* [Crayfish](https://github.com/Islandora/Crayfish)
* [Crayfish Commons](https://github.com/Islandora/Crayfish-Commons)
* [Syn](https://github.com/Islandora/Syn)
* [controlled\_access\_terms](https://github.com/Islandora/controlled_access_terms) (Drupal module)
* [islandora](https://github.com/Islandora/islandora/tree/8.x-1.x) (Drupal module)
* [islandora_defaults](https://github.com/Islandora/islandora_defaults) (Drupal module)
* [jsonld](https://github.com/Islandora/jsonld) (Drupal module)
* [openseadragon](https://github.com/Islandora/openseadragon) (Drupal module)

Drupal submodules, which are included in several of the above modules, share versions with their parents. 

!!! note "Drupal module versions switched from 8.x-1.x to 2.x"
    In October 2021, Islandora switched from the "[core compatibility](https://www.drupal.org/docs/8/understanding-drupal-version-numbers/what-do-version-numbers-mean-on-contributed-modules-and)" based numbering scheme (8.x-1.x) to a pure semantic versioning scheme for its Drupal modules. In accordance with [Drupal's requirements](https://www.drupal.org/node/1015226#semver-transition), this transition required us to bump the major version, from 1.x to 2.x, despite there not being any major API-breaking changes to the code itself.

## Implications for Release Process

Committers should now create (i.e. "tag") new versions of components when new bug fixes, features, or API changes are successfully added. This means that "releases" (new versions) will be happening individually, continually, and far more frequently than before. See [Releasing Islandora](../contributing/releasing-islandora.md).

## Module Interdependencies

When Islandora components require other Islandora components in their `composer.json` files, we prefer the version specification syntax `^2` to point to the latest-released compatible version within the specified major version.

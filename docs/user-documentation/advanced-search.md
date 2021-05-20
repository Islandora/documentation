# Islandora Advanced Search 

- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Configuring Solr](#configuring-solr)
- [Configure Collection Search](#configure-collection-search)
- [Configure Views](#configure-views)
  - [Collection Search](#collection-search)
  - [Paging](#paging)
  - [Sorting](#sorting)
- [Configure Facets](#configure-facets)
  - [Include / Exclude Facets](#include--exclude-facets)
- [Configure Blocks](#configure-blocks)
  - [Advanced Search Block](#advanced-search-block)


## Introduction

Advanced Search adds additional functionality beyond the [basic Solr search](./documentation/user-documentation/searching/). It enables the use
of Ajax with search blocks, facets, and search results.

![animated gif demonstrating advanced search in action](./assets/advanced_search_demo.gif)

## Requirements

Use composer to download the required libraries and modules.

```bash
composer require drupal/facets "^1.3"
composer require drupal/search_api_solr "^4.1"
composer require drupal/search_api "^1.5"
```

However, for reference, `islandora_advanced_search` requires the following
drupal modules:

- [facets](https://www.drupal.org/project/facets)
- [search_api_solr](https://www.drupal.org/project/search_api_solr)

## Installation

To download/enable just this module, use the following from the command line:

```bash
composer require islandora/islandora
drush en islandora_advanced_search
```

## Configuration

You can set the following configuration at **Administration** >> **Configuration** >> **Advanced Search Settings**
(admin/config/islandora/advanced_search):

![screenshot of the advanced search setting page](./assets/islandora_advanced_search_settings.png)

## Configuring Solr

Please review
[Configure Search](../searching) before continuing. The following assumes you already have a working Solr and the
Drupal Search API setup.

## Configure Collection Search

To support collection based searches you need to index the `field_member_of` for
every repository item as well define a new field that captures the full
hierarchy of `field_member_of` for each repository item.

Add a new `Content` solr field `field_decedent_of` to the solr index at
`admin/config/search/search-api/index/default_solr_index/fields`.

![screenshot of field_decent_of](./assets/advanced_search_field_decedent_of.png)

Then under `admin/config/search/search-api/index/default_solr_index/processors`
enable `Index hierarchy` and setup the new field to index the hierarchy.

![screenshot of checked checkbox](./assets/advanced_search_enable_index_hierarchy.png)

![screenshot of processor setting options](./assets/advanced_search_enable_index_hierarchy_processor.png)

The field can now be used limit a search to all the decedents of a given object.

!!! note "Re-Indexing" 
    You may have to re-index to make sure the field is populated.

## Configure Views

The configuration of views is outside of the scope of this document, please read
the [Drupal Documentation](https://www.drupal.org/docs/8/core/modules/views), as
well as the
[Search API Documentation](https://www.drupal.org/docs/contributed-modules/search-api).

### Collection Search

It will be typical that you require the following
_Relationships_ and _Contextual Filters_ when setting up a search view to enable
_Collection Search_ searches.

![screenshot of contexts configuration](./assets/advanced_search_view_advanced_setting.png)

Here a relationship is setup with `Member Of` field and we have **two**
contextual filters:

1. `field_member_of` (Direct decedents of the Entity)
2. `field_decedent_of` (All decedents of the Entity)

Both of these filters are configured the exact same way.

![screenshot of configuration settings](./assets/advanced_search_contextual_filter_settings.png)

These filters are toggled by the Advanced Search block to allow the search to
include all decedents or just direct decedents (*documented below*).

### Paging

The paging options specified here can have an affect on the pager block
(*documented below*).

![screenshot of paging configuration settings](./assets/advanced_search_pager_settings.png)

### Sorting

Additional the fields listed as _Sort Criteria_ as _Exposed_ will be made
available in the pager block (*documented below*).

![screenshot of sort criteria](./assets/advanced_search_sort_criteria.png)

## Configure Facets

The facets can be configured at `admin/config/search/facets`. Facets are linked
to a *Source* which is a *Search API View Display* so it will be typically
to have to duplicate your configuration for a given facet across each of the
displays where you want it to show up.

### Include / Exclude Facets

To be able to display exclude facet links as well as include links in the facets
block we have to duplicate the configuration for the facet like so.

![screenshot of facet configurations](./assets/advanced_search_include_exclude_facets.png)

Both the include / exclude facets must use the widget
`List of links that allow the user to include / exclude facets`

![screenshot of list of configuration options for facets](./assets/advanced_search_include_exclude_facets_settings.png)

The excluded facet also needs the following settings to appear and function
correctly.

The _URL alias_ must match the same value as the include facet except it must be
prefixed with `~` character that is what links to the two facets to each other.

![screenshot of url alias setting](./assets/advanced_search_exclude_facet_settings_url_alias.png)

And it must also explicitly be set to exclude:

![screenshot of exclude checkbox](./assets/advanced_search_exclude_facet_settings_exclude.png)

You may also want to enable _Hide active items_ and _Hide non-narrowing results_
for a cleaner presentation of facets.

## Configure Blocks

For each block type:

- Facet
- Pager
- Advanced Search

There will be **one block** per _View Display_. The block should be limited to
only appear when the view it was derived from is also being displayed on the
same page.

This requires configuring the `visibility` of the block as appropriate. For
collection based searches be sure to limit the display of the Facets block to
the models you want to display the search on, e.g:

![screenshot of block configuration](./assets/advanced_search_facet_block_settings.png)

### Advanced Search Block

For any valid search field, you can drag / drop and reorder the fields to
display in the advanced search form on. The configuration resides on the block
so this can differ across views / displays if need be. Additionally if the View
the block was derived from has multiple contextual filters you can choose which
one corresponds to direct children, this will enable the recursive search
checkbox.

![screenshot of advanced search block configuration](./assets/advanced_search_advanced_search_block_settings.png)

# Extending Islandora: The Islandora Cookbook

Because newer Islandora is very tightly integrated with Drupal, many use cases that required custom Islandora modules in Islandora Legacy and earlier can now be solved with a combination of configuration and modules from the Drupal community. Other use cases may still require custom code, but are not common enough, or complete enough, for that custom code to be a part of the core software.

Both scenarios are covered by the Islandora Cookbook below, which contains an annotated list of custom Islandora tools and modules developed by our community, a list of Drupal contributed modules that may be useful in an Islandora context, and a set of _recipes_ which each provide a detailed outline for solving a particular use case.

Islandora is also compatible with most Drupal themes.

## Introduction

In the spirit of [Islandora Awesome](https://github.com/Islandora-Labs/islandora_awesome), Islandora Cookbook is a curated list of great modules and other tools for Islandora. Because Islandora is more tightly integrated with Drupal, most of these 'recipes' require only Drupal modules as ingredients.

We offer this list for discovery, but do not officially provide support for any of these modules.

## Recipes

- [Amazon Alexa](recipes/alexa-search.md) - This recipe explains how to access Islandora with Amazon Alexa.
- [Collection Searching](https://github.com/Islandora-Labs/Islandora-Cookbook/blob/main/recipes/collection_search.md) -
This recipe provides instructions how to configure a shallow or deep search in a specific collection.
- [Date Range Slider Facet](https://github.com/Islandora-Labs/Islandora-Cookbook/blob/main/recipes/date_range_slider_facet.md) - This recipe explains how to include a facet for date field(s) that presents itself as a range slider.
- [Exhibitions and TimeLine JS](https://github.com/Islandora-Labs/Islandora-Cookbook/blob/main/recipes/exhibitions.md) - This recipe outlines how to build exhibits in Islandora, including the deployment of TimeLineJS.
- [Digital Preservation](https://github.com/mjordan/digital_preservation_using_islandora) - A detailed overview of how to approach digital preservation in Islandora. Includes features that are currently available, both within Islandora and by using compatible tools.

## Ingredients

Below are modules and tools that might be useful to solve common use cases, presented without specific recipes.

### Islandora contributed modules

Warning! - All modules are under development.

* [Basic Ingest](https://github.com/discoverygarden/basic_ingest) - Basic repository item ingest improvements.
* [Islandora RipRap](https://github.com/mjordan/islandora_riprap) - Fixity auditing
* [Islandora Repository Reports](https://github.com/mjordan/islandora_repository_reports) - Graphical reports on various aspects of an Islandora repository.
* [Islandora Bagger](https://github.com/mjordan/islandora_bagger) - Utility to generate Bags for objects using Islandora's REST interface using either a command-line tool or via a batch-oriented queue.
* [Islandora Citations](https://github.com/discoverygarden/islandora_citations) - An alternative to Citation Select (bundled with Starter Site). It uses CSL to render citations, allows you to set a default (which is rendered on page load), and lets you set the field mappings as a third party setting when editing each field itself. It works with Typed Relation fields (using the relation to set the CSL parameter such as author, editor, or contributor), and also works with Paragraphs.
* [Islandora RDM](https://github.com/roblib/islandora_rdm) - An entire suite of tools and documentation to turn Islandora into a fully functional Research Data Management platform.
* [Islandora Whole Object](https://github.com/mjordan/islandora_whole_object) - Islandora module that provides some Drupal blocks containing various representations of an Islandora object.
* [IP Range Access](https://github.com/mjordan/ip_range_access) - A context condition to check the user's IP address against a range, and provide 403 if not accepted.
* [Typed Relation with Display Name](https://github.com/rosiel/typed_relation_display_name/) - A field type that's a Typed Relation field plus a display name that is unique to the instantiation. Allows you to transcribe what's on the object, while still linking to a controlled form of a name.
* [Linked Data Lookup Field](https://www.drupal.org/project/linked_data_field) - allows you to autocomplete from external authorities. It creates a two-part field and stores the label and the URI. It is extensible and can be made to work with more APIs. Currently it supports Library of Congress Subject Headings, Global Research Identifier Database (GRID) entries, and Australian and New Zealand Standard Research Classification Fields of Research.

### Access Control

#### Access control by taxonomy tags

[Permissions by Term](https://www.drupal.org/project/permissions_by_term)

By default, Drupal allows you only to restrict access to Drupal nodes by coupling node content types to user roles. The Permissions by Term module extends Drupal by functionality for restricting view and edit access to single nodes via taxonomy terms. Since Islandora objects can have taxonomy terms, this can be used to control access at the node and collection level. The submodule Permissions by Entity extends this control to the media level.

#### Setting a date and time for publication

[Moderation Scheduler](https://www.drupal.org/project/moderation_scheduler)

Moderation Scheduler gives content editors the ability to schedule Islandora nodes to be published at specified dates and times in the future. Can be combined with [Views Bulk Edit](https://www.drupal.org/project/views_bulk_edit) to set the same scheduled publication date/time on multiple nodes at once.

#### Tombstoning

[Tombstones](https://www.drupal.org/project/tombstones)

A module that informs users who attempt to view deleted content that the resource has been removed instead of showing a generic 404 page. Fielded so that the date, reason, and alternative links can be given, along with a citation for the deleted object.

### Displays

#### Image slideshow

[Views Slideshow](https://www.drupal.org/project/views_slideshow)

Views Slideshow can be used to create a slideshow of any content (not just images) that can appear in a View. Powered by jQuery, it is heavily customizable: you may choose slideshow settings for each View you create. It can be used to create an easy, adjustable slideshow of images from an Islandora repository.

#### CSV File Formatter

[CSV File Formatter](https://www.drupal.org/project/csvfile_formatter)

This module provides a formatter that displays CSV files in the browser.

#### Timelines

[TimelineJS](https://www.drupal.org/project/views_timelinejs)

Integration with TimelineJS to create timeline displays.

#### Sort titles without initial articles (The, A, etc)

[Views Natural Sort](https://www.drupal.org/project/views_natural_sort)

This module adds a new sort option to Content views that sorts while skipping a configurable list of initial articles like "The", "A", "An", "L'", etc. Note that it works only with Content views and not with Search API (solr) views.

### Ingest

#### Batch uploading via CSV

[Migrate Islandora CSV](https://github.com/Islandora-CLAW/migrate_islandora_csv)

This repository is a tutorial that will introduce you to using the Drupal 8 Migrate tools to create Islandora content. Whether you will eventually use CSVs or other sources (such as XML or directly from a 7.x Islandora) this tutorial should be useful as it covers the basics and mechanics of migration.

This repository is also a Drupal Feature that, when enabled as a module, will create three example migrations ready for you to use with the Migrate API.

#### Alternative way to batch upload via CSV

[Islandora Workbench](https://github.com/mjordan/islandora_workbench)

Command-line tool for ingesting (and updating) nodes and media from anywhere - you don't need to access to the Drupal server's command line. Provides robust data validation, flexible organization of your input data (can use CSV, Google Sheets, or Excel files) plus creation of taxonomy terms on the fly.


### Content Management

#### Batch editing

[Views Bulk Edit](https://www.drupal.org/project/views_bulk_edit)

A powerful tool that turns Views into a means of batch editing nodes, including Islandora repository objects. Once installed, create a view, add the fields that you would like to edit, add a `Views bulk operations (Edit)` field to the view, and select which actions you would like to have available. The `Modify field values` action will allow you to batch edit the value for the same field across multiple objects. A demonstration of a simple bulk-editing view with a few fields and actions can be found [here](http://future.islandora.ca/islandora-batch-edit)

#### Clone a content type

[Content Type Clone](https://www.drupal.org/project/content_type_clone)

A tool that allows you to clone an existing content type. Can be used to copy and easily make your own version of the Repository Item Content Type with fewer or edited fields, without starting over. Has options to also copy all nodes from the old type to the new, and to delete from the old type when copying.

#### Prevent orphaned entity relationships

* [Entity Reference Integrity](https://www.drupal.org/project/entity_reference_integrity)
* [Entity Reference Integrity Extras](https://github.com/discoverygarden/entity_reference_integrity_extra)
* [Entity Reference Purger](https://www.drupal.org/project/entity_reference_purger)

Normally when deleting content (nodes, taxonomy terms, etc), any content that references the deleted entity isn't altered so you end up with orphan/zombie references, which are visible in the JSON representation but invisible otherwise. With Entity Reference Integrity's submodule (`entity_reference_integrity_enforce`), you won't be able to delete content that's referenced from elsewhere, preserving your database integrity.

Because the Drupal module doesn't play with Typed Relation fields, DGI's `entity_reference_integrity_extras` module is useful. 

Alternatively, Entity Reference Purger takes care of deleting the references on entity delete. Warning: it is unknown whether Entity Reference Purger respects whether the user has permission to edit the referencing entity/field. There are open issues that it does not work well with Workflow (see Content Mangagement Workflows below) or translation. The module is not covered by the security badge.

#### Content Management Workflows

[Content moderation](https://www.drupal.org/project/content_moderation)

This module lets you set up workflows that "transition" content between "states", which may be published or unpublished. It also allows you to set revisions as mandatory (normally an editor can decide to make an edit without creating a revision, making it very hard to track for auditing).

#### Shared content between sites

[Entity Share](https://www.drupal.org/project/entity_share)

Share entities between different Drupal instances. Works with nodes, taxonomy terms, media, etc.

#### External content

[External Entities](https://www.drupal.org/project/external_entities)

Lets you use sources of content external to Drupal as though they were internal.

#### Paragraphs for structured/hierarchical content

[Paragraphs](https://www.drupal.org/project/paragraphs)

Paragraphs is based on Entity Reference Revisions and allows you to create an on-the-fly entity with structured fields. Paragraphs could be used for complex titles with title type, subtitle, part name, etc. They can be used for grouping fields together that are, as a block, repeatable. 

### Richer Content

#### Displaying equations and formulae

[MathJax](https://www.drupal.org/project/mathjax)

This module is a plug-and-play solution for rendering [LaTeX](https://www.latex-project.org/). It uses a CDN so no library has to be installed on your system, and the processing is done in the user's browser. In terms of Drupal, it provides a text filter that must be enabled in at least one text format (such as Full HTML). With this you can enter mathematical formulae in abstracts! Need to recognize unfamiliar symbols? Try this [LaTeX symbol reference](https://oeis.org/wiki/List_of_LaTeX_mathematical_symbols) thanks to the On-Line Encyclopedia of Integer Sequences.


### Search

#### Custom search weighting

[Search Overrides](https://www.drupal.org/project/search_overrides)

This module provides a method for users with the necessary permissions to manually override the results being returned by Search API Solr. They will be able to choose a specific search term, and pick which nodes should be at the top, and also choose to exclude nodes so they will not be shown in the results. Currently, only nodes are supported.

#### Render field as link to faceted search

[Entity Reference Facet Link](https://www.drupal.org/project/entity_reference_facet_link)

Provides a field formatter that points an entity reference field to a facet search for that value. Could be used in search result displays with taxonomy terms (for example) to stay within the "search ecosystem". Does not work with Typed Relation fields.

### Auditing

### Logging administrative "events"

[Events Logging](https://www.drupal.org/project/events_logging)

This module provides a separate log to record "events" such as the creation, editing, and deletion of content (actually any entities you configure it to, including config entities). This can provide a log of who did what, when. The log is not mixed in with the "Watchdog"/"Recent log entries" log, but still uses the database (unless you have another log method enabled). 

When content is updated, the log only says that it was updated but does not say how or provide a diff. It does not seem to have the ability to link log messages to the revisions that may have been created during edits. 

### Other

#### Gather user feedback

[Content Feedback](https://www.drupal.org/project/content_feedback)

The Content Feedback module allows users and visitors to quickly send feedback messages about the currently displayed content, and can be applied to Islandora nodes. All content feedback messages are listed and grouped by status in an administrative feedback list.

#### Sending your content to Archive.org

[Wayback Submit to Archive.org](https://www.drupal.org/project/wayback_submit_archive)

A tool for automatically submitting the contents of your site to Archive.org. The Wayback Submit module will submit all node types on schedule, according to criteria set by the site admin (only certain node types, only certain views, etc).

#### Adding anti-bot protection to your site

[Turnstile](https://www.cloudflare.com/application-services/products/turnstile/)

Turnstile provides an alternative to Captchas for filtering out bot traffic.

Turnstile can be implemented via a [Drupal module](https://www.drupal.org/project/turnstile_protect) or, for sites using Docker, as a [Traefik plugin](https://github.com/libops/captcha-protect).

## Other resources

- [Drupal Contributed Modules](https://www.drupal.org/docs/8/modules)
- [Drupal Contributed Themes](https://www.drupal.org/docs/8/themes)

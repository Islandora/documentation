# Collections

Collections are groups of related content that can be viewed or managed as a unit. Islandora-specific use cases include:

- an archival fonds that needs to be grouped together, with internal hierarchy
- various collections of artifacts, grouped for display
- theses and dissertations, which are organized and managed separately from other objects.

Islandora provides:

- a generic "member of" relationship field (field_member_of) to group objects under a "parent" object. This mechanism is shared by _Collections_ with similar use cases: [Paged Content](../user-documentation/paged-content.md) and _Compound Objects_. Islandora on its own does not prescribe any particular [Content Type](../user-documentation/content_types.md), so this field can be configured for any [node bundle](https://www.drupal.org/docs/drupal-apis/entity-api/bundles) intended to represent Islandora resources.
- a "Children" tab on resources, which provides a management interface to access, re-order, add, or delete the members of a resource.

## Collection configuration provided by Islandora Defaults

[Islandora Defaults](../reference/islandora_defaults_reference.md) is an optional collection of presets for Islandora, intended to provide a more user-friendly out-of-the-box experience and starting point for more specific customization. Islandora Defaults provides:

- a [Content Types](../user-documentation/content_types.md) "Repository Item" that uses the "member_of" field, so that users may add nodes of this type to a collection (or paged content, or compound resource),
- logic (a [Context](../user-documentation/context.md)) such that if a resource is tagged as a "collection", a view of its members will show on the collection's page. 

For more details, see the tutorial on [How to create and add to a collection](../tutorials/how-to-create-collection.md)

## Bulk management of members of a collection

Bulk management of items can be done using the Drupal contrib module [Views Bulk Edit](https://www.drupal.org/project/views_bulk_edit). In short, build a view using this module, and you will be able to perform Drupal Actions on sets of objects. Neither Islandora nor Islandora Defaults provide out-of-the-box management tools, but the sandbox provides some sample content and views that use Views Bulk Edit. 

For more information see the video tutorial on [Batch Editing](https://youtu.be/ZMp0lPelOZw).





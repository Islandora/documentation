# Collections

Collections are groups of related content that can be viewed or managed as a unit.  Islandora-specific use cases include:

- an archival fonds that needs to be grouped together, with internal hierarchy
- various collections of artifacts, grouped for display
- theses and dissertations, which are organized and managed separately from other objects.

Islandora provides:

- a field storage called "Member of" (field_member_of) that can be used to declare items to be members of other resources.
- a "Children" tab on resources, which provides a management interface to access, re-order, add, or delete the members of a resource. 

Islandora on its own does not prescribe any particular content types, so this field storage can be implemented on any node bundle inteneded to represent Islandora resources. Islandora provides a generic "member of" relationship field to group objects under a "parent" object. This mechanism is shared by similar use cases: Paged Content and Compound Objects.

## Bulk management of members of a collection

Bulk management of items can be done using the Drupal contrib module [Views Bulk Edit Drupal module](https://www.drupal.org/project/views_bulk_edit). In short, build a view using this views field, and you will be able to perform Drupal Actions on objects. Neither Islanodra nor Islandora Defaults provide out-of-the-box management tools, but the sandbox provides some sample content and views that use Views Bulk Edit. 

For more information see [forthcoming page] Concept: Bulk Edit

## Configuration provided by Islandora Defaults

Islandora (Defaults) provides:

- a content type ("Repository Item") that has the "member_of" field, so that users may add nodes of this type to a collection (or paged content, or compound resource),
- logic (a context) such that if a resource is tagged as a "collection", a view of its members will show on the collection's page. 

For more details, see the tutorial on  [How to create and add to a collection](../tutorials/how-to-create-collection.md)

## Tutorials

- [How to create and add to a collection](../tutorials/how-to-create-collection.md)






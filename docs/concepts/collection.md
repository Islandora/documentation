# Collections

Collections are groups of related content that can be viewed or managed as a unit.  Islandora-specific use cases include:

- an archival fonds that needs to be grouped together, with internal hierarchy
- various collections of artifacts, grouped for display
- theses and dissertations, which are organized and managed separately from other objects.

Islandora can:

- declare items as members of other objects
- after declaring an item "a collection", display a view of its members

A minimal installation of Islandora (Islandora Core) does not provide pre-configured options to perform management operations on all objects of a collection. Bulk actions such as deleting, changing visibility or adjusting permissions on all objects in a collection can be achieved with the [Views Bulk Edit Drupal module](https://www.drupal.org/project/views_bulk_edit) and other Drupal contrib modules, such as [Permissions By Term](https://www.drupal.org/project/permissions_by_term). The Islandora sandbox comes with pre-configured views for managing groups of objects. 


## Configuration provided by Islandora Defaults
Islandora (Defaults) provides:

- A membership relationship for members (“children”) to point to their parent collection
- A View (template) to display the members of a collection on the collection’s page
- Batch-editing tools that can be used on collections

Islandora uses a generic "member of" relationship field to group objects under a "parent" object. This mechanism is shared by Paged Content and by Compound Objects.

In Islandora Defaults, when content is given the Model of “Collection”, then a Context kicks in (the context is called “Collection”) that causes a view block (the View’s name is Members) to appear on the page. 

## Tutorials

- [How to create and add to a collection](../tutorials/how-to-create-collection.md)

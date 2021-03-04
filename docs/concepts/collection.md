# Collections

Collections are groups of related content that can be viewed or managed as a unit.  Islandora-specific use cases include:

- an archival fonds that needs to be grouped together, with internal hierarchy
- various collections of artifacts, grouped for display
- theses and dissertations, which are organized and managed separately from other objects.

Islandora:

- provides a field storage called "Member of" (field_member_of) that can be used to declare items to be members of other resources.
- provides a "Children" tab on resources, which provides a management interface to access the members of a resources. 

Islandora on its own does not prescribe any particular content types, so this field storage can be implemented on any node bundle inteneded to represent Islanodra resources.

## Configuration provided by Islandora Defaults
Islandora (Defaults) provides:

- A content type with the "member_of" field that can be used to add a resource to a collection (or paged content, or compound resource)
- logic (a context) such that if a resource is tagged as a "collection", a view of its members will show on the collection's page. 

Islandora uses a generic "member of" relationship field to group objects under a "parent" object. This mechanism is shared by Paged Content and by Compound Objects.

In Islandora Defaults, when content is given the Model of “Collection”, then a Context kicks in (the context is called “Collection”) that causes a view block (the View’s name is Members) to appear on the page. 

Islanodra defaults also provides configuration so that:

 - 

Islandora sandbox provides sample content and demonstration views of collection members, as well as some administrative views using the [Views Bulk Edit Drupal module](https://www.drupal.org/project/views_bulk_edit) to provide tools to manage members of a collection. 



## Tutorials

- [How to create and add to a collection](../tutorials/how-to-create-collection.md)

# Collections

Collections are groups of related content that can be viewed or managed as a unit. Islandora-specific use cases include:

- an archival fonds that needs to be grouped together, with internal hierarchy
- various collections of artifacts, grouped for display
- theses and dissertations, which are organized and managed separately from other objects.

## Islandora features

Islandora provides:

- a mechanism for grouping nodes under a "Parent" node through the generic "Member Of" relationship field (`field_member_of`). This mechanism is also used by _[Paged Content](../user-documentation/paged-content.md)_ and _Compound Objects_. Islandora on its own does not prescribe any particular [Content Type](../user-documentation/content_types.md), so this field can be configured for any [node bundle](https://www.drupal.org/docs/drupal-apis/entity-api/bundles) intended to represent Islandora resources.
- a "Children" tab on resources, which provides a management interface to access, re-order, add, or delete the members of a resource based on the Member Of field.
- a "Model" field (`field_model`) which can take various values including "Collection".

## Islandora Starter Site features

[Islandora Starter Site](https://github.com/Islandora/islandora-starter-site) is an optional set of presets for Islandora, intended to provide a more user-friendly out-of-the-box experience and starting point for more specific customization. 

Islandora Starter Site provides:

- a [Content Type](../user-documentation/content_types.md) "Repository Item" that uses the `field_member_of` field, so that users may add nodes of this type to a collection (or paged content, or compound resource),
- A [View](../tutorials/create_update_views.md) showing the members of the collection.
- logic (a [Context](../user-documentation/context.md)) such that if a resource is tagged as a "Collection" (in the "Model" field, then a view of its members will show on the collection's page.

For more details, see the tutorial on [How to create and add to a collection](../tutorials/how-to-create-collection.md)

## Bulk management of members of a collection

Bulk management of items can be done from a Views Bulk Operations-compatible View, including the main Drupal Content View (at admin/content) and the Manage Members View (at node/[node]/members). This is possible due to the Drupal contrib modules [Views Bulk Operations (VBO)](https://www.drupal.org/project/views_bulk_operations) and [Views Bulk Edit](https://www.drupal.org/project/views_bulk_edit). VBO allows you to perform Drupal Actions on objects. These Actions include making Islandora Derivatives such as "Audio - Generate a Service File from an Original File" and also to perform Drupal core Actions such as publishing/unpublishing content. The Views Bulk Edit module extends VBO and allows you to edit field values (such as the Member Of field, which would change which collection a group of nodes are in). 

For instance, if you want to move a number of nodes from one collection into another, then you can 

- select them all in a View such as the Members view of a collection
- select "Edit content" from the Action dropdown, and click "Apply to selected Items"
- under "SELECT FIELDS TO CHANGE", select "Member of"
- in the Member Of autocomplete that appears, type the name of the target collection
- Careful! Under "Change method", choose between "Replace the current value" and "Add a new value to the multi-value field". The first one will wipe out ALL existing collections including the one you're working on - which may be what you're after.
- Select "Confirm" at the bottom of the form to save your changes.

For more information see the video tutorial on [Batch Editing](https://youtu.be/ZMp0lPelOZw).


## Deleting A Collection

!!! Warning "Deleting Collections creates orphans."
    When you delete a Collection object, it will be deleted from the "Member of" field of all its Children. This means that it will not be possible to track down the children after the Collection has been deleted. If you intend to move the children, see above. If you intend to delete the children, this must be done before-hand. This is a [known issue](https://github.com/Islandora/documentation/issues/628).

!!! Warning "Deleting content creates orphan media."
    Deleting content (Collections or otherwise) will NOT delete any attached Media. This is a [known issue](https://github.com/Islandora/documentation/issues/909).


## Permissions on a Collection

No reliable access control methods currently allow you to assign people to administer and manage only specific collections.

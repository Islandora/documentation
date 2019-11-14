Collections are groups of repository content that can be described themselves. Repository content is aggregated in a collection by setting the `field_member_of`
entity reference field to point to a collection. Generally speaking, anything can behave as a collection, and anything with `field_member_of` can be a member of a collection.
However, collections are 'officially' designated as such by applying the "Collection" taxonomy term to an item's `field_model` field.  
By default, Islandora 8 provides a context configured to display a list of child resource node teasers when the current resource node uses that term.

!!! Tip "Collections and Deleting"
    Collections and their members are
    independent of each other, and removing something from a collection does not delete it.  **Similarly, deleting a
    collection does not delete its members.**

## Creating a Collection

From the front page, click on Add content. This is under Tools.

![Click on add content, under tools](../assets/collections_add_content.jpg)

Then click on 'Repository Item' to give your collection the default metadata profile for Islandora 8.

![Click on Repository Item](../assets/collections_repository_item.jpg)

Fill out the form.

Near the end of the form, under System, select Collection from the Model dropdown list.

![Near end of form, under System, select collection](../assets/collections_system.jpg) 

Click Save when done.

![The collection has been created. Now let's add some members to this collection.](../assets/collections_parent_node.jpg)

The collection has been created. Now let's add some members to this collection.

## Add Existing Items to a Collection

To populate a collection with existing items, return to any existing content and click on its Edit tab. This brings up the form for this item.

![To populate a collection with existing items, return to any existing content and click on its Edit tab.](../assets/collections_edit_photo_collection.jpg)

Scroll down to the bottom and the System section. In the Member of section, start typing in the name of the collection this item should belong to. Select the name of the collection 
you want from the autocomplete.

![Scroll down to the bottom and the System section. In the Member of section, start typing in the name of the collection this item should belong to.](../assets/collections_member_of.jpg)

The correct collection is now selected.  Click Save when ready.

![The correct collection is now selected. Click Save when ready.](../assets/collections_member_of_selected.jpg)

To confirm, return to the collection and verify the new item appears in the collection's 'Members' block.

![To confirm, return to the collection and verify the new item appears in the collection's 'Members' block.](../assets/collections_snowfall.jpg)

## Add a New Item as a Member of a Collection

To create an item and add it as a member to a collection in one step, visit a collection and click on its `Members` tab. From the
`Members` tab, you can manage the members of a collection and perform actions on them.

![From the 'Members' tab, you can manage the members of a collection and perform bulk operations on them.](../assets/resource_nodes_children_tab.png)

Click on the `+Add member` button, and then select 'Repository Item' to give your new item a Content Type.  Only content types that
have the `field_member_of` field will be available from this list.

![Click on the '+Add member' button, and then select 'Repository Item' to give your new item a Content Type.](../assets/collections_select_content_type.png)

You are taken to the creation form for a Repository Item, but if you scroll down to the `System` section, you should see the widget
for 'Member Of' is already filled out for you with the appropriate collection.

![You should see the widget for 'Member Of' is already filled out for you with the appropriate collection.](../assets/collections_member_of_selected.jpg)

Click 'Save' at the end of the form to create the new item and add it as a member of the collection.

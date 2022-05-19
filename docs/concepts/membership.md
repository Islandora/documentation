# Membership (field_member_of)

Islandora provides a field, `field_member_of`, which is designed to link nodes to other nodes in a containment relation. For example, it can mean membership in a collection, membership of a page within a book, or membership in a complex object. While it is not necessary to use this field for all containment relationships, some things only work if this field is present on your content type.

!!! warn "Mandatory Field"
    In order for a node to be considered an "Islandora object" and for the "Media" and "Children" tabs to appear on the node, this field must be present on that node's content type.


The "Children" tab is provided by Islandora, and provides managers a View of objects that are, through `field_member_of`, members of the current node (i.e. "children"). The Children tab also has links to add children, individually or in bulk. When children are added, it creates nodes where `field_member_of` is auto-populated to the current node. Islandora condition plugins also reference `field_member_of`, including "Node has parent". 

How a node presents its children to the public is left to the implementation. For example, Islandora Defaults provides a view of collection members that shows up on nodes tagged Collection (in [`field_model`](islandora-models.md)), and a view of pages in a multipaged OpenSeadragon that shows up on nodes tagged "Paged Content" as well as "OpenSeadragon". 

The breadcrumbs use this field but is almost exposed configurable. the context condition is configurable if you can edit configs. 

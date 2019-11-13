As we learned in the [introduction](user-intro.md), objects in an Islandora repository are 
represented as a combination of resource nodes, media, and files in Drupal.
Because of this, their metadata profile, display, form (and much more) are configurable through
the Drupal UI.  This gives repository administrators a huge degree of control over their repository
without any need for coding. Much more so than ever before. And since we're using a core Drupal
solution for modeling our resource nodes and media, compatibility with third-party modules is virtually guaranteed.
This opens up a plethora of solutions from the Drupal community that will save you untold time
and effort when implementing your repository with Islandora.

## Properties

Resource nodes have some basic properties that are common to all nodes, regardless 
of its content type.  These properties are not fields.  This means that their settings cannot be
changed and they cannot be removed. Their name, what type of data they hold, etc... are all baked in.
Here's an example of the basic properties you'll see on a node:

```
nid: 1
uid: 1
title: "I am an Islandora 8 object"
created: 1550703004
changed: 1550703512
uuid: 02932f2c-e4c2-4b7e-95e1-4aceab78c638
type: islandora_object
status: 1
```

As you can see, it's all system data used at the Drupal level to track the basics.

Property | Value
------------ | -------------
nid | The local ID for the node
uuid | The global ID for any entity
title | The title for the node
created | Timestamp of when the node was created
changed | Timestamp of when the node was last updated
type | Content type (e.g. which group of fields are present on the node)
status | Published, unpublished, etc...

!!! note "Compared to Islandora 7"
    These node properties directly correspond to following Islandora 7.x object properties:

    Islandora 7 | Islandora 8
    ----------- | -----------
    owner | uid
    dc.title | title
    PID | uuid
    status | status

## Fields

In addition to the basic node properties identified above, resource nodes (like all Drupal nodes) can have fields.
Most of what we would think of as descriptive metadata is stored as fields. Resource nodes use 'content types' to define a specific set of required and optional
fields it has; we can think of content types as metadata profiles for our objects.
For example, you might have a content type for a set of repository objects that have very specialized metadata requirements but
another content type for generic repository objects that share a more general set of metadata fields.
A resource node's content type is set on its creation and is immutable.
The section on [metadata](metadata.md) describes in more detail how fields on Islandora objects work.

Islandora has a notion of a _content model_, which is used to identify what type of content is
being represented by a node (e.g. an image, a video, a collection of other items, etc...). This is done
using a special field, _Model_, which accepts taxonomy terms from the _Islandora Models_ vocabulary.
By applying a term from the Islandora Models vocabulary to a node, Islandora will become aware
of how to handle the node in response to certain events, like choosing a viewer or generating derivatives.

![Model tags](../assets/objects_model_tags.png)

!!! note "Compared to Islandora 7"
    Content models in Islandora 7 were immutable and contained restrictions as to what
    types of datastreams could be associated with an object.  Islandora 8 imposes no such
    restrictions. Content models can be changed at any time, and they in no way dictate what
    types of media can be associated with a node.

## Media

All resource nodes can be linked to any number of media.  The media associated with a resource node can be managed using the "Media" tab when viewing a node.  Much like
the "Members" tab, Actions can be performed in bulk using the checkboxes and Actions dropdown.

![Media tab](../assets/objects_media_tab.png)

See [the media section](media.md) for more details.

## Display Hints

Drupal uses "View Modes" to provide alternative ways to present content to users. You may be familiar with the "full" and "teaser" versions of nodes, which are rendered using two corresponding kinds of View modes. Islandora also uses view modes to control how media content is displayed. Islandora provides two view modes for Media, one which renders the OpenSeadragon viewer and the other which renders the PDFjs viewer. These two View modes can be set explicitly in the node edit form or you can configure Islandora to use a specific View mode for all media with a specific Mime type using [Contexts](context.md).

To set it explicitly on the resource node's edit form, simply check the View mode you want to use for that node in the _Display hints_ field:

![Display hints](../assets/objects_display_hints.png)

The selected View mode will then be used when the resource node's page is rendered.

At a global level, there are a couple of ways to tell Drupal to use the PDFjs viewer to render the content of the media field whenever the media has a Mime type of `application/pdf`.

The first way is to edit the "PDFjs" Context. By default, this Context tells Drupal to use the PDFjs viewer if the node has the term "PDFjs" (yes, that's a taxonomy term):

![Default PDFjs Context](../assets/objects_pdfjs_context_default.png)

If you add the Condition "Media has Mime type" and configure it to use `application/pdf` as the Mime type, like this:

![PDFjs Context with Mimetype Condition](../assets/objects_pdfjs_context_with_mimetype.png)

Context will use whichever Condition applies (as long as you don't check "Require all conditions"). That is, if the "PDFjs" display hint option in the node edit form is checked, *or* if the node's media has a Mime type of `application/pdf`, the media content will be rendered using the PDFjs viewer.

The second way to use the media's Mime type to render its content with the PDFjs viewer is to create a separate Context that will detect the media's Mime type and use the configured View mode automatically. To do this, create a new Context. Add a "Media has Mime type" condition and specify the Mime type, and then add a "Change View mode" Reaction that selects the desired view mode:

![Display hints](../assets/objects_view_mode_context.png)

Finally, save your Context. From that point on, whenever the media for a node has the configured Mime type, Drupal will render the media using the corresponding view mode.

The node-level and global approaches are not exclusive to one another. One Context can override another depending on the order of execution. Whichever Condition applies last between the node-level Condition (which in this case is the "Node has term" condition) the global Condition (which is "Media has Mime type"), that one will override the other. An example of having the View mode specified in the node edit form intentionally override the View mode based on Mime type is to have media with the `image/jp2` mime-type configured to use to use the OpenSeadragon viewer, but to manually select the OpenSeadragon view mode for nodes with JPEG media (for example, a very large JPEG image of a map, where the OpenSeadragon's pan and zoom features would be useful).

## Members

Islandora has a notion of _membership_, which is used to create a parent/child relationship between
two resource nodes. Any two nodes can be related in this way, though typically, the parent node has a content
model of _Collection_ or _Paged Content_. Membership (to a Collection or otherwise) is denoted using another special
field, "Member Of".  The "Member Of" field _can_ hold multiple references, so it is possible for a
single child to belong to multiple parents, but may also complicate the creation of breadcrumbs.

!!! Note "Compared to Islandora 7"
    In Islandora 7, there was a distinction between belonging to a collection and belonging to
    a compound object.  In Islandora 8, this distinction is not present, essentially allowing every 
    resource node to be part of a compound object.

!!! Note "Child v. Member"
    Islandora 8 uses the "child" and "member" descriptor for resource nodes that 
    store a reference to another resource node in the "Member Of" field interchangeably. 
    Administrators will more often see the "member" terminology more often while 
    front-end users will usually see "child" terminology.

For any node, its **Children** tab can be used to see all its members.  You can also perform Actions in
bulk on members using the checkboxes and the Actions dropdown as well as clicking
on the **Re-order Children** tab to adjust the order in which they display.

![Members tab](../assets/objects_children_tab.png)

Drupal enables site to display a resource node's child resource nodes in a multitude of ways, usually
centered around constructing and theming views. By default collections show child components as a simple list
of child teasers which can be configured @ `/admin/structure/views/view/members`.

### Paged Content

Paged content, books, periodicals, photographs with the front and back, etcetera, can use the members structure.
This involves creating a resource node for the root record (e.g. the book or the photograph)
and child resource nodes for each sub-component (e.g. "Page 1" and "Page 2" or "recto" and "verso")
with their corresponding media. Each "child" resource node contains a reference to their "parent" resource node
using the `field_member_of` property.

Similar to the collection view showing members of a collection, Islandora provides a 
view to produce IIIF Manifest listing all the child resource nodes' service files
to a block displaying an OpenSeadragon viewer which is displayed only on resource
nodes with _Paged Content_ selected as their Islandora Model. However, repository
managers can use any method they like, such as a slide-show, to display child resource nodes.

By default child resource nodes are un-ordered. To order the sub-components of a
paged content resource node, Islandora provides a _Weight_ field to store a 
integer value on  child resource nodes. 
Children resource nodes with smaller weight values will come
before child resource nodes with larger weight values. This follows the usual ordering
pattern of `1` (smaller) coming before `2` (larger). 

Weight values do not need 
to be sequential, just ordered from smallest to largest. For example, the first 
child resource node can have a value of `10` and the next could have a value of 
`20` and they will be ordered accordingly. Should a new child be added with the 
weight value of `15` it will automatically be sorted after the child with the
weight value `10` and before the child with the weight value `20`.

Child resource nodes can be reordered using a drag-and-drop interface by clicking
on the _Re-order Children_ button on the _Children_ tab of the parent resource node.

![Re-order Children button](../assets/objects_reorder_children_button.png)

![Re-order Children form](../assets/objects_reorder_children_form.png)

Re-ordering children resource nodes on this page and clicking _Save_ will cause 
each child resource node's weight value to be updated using sequential values.

!!! note "Why not Drupal's book or weight modules?"
    Drupal provides the [book module in core](https://www.drupal.org/docs/8/core/modules/book)
    for creating multi-level ordered content, such as books and manuals.
    However, this module stores structure and pagination separately from the nodes making serializing
    these relationships as RDF we can provide to Fedora more difficult than simply using `field_member_of`
    with an RDF mapping. Support for the book module may be added in the future.
    
    Drupal also has a [weight module](https://www.drupal.org/project/weight) 
    that provides a weight field and a drag-and-drop
    reordering user-interface. However, this module requires users to set a 
    specified range of values which includes their negative corresponding value. 
    E.g. a range setting of '20' will require all children to have a value between 
    '-20' to '20'. This presumes a repository manager can predict how many pages
    the largest paged content item in their repository will be beforehand. Also,
    these weight values are serialized into RDF using the Collections Ontology 'index'
    predicate which assumes positive integer values which cannot be guaranteed
    using the weight module.

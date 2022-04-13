# From Objects to Resource Nodes: Shifting Concepts from Islandora Leagcy to Islandora

This document attempts to show the shift in how objects work from Islandora Legacy to Islandora, from a non-developer perspective.

## Traditional objects become resource nodes

The conventional Islandora Legacy definition of an object is a file loaded in the repository with associated derivatives. In Islandora Legacy, objects (video files, audio files, PDFs, etc.) are loaded through the user interface, and Datastreams are generated automatically. These consist of access and display copies, the metadata, OCH/HOCR, technical metadata, and more. All of these Datastreams are directly connected to the object and accessed through the admin interface.

In Islandora, the traditional Islandora Legacy objects (video files, audio files, etc. that were represented in different content models) are now Drupal nodes. Islandora object nodes are a special kind of Drupal node, distinct from nodes that exist for other content types such as a blog post, an article, a page (like the About page on a site), and others. These Islandora objects are still loaded through the interface and described with the data entry form, and derivatives are still generated. However, the Datastreams are no longer connected to the original object in the same immutable way. Each of these Datastreams can be manipulated through Drupal by non-developers. You can create a variety of ways to view this metadata and information related to the objects. Doing so requires knowledge of Drupal 8, but this essentially means that there are many ways to view the metadata and access the related objects in Islandora.

In Islandora it is therefore helpful to think of objects as resource nodes. The term reflects the new nature of objects in Islandora. A resource node does not just refer to the individual object file, but encompasses multiple elements that all relate to each other, even if they are no longer directly connected like objects in Islandora Legacy.

The typical elements of a resource node:

-   A content type defining metadata fields defined for the node. A content type may include any number of custom fields defined to store descriptive metadata about the object represented by the node. To function as an Islandora resource node, a content type must define two further fields:
    - A field denoting the 'type' of thing represented by the node (image, book, newspaper, etc.). The value of this field is used by Islandora to control views, derivative processing, and other behavior.[^1]
	- A field in which to record the node's [membership](resource-nodes.md#members) in another node. If populated, this field creates a hierarchical relationship between parent (the node recorded in the field) and child (the node in which the parent is recorded). This may be left empty, but is required for building hierarchies for collections, subcollections, and members of collections, as well as objects (books, "compound objects", etc.) consisting of [paged content](paged-content.md).[^2]
-   Media files (the actual files of JPEGs, MP3s, .zip, etc.) that get loaded through the form
-   Derivative files (thumbnails, web-friendly service files, technical metadata, and more)

These resource nodes are what the librarian, student, archivist, technician, or general non-developer creates through the data entry form. It is possible to configure all elements of a resource node in Islandora through Drupal. This fact allows control over how one accesses the node and how nodes are displayed and discovered online by non-developers. It also allows a repository to take full advantage of all third-party Drupal modules, themes, and distributions available.

## More Information

The following pages expand on the concepts discussed above:

- [Resource Nodes](resource-nodes.md)
- [Media](media.md)
- Content Types: [Metadata](metadata.md#content-types) -- [Create / Update a Content Type](content_types.md)

### Copyright and Usage

This document was originally developed by [Alex Kent](https://github.com/alexkent0) and has been adapted for general use by the Islandora community.

[![CC BY-NC 4.0](https://mirrors.creativecommons.org/presskit/buttons/88x31/svg/by-nc.svg)](https://creativecommons.org/licenses/by-nc/4.0/)

[^1]: In `islandora_defaults`, this is the `field_model` field, which is populated by taxonomy terms in the `islandora_models` taxonomy vocabulary provided by the `islandora_core_feature` submodule of `Islandora/islandora`

[^2]: In `islandora_defaults`, this is the `field_member_of` field.

# From Objects to Resource Nodes: Shifting Concepts from Islandora 7 to 8

This document attempts to show the shift in how objects work from Islandora version 7 to version 8, from a non-developer perspective.

## Traditional objects become resource nodes

The conventional Islandora 7 definition of an object is a file loaded in the repository with associated derivatives. In Islandora 7, objects (video files, audio files, PDFs, etc.) are loaded through the user interface, and Datastreams are generated automatically. These consist of access and display copies, the metadata, OCH/HOCR, technical metadata, and more. All of these Datastreams are directly connected to the object and accessed through the admin interface.

In Islandora 8, the traditional Islandora 7 objects (video files, audio files, etc. that were represented in different content models) are now Drupal nodes. Islandora object nodes are a special kind of Drupal node, distinct from nodes that exist for other content types such as a blog post, an article, a page (like the About page on a site), and others. These Islandora objects are still loaded through the interface and described with the data entry form, and derivatives are still generated. However, the Datastreams are no longer connected to the original object in the same immutable way. Each of these Datastreams can be manipulated through Drupal by non-developers. You can create a variety of ways to view this metadata and information related to the objects. Doing so requires knowledge of Drupal 8, but this essentially means that there are many ways to view the metadata and access the related objects in Islandora 8.
 
In Islandora 8 it is therefore helpful to think of objects as resource nodes. The term reflects the new nature of objects in Islandora 8. A resource node does not just refer to the individual object file, but encompasses multiple elements that all relate to each other, even if they are no longer directly connected like objects in Islandora 7.

The typical elements of a resource node:

-   Content types (one or more)
-   Media files (the actual files of JPEGs, MP3s, .zip, etc.) that get loaded through the form
-   Metadata fields submitted in the data entry, for example:
    -   A field denoting the 'type' of thing you're persisting (image, book, newspaper, etc...)
    -   A field that creates the familiar collection hierarchy
    -   Descriptive custom fields
-   Derivative files (thumbnails, web-friendly service files, technical metadata, and more)

These resource nodes are what the librarian, student, archivist, technician, or general non-developer creates through the data entry form. It is possible to configure all elements of a resource node in Islandora 8 through Drupal. This fact allows control over how one accesses the node and how nodes are displayed and discovered online by non-developers. It also allows a repository to take full advantage of all third-party Drupal modules, themes, and distributions available.

## More Information

The following pages expand on the concepts discussed above:

- [Resource Nodes](resource-nodes.md)
- [Media](media.md)
- [Content Types](content_types.md)

### Copyright and Usage

This document was originally developed by [Alex Kent](https://github.com/alexkent0) and has been adapted for general use by the Islandora community. 

[![CC BY-NC 4.0](https://mirrors.creativecommons.org/presskit/buttons/88x31/svg/by-nc.svg)](https://creativecommons.org/licenses/by-nc/4.0/)

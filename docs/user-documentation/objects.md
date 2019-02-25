## What is an Islandora 8 object?

### Object structure and properties

Islandora 7 has the concept of an "object" that is based on the Fedora 3.x object model: an object has properties such as an owner, date created, date modified, and status, and each object contains a set of files (known as "datastreams") identified by datastream IDs, such as RELS-EXT, DC, MODS, and OBJ. The relationships between the object and its datastreams is highly constrained: datastreams cannot exist without a parent object, and each object can have only one DC datastream, one RELS-EXT datastream, and so on.

In Islandora 8, we can also talk about "objects", but the underlying relationships between a collection of properties and its associated files differ substantially from those that make up an Islandora 7.x object. An object in Islandora 8 is a Drupal node, along with associated "media" (which is Drupal 8's name for files and their properties; media are described in detail [elsewhere](datastreams.md).) For example, an Islandora 8 object created by the Drupal user "admin" (user ID 1) on February 24, 2019, has the following properties:

```
userid: 1
title: "I am an Islandora 8 object"
created: 1550703004
changed: 1550703512
uuid: 02932f2c-e4c2-4b7e-95e1-4aceab78c638
type: islandora_object
status: 1
```

These are Drupal node properties, which all Drupal nodes have (although not all Drupal nodes have a type of "islandora_object", some have a type of "page" or "article" or "some_custom_content_type"). These node properties directly correspond to an Islandora 7.x object's properties:

Islandora 7.x object properties | Islandora 8.x node/object properties
------------ | -------------
owner | userid
dc.title | title
PID | uuid
cmodel | type
status | status

In addition to these basic node properties, Islandora 8 objects (like all Drupal nodes) can have fields, which is where most of what we would think of as metadata is stored. The section on [metadata](metadata.md) describes how fields work.

### Media

If Drupal nodes are the Islandora 8 equivelant of Islandora 7.x objects, "media" are the equivalent of Islandora 7.x datastreams. In Drupal 8, media are wrappers around files (images, audio files, video files, XML files, etc.) that provide information about the files, such as their MIME type, size, created data, etc. Like Drupal nodes, media can have fields and tags. One important set of tags defined by Islandora is the "Media Use" vocabulary, and it is this vocabulary that defines what is the closest to what we know in Islandora 7.x as datastream IDs. Tags from this vocabulary include "Original File" (analogous to Islandora 7.x' OBJ datastream ID), "Preservation master", "Thumbnail" (analogous to the TN datastream ID), "FITS File", and "Extracted text" (analogous to the OCR datastream ID).

Unlike Islandora 7.x objects, Islandora 8 objects do not know what media are attached to themselves; instead, each media knows what Drupal node it is associated with. In other words, the relationship bewtween nodes and media is defined from the media's perspective, not the node's perspective. From an admin user's perspective, all the media associated with an Islandora 8 object are listed in the object's "Media" tab, which appears alongside its "View", "Edit", and "Delete" tabs:

![Media tab](../assets/media_tab.png)


From a general user's perspective, the media in Islandora 8 are rendered within the parent node just like they are rendered within the parent object in Islandora 7.x. Additional information about media in Islandora 8 is availble [elsewhere](datastreams.md).

### Fedora

Islandora 7.x basically inherits its object model from Fedora 3.x. In 7.x, Fedora stores all properties and content associated with an object - not only its owner, dc.title, status, PID, and status, but also any content files such as OBJ, DC, MODS, and RELS-EXT. In Islandora 7.x, Fedora is the authoritative, primary source for all aspects of an object. Fedora 3.x is not an optional component of an Islandora 7.x repository, it is the primary datastore.

In Islandora 8, using Fedora is optional. That's right, optional. Drupal, and not Fedora, is the primary source of all aspects of an Islandora 8 object, and, with some variations, Drupal, not Fedora, is the primary datastore in an Islandora repository. If Fedora is present in an Islandora 8 repository, content in it is a tightly syncronized copy of object properties and files managed by Drupal.

Even though Fedora is optional in Islandora 8, most repositories will use it since it provides its own set of services that are worth taking advantage of, such as:

* flexible, and configurable, disk storage architecture
* fixity digest generation
* Momento versioning
* integration with RDF/Linked Data triplestores
* integration with microservices
* access control mechanisms

In Islandora repositories that use Fedora, all properties about Drupal nodes are mirrored in Fedora as RDF properties. But, even if an Islandora instance does not use Fedora, Drupal can provide an object's properties as RDF (again, Drupal is the primary source of data in Islandora 8). In addition, the Drupal media associated with Islandora 8 objects are persisted to Fedora, although exactly which media is configurable within the Islandora 8 admin interface. Just as Drupal out of the box has a public and private filesystem, Islandora adds a third filesystem to Drupal called, not surprisigly, "fedora", and it is to this filesystem that media are persisted. We will provide more information about Fedora's role in an Islandora 8 repository in the [metadata](metadata.md) and [media](media.md) sections.


### Pre March 2019 sprint notes below

although most Islandora instances will use it). If an Islandora 8 site uses Fedora, Fedora stores copies of some (or optionally all) of an object's media and also some of statements about the object. These statements are stored in Fedora as RDF, and include things like creator, date modified, date updated, and so on. Even if an Islandora instance does not use Fedora, Drupal can provide an object's properties as RDF.

An object has one or more media associated with it. Unlike in Islandora 7.x, where each object is aware of all of its datastreams, in Islandora 8, the object doesn't know what media are associated with it. The relationship is the other way around: each media knows what node it is attached to. The relationship that defines a media's parent object is expressed by Drupal as .. and within Fedora, as ...

todo: insert a diagram.

## What is an Islandora object?

!!! todo
    * It’s a node, with maybe one or more medias and files attached (see “Datastreams are Media” section)
    * The node has metadata in its fields (See "Metadata are Fields" section.)
    * It has a URI, and a JSON-LD view in Drupal
    * It goes into Fedora. (See todo make "In Fedora" section)
    * By Drupal default, all published nodes are public, so see todo make Access Control section.
    * As a Drupal node, it’s available to Views and gets indexed in Solr.
    * We suggest using a taxonomy term on the node to control elements of its display or editing, e.g. in Islandora Demo there is a taxonomy term that sets the type: Audio, Binary, Collection, Image, and Video
    * How do I make an Islandora object? (Make a node, then add a Media)


!!! tip "7.x migration note"
    #### Where are my Solution Packs?
    If you are familiar with Islandora 7.x, a solution pack is a bundle representing objects of a certain 'type', that are associated with metadata forms, an ingest workflow that allows specific filetypes, derivative settings based on the uploaded object, an optional content viewer, and the look and feel of an object's page including what metadata to display to the user.

    In CLAW, all of these can be configured independently, often based on tags (taxonomy terms) attached to the nodes or media objects. You may, if desired, create separate bundles (content types) analogous to the Content Models in 7.x, however that is not necessary.

    Using the Features module, it is possible to bundle the configuration of a content type, as well as the configuration around it, as a Feature so that it can be exported and deployed on other sites. An example of this is the Islandora Demo module, which actually bundles several content "types" - Audio, Binary, Collection, Image, and Video. todo: see more at islandora demo?

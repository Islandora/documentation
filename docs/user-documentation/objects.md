## What is an Islandora 8 object?

### Object structure and properties

Islandora 7 has the concept of an "object" that is based on the Fedora 3.x object model: an object has properties such as an owner, date created, date modified, and status, and each object contains a set of files (known as "datastreams") identified by datastream IDs, such as RELS-EXT, DC, MODS, and OBJ. The relationships between the object and its datastreams is highly constrained: datastreams cannot exist without a parent object, and each object can have only one DC datastream, one RELS-EXT datastream, and so on.

In Islandora 8, we can also talk about "objects", but the underlying relationships between a collection of properties and its associated files differ substantially from those that make up an Islandora 7.x object. An object in Islandora 8 is a Drupal node, along with associated "media" (which is Drupal 8's name for files and their properties; media are described in detail [elsewhere](datastreams.md).) For example, an Islandora 8 object created by the Drupal user "admin" (user ID 1) on February 24, 2019, has the following properties:

```
uid: 1
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
owner | uid
dc.title | title
PID | uuid
content model | a tag from the Islandora Models vocabulary
status | status

Islandora's 7.x's content models do not exist in Islandora 8. The primary way that Islandora 8 identifies what we think of as a conetent model in Islandora 7.x is using taxonomy terms from the "Islandora Models" vocabulary. To indicate that an Islandora 8 object is an image, video, etc, the user selects a model from a select list:

![Media tab](../assets/object_model_tags.png)

One implication of assigning a "content model" using a Drupal vocabulary is that in Islandora 7.x, an object's content model is immutable, but since it is possible to change the value of a taxonomy term assigned to a node, in Islandora 8, an object's model can be changed easily.

### Drupal Content Types as Islandora Metadata Profiles

In addition to the basic node properties identified above, Islandora 8 objects (like all Drupal nodes) can have fields, which is where most of what we would think of as descriptive metadata is stored. Since a specific set of fields is assigned to a Drupal content type, we can create different "metadata profiles", or in other words groups of required and optional fields, as different Drupal content types. For example, you might have a content type for a set of repository objects that have very specialized metadata requirements but another content type for generic repository objects that share a more general set of metadata fields. The section on [metadata](metadata.md) describes in more detail how fields on Islandora objects work.

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
* Memento versioning
* integration with RDF/Linked Data triplestores
* Integration with Microservices via API-X
* WebAC Policies for access control

In Islandora repositories that use Fedora, all properties about Drupal nodes are mirrored in Fedora as RDF properties. But, even if an Islandora instance does not use Fedora, Drupal can provide an object's properties as RDF (again, Drupal is the primary source of data in Islandora 8). In addition, the Drupal media associated with Islandora 8 objects are persisted to Fedora, although exactly which media is configurable within the Islandora 8 admin interface. Just as Drupal out of the box has a public and private filesystem, Islandora adds a third filesystem to Drupal called, not surprisigly, "fedora", and it is to this filesystem that media are persisted. We will provide more information about Fedora's role in an Islandora 8 repository in the [metadata](metadata.md) and [media](media.md) sections.



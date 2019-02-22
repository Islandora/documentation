## What is an Islandora 8 object?

Islandora 7 has the concept of an "object" that is based on the Fedora 3.x object model: an object has properties such as an owner, date created, date modified, and status, and each object contains a set of files (known as "datastreams"), such as RELS-EXT, DC, MODS, and OBJ. The relationships between the object and its datastreams is highly constrained: datastreams cannot exist without a parent object, and each object can have only one DC datastream, one RELS-EXT datastream, and so on.

In Islandora 8, we can also talk about "objects", but the underlying relationships between a collecion of properties and its associated files differ substantially from those that make up an Islandora 7.x object as we visualize it. An object in Islandora 8 is a Drupal node, along with associated "media" (which is Drupal 8's name for files and their associated properties). For example, a node created by the Drupal user "admin" (user ID 1) on February 24, 2019, has the following properties:

```
schema:author <http://localhost:8000/user/1?_format=jsonld>
schema:dateCreated <>
dcterms:title "I am an Islandora 8 object"@en
```

In Islandora 8, using Fedora is optional (although most Islandora instances will use it). If an Islandora 8 site uses Fedora, Fedora stores copies of some (or optionally all) of an object's media and also some of statements about the object. These statements are stored in Fedora as RDF, and include things like creator, date modified, date updated, and so on. Even if an Islandora instance does not use Fedora, Drupal can provide an object's properties as RDF.

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

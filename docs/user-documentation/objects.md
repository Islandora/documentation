Islandora has the concept of an "Object", a bundle of files and their associated metadata. It's a remnant of the Islandora 7.x model, which was grounded in the Fedora 3.x object model, but we haven't entirely dropped it because our community still relies on this concept.

However, in CLAW, an object is distributed. In Drupal it is distributed over a graph of related Drupal Entities, and in Fedora it is distributed over a number of Fedora "objects" including RDF and non-RDF sources.

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
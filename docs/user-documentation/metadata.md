# Metadata are stored in Fields

!!! note "Drupal 8 terminology"
    In Drupal 8, Fields can be attached to _entity sub-types_ (e.g. Content types, Vocabularies) or _entities_ (Users, Files).
    For more on Fields, see ["2.3 Content Entities and Fields"](https://www.drupal.org/docs/user_guide/en/planning-data-types.html) and
    ["6.3 Adding Basic Fields to a Content Type"](https://www.drupal.org/docs/user_guide/en/structure-fields.html) in the Official Guide.


!!! todo
    * In CLAW, metadata associated with an object are stored natively in Drupal, in _fields_.
    * Islandora maps fields can be mapped to RDF (and must be if you want them to get into Fedora).
    * It is recommended to store descriptive metadata at the Node level, and technical metadata at the Media level.
    * Everything except "Title" is configurable, and you can edit it however you want
    * Islandora Demo provides an example of this
    * In Islandora Demo, we reserve Nodes for "Islandora Objects", the things that we associate with some file that requires preservation. We use Taxonomy terms for modelling simple or complex people, places, subjects, and organizations.
    * How do I configure what fields exist? (go to __Manage > Structure > Content Types__ and click Manage fields)
    * How do I configure what fields show up on the display? (Go to Manage > Structure > Content Types and click Manage Display)
    * How do I configure what display mode gets selected? (Rules/Context and taxonomy terms? Views?)
    * How do I configure what fields show up in the edit form? (Go to Manage > Structure > Content Types and click "Manage Form Display")
    * How do I configure what form mode gets displayed? (??)
    * We provide some cool fields that extend Drupal's built-in field types (in what module?)
        * ETDF Date
        * Typed Relation


## Fields

All normal field types are available to use for Islandora, including text, textarea,
 * do we need to warn against using HTML in textareas, in cases where it will be exported to Fedora?

### Custom field: EDTF Date

### Custom field: Typed Relation


## Mappings

Fields get mapped to RDF in (a yml file). See the RDF and Fedora section.

## Islandora demo

TODO: document the fields and display modes in Islandora Demo.

!!! tip "7.x Migration Note"
    #### What about my XML?
    In 7.x, metadata were (usually) stored within XML datastreams such as MODS or DC. In CLAW we suggest breaking out
    individual metadata elements into fields attached to the node (or to an associated entity, if appropriate). The Metadata Interest Group is developing a default mapping which will provide a basic, yet customizable, transform between MODS metadata and Drupal fields in Islandora Demo.

     It is still possible to attach an XML file to a CLAW object as a Media (see Datastreams), however there is no mechanism in CLAW
    for editing XML in a user-friendly way.
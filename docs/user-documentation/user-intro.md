This _User Documentation_ section is aimed at site admins and repository managers who need to understand and configure their Islandora.
It will go in depth on how Islandora allows you to use the various features of Drupal to construct and display repository items.

Islandora, like Drupal, provides tools to create a site, but does not force you to conform to any specific site structure,
organization, or navigation. There is a hope that we can provide something useful out of the box, while also allowing the
full suite of Drupal configuration options. This out-of-the-box configuration is the Islandora Demo module.

It is recommended to be familiar with the basics of Drupal, including content types, fields, users, and views.
The [Official Drupal 8 User Guide](https://www.drupal.org/docs/8) and the
[Community Guide to Drupal 8](https://www.drupal.org/docs/user_guide/en/index.html) are a good place to start.

## Object Modeling

In Islandora, when we say _object_, we mean a collection of properties describing something.
And when we say _datastream_, we mean a file that is a digital representation of an object.
Content in our repositories is stored as an object that is associated with any number of datastreams.
In Drupal terms, that makes everything an _entity_, where

- An object is a _node_ (a.k.a. _content_).
- Nodes have properties that can be configured called _fields_.
- Fields for nodes are grouped together as _content types_.
- Datastreams are _media_, which are _files_ that can have their own fields and _media types_.
- Metadata used to categorize entities are _taxonomy terms_, which also have their own fields and _vocabularies_.
They can represent everything from simple labels to more complex concepts such as people, places, and subjects.

## Fedora

Islandora 7.x basically inherits its object model from Fedora 3.x. In 7.x, Fedora stores all properties and content associated with an object - not only its owner, dc.title, status, PID, and status, but also any content files such as OBJ, DC, MODS, and RELS-EXT. In Islandora 7.x, Fedora is the authoritative, primary source for all aspects of an object. Fedora 3.x is not an optional component of an Islandora 7.x repository, it is the primary datastore.

In Islandora 8, using Fedora is optional. That's right, optional. Drupal, and not Fedora, is the primary source of all aspects of an Islandora 8 object, and, with some variations, Drupal, not Fedora, is the primary datastore in an Islandora repository. If Fedora is present in an Islandora 8 repository, content in it is a tightly synchronized copy of object properties and files managed by Drupal.

Even though Fedora is optional in Islandora 8, most repositories will use it since it provides its own set of services that are worth taking advantage of, such as:

* flexible, and configurable, disk storage architecture
* fixity digest generation
* Memento versioning
* integration with RDF/Linked Data triplestores
* Integration with Microservices via API-X
* WebAC Policies for access control

In Islandora repositories that use Fedora, all properties about Drupal nodes are mirrored in Fedora as RDF properties. But, even if an Islandora instance does not use Fedora, Drupal can provide an object's properties as RDF (again, Drupal is the primary source of data in Islandora 8). In addition, the Drupal media associated with Islandora 8 objects are persisted to Fedora, although exactly which media is configurable within the Islandora 8 admin interface. Just as Drupal out of the box has a public and private filesystem, Islandora adds a third filesystem to Drupal called, not surprisingly, "fedora", and it is to this filesystem that media are persisted. We will provide more information about Fedora's role in an Islandora 8 repository in the [metadata](metadata.md) and [media](media.md) sections.


## Architecture
 
### Conceptual Diagram
 
Many users of Islandora may be familiar with the metahprocial diagram of Islandora 7 as a cheeseburger, which provides a memorable approximation of how the different parts of the software stack interact in a vertically-integrated, relatively customizable fashion (ie, Drupal, Solr, Islandora, and Fedora are stable layers, and the "toppings" stand in for Solution Packs and other utilities that can be added or removed to customize Islandora):
 
![Islandora 7 as a cheeseburger](../assets/user-intro-cheeseburger.png)
 
For a similar conceptual approach to Islandora 8, we present it as a bento box: a very modular platform, in which each piece may be removed and replaced with something different, without disrupting other parts of the stack:
 
![Islandora 8 as a bento box](../assets/user-intro-bento.png)
 
For a true diagram of how the various parts of Islandora 8 interact, please see the full [Architecture Diagram](/technical-documentation/diagram.md)




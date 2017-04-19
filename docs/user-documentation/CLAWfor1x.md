# What's Different Between Islandora 1.x and Islandora CLAW

In the most basic terms, Islandora CLAW is the version of Islandora that works with [Fedora 4](https://wiki.duraspace.org/display/FEDORA4x/Fedora+4.x+Documentation). Because Fedora 4 is a vastly different platform than Fedora 3, so too is Islandora CLAW a major departure from what came before. Switching to CLAW represents not just a typical upgrade with improvements, features, and bug fixes, but rather a major shift in how objects are stored and managed. 

Moving from Islandora 7.x-1.x to CLAW requires a migration of objects, which you can learn about [here](../migration/migration.md). It also requires some adjustments in how you think about your objects and their relationships, and how to manage them in Islandora, which we will cover below.

You can also check out some of the documentation provided by the Fedora project:
* [Concept Mapping - Fedora 3 to 4](https://wiki.duraspace.org/display/FEDORA4x/Concept+Mapping+-+Fedora+3+to+4)
* [The Fedora 4 object model](https://wiki.duraspace.org/display/FEDORA4x/The+Fedora+4+object+model)
* [Fedora 3 to 4 Upgration](https://wiki.duraspace.org/display/FF/Fedora+3+to+4+Upgration)
* [LDP-PCDM-F4 In Action](https://wiki.duraspace.org/display/FEDORA4x/LDP-PCDM-F4+In+Action)

## Fedora 

### Repository Structure

Fedora 3 stored all objects at the top level of the repository, although presentation of the objects could mimic a directory structure by having objects 'in' collections and collections 'in' other collections. This image is a helpful oversimplification:

![image](https://cloud.githubusercontent.com/assets/2371345/10912108/525c2a0e-821e-11e5-9c5b-d853b62f1e5a.png)

Fedora 4 differs considerably in that there is an innate tree hierarchy to the repository rather than a flat structure. Put less simply, "[a Fedora 4 repository consists of a directed acyclic graph of resources where edges represent a parent-child relation](https://wiki.duraspace.org/display/FEDORA4x/The+Fedora+4+object+model)".

### Object Structure
Fedora 3 objects are FOXML (Fedora Object eXtensible Markup Language) documents, with three elements:

* `Digital Object Identifier`: A unique, persistent identifier for the digital object. Also knowns as the PID.
* `System Properties`: A set of system-defined descriptive properties that is necessary to manage and track the object in the repository.
* `Datastream(s)`: The element in a Fedora digital object that represents a content item.

In Fedora 4 , what we would have called `objects` are now referred to as `resources` and are not composed of XML; instead, they are stored in ModeShape as nodes with RDF properties. They can contain the following elements:

* `Resource`: Roughly equivalent to a Fedora 3 object - a conceptual representation of a thing that can contain files or other containers.
* `Non-RDF Source`: Roughly equivalent to a datastream. A Non-RDF Source (or binary) is simply a bitstream (e.g. JPG, PDF, XML, MP3, etc.).

### Datastreams
In Islandora CLAW, RDF datastreams (RELS-EXT and RELS-INT) are stored as RDF in Fedora. Binary datastreams are files or `nonRdfResources` (see [PCDM](https://github.com/duraspace/pcdm/wiki)). Descriptive metadata datastreams (MODS, DC, DwC, PBCore, etc) are stored as RDF.

#### PIDs
Every object in a Fedora 3 repository had a Persistent Identifier following the pattern `namespace:pid`. Fedora 4 resources do not have PIDs. Instead, since Fedora 4 is an [LDP server](https://www.w3.org/ns/ldp), their identifiers are fundamentally their URIs. The PIDs of objects migrated from a Fedora 3 repository can still be stored in Fedora 4, as additional properties on the new Fedora 4 resource.

Since resources are stored as `nodes` on the Drupal side of Islandora CLAW, they also have Drupal UUIDs.

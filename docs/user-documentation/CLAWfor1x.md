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

In Fedora 4 , what we would have called `objects` are now referred to as [`Resources`](https://www.w3.org/TR/ld-glossary/#resource) (and *everything* in Fedora 4 is a `Resource`). Instead of being composed of XML as they were in Fedora 3, they are stored in [ModeShape](http://modeshape.jboss.org/) as nodes with RDF properties. A `Resource` in Islandora CLAW may [contain](https://www.w3.org/TR/ldp/#dfn-containment) RDF data or binary files, similar to the way Islandora 7.x-1.x objects stored descriptive metadata and binary files in datastreams.

Unlike Islandora 7.x-1.x objects that store metadata and binary files in a predefined way depending on the content model, Islandora CLAW uses [Linked Data Platform Containers](https://www.w3.org/TR/ldp/#dfn-linked-data-platform-container), or LDPCs, to allow resources to contain each other in a flexible way. LDPCs allow one `Resource` to act as a collection of other `Resources` similar to the way an Islandora 7.x-1.x collection contains objects, or objects contain datastreams. When part of a `Resource`, binary files (such as JPG, PDF, MP3, etc) are referred to as [`Non-RDF Sources`](https://www.w3.org/TR/ldp/#dfn-linked-data-platform-non-rdf-source) because their content is not RDF data. `Resources` that contain only RDF data are called [`RDF Sources`](https://www.w3.org/TR/ldp/#ldpr-resource). 

CLAW makes use of the [Portland Common Data Model (PCDM)](https://github.com/duraspace/pcdm/wiki) as a layer of abstraction over LDPCs to make containment simpler to understand for users; a `pcdm:Collection` may contain other `pcdm:Collections` or `pcdm:Objects` (similar to an Islandora 7.x-1.x collection content model), and a `pcdm:Object` may contain other `pcdm:Objects` (similar to the way an Islandora 7.x-1.x compound object has child objects) or `pcdm:Files` (similar to the way Islandora 7.x-1.x objects have datastreams).

### Datastreams
In Islandora 7.x-1.x, every object has a specific content model which defined what datastreams it could have and which were absolutely required. Some of these Islandora 7.x-1.x datastreams contained metadata about the object (RELS-EXT, RELS-INT, DC, MODS, PREMIS, etc) while others contained binary files (JPG, PDF, MP3, PNG, TIFF, etc). In Islandora CLAW, all metadata about a resource is stored as RDF attributes directly on the resource itself, whether that resource is a `pcdm:Collection`, `pcdm:Object` or a `pcdm:File`, so we no longer need to separate metadata by type (MODS, DC, PREMIS, etc) and store it in binary files as we did in Islandora 7.x-1.x. 

Binary files, such as JPGs, PNGs, MP3s, and PDFs, are handled via `pcdm:Files` which are contained by a parent `pcdm:Object`, similar to how an Islandora 7.x-1.x cmodel may hold a PDF or JPG as a datastream. Unlike Islandora 7.x-1.x, these binary files can actually have their own technical metadata attached them. This is because `pcdm:Collections`, `pcdm:Objects` and even `pcdm:Files` are all `RDF Sources` containing only RDF data, with `pcdm:Files` having links to the URL of the `Non-RDF Source` (binary file) they represent as part of their RDF data in addition to whatever other metadata you may want about the file. Using this system, a `pcdm:Object` can contain as many `pcdm:Files` as necessary, and each `pcdm:File` can have separate metadata about itself and its relationship to other `pcdm:Files` attached to the parent `pcdm:Object`.

#### PIDs
Every object in a Fedora 3 repository had a Persistent Identifier following the pattern `namespace:pid`. Fedora 4 resources do not have PIDs. Instead, since Fedora 4 is an [LDP server](https://www.w3.org/ns/ldp), their identifiers are fundamentally their URIs. The PIDs of objects migrated from a Fedora 3 repository can still be stored in Fedora 4, as additional properties on the new Fedora 4 resource.

Since resources are stored as `nodes` on the Drupal side of Islandora CLAW, they also have Drupal UUIDs.

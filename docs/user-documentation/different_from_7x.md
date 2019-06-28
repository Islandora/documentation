# What's Different Between Islandora 1.x and Islandora 8.x

In the most basic terms, Islandora 8 is the version of Islandora that works with [Fedora 5](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home). Because Fedora 4 and 5 are vastly different than Fedora 3, so too is Islandora 8 a major departure from what came before. Switching to Islandora 8 represents not just a typical upgrade with improvements, features, and bug fixes, but rather a major shift in how objects are stored and managed.

Moving from Islandora 7.x-1.x to 8.x-1.x requires a migration of objects, which you can learn about [here](../migration/migration.md). It also requires some adjustments in how you think about your objects and their relationships, and how to manage them in Islandora, which we will cover below.

You can also check out some of the documentation provided by the Fedora project:
* [Concept Mapping - Fedora 3 to 4](https://wiki.duraspace.org/display/FEDORA4x/Concept+Mapping+-+Fedora+3+to+4)
* [The Fedora 5 object model](https://wiki.duraspace.org/display/FEDORA5x/The+Fedora+object+model)
* [Fedora 3 to 4 Upgration](https://wiki.duraspace.org/display/FF/Fedora+3+to+4+Upgration)
* [LDP-PCDM-F4 In Action](https://wiki.duraspace.org/display/FEDORA4x/LDP-PCDM-F4+In+Action)

## Fedora

### Repository Structure

Fedora 3 stored all objects at the top level of the repository, although presentation of the objects could mimic a directory structure by having objects 'in' collections and collections 'in' other collections. This image is a helpful oversimplification:

![image](https://cloud.githubusercontent.com/assets/2371345/10912108/525c2a0e-821e-11e5-9c5b-d853b62f1e5a.png)

Fedora 4/5 differs considerably in that there is an innate tree hierarchy to the repository rather than a flat structure. Put less simply, "[a Fedora repository consists of a directed acyclic graph of resources where edges represent a parent-child relation](https://wiki.duraspace.org/display/FEDORA5x/The+Fedora+object+model)".

### Object Structure
Fedora 3 objects are FOXML (Fedora Object eXtensible Markup Language) documents, with three elements:

* `Digital Object Identifier`: A unique, persistent identifier for the digital object. Also known as the PID.
* `System Properties`: A set of system-defined descriptive properties that is necessary to manage and track the object in the repository.
* `Datastream(s)`: The element in a Fedora digital object that represents a content item.

In Fedora 4/5 , what we would have called `objects` are now referred to as [`Resources`](https://www.w3.org/TR/ld-glossary/#resource) (and *everything* in Fedora 4/5 is a `Resource`). Instead of being composed of XML as they were in Fedora 3, they are stored in [ModeShape](http://modeshape.jboss.org/) as nodes with RDF properties. `Resources` come in two flavors: [`RDF Sources`](https://www.w3.org/TR/ldp/#ldpr-resource), which are `Resources` having only RDF data, and [`Non-RDF Sources`](https://www.w3.org/TR/ldp/#dfn-linked-data-platform-non-rdf-source), which are `Resources` that are binary files (HTML, PDFs, images, audio, video, etc). The terms [`RDF Source`] and [`Non-RDF Source`] both come from the [W3C's](https://www.w3.org/) [Linked Data Platform](https://www.w3.org/TR/ldp/) specification, which also defines the idea of [Linked Data Platform Containers](https://www.w3.org/TR/ldp/#dfn-linked-data-platform-container), or `LDPCs`. An `LDPC` is an `RDF Source` that functions as a collection of `Resources`, similar to the way Islandora 7.x-1.x compound objects exist only as a way to tie together its children. An `LDPC` may contain `Non-RDF Sources`, as well as other `RDF Sources` acting as `LDPCs`; you can have a container of containers just like how Islandora 7.x-1.x can have a collection of collections.

Islandora 8.x-1.x makes use of the [Portland Common Data Model (PCDM)](https://github.com/duraspace/pcdm/wiki) as a layer of abstraction over `LDPCs` to make containment simpler to understand for users; a `pcdm:Collection` may contain other `pcdm:Collections` or `pcdm:Objects` (similar to an Islandora 7.x-1.x collection content model), and a `pcdm:Object` may contain other `pcdm:Objects` (similar to the way an Islandora 7.x-1.x compound object has child objects) or `pcdm:Files` (similar to the way Islandora 7.x-1.x objects have datastreams).

### Datastreams
In Islandora 7.x-1.x, every object has a specific content model which defined what datastreams it could have and which were absolutely required. Some of these Islandora 7.x-1.x datastreams contained metadata about the object while others contained binary files (JPG, PDF, MP3, PNG, TIFF, etc). In Islandora 8.x-1.x, all metadata about a resource is stored as RDF attributes directly on the resource itself, whether that resource is a `pcdm:Collection`, `pcdm:Object` or a `pcdm:File`, so we no longer need to separate metadata by type (RELS-EXT, RELS-INT, MODS, DC, PREMIS, etc) and store it in binary files as we did in Islandora 7.x-1.x.

Binary files, such as JPGs, PNGs, MP3s, and PDFs, are handled via `pcdm:Files` which are contained by a parent `pcdm:Object`, similar to how an Islandora 7.x-1.x cmodel may hold a PDF or JPG as a datastream. Unlike Islandora 7.x-1.x, these binary files can actually have their own technical metadata attached them. This is because `pcdm:Collections`, `pcdm:Objects` and even `pcdm:Files` are all `RDF Sources` containing only RDF data, with `pcdm:Files` having links to the URL of the `Non-RDF Source` (binary file) they represent as part of their RDF data in addition to whatever other metadata you may want about the file. Using this system, a `pcdm:Object` can contain as many `pcdm:Files` as necessary, and each `pcdm:File` can have separate metadata about itself and its relationship to other `pcdm:Files` attached to the parent `pcdm:Object`, serving the same purpose RELS-INT datastreams served in Islandora 7.x-1.x.

Note that you *can* use a `pcdm:File` to represent a file of metadata, such as MODS, DC, or PBCore, in case you would like to preserve a copy of an object's legacy metadata when migrating into Fedora 4. These metadata files will be treated like any other binary file in Islandora 8.x-1.x, and will not be indexed or editable through the GUI.

#### PIDs
Every object in a Fedora 3 repository had a Persistent Identifier following the pattern `namespace:pid`. Fedora 4/5 resources do not have PIDs. Instead, since Fedora 4 is an [LDP server](https://www.w3.org/ns/ldp), their identifiers are fundamentally their URIs. The PIDs of objects migrated from a Fedora 3 repository can still be stored in Fedora 4/5, as additional properties on the new Fedora 4 resource.

Since resources are stored as `nodes` on the Drupal side of Islandora 8.x-1.x, they also have Drupal UUIDs.

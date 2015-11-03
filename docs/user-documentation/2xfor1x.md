#Getting Started

##Repository Structure

Fedora 3 stored all objects at the top level of the repositry, although presentation of the objects could mimic a directory structure by having objects 'in' collections and collections 'in' other cllections.  in that there is an innate tree hierarchy to the repository rather than a flat structure. Put less simply, "a Fedora 4 repository consists of a directed acyclic graph of resources where edges represent a parent-child relation" [Fedora 4](https://wiki.duraspace.org/display/FEDORA4x/The+Fedora+4+object+model)

##Object Structure
A Fedora 3 digital object had three elements:

* Digital Object Identifier: A unique, persistent identifier for the digital object.
* System Properties: A set of system-defined descriptive properties that is necessary to manage and track the object in the repository.
* Datastream(s): The element in a Fedora digital object that represents a content item.

##

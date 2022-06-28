# RDF in an Islandora Repository

Islandora was build on the idea that a repository can be represented
in Drupal in a way that can be mapped to RDF and present the repository 
as Linked Data.

## Portland Common Data Model

Islandora uses the [Portland Common Data Model (PCDM)](https://github.com/duraspace/pcdm/wiki)
 to arrange elements
of the repository. Nodes correspond to pcdm:Objects, and Media to pcdm:Files.
The "Member of" field on nodes allows us to create pcdm:memberOf relationships
(the opposite of pcdm:hasMember),
and the "Media of" field represents pcdm:fileOf relationships (the opposite of
pcdm:hasFile). 

![PCDM object model](../assets/rdf-pcdm-coll-obj-file.png)


PCDM, plus additional metadata mappings, is the organizing model which allows us to create an RDF version 
of the repository in Fedora and/or in an RDF triplestore like Blazegraph.

## Mapping to RDF (RDF Module)

In Islandora, the RDF module provides a way to map metadata from 
fields into RDF. This provides a mechanism of using configuration 
entities (YAML files, as there is no robust UI) to map fields on nodes,
media, and taxonomy terms to RDF predicates.

Namepspaces in RDF must be registered before they can be used. The 
Islandora module registers a number of namespaces and more can be 
added using `hook_rdf_namespaces()`. See [RDF Mappings](../islandora/rdf-mapping.md#rdf-mappings) 
for more details.

## Exposure as JSON-LD

Nodes, Media, and taxonomy terms can have their RDF (per their mappings)
exposed to the world as RDF formatted in the JSON-LD syntax thanks to
the Islandora-built JSON-LD Drupal module. The JSON-LD module converts 
the RDF metadata, with some alterations, to a JSON-LD format that can be
consumed by RDF consumers such as Fedora and Blazegraph.

## Syncing to Fedora and Blazegraph

Islandora provides the pathways for objects and media in the repository
to be synced to Fedora and Blazegraph.

Objects are sent to Fedora and Blazegraph through an "Indexing" Drupal Action, which, after being 
put on a queue, is read by an indexer which pushes the JSON-LD information
to the appropriate target.

Files can be stored in Fedora directly, using the Flysystem module. Whether
or not a file is in Fedora, information about that file can be synced (from Drupal Media) into Fedora.

Neither Fedora nor Blazegraph are read as part of the standard Islandora configuration.

# Node Concepts

This page describes the core fields and features that Islandora uses to 
manage content as nodes in an Islandora repository.

## Nodes hold metadata

In Islandora, Drupal nodes are created to hold descriptive 
[metadata](../user-documentation/metadata.md) about content in the respository.
This metadata is held in the usual way for nodes, which is by populating Drupal fields. Fields are 
configured on a Drupal content types, which serve as metadata profiles.

## Nodes can have members/children

To build the capacity for creating hierarchical structures like collections, books and their pages, and
complex objects, Islandora introduces a special field, "Member of" (`field_member_of`) which must
be present on all Islandora content types. It enables a "Children" tab to display on Islandora nodes which 
lists the children of that node, and allows a repository manager to perform bulk operations on them.
It also enables a repository manager to create children of nodes directly, individually or in bulk.

## Nodes have models

Within a single content type (i.e. metadata profile), Islandora provides the ability to designate 
some objects as different "types" than others. Key behaviours, such as what [derivatives](derivatives.md) are created
or what [viewer](../user-documentation/file_viewers.md) is used, can be configured 
(see [Contexts](../user-documentation/context.md)) based on this value. The available values
are taxonomy terms in the Islandora Models vocabulary, and they are attached to nodes via the special 
mandatory field, "Model" (`field_model`), which must be present on all Islandora content types.
These values are installed through a Drupal Migration after the 
Islandora module is installed. All installation methods perform this migration, so out of the box,
 the following values should be available in the Islandora Models vocabulary:


| Name             	| External URI  	| 
|-------------------	|--------------------------------------------	|
| Audio             	| http://purl.org/coar/resource_type/c_18cc  	|
| Binary            	| http://purl.org/coar/resource_type/c_1843  	|
| Collection        	| http://purl.org/dc/dcmitype/Collection     	|
| Image             	| http://purl.org/coar/resource_type/c_c513  	|
| Video             	| http://purl.org/coar/resource_type/c_12ce  	|
| Digital Document  	| https://schema.org/DigitalDocument         	|
| Paged Content     	| https://schema.org/Book                    	|
| Page              	| http://id.loc.gov/ontologies/bibframe/part 	|
| Publication Issue 	| https://schema.org/PublicationIssue        	|
| Compound Object   	| http://vocab.getty.edu/aat/300242735       	|
| Newspaper         	| https://schema.org/Newspaper               	| 

With Islandora alone, choosing a value from this list will have zero effects. The contingent behaviour
must be configured during repository implementation. Islandora Defaults provides an example of what
behaviours are possible for these types. 


!!! note "The External URI field"
    This vocabulary, like many others in Islandora, includes an External URI field. This is intended to be used when transforming Islandora content into RDF, but also serves to make it easier to share configuration. Islandora provides code so that context conditions and derivative configs can be created without referencing the taxonomy term by ID, rather, using the taxonomy term's External URI. Since terms IDs are likely to change across sites, this makes our configs more portable. 

## Nodes are attached to Media

In an Islandora repository, the files in the repository are uploaded as Media, which are linked
to the node providing the descriptive metadata. Media belonging to a specific node can be found
in the Islandora-provided "Media" tab on that node. For more, see the [Media in Islandora] section.

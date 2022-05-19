# Islandora Models (field_model)

The Islandora provides a field, `islandora_model`, which allows you to select a type, from a taxonomy vocabulary, for your Islandora content. These types are designed to be machine-actionable, that is, certain derivatives may run or certain view modes may be triggered based on this value. 

!!! warning "Mandatory Field"
    In order for a node to be considered an "Islandora object" and for the "Media" and "Children" tabs to appear on the node, this field must be present on that node's content type. See "Membership" and "Media belonging to nodes" for more on those features. 


The Islandora module provides, by a migration, the values listed below into the Islandora Models vocabulary.


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

Further documentation of what these can do can be found under the Islandora Content Models section of the documentation. 

## The External URI field
This vocabulary, like many others in Islandora, includes an External URI field. This is intended to be used when transforming Islandora content into RDF, but also serves to make it easier to share configuration. Islandora provides code so that context conditions and derivative configs can be created without referencing the taxonomy term by ID, rather, using the taxonomy term's External URI. Since terms IDs are likely to change across sites, this makes our configs more portable. 


# OAI-PMH

The [Open Archives Initiative Protocol for Metadata Harvesting](https://www.openarchives.org/pmh/), commonly referred to as OAI-PMH, is a specification for exposing repository metadata for harvesting. OAI-PMH specifies six services which can be invoked over HTTP(s). The [full specification](http://www.openarchives.org/OAI/openarchivesprotocol.html) details the services:

| Service | URL on localhost:8000 |
|---|---|
| Identify |  [http://localhost:8000/oai/request?verb=Identify](http://localhost:8000/oai/request?verb=Identify)|
| ListMetadataFormats | [http://localhost:8000/oai/request?verb=ListMetadataFormats](http://localhost:8000/oai/request?verb=ListMetadataFormats)|
| ListSets | [http://localhost:8000/oai/request?verb=ListSets](http://localhost:8000/oai/request?verb=ListSets)|
| GetRecord | [http://localhost:8000/oai/request?verb=GetRecord&metadataPrefix=oai_dc&identifier=oai:localhost:node-1](http://localhost:8000/oai/request?verb=GetRecord&metadataPrefix=oai_dc&identifier=oai:localhost:node-1) |
| ListIdentifiers | [http://localhost:8000/oai/request?verb=ListIdentifiers&metadataPrefix=oai_dc](http://localhost:8000/oai/request?verb=ListIdentifiers&metadataPrefix=oai_dc)|
| ListRecords | [http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc](http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc)|

The module [Islandora OAI-PMH (`islandora_oaipmh`)](https://github.com/Islandora/islandora_defaults/tree/2.x/modules/islandora_oaipmh) is a submodule of Islandora Defaults. It provides default configuration for the [Drupal REST OAI-PMH module](https://www.drupal.org/project/rest_oai_pmh) so that repository content can be harvested.

Enabling Islandora OAI-PMH provides:

- a View ("OAI-PMH") that defines sets of objects for exposure at the oai_dc [endpoint](http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc). By default:
  - there is one set per "Collection" object containing that object's children
  - there is one set of all Repository Item objects that are not members of any Collection, and are not themselves Collections.	
  - disabled by default, there is a set of all Repository Item objects that are not Collections.
- Default configuration (at `/admin/config/services/rest/oai-pmh`) to connect this view with the REST OAI-PMH module.
- the ability to define additional sets by building additional views. Additional sets can be created by making views with the Entity Reference view display mode and enabling them on the REST OAI-PMH configuration page: /admin/config/services/rest/oai-pmh.

The REST OAI-PMH module indexes (caches) set membership, so new Items may not appear immediately in their respective sets. Indexing will happen automatically during cron runs but can be triggered manually at `/admin/config/services/rest/oai-pmh/queue`.

## OAI-DC Metadata Format/Metadata Mappings

The OAI-PMH module makes use of one of two modules to provide metadata mappings: the RDF module or the Metatag module. By configuring OAI-PMH to use the RDF module (appears as "OAI Dublin Core (RDF Mapping)" and is enabled by default with Islandora OAI-PMH), the OAI-PMH module will use the RDF mapping as configured for your content type (the same mapping that is used for Fedora and Blazegraph, e.g. rdf.mapping.node.islandora_object.yml). 

However,
- any field mappings that are not part of the Dublin Core namespace will be filtered out.
- any field mappings using Dublin Core Terms (e.g. http://purl.org/dc/terms/extent) will be mapped to their Dublin Core Elements equivalents (e.g. http://purl.org/dc/elements/1.1/format)
- the ability to vary the mappings in Linked Agent fields by relationship, used in JSON-LD mappings to Blazegraph and Fedora, is not available. If you want to expose data in a Linked Agent field to OAI-PMH, you must provide a DC mapping for that field in the RDF mapping. By default, the Repository Item RDF mapping does not include a mapping for the Linked Agent field.  


!!! tip "Field values not showing up in OAI-DC record?"
    If you want the value of a field to be emitted in the OAI-DC record, you must assign a Dublin Core predicate for that field in your content type's RDF mapping. If you are wondering why a field is not showing up in the OAI-DC record, the content type's RDF mapping is the first thing to check.

The REST OAI-PMH module does not support metadata formats other than OAI-DC, but it supports some alternate methods of defining mappings to OAI-DC. Consult [that module's documentation](https://www.drupal.org/project/rest_oai_pmh) for more information.

## Creating additional metadata formats
This involves creating a new plugin.

The [Drupal rest_oai_pmh module's DefaultMap plugin](https://git.drupalcode.org/project/rest_oai_pmh/-/blob/2.0.x/src/Plugin/OaiMetadataMap/DefaultMap.php) provides a basic model to follow for creating a plugin.

Exact implementation of your plugin will depend on your data model. The rest_oai_pmh module by default expects a flat list of fields and field values to output. This means that if your data model uses anything like Typed Relation field types, Paragraphs, or other complex nested entity modeling, you will need to add custom logic to build the values to emit via OAI-PMH for those fields.

# OAI-PMH

The [Open Archives Initiative Protocol for Metadata Harvesting](https://www.openarchives.org/pmh/), commonly referred to as OAI-PMH is a specification for exposing repository metadata. Repositories can expose or harvest metadata, or both. OAI-PMH specifies six services which can be invoked over HTTP(s). The [full specification](http://www.openarchives.org/OAI/openarchivesprotocol.html) details the services:

- Identify: [http://localhost:8000/oai/request?verb=Identify](http://localhost:8000/oai/request?verb=Identify)
- ListMetadataFormats: [http://localhost:8000/oai/request?verb=ListMetadataFormats](http://localhost:8000/oai/request?verb=ListMetadataFormats)
- ListSets: [http://localhost:8000/oai/request?verb=ListSets](http://localhost:8000/oai/request?verb=ListSets)
- GetRecord: [http://localhost:8000/oai/request?verb=GetRecord&metadataPrefix=oai_dc&identifier=oai:localhost:node-1](http://localhost:8000/oai/request?verb=GetRecord&metadataPrefix=oai_dc&identifier=oai:localhost:node-1)
- ListIdentifiers: [http://localhost:8000/oai/request?verb=ListIdentifiers&metadataPrefix=oai_dc](http://localhost:8000/oai/request?verb=ListIdentifiers&metadataPrefix=oai_dc)
- ListRecords: [http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc](http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc)

The Drupal module [islandora_defaults](https://github.com/Islandora/islandora_defaults) includes the [islandora_oaipmh](https://github.com/Islandora/islandora_defaults/tree/8.x-1.x/modules/islandora_oaipmh) module. It depends on the [Drupal REST OAI-PMH module](https://www.drupal.org/project/rest_oai_pmh). The combination of these modules will allow your repository content (which are not modelled as 'Collection') to be harvested.

Enabling Islandora OAI-PMH provides:

- an oai_dc [endpoint](http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc) of all objects available as one set (`"oai_pmh:all_repository_items"`) which is built using a [configurable View](http://localhost:8000/admin/structure/views/view/oai_pmh).
- [default configuration](http://localhost:8000/admin/config/services/rest/oai-pmh) to connect this view with the REST OAI-PMH module.
- the ability to define additional sets by building additional views. Additional sets can be created by making views with the Entity Reference view display mode and enabling them on the rest_oai_pmh configuration page: /admin/config/services/rest/oai-pmh.

The rest_oai_pmh module indexes set membership, so repository items may not appear immediately in their respective sets. Indexing will happen automatically during cron runs but can be triggered manually at `/admin/config/services/rest/oai-pmh/queue`.

## OAI-DC Metadata Format/Metadata Mappings
By default, the mappings to OAI-DC are provided via the RDF mapping for the content type(s) output to OAI-PMH (e.g. rdf.mapping.node.islandora_object.yml). Any fields where the RDF predicate is not in the Dublin Core namespace are filtered out.

When you look at the [default OAI-PMH configuration](http://localhost:8000/admin/config/services/rest/oai-pmh), you will see that the "OAI Dublin Core (RDF Mapping)" is used to produce the oai_dc metadata format. This is not a separate RDF mapping, but refers to the RDF mapping(s) for content types output to OAI-PMH.

!!! tip "Field values not showing up in OAI-DC record?"
    If you want the value of a field to be emitted in the OAI-DC record, you must assign a Dublin Core predicate for that field in your content type's RDF mapping. If you are wondering why a field is not showing up in the OAI-DC record, the content type's RDF mapping is the first thing to check.

The OAI-DC metadata format is aided by the Islandora OAI-PMH module to include the RDF Mapping for the linked agent field (from Islandora Defaults). Including agent links in the OAI-PMH metadata currently requires updating the RDF mapping to include a Dublin Core predicate for that field or any other additional fields.

The rest_oai_pmh module supports some alternate methods of defining mappings to OAI-DC. Consult [that module's documentation](https://www.drupal.org/project/rest_oai_pmh) for more information.

## Creating additional metadata formats
This involves creating a new plugin.

The [Drupal rest_oai_pmh module's DefaultMap plugin](https://git.drupalcode.org/project/rest_oai_pmh/-/blob/2.0.x/src/Plugin/OaiMetadataMap/DefaultMap.php) provides a basic model to follow for creating a plugin.

Exact implementation of your plugin will depend on your data model. The rest_oai_pmh module by default expects a flat list of fields and field values to output. This means that if your data model uses anything like Typed Relation field types, Paragraphs, or other complex nested entity modeling, you will need to add custom logic to build the values to emit via OAI-PMH for those fields.

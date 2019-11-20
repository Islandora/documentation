# OAI-PMH

The [Open Archives Initiative Protocol for Metadata Harvesting](https://www.openarchives.org/pmh/), commonly referred to as OAI-PMH is a specification for exposing repository metadata. Repositories can expose or harvest metadata, or both. OAI-PMH specifies six services which can be invoked over HTTP(s). The [full specification](http://www.openarchives.org/OAI/openarchivesprotocol.html) details the services:
- Identify - [http://localhost:8000/oai/request?verb=Identify](http://localhost:8000/oai/request?verb=Identify)
- ListMetadataFormats - [http://localhost:8000/oai/request?verb=ListMetadataFormats](http://localhost:8000/oai/request?verb=ListMetadataFormats)
- ListSets - [http://localhost:8000/oai/request?verb=ListSets](http://localhost:8000/oai/request?verb=ListSets)
- GetRecord - [http://localhost:8000/oai/request?verb=GetRecord&metadataPrefix=oai_dc&identifier=oai:localhost:node-1](http://localhost:8000/oai/request?verb=GetRecord&metadataPrefix=oai_dc&identifier=oai:localhost:node-1)
- ListIdentifiers - [http://localhost:8000/oai/request?verb=ListIdentifiers&metadataPrefix=oai_dc](http://localhost:8000/oai/request?verb=ListIdentifiers&metadataPrefix=oai_dc)
- ListRecords - [http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc](http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc)

The Drupal feature [islandora_defaults](https://github.com/Islandora/islandora_defaults) includes the [islandora_oaipmh](https://github.com/Islandora/islandora_defaults/tree/8.x-1.x/modules/islandora_oaipmh) module. It depends on the [Drupal REST OAI-PMH module](https://www.drupal.org/project/rest_oai_pmh). The combination of these modules will allow your repository content (which are not modelled as 'Collection') to be harvested.

Enabling Islandora OAI-PMH provides:
- an oai_dc [endpoint](http://localhost:8000/oai/request?verb=ListRecords&metadataPrefix=oai_dc) of all objects available as one set ("oai_pmh:all_repository_items") which is built using a [configurable View](http://localhost:8000/admin/structure/views/view/oai_pmh).
- [default configuration](http://localhost:8000/admin/config/services/rest/oai-pmh) to connect this view with the REST OAI-PMH module.
- the ability to define additional sets by building additional views. Additional sets can be created by making views with the Entity Reference view display mode and enabling them on the rest_oai_pmh configuration page: /admin/config/services/rest/oai-pmh.

The OAI-DC metadata format is aided by the Islandora OAI-PMH module to include the RDF Mapping for the linked agent field (from Islandora Defaults). Including agent links in the OAI-PMH metadata currently requires updating the RDF mapping to include a Dublin Core predicate for that field or any other additional fields. Alternatively, the rest_oai_pmh module also supports defining mappings with the metatag module or creating a custom metadata profile using the Twig templating system.

The rest_oai_pmh module indexes set membership, so repository items may not appear immediately in their respective sets. Indexing will happen automatically during cron runs but can be triggered manually at /admin/config/services/rest/oai-pmh/queue.

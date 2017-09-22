## RDF Mapping
RDF Mapping is aligning drupal fields to RDF ontology properties. For example the `title` field of a content model can be mapped to `dcterms:title` and/or `schema:title`. In Islandora CLAW, triples expressed by these mappings get synced to Fedora and indexed in the Blazegraph triplestore.  

Drupal provides a default rdf mapping for its core entities. For instance, the article content type's rdf mapping is specified in the `rdf.mapping.node.page.yml`. One can export this RDF mapping by going to Configuration Synchronization: `http://localhost:8000/admin/config/development/configuration/single/export`, selecting `RDF` for Configuration type and chosing `node.article`.

RDF Mappings are defined/stored in Drupal as a YAML file. Currently, Drupal 8 does not have a UI to enable the user to create/update RDF Mappings to ontologies other than Schema.org. Currently there is a project underway to develop a [UI](https://github.com/Islandora-CLAW/CLAW/issues/647) to support RDF mappings to any ontology.  However, until then exporting/importing RDF yml files via Configuration Synchronization is the primary method to create/update RDF mappings.  

### Altering an existing RDF mapping

### Defining a new RDF mapping


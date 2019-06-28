## RDF Mapping
RDF mapping is aligning drupal fields to RDF ontology properties. For example the `title` field of a content model can be mapped to `dcterms:title` and/or `schema:title`. In Islandora 8, triples expressed by these mappings get synced to Fedora and indexed in the Blazegraph triplestore.  

Drupal provides default RDF mappings for its core entities. For instance, the article content type's RDF mapping is specified in the `rdf.mapping.node.page.yml` config. One can export this RDF mapping by going to Configuration Synchronization: `http://localhost:8000/admin/config/development/configuration/single/export`, selecting `RDF` for Configuration type and choosing `node.article`.

RDF mappings are defined/stored in Drupal as a YAML file. Currently, Drupal 8 does not have a UI to create/update RDF mappings to ontologies other than Schema.org. There is a project underway to develop a [UI](https://github.com/Islandora-CLAW/CLAW/issues/647) to support RDF mappings to any ontology. However, until then exporting/importing RDF yml files via Configuration Synchronization is the primary method to create/update RDF mappings.

### Structure of RDF YAML file
Below is an example of RDF mapping. It is the current version of the RDF mapping of Islandora collection content model (`rdf.mapping.node_type.collection.yml`). `types` specify the `rdf:type` of the resource or content model. `fieldMappings` specify all fields of that bundle and their RDF property mappings. One field can be mapped to more than one RDF property. It is a simple flat list.

types and fieldMappings
```yml
langcode: en
status: true
dependencies:
  config:
    - node.type.islandora_collection
  module:
    - islandora
  enforced:
    module:
      - islandora_collection
id: node.islandora_collection
targetEntityType: node
bundle: islandora_collection
types:
  - 'pcdm:Collection'
  - 'schema:CollectionPage'
fieldMappings:
  field_description:
    properties:
      - 'dc:description'
  field_memberof:
    properties:
      - 'pcdm:memberOf'
    mapping_type: rel
  title:
    properties:
      - 'dc:title'
  created:
    properties:
      - 'schema:dateCreated'
    datatype_callback:
      callable: 'Drupal\rdf\CommonDataConverter::dateIso8601Value'
  changed:
    properties:
      - 'schema:dateModified'
    datatype_callback:
      callable: 'Drupal\rdf\CommonDataConverter::dateIso8601Value'
  uid:
    properties:
      - 'schema:author'
    mapping_type: rel
```

### Viewing RDF mapping of a resource
Please see the following tutorial to configure and view resources via REST request:
[An Introduction to RESTful Web Services in Drupal 8](https://drupalize.me/blog/201401/introduction-restful-web-services-drupal-8).  To get the JsonLD mapping, use jsonld format. Example request url: `http://localhost:8000/node/1?_format=jsonld`

### Altering an existing RDF mapping
Each Islandora content models come with a RDF mapping. It can be exported similar to above article's rdf mapping, modified and imported back by going to here: `http://localhost:8000/admin/config/development/configuration/single/import`.  

### Defining a new RDF mapping
A RDF mapping need to be created for a new or custom content model/type. The following steps describe the procedure:
* Export RDF mapping of a similar content type
* Export form display yml to get the list of the fields (ex `core.entity_form_display.node.your_content_type.default.yml`)
* Remove any cache and uuid related elements
* Add/modify the fields and ontology properties as needed, following the same syntax
* Name it following the convention (ex `rdf.mapping.node_type.your_content_type.yml`)
* Import it by going to `http://localhost:8000/admin/config/development/configuration/single/import`

# Linked data in Islandora
The purpose of this page is to provide a guided reading list to anyone who wants to get up to speed on the basics of linked data within the Islandora community. Those who make their way through the readings will be able to talk competently about linked data and better understand the design decisions made in Islandora. The list starts with the fundamentals of linked data (RDF, SPARQL, serializations and ontologies) and moves toward more advanced topics specific to the use cases of a Fedora 4 based digital repository system.

## Reading list

### Basics of linked data
This section seeks to give the reader a foundational understanding of what linked data is, why it is useful, and a very superficial understanding of how it works.

- [Tim Berners-Lee’s description of Linked Data](https://www.w3.org/DesignIssues/LinkedData.html)
- [Manu Sporny's "What is Linked Data?" YouTube Video](https://www.youtube.com/watch?v=4x_xzT5eF5Q)
- [Wikipedia article on Linked Data](https://en.wikipedia.org/wiki/Linked_data)
- [Wikipedia article on Semantic Web](https://en.wikipedia.org/wiki/Semantic_Web)
- [Wikipedia article on URIs](https://en.wikipedia.org/wiki/Uniform_Resource_Identifier)
- [Wikipedia article on the W3C](https://en.wikipedia.org/wiki/World_Wide_Web_Consortium)
- [W3C’s description of Linked Data](https://www.w3.org/standards/semanticweb/data)
- [W3C’s Linked Data Glossary](https://www.w3.org/TR/ld-glossary/)
- [W3C’s Architecture of the World Wide Web](https://www.w3.org/TR/webarch/)

### Understanding RDF
This section is all about RDF, the Resource Description Framework, which defines the way linked data is structured.

- [Wikipedia article on RDF](https://en.wikipedia.org/wiki/Resource_Description_Framework)
- [D-Lib’s Intro to RDF](http://www.dlib.org/dlib/may98/miller/05miller.html)
- [W3C’s RDF 1.1 Primer](https://www.w3.org/TR/rdf11-primer/)
- [W3C’s RDF 1.1 Concepts](https://www.w3.org/TR/rdf11-concepts/)

### Querying linked data with SPARQL
This section takes a look at SPARQL, the query language that allows you to ask linked data very specific questions. The queryable nature of linked data is one of the things that makes it so special. Try some SPARQL queries on DBpedia's endpoint to get some hands-on experience.

- [Wikipedia article on SPARQL](https://en.wikipedia.org/wiki/SPARQL)
- [W3C’s SPARQL 1.1 Overview](https://www.w3.org/TR/sparql11-overview/)
- [W3C’s SPARQL 1.1 Query Language](https://www.w3.org/TR/sparql11-query/)
- [DBpedia's SPARQL Endpoint](https://dbpedia.org/sparql)

### RDF serialization formats
RDF data can be translated into many different formats. RDF/XML is the original way that RDF data was shared, but there are much more human-friendly serialization formats like Turtle which is great for beginners. JSON-LD is the easiest format for applications to use, and is the serialization format that Islandora uses internally. Make sure to check out the [JSON-LD Playground](http://json-ld.org/playground/) for an interactive learning experience.

- [Wikipedia article on Serialization](https://en.wikipedia.org/wiki/Serialization)
- [W3C’s RDF/XML Syntax Specification](https://www.w3.org/TR/REC-rdf-syntax/)
- [W3C’s RDF 1.1 Turtle](https://www.w3.org/TR/turtle/)
- [W3C’s JSON-LD 1.0](https://www.w3.org/TR/json-ld/)
- [JSON-LD Website](http://json-ld.org/)
- [JSON-LD Playground](http://json-ld.org/playground/)

### Ontology and vocabulary basics
Ontologies and vocabularies are created by communities of people to describe things, and once created, anyone can use an ontology or vocabulary to describe their resources. This section goes over some of the more popular ontologies & vocabularies in use.

- [Wikipedia article on Ontologies](https://en.wikipedia.org/wiki/Ontology_(information_science))
- [W3C’s description of Ontologies/Vocabularies (sameish thing)](https://www.w3.org/standards/semanticweb/ontology)
- [Wikipedia article on Friend of a Friend (FOAF) ontology](https://en.wikipedia.org/wiki/FOAF_(ontology))
- [FOAF 0.99 Vocabulary Specification](http://xmlns.com/foaf/spec/)
- [Socially Interconnected Online Communities Ontology (SIOC)](http://sioc-project.org/)
- [Dublin Core in RDF](http://dublincore.org/documents/dc-rdf/)

### Building ontologies
One isn't limited to the ontologies & vocabularies that already exist in the world, anyone is free to create their own. This section goes over ontologies that exist to help those trying to create their own ontologies.

- [Wikipedia article on RDF Schema (RDFS)](https://en.wikipedia.org/wiki/RDF_Schema)
- [W3C’s RDF Schema (RDFS) 1.1](https://www.w3.org/TR/rdf-schema/)
- [Wikipedia article on Simple Knowledge Organization System (SKOS)](https://en.wikipedia.org/wiki/Simple_Knowledge_Organization_System)
- [ALA’s SKOS: A Guide for Information Professionals](http://www.ala.org/alcts/resources/z687/skos)
- [Wikipedia article on Web Ontology Language (OWL)](https://en.wikipedia.org/wiki/Web_Ontology_Language)
- [W3C’s OWL 2 Primer](https://www.w3.org/TR/owl2-primer/)
- [W3C’s OWL 2 Quick Reference](https://www.w3.org/TR/owl2-quick-reference/)

### Repository-specific ontologies
Most ontologies are very specific to certain use cases, and digital repository systems are no different. This section covers ontologies that are of specific interest to users of Islandora, or any Fedora 4 based digital repository system.

- [MODS RDF Namespace Document](http://www.loc.gov/standards/mods/modsrdf/v1/)
- [MODS RDF Ontology Primer](https://www.loc.gov/standards/mods/modsrdf/primer.html)
- [MODS RDF Ontology Primer 2: MODS XML to RDF Conversion](https://www.loc.gov/standards/mods/modsrdf/primer-2.html)
- [PREMIS RDF Namespace Document](http://id.loc.gov/ontologies/premis.html)
- [Linked Data Platform (LDP) 1.0 Primer](https://www.w3.org/TR/ldp-primer/)
- [LDP 1.0 Specification](https://www.w3.org/TR/ldp/)
- [Portland Common Data Model (PCDM) wiki)](https://github.com/duraspace/pcdm/wiki)
- [PCDM ontologies list](http://pcdm.org/)
- [PCDM Models ontology (defines Collections, Objects & Files)](http://pcdm.org/2016/04/18/models)
- [Fedora ontologies](http://fedora.info/)

## RDF generation
### Summary
In Islandora, the **JSON-LD Module** transforms nodes (or media, or taxonomy terms) into the RDF that is synced into Fedora and the Triplestore. It uses RDF mappings, a concept defined by the **RDF Module**, and exposes them through the **REST API** at `?_format=jsonld`.

### Background

A quick overview of JSON-LD, the RDF module, and the REST API.

#### The JSON-LD syntax
[JSON-LD](https://www.w3.org/2013/dwbp/wiki/RDF_AND_JSON-LD_UseCases) is a syntax which can be used to express RDF (like Turtle, or RDF XML), that is written in JSON, because devs like JSON and it's web-friendly. The JSON-LD syntax was designed for including Linked Data within HTML of web pages (similar to microdata or RDFa). Instead of nesting the RDF predicates within _existing_ HTML tags as RDFa does, JSON-LD lets you put a solid blob of Linked Data inside a `<script>` tag. JSON-LD can also function as a standalone document, which is how we're using it.

#### RDF (Drupal Module)
The **RDF Module** is part of Drupal Core, but has no official documentation. The RDF Module embeds RDFa, a form of linked data, within the Drupal-generated HTML when you load the web page for a node, media, or taxonomy term. Official line is that this will allow Google to provide "rich snippets" such as star-ratings, contact info, and business hours. As an example of Drupal-provided RDFa:

```html
<h1 class="page-header">
   <span property="schema:title">My cat</span>
</h1>
```
The `property="schema:title"` is markup generated by Drupal's RDF module that identifies the value "My cat" as the schema.org `title` of this page. A node's fields (such as `field_tags`) and properties (such as `author`) can be mapped to RDF according to a bundle-specific "mapping" that is stored within Drupal. In Drupal8-ese, RDF mappings are configuration entities. Drupal doesn't have a good UI for editing RDF mappings, but you can create, read, and update them as YAML files using Drupal's Configuration Synchronization interface (see section below on How to Edit an RDF Mapping)..

#### REST API
The pattern of using `?_format=` to get a different representation of content is provided by the RESTful Web Services (rest) module. It allows other services to interact with Drupal entities through HTTP requests (`GET`, `POST`, `PATCH`, and `DELETE`). Which operations are allowed, and with what formats (such as `xml`, `json`, and `jsonld`) is configured at `admin/config/services/rest/`. Note that only `jsonld` uses RDF mappings; the `json` and `xml` formats expose a structured object based on how Drupal sees the entity. Access to these alternate formats through the REST API corresponds to permissions on the entity, so anyone with `access content` permission can view the JSON-LD version of that content. This is new as of[ Drupal 8.2](https://www.drupal.org/docs/8/api/restful-web-services-api/restful-web-services-api-overview#practical).

For more information on interacting with Drupal entities via REST requests, see [An Introduction to RESTful Web Services in Drupal 8](https://drupalize.me/blog/201401/introduction-restful-web-services-drupal-8).

### JSON-LD module

Using the RDF mapping configurations provided by the RDF module, the JSON-LD Module exposes the RDF-mapped entity in JSON-LD, through the REST API, at `node/[nid]?_format=jsonld` (for nodes; for media and terms, at `media/[mid]?_format=jsonld` and `taxonomy/term/[tid]?_format=jsonld`).

- The JSON-LD module will only work with mappings that include a value under `types` (which maps to `rdf:type` - see below, under Structure of an RDF Mapping).
- The JSON-LD module provides a hook so other modules can alter the entity before it gets mapped. The `islandora` module uses this hook to trigger any "Map URI to Predicate" and "Alter JSON-LD Type" reactions that are configured in Contexts. The Islandora Starter Site provides two Contexts - "All Media" and "Content" - that configure these to occur on Media and Repository Item nodes.
- The JSON-LD module adds RDF datatypes to the RDF values, and includes a mapping of Drupal field types to RDF datatypes.
- The JSON-LD module provides a hook to alter its Drupal field type to RDF datatype mapping.
- The JSON-LD module has a configuration option that can cause the `?_format=jsonld` to be part of, or not part of, the URIs of Drupal objects. On an out-of-the-box islandora-playbook, this string is stripped, but by default on a fresh install of the jsonld module, it is not.

#### Sample JSON-LD

```json
{
   "@graph":[
      {
         "@id":"http:\/\/future.islandora.ca\/node\/8",
         "@type":[
            "http:\/\/pcdm.org\/models#Object"
         ],
         "http:\/\/purl.org\/dc\/terms\/title":[
            {
               "@value":"lasmomias de uninpahu",
               "@language":"fa"
            }
         ],
         "http:\/\/schema.org\/author":[
            {
               "@id":"http:\/\/future.islandora.ca\/en\/user\/1"
            }
         ],
         "http:\/\/schema.org\/dateCreated":[
            {
               "@value":"2019-06-04T14:32:05+00:00",
               "@type":"http:\/\/www.w3.org\/2001\/XMLSchema#dateTime"
            }
         ],
         "http:\/\/schema.org\/dateModified":[
            {
               "@value":"2019-06-04T17:02:51+00:00",
               "@type":"http:\/\/www.w3.org\/2001\/XMLSchema#dateTime"
            }
         ],
         "http:\/\/purl.org\/dc\/terms\/description":[
            {
               "@value":"mpermmbklmh",
               "@language":"fa"
            }
         ],
         "http:\/\/purl.org\/dc\/terms\/created":[
            {
               "@value":"2015-10-15",
               "@type":"http:\/\/www.w3.org\/2001\/XMLSchema#string"
            },
            {
               "@value":"2015-10-15",
               "@type":"http:\/\/www.w3.org\/2001\/XMLSchema#date"
            }
         ],
         "http:\/\/purl.org\/dc\/terms\/extent":[
            {
               "@value":"1 item",
               "@type":"http:\/\/www.w3.org\/2001\/XMLSchema#string"
            }
         ],
         "http:\/\/pcdm.org\/models#memberOf":[
            {
               "@id":"http:\/\/future.islandora.ca\/node\/7"
            }
         ],
         "http:\/\/purl.org\/dc\/terms\/type":[
            {
               "@id":"http:\/\/future.islandora.ca\/taxonomy\/term\/3"
            }
         ],
         "http:\/\/purl.org\/dc\/terms\/subject":[
            {
               "@id":"http:\/\/future.islandora.ca\/taxonomy\/term\/27"
            }
         ]
      },
      {
         "@id":"http:\/\/future.islandora.ca\/en\/user\/1",
         "@type":[
            "http:\/\/schema.org\/Person"
         ]
      },
      {
         "@id":"http:\/\/future.islandora.ca\/node\/7",
         "@type":[
            "http:\/\/pcdm.org\/models#Object"
         ]
      },
      {
         "@id":"http:\/\/future.islandora.ca\/taxonomy\/term\/3",
         "@type":[
            "http:\/\/schema.org\/Thing"
         ]
      },
      {
         "@id":"http:\/\/future.islandora.ca\/taxonomy\/term\/27",
         "@type":[
            "http:\/\/schema.org\/Thing"
         ]
      }
   ]
}
```

### RDF mappings
If using the Islandora Starter Site, the RDF mappings are set in the config files with names like rdf.mapping.[...].yml. The starter site config files will override configs set by modules. If you are building a site from scratch (not using the Islandora Starter Site, there are relevant configs in the following folders:

- `[drupal modules directory]/islandora/modules/islandora_core_feature/config/install/` (media and taxonomy terms)
- `[drupal modules directory]/controlled_access_terms/modules/controlled_access_terms_defaults/config/install/` (the default `corporate_body`, `family`, `geo_location`, `person`, `resource_type` and `subject` vocabularies)
- `[drupal web root]/core/profiles/standard/config/install/` (articles, pages, comments, and tags).

Once loaded by modules, configuration .yml files are not live so **editing them will not change the existing configuration**. However, for modules that are Features, it is possible to re-import the changed configuration files at `admin/config/development/features` (todo: link to further reading on Features).

#### How to edit an RDF mapping

Once loaded, RDF mappings can be customized for the needs of a particular site through Drupal's Configuration Synchronization UI at `admin/config/development/configuration`. They can be exported, modified, and re-imported one-at-a-time by choosing the "Single Item" option on the Export/Import tabs.  You can also create new RDF mappings (e.g. for a custom content type) and load them through this interface, by copying an existing mapping and changing the appropriate values.

!!! note "Contributed module for RDF Mappings"
    A custom module `rdfui` exists, and is installed-but-not-enabled on boxes provisioned by the islandora-playbook. We don't use it because it is very rudimentary and limited to the schema.org vocabulary. We have an [open ticket](https://github.com/Islandora/documentation/issues/647) to develop a UI to support RDF mappings to any ontology. Contributions welcome.

- A number of namespaces such as `ldp`, `ebucore`, `pcdm`, are `premis` are registered in `islandora.module` using `hook_rdf_namespaces()`. To register your own namespaces, you will need to create a custom module that implements that hook.
- If you import a configuration that uses a namespace that is not registered, bad things will happen silently.


#### Structure of an RDF mapping
Below is an example of an RDF mapping as a .yml (YAML) file. It is the RDF mapping (current at time of writing) of the Repository Item (`islandora_object`) bundle, provided by the Islandora Starter Site and exportable as `rdf.mapping.node.islandora_object.yml`).

- The top level key `types` specifies the `rdf:type` of the resource or content model. `field_model`, a required field of Islandora objects, also gets mapped to `rdf:type` through an arcane back-end process.
- The top level key `fieldMappings` specifies fields attached to that bundle and their RDF property mappings. One field can be mapped to more than one RDF property. It is a simple flat list.

#### Mapping types
`mapping_type:`: There are several mapping types which are provided out of the box.
- `rel` - standing for relationship, expresses a relationship between two resources
- `property` - the default, or if a relationship is not provided, expresses the relationship between a resource and some literal text.

#### Datatype callbacks
`datatype_callback`: This is a custom function that transforms the output of the field. There are some provided to us by Drupal and some added by Islandora, such as:
- `Drupal\controlled_access_terms\EDTFConverter::dateIso8601Value` which converts dates to ISO format
- `Drupal\jsonld\EntityReferenceConverter::linkFieldPassthrough` which converts a referenced entity to the URI on the entity (which is configurable with the `link_field` argument
An example usage of the `Drupal\jsonld\EntityReferenceConverter::linkFieldPassthrough` is as follows:
```yml
field_subject:
    properties:
      - 'dcterms:subject'
    datatype_callback:
      callable: 'Drupal\jsonld\EntityReferenceConverter::linkFieldPassthrough'
      arguments:
        link_field: 'field_authority_link'
```
Which would convert a reference to the subject's taxonomy term entity to a reference to the URI provided in `field_authority_link` of that subject's taxonomy term entity.


#### Sample RDF mapping
```yml
langcode: en
status: true
dependencies:
  config:
    - node.type.islandora_object
  module:
    - node
id: node.islandora_object
targetEntityType: node
bundle: islandora_object
types:
  - 'pcdm:Object'
fieldMappings:
  field_alternative_title:
    properties:
      - 'dc:alternative'
  field_edtf_date:
    properties:
      - 'dc:date'
    datatype_callback:
      callable: 'Drupal\controlled_access_terms\EDTFConverter::dateIso8601Value'
  field_edtf_date_created:
    properties:
      - 'dc:created'
    datatype_callback:
      callable: 'Drupal\controlled_access_terms\EDTFConverter::dateIso8601Value'
  field_edtf_date_issued:
    properties:
      - 'dc:issued'
    datatype_callback:
      callable: 'Drupal\controlled_access_terms\EDTFConverter::dateIso8601Value'
  field_description:
    properties:
      - 'dc:description'
  field_extent:
    properties:
      - 'dc:extent'
  field_identifier:
    properties:
      - 'dc:identifier'
  field_member_of:
    properties:
      - 'pcdm:memberOf'
    mapping_type: rel
  field_resource_type:
    properties:
      - 'dc:type'
    mapping_type: rel
  field_rights:
    properties:
      - 'dc:rights'
  field_subject:
    properties:
      - 'dc:subject'
    mapping_type: rel
  field_weight:
    properties:
      - 'co:index'
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

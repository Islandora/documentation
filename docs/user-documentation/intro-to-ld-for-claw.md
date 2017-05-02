# Introduction to Linked Data for CLAW
The purpose of this page is to provide a guided reading list to anyone who wants to get up to speed on the basics of linked data within the Islandora community. Those who make their way through the readings will be able to talk competently about linked data and better understand the design decisions made in Islandora CLAW. The list starts with the fundamentals of linked data (RDF, SPARQL, serializations and ontologies) and moves toward more advanced topics specific to the use cases of a Fedora 4 based digital repository system.

# Basics of Linked Data
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

# Understanding RDF
This section is all about RDF, the Resource Description Framework, which defines the way linked data is structured.
- [Wikipedia article on RDF](https://en.wikipedia.org/wiki/Resource_Description_Framework)
- [D-Lib’s Intro to RDF](http://www.dlib.org/dlib/may98/miller/05miller.html)
- [W3C’s RDF 1.1 Primer](https://www.w3.org/TR/rdf11-primer/)
- [W3C’s RDF 1.1 Concepts](https://www.w3.org/TR/rdf11-concepts/)

# Querying Linked Data with SPARQL
This section takes a look at SPARQL, the query language that allows you to ask linked data very specific questions. The queryable nature of linked data is one of the things that makes it so special. Try some SPARQL queries on DBpedia's endpoint to get some hands-on experience.
- [Wikipedia article on SPARQL](https://en.wikipedia.org/wiki/SPARQL)
- [W3C’s SPARQL 1.1 Overview](https://www.w3.org/TR/sparql11-overview/)
- [W3C’s SPARQL 1.1 Query Language](https://www.w3.org/TR/sparql11-query/)
- [DBpedia's SPARQL Endpoint](https://dbpedia.org/sparql)

# RDF Serialization Formats
RDF data can be translated into many different formats. RDF/XML is the original way that RDF data was shared, but there are much more human-friendly serialization formats like Turtle which is great for beginners. JSON-LD is the easiest format for applications to use, and is the serialization format that CLAW uses internally. Make sure to check out the [JSON-LD Playground](http://json-ld.org/playground/) for an interactive learning experience.
- [Wikipedia article on Serialization](https://en.wikipedia.org/wiki/Serialization)
- [W3C’s RDF/XML Syntax Specification](https://www.w3.org/TR/REC-rdf-syntax/)
- [W3C’s RDF 1.1 Turtle](https://www.w3.org/TR/turtle/)
- [W3C’s JSON-LD 1.0](https://www.w3.org/TR/json-ld/)
- [JSON-LD Website](http://json-ld.org/)
- [JSON-LD Playground](http://json-ld.org/playground/)

# Ontology & Vocabulary Basics
Ontologies & vocabularies are created by communities of people to describe things, and once created, anyone can use an ontology or vocabulary to describe their resources. This section goes over some of the more popular ontologies & vocabularies in use.
- [Wikipedia article on Ontologies](https://en.wikipedia.org/wiki/Ontology_(information_science))
- [W3C’s description of Ontologies/Vocabularies (sameish thing)](https://www.w3.org/standards/semanticweb/ontology)
- [Wikipedia article on Friend of a Friend (FOAF) ontology](https://en.wikipedia.org/wiki/FOAF_(ontology))
- [FOAF 0.99 Vocabulary Specification](http://xmlns.com/foaf/spec/)
- [Socially Interconnected Online Communities Ontology (SIOC)](http://sioc-project.org/)
- [Dublin Core in RDF](http://dublincore.org/documents/dc-rdf/)

# Building Ontologies
One isn't limited to the ontologies & vocabularies that already exist in the world, anyone is free to create their own. This section goes over ontologies that exist to help those trying to create their own ontologies.
- [Wikipedia article on RDF Schema (RDFS)](https://en.wikipedia.org/wiki/RDF_Schema)
- [W3C’s RDF Schema (RDFS) 1.1](https://www.w3.org/TR/rdf-schema/)
- [Wikipedia article on Simple Knowledge Organization System (SKOS)](https://en.wikipedia.org/wiki/Simple_Knowledge_Organization_System)
- [ALA’s SKOS: A Guide for Information Professionals](http://www.ala.org/alcts/resources/z687/skos)
- [Wikipedia article on Web Ontology Language (OWL)](https://en.wikipedia.org/wiki/Web_Ontology_Language)
- [W3C’s OWL 2 Primer](https://www.w3.org/TR/owl2-primer/)
- [W3C’s OWL 2 Quick Reference](https://www.w3.org/TR/owl2-quick-reference/)

# Repository-Specific Ontologies
Most ontologies are very specific to certain use cases, and digital repository systems are no different. This section covers ontologies that are of specific interest to users of CLAW, or any Fedora 4 based digital repository system.
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
- [CLAWntology](https://github.com/Islandora-CLAW/CLAWntology)

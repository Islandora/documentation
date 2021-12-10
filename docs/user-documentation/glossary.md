
The following glossary of terms addresses an Islandora context. When comparing new Islandora and Fedora to older versions it may also be helpful to reference [the Islandora 7 Glossary](https://wiki.duraspace.org/display/ISLANDORA/APPENDIX+E+-+Glossary).

## Alpaca
Event-driven middleware based on [Apache Camel](http://camel.apache.org/) that handles communication between various components of Islandora, for instance synchronizing Drupal data with a Fedora repository, the Blazegraph triple store. 

## Ansible
Open source software for provisioning, configuration management and application deployment. In the context of Islandora, Ansible can be used to install and maintain the Islandora software stack more conteniently and efficiently on a server or group of servers. The configuration and installation instructions are captured in a human-readable list of tasks, called 'Playbook'. The Islandora Playbook for Ansible is one of the installation methods currently supported by the Islandora community.

## Apache
Refers to the [Apache Software Foundation](https://www.apache.org/), a not-for-profit organization supporting various open source software development projects. The Islandora software stack is comprised of different components that are developed under the umbrella of the Apache Software Foundation, for instance [Apache ActiveMQ](https://activemq.apache.org/), [Apache Camel](http://camel.apache.org/), the [Apache HTTP server (webserver)](https://httpd.apache.org/), [Apache Karaf](https://karaf.apache.org/), [Apache Solr](https://solr.apache.org/), and [Apache Tomcat](https://tomcat.apache.org/).

Can in a narrower sense refer to the Apache HTTP server.

## API
See Application Programming Interface

## Application Programming Interface
Also API; a connection between computers or between computer programs. It is a type of software interface, offering a service to other pieces of software.

## Checksum
Checksums are a sequence of numbers and letters to check data for errors. If you know the checksum of an original file, you can use a checksum utility to confirm your copy is identical. Checksums can be used to check the Fixity of a file.

## CLAW
 CLAW (CLAW Linked Asset WebDataFrameWork) was the development code name for the software released in June, 2019 as _Islandora 8_, now called _Islandora_.
 
## Collection
A [collection](../concepts/collection.md) is a way of grouping related resources together, much like a directory on a computer. Collections can “contain” any number of related resource nodes and sub-collections.

## Content entity
Drupal websites are made up of two types of entities: content entities and configuration (config) entities. Content entities include nodes, media, taxonomy terms, users, comments, and files. Configuration entities include all stored settings, for instance views, custom blocks, menus, or roles. 

Content entities are grouped into distinct content entity types, which play a specific functional role in a website application built with Drupal. Most 'things' in a Drupal website are content entities. These include web content (Nodes, which can have subtypes like article, basic page, repository item, etc.), as well as Users, Comments, Files, and Taxonomy terms. 

Each type of Content entity has a different set of behaviors. The interactions of these behaviors creates the site’s functionality. For example, a User may log into a site with a username and password that is stored as part of the content entity. If Users have the relevant permissions, they can write a blog post (a subtype of the Node content entity type, with a specific data structure and behavior), and assign some tags (Taxonomy terms) to the blog post.

Drupal comes with several content entity types preconfigured. Drupal's default configuration includes the Basic Page and (blog) Article content types, as well as Audio, Image and Video Media types. _Islandora Defaults_ adds the Repository Item content type, alters the existing Media types to store files in Fedora, and introduces the Extracted Text and FITS Media types.

## Content model
Deprecated concept used in Islandora Legacy; see Islandora Model.

## Content type
A general type of Content in Drupal. A content type defines a set of fields and how they are displayed. Content types are sub-types of Nodes, one of the Drupal Content entities. The default Drupal Content types are 'Article' and 'Basic page'. _Islandora Defaults_ adds 'Repository Item' as a Content type, defining metadata fields typically used to describe digital resources. 

## Crayfish
A collection of Islandora [microservices](#microservice). Some of the microservices are built specifically for use with a Fedora Repository, while others are just for general use within Islandora.

## Datastream
Deprecated terminology, refers to how Fedora 3/Islandora Legacy stored files as part of a resource ('object') in the Fedora repository. Replaced by [Drupal Media entities](https://www.drupal.org/docs/8/core/modules/media/overview), which serve as an abstraction for [Files](https://www.drupal.org/docs/8/core/modules/file/overview) managed by Drupal.

## Derivative
A version of a file which is derived from an uploaded file. For example, a thumbnail generated from an uploaded image. Islandora uses [microservices](#microservice) to generate derivatives.

## Docker
[Docker](https://www.docker.com/) is a platform that use OS-level virtualization to deliver software in packages called containers. Islandora uses Docker as part of ISLE, a suite of Docker containers that run the various components of Islandora required by Islandora Defaults. 
 
## Drupal 
Drupal is an open source web content management system (CMS) written in PHP. Known for being extremely flexible and extensible, Drupal is supported by a community of over 630,000 users and developers. Drupal sites can be customized and themed in a wide variety of ways.

## Drupal Core
The files, themes, profiles, and modules included with the standard project software download.
 
## Drupal Roles
Roles are a way of assigning specific permissions to a group of users. Any user assigned to a role will have the same permissions as all other users assigned to that role. This allows you to control which users have permission to view, edit, or delete content in Drupal. Islandora provides a special role called _fedoraAdmin_ that is required to have actions in Drupal reflected in Fedora.
 
## Entity 
A Drupal term for an item of either content or configuration data. Examples include Nodes (content items), Blocks, Taxonomy terms, and definitions of content types; the first three are content entities, and the last is a configuration entity. In common usage, the term often refers to Drupal content entities like Nodes or Taxonomy terms. 
 
## Fedora (Repository Software)
Fedora is a digital asset management architecture upon which institutional repositories, digital archives, and digital library systems might be built. Fedora is the underlying architecture for a digital repository, and is not a complete management, indexing, discovery, and delivery application. 
 
The Fedora repository functions as the standard _smart storage_ for Islandora.
 
## Field
Data of a certain type that is attached to a content entity. For instance, on a Resource Node content type, you might have fields for a title, description, display hints, subjects, and other metadata. 
 
## Field instance

## Field type

## FITS
 
## Fixity
Also file fixity; digital preservation term meaning that a digital file remains unchanged ('fixed') over time. Fixity checking verifies that a file has not been corrupted or manipulated during a transfer process or while being stored. Typically, a fixity checking process computes checksums or cryptographic hashes for a file and compares the result to a reference value stored earlier. The contributed Islandora module 'RipRap' supports fixity checking and error reporting.

## Flysystem
Flysystem is a filesystem abstraction library for PHP. Islandora uses Flysystem to swap about different backend filesystem applications. Islandora provides a custom Flysystem adapter for Fedora.

## GLAM

## IIIF
The [International Image Interoperability Framework](https://iiif.io/). A set of open standards that help archives, libraries, and museums make the most of their digitized collections with deep zoom, annotation capabilities, and more, and also the community of users and developers that suport the framework.
 
## Ingest
To ingest an object is to add an entry for it in Islandora. This is done primarily through the Drupal interface. 
 
The word 'ingest' is used in repository language because repository software, rather than having an object and associated components forced upon it, takes in and processes the object itself, creating derivatives automatically. Often, when referring to the process of ingesting objects, Islandora and its associated documentation use other terms such as 'import' or 'add resource node'. In such contexts, these terms generally refer to the ingest process.

## Islandora model

## Islandora Playbook
A set of human-readable YAML files, containing instructions for automatically configuring a server environment and installing the different components of the Islandora software stack. The instructions recorded in Playbook are executed by Ansible. The Islandora Playbook for Ansible is one of the installation methods currently supported by the Islandora community.

## ISLE
ISLE, or ISLandora Enterprise, is a community initiative to ease the installation and maintenance of Islandora by using [Docker](#docker).

## JSON-LD

## Linked agent

## Linked data

## Matomo 
[Matomo](https://matomo.org/), formerly called Piwik, is a software for tracking visits to websites. It is an open source alternative to Google Analytics and allows the generation of website usage reports.

## Media
A drop-in replacement for the Drupal core upload field with a unified User Interface where editors and administrators can upload, manage, and reuse files and multimedia assets. 

## Memento
Protocol specification that allows a web client to request an earlier/historic state web resource (if available). Fedora implements the Memento protocol to store and serve versions of content in a Fedora repository.

## Microservice
A software development technique — a variant of the service-oriented architecture (SOA) structural style — that arranges an application as a collection of loosely coupled services. In a microservices architecture, services are fine-grained and the protocols are lightweight.
 
## Module
Software (usually PHP, JavaScript, and/or CSS) that extends site features and adds functionality. Drupal modules conform to a specific structure allowing them to integrate with the Drupal architecture. 
 
## Node
A node is any piece of individual content, such as a page, article, forum topic, or a blog entry. All content on a Drupal website is stored and treated as "nodes". For information about nodes specific to Islandora, see "Resource Node."

## OAI-PMH

## Ontology
In computer science and information science, an ontology encompasses a representation, formal naming and definition of the categories, properties and relations between concepts, data and entities. In the narrower context of the Resource Description Framework (RDF), an ontology is a formal, machine-readable description of the 'vocabulary' that can be used in a knowledge graph. An RDF ontology for instance specifies _classes_ of things or concepts (e.g. the class of all book authors) and _properties_ of classes/class instances (e.g. an author's name, birth date, shoe size; also the fact that an author has written something that is in the class of books).

## Open Source
Open source describes a method of software development that promotes access to the end product's source code. Islandora is an open source product with an active development community, operating under the GPL license (2.0) for Drupal components and the MIT license for non-Drupal components. 

## PR
See Pull request

## Pull request
Also PR; 

## RDF
See Resource Description Framework

## Repository Item
See: Resource Node

## Resource Description Framework
Also RDF; 

## Resource Node
A resource node is a Drupal node that represents a single conceptual item or object stored in an Islandora repository. It acts as a container for all files and metadata associated with that item, and is the place where the item 'lives' as a visitable url. 
 
For example, A video stored in Islandora will have a resource node, which metadata stored in fields, a preservation master file, and may have a thumbnail, web-friendly derivatives, and technical metadata associated with the resource node. The resource node may also belong to one or more collections. 

## Taxonomy
 
## Taxonomy term
In the context of Islandora, a taxonomy term is a Drupal entity of the type 'taxonomy'. Drupal generally uses taxonomy terms to classify content (tag or category). In Islandora, taxonomies are used to establish controlled vocabularies for describing resources, for instance for standardised spellings of names or subject terms.
 
## Theme
Software and asset files (images, CSS, PHP code, and/or templates) that determine the style and layout of the site. The Drupal project distinguishes between core and contributed themes. 

## Vagrant
[Vagrant](https://www.vagrantup.com/) is an open-source software product for building and maintaining portable virtual software development environments (virtual machines). The Islandora Playbook includes a 'vagrantfile', a set of instructions that allows users to create a local virtual machine environment which will subsequently run Ansible to execute the configuration and installation steps recorded in the Islandora Playbook.
 
## VBO
See Views Bulk Operations.

## View 
Also: Drupal View; a database query used to generate lists or tables of content. Drupal provides a powerful administrator interface for creating and editing views without any coding.

## Viewer
A "Viewer" is an add-on package that allows a solution pack to embed, display, or playback a particular object in a web accessible format. Viewers are typically a combination of a Drupal Library and a Drupal Module. The Drupal Library is the package of code that represents the player or display mechanism, and the Drupal Module is the code package that allows the Drupal Library to be accessed from within the Drupal environment.

## Views Bulk Operations
Also VBO;
 
## Virtual Machine Image
The Virtual Machine Image allows you to mount a fully working version of Islandora on your local machine as a separate virtual machine.

## Vocabulary
A Drupal configuration entity that holds taxonomy terms. The vocabulary defines what fields are available on each term.

## Weight
Drupal field that stores an integer value on an entity, allowing to represent the relative order of the entity in relation to other entities of the same type or sub-type. Used by Islandora to store the order of components in compound objects, for instance pages in paged content items (books, serials). 

---
 
Some definitions adapted from [Wikipedia](https://en.wikipedia.org/) and [Drupal.org](https://www.drupal.org/docs/user_guide/en/glossary.html)

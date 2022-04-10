The following glossary of terms addresses an Islandora context. When comparing new Islandora and Fedora to older versions it may also be helpful to reference [the Islandora 7 Glossary](https://wiki.duraspace.org/display/ISLANDORA/APPENDIX+E+-+Glossary).

## Alpaca
Event-driven middleware based on [Apache Camel](http://camel.apache.org/) that handles communication between various components of Islandora, for instance synchronizing [Drupal](#drupal) data with a [Fedora](#fedora-repository-software) repository and the [Blazegraph](#blazegraph) triple store.

## Ansible
Open source software for provisioning, configuration management and application deployment. In the context of Islandora, Ansible can be used to install and maintain the Islandora software stack more conteniently and efficiently on a server or group of servers. The configuration and installation instructions are captured in a human-readable list of tasks, called 'Playbook'. The [Islandora Playbook](#islandora-playbook) for Ansible is one of the installation methods currently supported by the Islandora community.

## Apache
Refers to the [Apache Software Foundation](https://www.apache.org/), a not-for-profit organization supporting various open source software development projects. The Islandora software stack is comprised of different components that are developed under the umbrella of the Apache Software Foundation, for instance [Apache ActiveMQ](https://activemq.apache.org/), [Apache Camel](http://camel.apache.org/), the [Apache HTTP server (webserver)](https://httpd.apache.org/), [Apache Karaf](https://karaf.apache.org/), [Apache Solr](https://solr.apache.org/), and [Apache Tomcat](https://tomcat.apache.org/).

Can in a narrower sense refer to the Apache HTTP server.

## API
See Application Programming Interface

## Application Programming Interface
Also API; a connection between computers or between computer programs. It is a type of software interface, offering a service to other pieces of software.

## Blazegraph
Blazegraph is an open source triplestore and graph database. Islandora ships Blazegraph as part of the software stack. Metadata about [Resource nodes](#resource-node) is synchronized between the [Drupal](#drupal) database and Blazegraph. Data in the Blazegraph triple store can be queried using SPARQL.

## Checksum
Checksums are a sequence of numbers and letters to check data for errors. If you know the checksum of an original file, you can use a checksum utility to confirm your copy is identical. Checksums can be used to check the [Fixity](#fixity) of a file.

## CLAW
 CLAW (CLAW Linked Asset WebDataFrameWork) was the development code name for the software released in June, 2019 as _Islandora 8_, now called _Islandora_.

## Collection
A [collection](../concepts/collection.md) is a way of grouping related resources together, much like a directory on a computer. Collections can contain any number of related resource [Nodes](#node) and sub-collections.

## Content entity
[Drupal](#drupal) websites are made up of two types of entities: content entities and configuration (config) entities. Content entities include [Nodes](#node), [Media](#media), [Taxonomy terms](#taxonomy-term), users, comments, and files. Configuration entities include all stored settings, for instance [Views](#view), custom blocks, menus, or roles.

Content entities are grouped into distinct [content entity types](#content-type), which play a specific functional role in a website application built with [Drupal](#drupal). Most 'things' in a [Drupal](#drupal) website are content entities. These include web content ([Nodes](#node), which can have subtypes like article, basic page, repository item, etc.), as well as users, comments, files, and [Taxonomy terms](#taxonomy-term).

Each type of Content entity has a different set of behaviors. The interactions of these behaviors creates the site’s functionality. For example, a User may log into a site with a username and password that is stored as part of the content entity. If Users have the relevant permissions, they can write a blog post (a subtype of the [Node](#node) content entity type, with a specific data structure and behavior), and assign some tags ([Taxonomy terms](#taxonomy-term)) to the blog post.

[Drupal](#drupal) comes with several [content entity types](#content-type) preconfigured. [Drupal](#drupal)'s default configuration includes the Basic Page and (blog) Article content types, as well as Audio, Image and Video Media types. _Islandora Defaults_ adds the Repository Item [content type](#content-type), alters the existing [Media](#media) types to store files in [Fedora](#fedora-repository-software), and introduces the Extracted Text and FITS Media types.

## Content model
Deprecated concept used in Islandora Legacy; see [Islandora Model](#islandora-model).

## Content type
A general type of Content in [Drupal](#drupal). A content type defines a set of data [fields](#field) and how they are displayed. Content types are sub-types of [Nodes](#node), one of the [Drupal](#drupal) [Content entities](#content-entity). The default [Drupal](#drupal) Content types are 'Article' and 'Basic page'. _Islandora Defaults_ adds 'Repository Item' as a Content type, defining metadata fields typically used to describe digital resources.

## Crayfish
A collection of Islandora [microservices](#microservice). Some of the microservices are built specifically for use with a [Fedora](#fedora-repository-software) repository, while others are just for general use within Islandora.

## Datastream
Deprecated terminology, refers to how [Fedora 3](#fedora-repository-software)/Islandora Legacy stored files as part of a resource ('object') in the [Fedora](#fedora-repository-software) repository. Replaced by [Drupal Media entities](https://www.drupal.org/docs/8/core/modules/media/overview), which 'wraps' [Files](https://www.drupal.org/docs/8/core/modules/file/overview) in an intermediate structure. This allows Fields to be attached to files, for instance for storing technical metadata.

## Derivative
A version of a file which is derived from an uploaded file. For example, a thumbnail generated from an uploaded image. Islandora uses [microservices](#microservice) to generate derivatives.

## Docker
[Docker](https://www.docker.com/) is a platform that use OS-level virtualization to deliver software in packages called containers. Islandora uses Docker as part of [ISLE](#isle), a suite of Docker containers that run the various components of Islandora required by Islandora Defaults.

## Drupal
Drupal is an open source web content management system (CMS) written in PHP. Known for being extremely flexible and extensible, Drupal is supported by a community of over 630,000 users and developers. Drupal sites can be customized and themed in a wide variety of ways.

## Drupal Core
The files, themes, profiles, and modules included with the standard project software download.

## Drupal Roles
Roles are a way of assigning specific permissions to a group of users. Any user assigned to a role will have the same permissions as all other users assigned to that role. This allows you to control which users have permission to view, edit, or delete content in [Drupal](#drupal). Islandora provides a special role called _fedoraAdmin_ that is required to have actions in [Drupal](#drupal) reflected in [Fedora](#fedora-repository-software).

## Entity
A [Drupal](#drupal) term for an item of either content or configuration data. Examples include [Nodes](#node) (content items), Blocks, [Taxonomy terms](#taxonomy-term), and definitions of [content types](#content-type); the first three are content entities, and the last is a configuration entity. In common usage, the term often refers to [Drupal](#drupal) content entities like [Nodes](#node) or [Taxonomy terms](#taxonomy-term).

## Fedora (Repository Software)
Fedora is a digital asset management architecture upon which institutional repositories, digital archives, and digital library systems might be built. Fedora is the underlying architecture for a digital repository, and is not a complete management, indexing, discovery, and delivery application.

The Fedora repository functions as the standard _smart storage_ for Islandora.

## Field
Data of a certain type that is attached to a content entity. For instance, on a Resource Node [content type](#content-type), you might have fields for a title, description, display hints, subjects, and other metadata.

## Field instance

## Field type

## FITS
[File Information Tool Set](https://projects.iq.harvard.edu/fits), a set of software components for identifying, validating and extracting of technical metadata for a wide range of file formats.

## Fixity
Also file fixity; digital preservation term meaning that a digital file remains unchanged ('fixed') over time. Fixity checking verifies that a file has not been corrupted or manipulated during a transfer process or while being stored. Typically, a fixity checking process computes [checksums](#checksum) or cryptographic hashes for a file and compares the result to a reference value stored earlier. The [Riprap](https://github.com/mjordan/riprap) [microservice](#microservice) and the contributed [Riprap Islandora module](https://github.com/mjordan/islandora_riprap) support fixity checking and error reporting in Islandora.

## Flysystem
Flysystem is a filesystem abstraction library for PHP. Islandora uses Flysystem to swap about different backend filesystem applications. Islandora provides a custom Flysystem adapter for [Fedora](#fedora-repository-software).

## GLAM
Acronym for "galleries, libraries, archives, and museums".

## IIIF
The [International Image Interoperability Framework](https://iiif.io/). A set of open standards that help archives, libraries, and museums make the most of their digitized collections with deep zoom, annotation capabilities, and more, and also the community of users and developers that suport the framework.

## Ingest
To ingest an object is to add an entry for it in Islandora. This can be done through the [Drupal](#drupal) graphical user interface or one of the Drupal [APIs](#api) ([REST](https://www.drupal.org/docs/drupal-apis/restful-web-services-api/restful-web-services-api-overview), [Migrate API](https://www.drupal.org/docs/drupal-apis/migrate-api/migrate-api-overview)). The third-party contributed software [Islandora Workbench](https://github.com/mjordan/islandora_workbench) uses the Drupal REST API for convenient bulk ingest.

In the context of digital repositories, ingest refers to the process by which the repository software imports and subsequently processes an object, creating derivatives automatically, and running any other processing that is configured to occur when an object is added. This would be distinguished by software which simply stores objects after import (with or without associated files) and performs no processing. The Islandora GUI and the documentation sometimes use other terms such as 'import' or 'add resource node'. In such contexts, these terms generally refer to the ingest process.

## Islandora model

## Islandora playbook
A set of human-readable [YAML](#yaml) files, containing instructions for automatically configuring a server environment and installing the different components of the Islandora software stack. The instructions recorded in Playbook are executed by [Ansible](#ansible). The Islandora Playbook for Ansible is one of the installation methods currently supported by the Islandora community.

## ISLE
ISLE, or ISLandora Enterprise, is a community initiative to ease the installation and maintenance of Islandora by using [Docker](#docker). ISLE is one of the installation methods currently supported by the Islandora community.

## JSON-LD
[JSON-LD (JavaScript Object Notation for Linked Data)](https://json-ld.org/) is a method of encoding [linked data](#linked-data) using JSON.

## Linked data
In computing, linked data is structured data which is interlinked with other data so it becomes more useful through semantic queries. Linked data typically employs the [Resource Description Framework](#resource-description-framework) for data modelling.

## Matomo
[Matomo](https://matomo.org/), formerly called Piwik, is a software for tracking visits to websites. It is an open source alternative to Google Analytics and allows the generation of website usage reports.

## Media
Media are a [Drupal](#drupal) [Content entity](#content-entity) type, which allows to manage Media items (Files) like images, documents, slideshows, YouTube videos, tweets, Instagram photos, etc. The Media module provides a unified User Interface where editors and administrators can upload, manage, and reuse files and multimedia assets. In the context of Islandora, Media entities 'wrap' files and provide a place to store file-specific metadata.

See https://www.drupal.org/docs/8/core/modules/media/overview for more information on the Drupal foundations, and refer to https://islandora.github.io/documentation/user-documentation/media/ for how Islandora uses Media.

## Memento
Protocol specification that allows a web client to request an earlier/historic state web resource (if available). Fedora implements the Memento protocol to store and serve versions of content in a Fedora repository.

## Microservice
A software development technique — a variant of the service-oriented architecture (SOA) structural style — that arranges an application as a collection of loosely coupled services. In a microservices architecture, services are fine-grained and the protocols are lightweight.

## Module
Software (usually PHP, JavaScript, and/or CSS) that extends site features and adds functionality. [Drupal](#drupal) modules conform to a specific structure allowing them to integrate with the [Drupal](#drupal) architecture.

## Node
A node is any piece of individual content, such as a page, article, forum topic, or a blog entry. All content on a [Drupal](#drupal) website is stored and treated as "Nodes". For information about Nodes specific to Islandora, see [Resource Node](#resource-node).

## OAI-PMH
The [Open Archives Initiative Protocol for Metadata Harvesting (OAI-PMH)](http://www.openarchives.org/pmh/) is a protocol developed for harvesting metadata descriptions of records in an archive so that services can be built using (aggregated) metadata from many archives. Islandora allows to publish metadata in a way conformant to OAI-PMH, acting as a so-called OAI-PMH endpoint.

## Ontology
In computer science and information science, an ontology encompasses a representation, formal naming and definition of the categories, properties and relations between concepts, data and entities. In the narrower context of the [Resource Description Framework](#resource-description-framework) (RDF), an ontology is a formal, machine-readable description of the 'vocabulary' that can be used in a knowledge graph. An RDF ontology for instance specifies _classes_ of things or concepts (e.g. the class of all book authors) and _properties_ of classes/class instances (e.g. an author's name, birth date, shoe size; also the fact that an author has written something that is in the class of books).

## Open Source
Open source describes a method of software development that promotes access to the end product's source code. Islandora is an open source product with an active development community, operating under the [GPL license (2.0)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) for [Drupal](#drupal) components and the [MIT license](https://opensource.org/licenses/MIT) for non-[Drupal](#drupal) components.

## PR
See [Pull request](#pull-request)

## Pull request
Also PR; sometimes also known as merge requests; technical term from distributed version control systems for software code like Git. Code contributors can request that the maintainer of a code repository 'pulls' the code change into the repository after approval.

## RDF
See [Resource Description Framework](#resource-description-framework)

## Repository Item
See: [Resource Node](#resource-node)

## Resource Description Framework
Also RDF; family of World Wide Web Consortium (W3C) specifications originally designed as a data model for metadata. It has come to be used as a general method for conceptual description or modeling of information that is implemented in web resources. The data is modelled as a set of statements, also known as triples. A collection of RDF statements intrinsically represents a directed graph. Data represented according to the RDF specifications can be serialized in different ways, for instance using [JSON-LD](#json-ld).

## Resource Node
A Resource node is a [Drupal](#drupal) [Node](#node) that represents a single conceptual item or object stored in an Islandora repository. It acts as a container for all files and metadata associated with that item, and is the place where the item 'lives' as a visitable URI.

The term 'Resource node' is specific to Islandora. Typically, Resource nodes in an Islandora installation will use a specific [Content type](#content-type) for the digital assets stored in the repository.

For example, a video stored in Islandora will have a Resource node, with metadata stored in [Fields](#field). Attached to the Resource node is a [Media](#media) entity, which encapsulates the preservation-grade file. The Resource node may be linked to further [Media](#media), for instance for a thumbnail, web-friendly derivative, and technical metadata associated with the resource node. The Resource node may also belong to one or more collections.

## Taxonomy
[Drupal](#drupal) core module for managing vocabularies (lists) of [taxonomy terms](#taxonomy-term). [Drupal](#drupal) generally uses [terms contained in taxonomies or vocabularies](#taxonomy-term) to classify content (tag or category).

In Islandora, taxonomies are used to establish controlled vocabularies for describing resources, for instance for standardised spellings of names or subject terms. Vocabularies and [taxonomy terms](#taxonomy-term) are used in Islandora to establish locally controlled vocabularies for describing resources, for instance for standardised spellings of names or subject terms.

## Taxonomy term
In the context of Islandora, a taxonomy term is a [Drupal](#drupal) [Content entity](#content-entity) of the type 'taxonomy terms'. Taxonomy terms are used in Islandora to establish locally controlled vocabularies for describing resources, for instance for standardised spellings of names or subject terms.

## Theme
Software and asset files (images, CSS, PHP code, and/or templates) that determine the style and layout of the site. The [Drupal](#drupal) project distinguishes between core and contributed themes.

## Vagrant
[Vagrant](https://www.vagrantup.com/) is an open-source software product for building and maintaining portable virtual software development environments (virtual machines). The [Islandora Playbook](#islandora-playbook) includes a 'vagrantfile', a set of instructions that allows users to create a local virtual machine environment which will subsequently run [Ansible](#ansible) to execute the configuration and installation steps recorded in the [Islandora Playbook](#islandora-playbook).

## VBO
See [Views Bulk Operations](#views-bulk-operations).

## View
Also: [Drupal](#drupal) View; a database query used to generate lists or tables of content. [Drupal](#drupal) provides a powerful administrator interface for creating and editing views without any coding.

## Viewer
A Viewer is an add-on package that allows a solution pack to embed, display, or playback a particular object in a web accessible format. Viewers are typically a combination of a [Drupal](#drupal) Library and a [Drupal](#drupal) [Module](#module). The [Drupal](#drupal) Library is the package of code that represents the player or display mechanism, and the [Drupal](#drupal) [Module](#module) is the code package that allows the [Drupal](#drupal) Library to be accessed from within the [Drupal](#drupal) environment.

## Views Bulk Operations
Also VBO; a [Drupal](#drupal) [Module](#module) for performing bulk/batch operations on [Nodes](#node) selected by a [View](#view) definition.

## Virtual Machine Image
The Virtual Machine Image allows you to mount a fully working version of Islandora on your local machine as a separate virtual machine.

## Vocabulary
A [Drupal](#drupal) configuration entity that holds taxonomy terms. The vocabulary defines what fields are available on each term.

## Weight
[Drupal](#drupal) field that stores an integer value on an entity, allowing to represent the relative order of the entity in relation to other entities of the same type or sub-type. Used by Islandora to store the order of components in compound objects, for instance pages in paged content items (books, serials).

## YAML
[YAML](https://yaml.org/) is a human-readable data-serialization language. It is commonly used for configuration files and in applications where data is being stored or transmitted. Software applications like [Drupal](#drupal) or [Ansible](#ansible) store configuration information in YAML files for easy transportability of a configuration.

---

Some definitions adapted from [Wikipedia](https://en.wikipedia.org/) and [[Drupal](#drupal).org](https://www.[Drupal](#drupal).org/docs/user_guide/en/glossary.html)

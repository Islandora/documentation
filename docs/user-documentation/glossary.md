# Glossary

The following glossary of terms addresses an Islandora context. When comparing new Islandora and Fedora to older versions it may also be helpful to reference [the Islandora 7 Glossary](https://wiki.duraspace.org/display/ISLANDORA/APPENDIX+E+-+Glossary).

## Alpaca
Islandora's event-driven middleware based on [Apache Camel](http://camel.apache.org/) that handles communication between various components of Islandora, for instance synchronizing [Drupal](#drupal) data with a [Fedora](#fedora-repository-software) repository and the [Blazegraph](#blazegraph) triple store.

## Ansible
Open source software for provisioning, configuration management and application deployment. In the context of Islandora, Ansible can be used to install and maintain the Islandora software stack more conveniently and efficiently on a server or group of servers. The configuration and installation instructions are captured in a human-readable list of tasks, called 'Playbook'. The [Islandora Playbook](#islandora-playbook) for Ansible is one of the installation methods currently supported by the Islandora community.

## Apache
Refers to the [Apache Software Foundation](https://www.apache.org/), a not-for-profit organization supporting various open source software development projects. The Islandora software stack consists of different components that are developed under the umbrella of the Apache Software Foundation, for instance [Apache ActiveMQ](https://activemq.apache.org/), [Apache Camel](http://camel.apache.org/), the [Apache HTTP server (webserver)](https://httpd.apache.org/), [Apache Karaf](https://karaf.apache.org/), [Apache Solr](https://solr.apache.org/), and [Apache Tomcat](https://tomcat.apache.org/).

Can in a narrower sense refer to the Apache HTTP server.

## API
See Application Programming Interface

## Application Programming Interface
Also API; a connection between computers or between computer programs. It is a type of software interface, offering a service to other pieces of software.

## Blazegraph
Blazegraph is an open source triplestore and graph database. Islandora ships Blazegraph as part of the software stack. Metadata about [Resource nodes](#resource-node) is synchronized between the [Drupal](#drupal) database and Blazegraph. Data in the Blazegraph triple store can be queried using SPARQL.

## Bundle
A bundle is the generic name for a sub-type of a [Content Entity](#content-entity) type in [Drupal](#drupal). To illustrate: Node and Taxonomy Term are both names of Content Entity types, and both have sub-types ("bundles"). The bundles of Node are called ["Content Types"](#content-type) and the bundles of [Taxonomy Term](#taxonomy-term) are called ["Vocabularies"](#vocabulary). Each bundle includes its own configurations of what [fields](#field) are present on the bundle and how they are entered and displayed. A bundle is thus part of the [configuration](#configuration) of your site. Some Content Entity Types, such as User, do not have bundles.

## Cantaloupe
[Cantaloupe](https://cantaloupe-project.github.io) is an image server written in Java. It implements the [IIIF](#iiif) Image API, which means it handles deep zooming of large images and other image manipulations. It is required to serve images to some [viewers](#viewer) such as [Mirador](#mirador) and [OpenSeadragon](#openseadragon).

## Checksum
Checksums are a sequence of numbers and letters to check data for errors. If you know the checksum of an original file, you can use a checksum utility to confirm your copy is identical. Checksums can be used to check the [Fixity](#fixity) of a file.

## CLAW
 CLAW (CLAW Linked Asset WebDataFrameWork) was the development code name for the software released in June 2019 as _Islandora 8_, now called _Islandora_.

## Collection
A [collection](../concepts/collection.md) is a way of grouping related resources together, much like a directory on a computer. Collections can contain any number of related resource [Nodes](#node) and sub-collections.

## Configuration
See also: [Configuration entity](#configuration-entity)

Contrast: [Content](#content)

In [Drupal](#drupal), your configuration is the total set of [configuration entities](#configuration-entity) that are live in your site. Configuration is usually managed through the Drupal GUI, and it can also be exported and imported. When it is active in your site, configuration lives in the Drupal database. When it is exported or serialized, configuration appears as a set of [YAML](#yaml) (.yml) files, one file per configuration entity. Configuration can be [overridden in the settings.php file](https://www.drupal.org/docs/drupal-apis/configuration-api/configuration-override-system).

## Configuration entity
See also: [Configuration](#configuration)

Contrast: [Content entity](#content-entity)

A [Drupal](#drupal) configuration entity (or "config entity") is an individual piece that makes up your site's [configuration](#configuration). It is usually represented as a single [YAML](#yaml) (.yml) file, though the actual ("live") configuration lives in the database. A config entity usually represents the results of saving a single form in the administration interface, and may contain multiple (usually related) individual settings. Each configuration entity can be exported or imported as a "single item" through the Configuration Synchronization GUI, or with the Devel module's "config editor" can be edited individually. However, config entities are often interrelated and manual editing is usually not recommended.

## Content
See also: [Content Entity](#content-entity)

Contrast: [Configuration](#configuration)

In [Drupal](#drupal), your content is the total set of things that have been created or uploaded "as content" in your website. This includes all [content entities](#content-entity) - the actual nodes, media, files, taxonomy terms, etc, but does not include anything that is [configuration](#configuration). Content can be exported and imported, but only between sites with exactly the same configuration.

Sometimes, "Content" is used to refer to [Nodes](#node) but not other content entities. This is the case when creating a new [View](#view) and one of the options is to make a view of "Content".

## Content entity
See also: [Content](#content)

Contrast: [Configuration entity](#configuration-entity)

In [Drupal](#drupal), content entities are the actual [nodes](#node), [media](#media), [taxonomy terms](#taxonomy-term), users, comments, and files that you've created on your site. For example, you may have 223 nodes, 534 media, 1000 taxonomy terms, 14 users, and 535 files in your site - those counts represent the numbers of content entities present in your site. "Node", "Media", "Taxonomy term" etc. are the high-level "types" of content entities. Some of these types have sub-types which are called [bundles](#bundle).

Content entities should not be confused with [content types](#content-type), which are [bundles](#bundle) of [nodes](#node), and are part of a site's [configuration](#configuration).

## Content model
Deprecated concept used in Islandora Legacy; see [Islandora Model](#islandora-model).

## Content type
A type of [Node](#node). Content types are the "[bundles](#bundle)" of Nodes, which are a type of [Content Entity](#content-entity) in [Drupal](#drupal). A content type importantly defines a set of [fields](#field) and how they are displayed. While a content type describes a type of content entity, the information that makes up the content type itself is all part of your site's [configuration](#configuration).

The standard Drupal Content types are 'Article' and 'Basic page'. _Islandora Starter Site_ adds 'Repository Item' as a Content type, defining metadata fields typically used to describe digital resources. You can easily create your own content types.

## Context
An "if-this-then-that" configuration created using the Drupal [Context](https://www.drupal.org/project/context) contrib module. Islandora extends the capabilities of Context by adding custom Conditions, custom Reactions, and by evaluating context at specific times to allow Contexts to be used for derivatives, indexing, and display.


## Crayfish
A collection of Islandora [microservices](#microservice). Some microservices are built specifically for use with a [Fedora](#fedora-repository-software) repository, while others are just for general use within Islandora.

## Datastream
Deprecated terminology, refers to how [Fedora 3](#fedora-repository-software)/Islandora Legacy stored files as part of a resource ('object') in the [Fedora](#fedora-repository-software) repository. Replaced by [Drupal Media entities](https://www.drupal.org/docs/8/core/modules/media/overview), which 'wraps' [Files](https://www.drupal.org/docs/8/core/modules/file/overview) in an intermediate structure. This allows Fields to be attached to files, for instance for storing technical metadata.

## Derivative
A version of a file which is derived from an uploaded file. For example, a thumbnail generated from an uploaded image. Islandora uses [microservices](#microservice) to generate derivatives. See the concept page for [Derivatives](../concepts/derivatives.md).

## Docker
[Docker](https://www.docker.com/) is a platform that use OS-level virtualization to deliver software in packages called containers. Islandora uses Docker as part of [ISLE](#isle), a suite of Docker containers that run the various components of Islandora.

## Drupal
Drupal is an open source web content management system (CMS) written in PHP. Known for being extremely flexible and extensible, Drupal is supported by a community of over 630,000 users and developers. Drupal sites can be customized and themed in a wide variety of ways. Drupal sites must include [Drupal Core](#drupal-core) and usually involve additional, Contributed code.

## Drupal Core
The files, themes, profile, and modules included with the standard project software download.

## Drupal Roles
Roles are a way of assigning specific permissions to a group of users. Any user assigned to a role will have the same permissions as all other users assigned to that role. This allows you to control which users have permission to view, edit, or delete content in [Drupal](#drupal). Islandora provides a special role called _fedoraAdmin_ that is required to have actions in Drupal reflected in [Fedora](#fedora-repository-software).

## Entity
A [Drupal](#drupal) term for an item of either content or configuration data. Examples include [Nodes](#node) (content items), Blocks, [Taxonomy terms](#taxonomy-term), and definitions of [content types](#content-type); the first three are [content entities](#content-entity), and the last is a [configuration entity](#configuration-entity). In common usage, the term often refers to Drupal content entities like [Nodes](#node) or [Taxonomy terms](#taxonomy-term).

## Fedora (Repository Software)
Fedora is a digital asset management architecture upon which institutional repositories, digital archives, and digital library systems might be built. Fedora is the underlying architecture for a digital repository, and is not a complete management, indexing, discovery, and delivery application.

The Fedora repository functions as the standard _smart storage_ for Islandora.

## FFmpeg
[FFmpeg](https://ffmpeg.org/) is a cross-platform audio and video processing software. In Islandora, FFmpeg is provided by the [Crayfish](#crayfish) [microservice](#microservice), [Homarus](#homarus).


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

## GUI
Acronym for "Graphical User Interface". Often refers to taking actions through Drupal's administrative interface in a web browser as opposed to effecting the same changes through Drush or programmatically. 

## Greenfield
An installation without legacy constraints. Usually refers to a brand new system where users load new content, as opposed to migrating content from a previous system.

## Imagemagick
[Imagemagick](https://imagemagick.org/index.php) is an open-source image processing library. In Islandora, Imagemagick is provided by the [Crayfish](#crayfish) [Microservice](#microservice), [Houdini](#houdini).

## hOCR
[hOCR](https://kba.github.io/hocr-spec/1.2/) is an open standard for representing OCR (Optical Character Recognition) results, including text positioning, as HTML. hOCR can be produced by [Tesseract](#tesseract), and can be displayed as an overlay on an image by [Mirador](#mirador).


## Homarus
Homarus is a [microservice](#microservice) wrapper for [FFMpeg](#ffmpeg). It is part of [Crayfish](#crayfish).

## Houdini
Houdini is a [microservice](#microservice) wrapper for [Imagemagick](#imagemagick). It is part of [Crayfish](#crayfish).


## Hypercube
Hypercube is a [microservice](#microservice) wrapper for [Tesseract](#tesseract). It is part of [Crayfish](#crayfish).

## IIIF
The [International Image Interoperability Framework](https://iiif.io/). Generally pronounced "triple-eye-eff." A set of open standards and APIs that help archives, libraries, and museums make the most of their digitized collections with deep zoom, annotation capabilities, and more, and also the community of users and developers that support the framework.

## IIIF Manifest
Defined in the [IIIF](#iiif) Presentation API, it is a document that includes "The overall description of the structure and properties of the digital representation of an object." In Islandora, it lists one or more files, in order, that can be displayed in a [viewer](#viewer) such as [Mirador](#mirador) or [OpenSeadragon](#openseadragon).


## Ingest
To ingest an object is to add an entry for it in Islandora. This can be done through the [Drupal](#drupal) graphical user interface or one of the Drupal [APIs](#api) ([REST](https://www.drupal.org/docs/drupal-apis/restful-web-services-api/restful-web-services-api-overview), [Migrate API](https://www.drupal.org/docs/drupal-apis/migrate-api/migrate-api-overview)). The third-party contributed software [Islandora Workbench](https://github.com/mjordan/islandora_workbench) uses the Drupal REST API for convenient bulk ingest.

In the context of digital repositories, ingest refers to the process by which the repository software imports and subsequently processes an object, creating derivatives automatically, and running any other processing that is configured to occur when an object is added. This would be distinguished by software which simply stores objects after import (with or without associated files) and performs no processing. The Islandora GUI and the documentation sometimes use other terms such as 'import' or 'add resource node'. In such contexts, these terms generally refer to the ingest process.

## Islandora 8 (8.x, 2.0)
Islandora 8, 8.x, 2.0, and CLAW are all deprecated names for the current version of Islandora. They referred to Islandora's use of Drupal 8, and being a major shift away from Islandora Legacy (formerly known as Islandora 7 or 7.x as it runs on Drupal 7).

## Islandora Install Profile
The Islandora Install Profile (in GitHub as [Islandora Install Profile Demo](https://github.com/Islandora-Devops/islandora_install_profile_demo), is a Drupal install profile that was developed by Born Digital, an Islandora vendor. It defines an Islandora with additional modules, themes, and configurations that were not defined in the Islandora Starter Site (formerly Islandora Defaults). The Install Profile and the Starter Site share the same function (though they approach it differently) and it is not possible to use both.

## Islandora Starter Site
The Islandora Starter Site is a way to install Drupal that provides a functional Islandora "out of the box." It was created from Islandora Defaults [now defunct] by discoverygarden inc, an Islandora vendor. The [Islandora Install Profile](#islandora-install-profile) and the Starter Site share the same function (though they approach it differently) and it is not possible to use both.


## Islandora model

"Islandora Models" is a taxonomy vocabulary that comes by default with Islandora. As of 2024-08-12, it includes the following terms:

- Audio
- Binary
- Collection
- Compound Object
- Digital Document
- Image
- Newspaper
- Page
- Paged Content
- Publication Issue
- Video

The Repository Item Content type (part of the Islandora Starter Site) has a “Model” field instance which is an Entity reference field configured to be populated by references to terms in this vocabulary. The “Model” field is one of only two required fields on the Repository Item in the default settings. 

Contexts and other system code (such as themes) may use this field to control the display and behavior of different Repository Item types. 


## Islandora playbook
A set of human-readable [YAML](#yaml) files, containing instructions for automatically configuring a server environment and installing the different components of the Islandora software stack. The instructions recorded in Playbook are executed by [Ansible](#ansible). The Islandora Playbook for Ansible is one of the installation methods currently supported by the Islandora community.

## ISLE
ISLE, or ISLandora Enterprise, is a community initiative to ease the installation and maintenance of Islandora by using [Docker](#docker). ISLE is one of the installation methods currently supported by the Islandora community.

## JSON-LD
[JSON-LD (JavaScript Object Notation for Linked Data)](https://json-ld.org/) is a method of encoding [linked data](#linked-data) using JSON.

## Linked data
In computing, linked data is structured data which is interlinked with other data so it becomes more useful through semantic queries. Linked data typically employs the [Resource Description Framework](#resource-description-framework) for data modelling.

## Manifest
See [IIIF Manifest](#iiif-manifest).


## Matomo
[Matomo](https://matomo.org/), formerly called Piwik, is a software for tracking visits to websites. It is an open source alternative to Google Analytics and allows the generation of website usage reports.

## Media
Media are a [Drupal](#drupal) [Content entity](#content-entity) type, which allows to manage Media items (Files) like images, documents, slideshows, YouTube videos, tweets, Instagram photos, etc. The Media module provides a unified User Interface where editors and administrators can upload, manage, and reuse files and multimedia assets. In the context of Islandora, Media entities 'wrap' files and provide a place to store file-specific metadata.

See https://www.drupal.org/docs/8/core/modules/media/overview for more information on the Drupal foundations, and refer to https://islandora.github.io/documentation/user-documentation/media/ for how Islandora uses Media.

## Memento
Protocol specification that allows a web client to request an earlier/historic state web resource (if available). Fedora implements the Memento protocol to store and serve versions of content in a Fedora repository.

## Mirador
[Mirador](https://projectmirador.org) is a javascript-based zoomable image [Viewer](#viewer). It is related to (and more fully-featured than) [OpenSeadragon](#openseadragon). It has the ability to do zooming, display multiple pages, and display positioned text (e.g. [hOCR](#hocr) or attributions). To render an image through Mirador, it must be provided a [IIIF Manifest](#iiif-manifest) and the images must be served through a [IIIF](#iiif)-friendly image server such as [Cantaloupe](#cantaloupe).

## Microservice
A software development technique — a variant of the service-oriented architecture (SOA) structural style — that arranges an application as a collection of loosely coupled services. In a microservices' architecture, services are fine-grained and the protocols are lightweight.

## Module
Software (usually PHP, JavaScript, and/or CSS) that extends site features and adds functionality. [Drupal](#drupal) modules conform to a specific structure allowing them to integrate with the Drupal architecture.

## Node
Usually refers to a piece of Drupal [Content](#content-entity) of the type 'Node'. This includes actual pages, articles, and [Resource nodes](#resource-node). Nodes must belong to a specific node bundle, called a ["Content Type"](#content-type).

## OAI-PMH
The [Open Archives Initiative Protocol for Metadata Harvesting (OAI-PMH)](http://www.openarchives.org/pmh/) is a protocol developed for harvesting metadata descriptions of records in an archive so that services can be built using (aggregated) metadata from many archives. Islandora allows to publish metadata in a way conformant to OAI-PMH, acting as a so-called OAI-PMH endpoint.

## Ontology
In computer science and information science, an ontology encompasses a representation, formal naming and definition of the categories, properties and relations between concepts, data and entities. In the narrower context of the [Resource Description Framework](#resource-description-framework) (RDF), an ontology is a formal, machine-readable description of the 'vocabulary' that can be used in a knowledge graph. An RDF ontology for instance specifies _classes_ of things or concepts (e.g. the class of all book authors) and _properties_ of classes/class instances (e.g. an author's name, birthdate, shoe size; also the fact that an author has written something that is in the class of books).

## Open Source
Open source describes a method of software development that promotes access to the end product's source code. Islandora is an open source product with an active development community, operating under the [GPL license (2.0)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) for [Drupal](#drupal) components and the [MIT license](https://opensource.org/licenses/MIT) for non-Drupal components.

## OpenSeadragon
[OpenSeadragon](https://openseadragon.github.io/) is javascript-based zoomable image [Viewer](#viewer). It has the ability to do zooming and display multiple pages. To render an image through OpenSeadragon, it must be provided in a [IIIF Manifest](#iiif-manifest).

## PR
See [Pull request](#pull-request)

## Pull request
Also PR; sometimes also known as merge requests; technical term from distributed version control systems for software code like Git. Code contributors can request that the maintainer of a code repository 'pulls' the code change into the repository after approval.

## RDF
See [Resource Description Framework](#resource-description-framework)

## Repository Item
A type of content entity that comes "out of the box" with the Islandora Starter Site.
See also: [Resource Node](#resource-node)

## Resource Description Framework
Also RDF; family of World Wide Web Consortium (W3C) specifications originally designed as a data model for metadata. It has come to be used as a general method for conceptual description or modeling of information that is implemented in web resources. The data is modelled as a set of statements, also known as triples. A collection of RDF statements intrinsically represents a directed graph. Data represented according to the RDF specifications can be serialized in different ways, for instance using [JSON-LD](#json-ld).

## Resource Node
A Resource node is a generic Islandora term for a [Drupal](#drupal) [Node](#node) that represents a single conceptual item or object stored in an Islandora repository. It acts as a stand-in for all files and metadata associated with that item, and is the place where the item 'lives' as a visitable URI.

The term 'Resource node' is specific to Islandora. Typically, Resource nodes in an Islandora installation will use a specific [Content type](#content-type) for the digital assets stored in the repository.

For example, a video stored in Islandora will have a Resource node, with metadata stored in [Fields](#field). Attached to the Resource node is a [Media](#media) entity, which encapsulates the preservation-grade file. The Resource node may be linked to further [Media](#media), for instance for a thumbnail, web-friendly derivative, and technical metadata associated with the resource node. The Resource node may also belong to one or more collections.

## Taxonomy term
A [Drupal](#drupal) [Content Entity](#content-entity) of the type 'taxonomy term'. Taxonomy terms belong to [vocabularies](#vocabulary) which define what [fields](#field) are available and how they behave. Drupal generally uses [terms contained in taxonomies or vocabularies](#taxonomy-term) to classify content (tag or category). Taxonomy terms are used in Islandora to establish locally controlled vocabularies for describing resources, for instance for standardised spellings of names or subject terms.

## Tesseract
[Tesseract](https://github.com/tesseract-ocr/tesseract) is an open-source OCR (Optical Character Recognition) software. It can perform OCR in multiple languages. It can produce OCR (plain text) and [hOCR](#hocr) (HTML, which includes positional data). In Islandora, Tesseract is provided by the [Crayfish](#crayfish) [Microservice](#microservice), [Hypercube](#hypercube).


## Theme
Software and asset files (images, CSS, PHP code, and/or templates) that determine the style and layout of the site. The [Drupal](#drupal) project distinguishes between core and contributed themes.

## Vagrant
[Vagrant](https://www.vagrantup.com/) is an open-source software product for building and maintaining portable virtual software development environments (virtual machines). The [Islandora Playbook](#islandora-playbook) includes a 'vagrantfile', a set of instructions that allows users to create a local virtual machine environment which will subsequently run [Ansible](#ansible) to execute the configuration and installation steps recorded in the [Islandora Playbook](#islandora-playbook).

## VBO
See [Views Bulk Operations](#views-bulk-operations).

## View
Drupal Views let you query the database to generate lists of [content](#content-entity), and format them as lists, tables, slideshows, maps, blocks, and many more. The Views UI module, part of Drupal Core, provides a powerful administrator interface for creating and editing views without any code. There is a large ecosystem of extension modules for Views.

Views power many of the Islandora features, including [viewers](#viewer), [IIIF Manifests](#iiif-manifest), and search.

## View Mode
A View Mode is a way that a piece of Drupal [content](#content-entity) can be rendered. View modes let you create alternate configurations for what [fields](#field) get displayed, in what order, and rendered in what field formatters. View modes are created under Manage > Display Modes > View Modes, but are configured at the [bundle](#bundle) level (after first enabling that view mode to have its own configuration). If the requested view mode does not have a custom configuration, then the "Default" view mode will be used.

In [Views](#view), you can choose to show "Rendered entities" (usually as opposed to "Fields"). Here, you can select which view mode to use to render the results.

## Viewer
A Viewer is any tool that allows [Drupal](#drupal) to embed, display, or play back a particular object in a web-accessible format. Viewers are typically projects unto themselves. To use a viewer within Drupal usually involves a Library containing the viewer's code, as well as a Drupal [Module](#module) that makes the viewer code appear within Drupal. Usually a viewer displays a single binary file, but some viewers (e.g. Mirador and OpenSeadragon) can display an entire [manifest](#iiif-manifest) (ordered list of files).

## Views Bulk Operations
Also called VBO; a [Drupal](#drupal) [Module](#module) for performing bulk/batch operations on [Nodes](#node) selected by a [View](#view) definition.

## Virtual Machine Image
The Virtual Machine Image allows you to mount a fully working version of Islandora on your local machine as a separate virtual machine.

## Vocabulary
A [Drupal](#drupal) configuration entity that holds [taxonomy terms](#taxonomy-term). The vocabulary defines what fields are available on each term and how the terms behave. Vocabularies are the ["bundles"](#bundle) of taxonomy terms.

## Weight
[Drupal](#drupal) field that stores an integer value on an entity, allowing to represent the relative order of the entity in relation to other entities of the same type or subtype. Used by Islandora to store the order of components in compound objects, for instance pages in paged content items (books, serials).

## YAML
[YAML](https://yaml.org/) is a human-readable data-serialization language. It is commonly used for configuration files and in applications where data is being stored or transmitted. Software applications like [Drupal](#drupal) or [Ansible](#ansible) store configuration information in YAML files for easy transportability of a configuration.

---

Some definitions adapted from [Wikipedia](https://en.wikipedia.org/) and [Drupal.org](https://www.drupal.org/docs/user_guide/en/glossary.html)

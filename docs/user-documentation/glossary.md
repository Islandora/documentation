The following glossary of terms addresses an Islandora 8 context. When comparing new Islandora and Fedora to older versions it may also be helpful to reference [the Islandora 7 Glossary](https://wiki.duraspace.org/display/ISLANDORA/APPENDIX+E+-+Glossary).
 
## CLAW
 CLAW (CLAW Linked Asset WebDataFrameWork) was the development code name for the software released in June, 2019 as _Islandora 8_.
 
## Collection
Collections are a way of grouping related objects together in Islandora. They function much like directories on a computer; a collection can “contain” any number of related resource nodes and sub-collections. 
 
## Drupal 
Drupal is an open source content management system (CMS) written in PHP. Known for being extremely flexible and extensible, Drupal is supported by a community of over 630,000 users and developers. Drupal sites can be customized and themed in a wide variety of ways.
 
Drupal functions as the _front-end_ of Islandora.
 
## Drupal Core
The files, themes, profiles, and modules included with the standard project software download.
 
## Drupal Roles
Roles are a way of assigning specific permissions to a group of users. Any user assigned to a role will have the same permissions as all other users assigned to that role. This allows you to control which users have permission to view, edit, or delete content in Drupal. Islandora provides a special role called _fedoraAdmin_ that is required to have actions in Drupal reflected in Fedora.
 
## Entity 
A Drupal term for an item of either content or configuration data, although in common usage, the term often refers to content entities. Examples include content items, custom blocks, taxonomy terms, and definitions of content types; the first three are content entities, and the last is a configuration entity. 
 
## Fedora (Repository Software)
Fedora is a digital asset management architecture upon which institutional repositories, digital archives, and digital library systems might be built. Fedora is the underlying architecture for a digital repository, and is not a complete management, indexing, discovery, and delivery application. 
 
The Fedora repository functions as the standard _smart storage_ for Islandora.
 
## Field
Data of a certain type that is attached to a content entity. For instance, on a Resource Node content type, you might have fields for a title, description, display hints, subjects, and other metadata. 
 
## Flysystem
Flysystem is a filesystem abstraction library for PHP. Islandora uses Flysystem to swap about different backend filesystem applications. Islandora provides a custom Flysystem adapter for Fedora.
 
## Ingest
To ingest an object is to add an entry for it in Islandora. This is done primarily through the Drupal interface. 
 
The word 'ingest' is used in repository language because repository software, rather than having an object and associated components forced upon it, takes in and processes the object itself, creating derivatives automatically. Often, when referring to the process of ingesting objects, Islandora and its associated documentation use other terms such as 'import' or 'add resource node'. In such contexts, these terms generally refer to the ingest process.
 
## Media
A drop-in replacement for the Drupal core upload field with a unified User Interface where editors and administrators can upload, manage, and reuse files and multimedia assets. 
 
## Microservice
A software development technique — a variant of the service-oriented architecture (SOA) structural style — that arranges an application as a collection of loosely coupled services. In a microservices architecture, services are fine-grained and the protocols are lightweight.
 
## Module
Software (usually PHP, JavaScript, and/or CSS) that extends site features and adds functionality. Drupal modules conform to a specific structure allowing them to integrate with the Drupal architecture. 
 
## Node
A node is any piece of individual content, such as a page, article, forum topic, or a blog entry. All content on a Drupal website is stored and treated as "nodes". For information about nodes specific to Islandora, see "Resource Node."

## Open Source
Open source describes a method of software development that promotes access to the end product's source code. Islandora is an open source product with an active development community, operating under the GPL license (2.0) for Drupal components and the MIT license for non-Drupal components. 
 
## Resource Node
A resource node is a Drupal node that represents a single conceptual item or object stored in an Islandora repository. It acts as a container for all files and metadata associated with that item, and is the place where the item 'lives' as a visitable url. 
 
For example, A video stored in Islandora will have a resource node, which metadata stored in fields, a preservation master file, and may have a thumbnail, web-friendly derivatives, and technical metadata associated with the resource node. The resource node may also belong to one or more collections. 
 
## Taxonomy term
A term used to classify content, such as a tag or a category. 
 
## Theme
Software and asset files (images, CSS, PHP code, and/or templates) that determine the style and layout of the site. The Drupal project distinguishes between core and contributed themes. 
 
## View 
A database query used to generate lists or tables of content. Drupal provides a powerful administrator interface for creating and editing views without any coding.
 
## Viewer
A "Viewer" is an add-on package that allows a solution pack to embed, display, or playback a particular object in a web accessible format. Viewers are typically a combination of a Drupal Library and a Drupal Module. The Drupal Library is the package of code that represents the player or display mechanism, and the Drupal Module is the code package that allows the Drupal Library to be accessed from within the Drupal environment.
 
## Virtual Machine Image
The Virtual Machine Image allows you to mount a fully working version of Islandora on your local machine as a separate virtual machine.
 
---
 
Some definitions adapted from [Wikipedia](https://en.wikipedia.org/) and [Drupal.org](https://www.drupal.org/docs/user_guide/en/glossary.html)

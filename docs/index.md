# About
![Islandora logo](https://assets.website-files.com/618eb1b213a102afa83e260d/61b376b9f04ddd1f0da90edf_logo-islandora.svg)

## This is Islandora

[Islandora](https://islandora.ca) is an open-source framework that provides the necessary tools to use a [Drupal](https://www.drupal.org) website as a fully-functional Digital Assets Management System. See [Quickstart](installation/quickstart.md) to get started.

Islandora:

[//]: # (We should probably replace as many of the links in this section as possible with links to within this documentation, and make it clear which are internal links and which link out.)
- **Is native Drupal** - With Islandora, you can create preservation-ready digital resources using Drupal nodes, media, files, and taxonomy terms. 
- **Provides a sensible starting place** - No two Islandora sites are the same, but we provide a starting point for Drupal - the [Islandora Starter Site](https://github.com/Islandora/islandora-starter-site/) - as well as several methods of [installation](installation/quickstart.md) that set up a working suite of services external to Drupal and show off Islandora's capabilities. 
- **Integrates with Fedora** - Drupal resources can be stored in Lyrasis's [Fedora Repository](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home) (version 5.0 or greater) as binaries (files) and RDF metadata.
- **Uses microservices** - Islandora provides an architecture for messaging and integration with any number of microservices, that provide services outside of the Drupal framework. Islandora's [Crayfish](https://github.com/Islandora/crayfish) suite of microservices provides functionality for synchronizing resources into Fedora and for automatically generating helper files, called derivatives.
- **Can handle messages at scale** - Islandora created [Alpaca](https://github.com/Islandora/Alpaca), an integration middleware based on Apache Camel, to handle messaging and queueing at an enterprise scale. To the user, this means large batch uploads can be processed gracefully.

[//]: # (Allowing bulk uploads to be processed without affecting the server... would be the ideal right? but right now they're on the same server. Is just saying "scalability" ok?)

- **Offers digital preservation features** - Using a robust storage layer for preservation (Fedora), and generating technical metadata with FITS, are digital preservation tools provided by our automated installation methods. Community members have created additional features for doing [digital preservation using Islandora](https://github.com/mjordan/digital_preservation_using_islandora), which are not yet part of our automated setup. 
- **Exposes data to harvesters** - Metadata about resources is available as linked data through the [JSON-LD serialization module](https://github.com/Islandora/jsonld), and can be made available through Drupal, Fedora, or a triplestore. Islandora Starter Site also offers a full configuration for exposing [OAI-PMH](user-documentation/metadata-harvesting.md#oai-pmh) so that metadata can be harvested by aggregators, and [IIIF support](https://github.com/Islandora/islandora/tree/8.x-1.x/modules/islandora_iiif) means images in Islandora can be viewed in any IIIF-compliant viewer.
- **Offers flexibility** - As Islandora content is Drupal content, migrations and batch editing can be done through Drupal's built-in migrate framework and  vocabularies can be created using Drupal taxonomies. Contributed Drupal modules such as [Solr Search API](https://www.drupal.org/project/search_api_solr) enable in-site search, and [Matomo Analytics](https://www.drupal.org/project/matomo) provides usage metrics for site analytics.
- **Is a community** - A [dedicated, active community of users and developers](https://groups.google.com/forum/#!forum/islandora) is working to push new features, collaborate on improvements, design custom solutions, and create extended functionality. Some of these for Islandora 8 take the form of [Recipes](https://github.com/Islandora-Labs/Islandora-Cookbook).

## Join the Community

The [Islandora community](https://www.islandora.ca/community) is an active group of users, managers, librarians, documenters, and developers from galleries, libraries, archives, museums, and other institutions worldwide. We welcome discussion and contribution through various mailing lists, channels, interest groups, and calls. The Islandora community operates under the [Islandora Code Of Conduct](https://www.islandora.ca/code-of-conduct). See our Contributing Guidelines for more information.


!!! note "Documentation for previous versions"
    Documentation for Islandora Legacy (6 and 7) is on the [Lyrasis documentation wiki](https://wiki.lyrasis.org/display/ISLANDORA/Start).

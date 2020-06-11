[Islandora](https://islandora.ca) is an open-source framework that provides the necessary tools turn a [Drupal 8](https://www.drupal.org) website into a fully-functional preservation repository for digital assets without the need for additional code, while remaining flexible enough to be extended and customized.

Out of the box, this includes:

- A suite of customized Drupal resource types and configurations, packaged as the [Islandora 8 Drupal module](https://github.com/Islandora/islandora), that streamline the process of creating preservable digital resources natively within Drupal as nodes, media, files, and taxonomy terms
- Complete integration between Drupal resources and Duraspace's [Fedora 5.x Repository](https://wiki.duraspace.org/display/FF/Fedora+Repository+Home), including RDF conversion and asset storage
- The [Crayfish](https://github.com/Islandora/crayfish) suite of microservices that Islandora uses to provide automated resource mapping to Fedora and automated generation of derivative files, and which can be extended to provide even more services
- The [Alpaca](https://github.com/Islandora/Alpaca) suite of middleware that asynchronously manages and queues microservices in the background - like the ones bundled with Crayfish - providing scalability
- A [batch import/migration framework](https://github.com/Islandora/migrate_islandora_csv) to serve as a launching point to get your existing data into Islandora 8
- Preservation tools like [controlled access terms](https://github.com/Islandora/controlled_access_terms) for subjects and agents and a [full configuration for exposing OAI-PMH](https://github.com/Islandora/islandora_defaults/tree/8.x-1.x/modules/islandora_oaipmh) to metadata harvesters
- Discoverability tools like [JSON-LD serialization](https://github.com/Islandora/jsonld) for linked data, [IIIF support](https://github.com/Islandora/islandora/tree/8.x-1.x/modules/islandora_iiif), and an [Openseadragon viewer](https://github.com/Islandora/openseadragon) for image display
- A [dedicated, active community](https://groups.google.com/forum/#!forum/islandora) working to push new features, collaborate on improvements, design custom solutions, and [create extended functionality](https://github.com/Islandora-Labs/islandora_awesome#the-islandora-8-list)

As native Drupal content, Islandora resources can also be integrated with existing Drupal tools like the [Solr Search API](https://www.drupal.org/project/search_api_solr), [Matomo Analytics](https://www.drupal.org/project/matomo), [RDF schema building](https://www.drupal.org/project/rdfui), and much more.

Islandora 8 comes with a comprehensive [default site configuration](https://github.com/Islandora/islandora_defaults) for Drupal to get you started, and even an [Ansible playbook](https://github.com/Islandora-Devops/islandora-playbook) that will quickly get you up and running so you can try it out.

!!! note "Islandora 6 and 7" Documenation for earlier versions of Islandora is on our [documentation wiki](https://wiki.lyrasis.org/display/ISLANDORA/Start).

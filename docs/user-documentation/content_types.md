# Create / Update a Content Type

!!! tip The screenshots and pre-existing data in this documentation assume that you are using the [Islandora Demo](https://github.com/Islandora-CLAW/islandora_demo) configuration.

!!! tip This page will address how to create and modify ingest forms (or rather, content types) via the GUI. For help working with forms via the API, please check out the [Further Reading](#further-reading) section for links to more advanced Drupal documentation.

Since objects in Islandora 8 are stored in Drupal as Nodes, we use the standard Drupal Content Types system to create and edit our ‘ingest forms’ [Content Types in Drupal 8](https://www.drupal.org/docs/8/administering-drupal-8-site/managing-content-0/working-with-content-types-and-fields). Islandora 8 forms are Drupal forms, and if you are already familiar with Drupal Field UI, you’re already well equipped to create and modify your own ingest forms in Islandora 8.

This page will address how to create and modify ingest forms (or rather, content types) via the GUI. For help working with forms via the API, please check out the Further Reading section for links to more advanced Drupal documentation.

## Modify a Content Type

If you have deployed your Islandora 8 with the Islandora Demo configuration, you will already have a Repository Item content type available, with pre-configured fields. In the Admin menu, go to Structure >> Content Types and find the Repository Item content type. Select *Manage Fields*.

![a screenshot of the Add Content Type page](/docs/assets/islandora8_managefields.png)

You will see a list of the fields that are already available in the *Repository Item* content type.

### Add a Field

Let’s add a new field where a user can indicate if the repository item needs to be reviewed:

1. Click “Add Field”
1. Since this is a “yes/no” decision, choose "Boolean" from the dropdown menu and give the Label field a name. [List of Drupal 8 FieldTypes, FieldWidgets, and FieldFormatters](https://www.drupal.org/docs/8/api/entity-api/fieldtypes-fieldwidgets-and-fieldformatters)
1. Next, configure how the field is stored in the Drupal database. For this field type you can select how many values will be allowed. Set this value to "Limited""1"
1. Then configure how the field is described (including its display label and the help text for when it appears on a form) and constraints on its use. In this case, the field will be required for this Content Type, and will be set to “on” by default. Choose "Required field" and set to Default Value. *Save settings.*



![a screenshot of the field settings page](/docs/assets/islandora8_fieldsettings.png)

The new field has been added:

![a screenshot of a "Needs Review?" field in the Drupal field UI](/docs/assets/islandora8_newfield.png)

And it appears in the ingest form when we try to create a new repository object. To test this, go to Content >> Add content >> Repository item:

![a screenshot of a "Needs Review?" field appearing at the bottom of a new node form](/docs/assets/islandora8_newfieldinform.png)

!!! tip New fields, with the exception of Typed Relation fields, are not automatically indexed in Fedora and the triple-store. Update the Content Type's RDF Mapping to enable indexing the field (see below).

!!! tip New fields will not automatically be searchable. They need to be added to the SOLR index configuration. See the ['Setup and Configure Search'](searching.md) page for more information.

!!! tip To add new behavior based on the results of this new field, check out [link to Context docs](docs/user-documentation/searching.md)

### Change the Form Display

Now let’s move our new field to a different part of the form. In the Admin menu, return to Structure > Content Types and find the Repository Item content type again. Select Manage form display.

1. All of the fields in this content type are available, in a list, with a simple drag-and-place UI. Drag the new field to the top of the form. We can also change the way the Boolean options are displayed, with radio buttons as opposed to a single checkbox. Different display options will be available from the dropdown menu depending on field type. For more information, please check out [List of Drupal 8 FieldTypes, FieldWidgets, and FieldFormatters](https://www.drupal.org/docs/8/api/entity-api/fieldtypes-fieldwidgets-and-fieldformatters)
1. Save.

When creating a new Repository Item, the new field appears at the top, as a set of radio buttons.

### Change the Content Display

Finally, let’s change how the results of this field are displayed. Initially the new field shows up at the bottom of repository object pages:

![a screenshot of a "Needs Review?" field in the node display](/docs/assets/islandora8_fieldindisplay.png)

In the Admin menu, return to Structure > Content Types and find the Repository Item content type again. Select Manage display from the dropdown menu.

1. Find the new field. You can change how the field title or Label is displayed. Click the dropdown menu to choose from inline/above/hidden/visually hidden. You can also replace the options displayed with variations on a binary choice. Click the gear to choose from the following: On/Off, Yes/No, Enabled/Disabled, 1/0. checkmark/X, or hide the field completely.
1. You can also drag the field into the Disabled section so that neither its label or its contents appear in the display, although the field is saved on the node.
1. Drag the field to "Disabled" and save.
1. You no longer see the field on the display, but it is available when editing the node.

## Create a Content Type

To create your own custom content type from scratch, please refer to [this guide](https://www.drupal.org/docs/8/administering-drupal-8-site/managing-content-0/create-a-custom-content-type) on Drupal.org.

Custom content types are not synced to Fedora or indexed by the triple-store by default. Repository managers must add them to the "Content" ('repository_content') context before their nodes are synced to Fedora and indexed by the triple-store. Any of the custom content type's nodes that were created before updating the context will need to have the indexing action manually triggered.

## Update / Create an RDF Mapping

RDF mapping aligns Drupal fields with RDF ontology properties. For example, the title field of a content model can be mapped to dcterms:title and/or schema:title. In Islandora 8, triples expressed by these mappings get synced to Fedora and indexed in the Blazegraph triplestore. RDF mappings are defined/stored in Drupal as a [YAML](https://yaml.org/) file (to learn more about YAML, there are [several tutorials on the web](https://duckduckgo.com/?q=yaml+tutorial). Currently, Drupal 8 does not have a UI to create/update RDF mappings to ontologies other than Schema.org. This requires repository managers to update the configuration files themselves. Consider using the RDF mappings included in islandora_demo as templates by copying and modifying one to meet your needs.

The Drupal 8 Configuration Synchronization export (e.g. `http://localhost:8000/admin/config/development/configuration/single/export`) and import (e.g. `http://localhost:8000/admin/config/development/configuration/single/import`) can be used to get a copy of the mappings for editing in a text editor before being uploaded again. Alternatively, a repository manager can update the configuration on the server and use Features to import the edits.

An RDF mapping configuration file has two main areas, the mapping's metadata and the mapping itself. Most of the mapping's metadata should be left alone unless you are creating a brand new mapping for a new Content Type or Taxonomy Vocabulary. A _partial_ example from [islandora_demo's islandora_object (Repository Item)](https://github.com/Islandora-CLAW/islandora_demo/blob/8.x-1.x/config/install/rdf.mapping.node.islandora_object.yml) is included below:

```
langcode: en
status: true
dependencies:
  config:
    - node.type.islandora_object
  enforced:
    module:
      - islandora_demo
  module:
    - node
id: node.islandora_object
targetEntityType: node
bundle: islandora_object
types:
  - 'pcdm:Object'
fieldMappings:
  title:
    properties:
      - 'dc:title'
  field_alternative_title:
    properties:
      - 'dc:alternative'
  field_edtf_date:
    properties:
      - 'dc:date'
    datatype_callback:
      callable: 'Drupal\controlled_access_terms\EDTFConverter::dateIso8601Value'
  field_description:
    properties:
      - 'dc:description'
```

The required mapping metadata fields when creating a brand-new mapping include the `id`, `status`, `targetEntityType`, and `bundle`. (`uuid` and `_core`, not seen in the example above but may be present in exported copies,  will be added by Drupal automatically.) `bundle` is the machine name for the Content Type or Taxonomy Vocabulary you are creating the mapping for. `targetEntityType` is `node` for Content Types or `taxonomy_term` for Taxonomy Vocabularies. The `id` configuration is a concatenation of target entity type and bundle ('node' and 'islandora_object' in the example above). The `id` is also used to name the configuration file: e.g. `rdf.mapping.node.islandora_object.yml` is `rdf.mapping.` plus the id (`node.islandora_object`) and then `.yml`.

The mapping itself consists of the `types`' and `fieldMappings` configurations.

All the mappings use RDF namespaces instead of fully-qualified URIs. For example, the type for islandora_object is `pcdm:Object` instead of `http://pcdm.org/models#Object`. Unfortunately, the available namespaces are defined in module hooks (hook_rdf_namespaces) rather than in a configuration file. Repository managers wanting to add additional namespaces need to create their own module and implement hook_rdf_namespaces. See the [islandora_demo](https://github.com/Islandora-CLAW/islandora_demo/blob/8.x-1.x/islandora_demo.module) hook implementation for an example.

Namespaces currently supported (ordered by the module that supplies them) include:
 * rdf
      * content: http://purl.org/rss/1.0/modules/content/
      * dc: http://purl.org/dc/terms/
      * foaf: http://xmlns.com/foaf/0.1/
      * og: http://ogp.me/ns#
      * rdfs: http://www.w3.org/2000/01/rdf-schema#
      * schema: http://schema.org/
      * sioc: http://rdfs.org/sioc/ns#
      * sioct: http://rdfs.org/sioc/types#
      * skos: http://www.w3.org/2004/02/skos/core#
      * xsd: http://www.w3.org/2001/XMLSchema#
 * islandora
      * ldp: http://www.w3.org/ns/ldp#
      * dc11: http://purl.org/dc/elements/1.1/
      * nfo: http://www.semanticdesktop.org/ontologies/2007/03/22/nfo/v1.1/
      * ebucore: http://www.ebu.ch/metadata/ontologies/ebucore/ebucore#
      * fedora: http://fedora.info/definitions/v4/repository#
      * owl: http://www.w3.org/2002/07/owl#
      * ore: http://www.openarchives.org/ore/terms/
      * rdf: http://www.w3.org/1999/02/22-rdf-syntax-ns#
      * islandora: http://islandora.ca/CLAW/
      * pcdm: http://pcdm.org/models#
      * use: http://pcdm.org/use#
      * iana: http://www.iana.org/assignments/relation/
 * islandora_demo
      * relators: http://id.loc.gov/vocabulary/relators/
 * controlled_access_terms
      * wgs84_pos: http://www.w3.org/2003/01/geo/wgs84_pos#
      * org: https://www.w3.org/TR/vocab-org/#org:
      * xs: http://www.w3.org/2001/XMLSchema#

The `types` corresponds to the `rdf:type` predicate (which corresponds to JSON-LD's `@type`) and can have multiple values. This type value will be applied to every node or taxonomy term using the mapped content type or vocabulary.

In some cases a repository may want a node or taxonomy term's `rdf:type` to be configurable. For example, the Corporate Body Vocabulary (provided by the Controlled Access Terms Default Configuration module) has `schema:Organization` set as the default type in the RDF mapping. However, more granular types may apply to one organization and not another, such as `schema:GovernmentOrganization` or `schema:Corporation`. The `alter_jsonld_type` Context reaction allows Content Types and Taxonomy Vocabularies to add a field's values as `rdf:types` to its JSON-LD serialization (the format used to index a node or taxonomy term in Fedora and the triple-store).

`fieldMappings` specifies the fields to be included, their RDF property mappings, and any necessary data converters (the `datatype_callback`). One field can be mapped to more than one RDF property by adding them to the field's properties list. The `datatype_callback` is defined by the 'callable' key and the fully qualified static method used to convert it to the desired data format. For example, fields of the Drupal datetime type need to be converted to ISO 8601 values, so we use the `Drupal\rdf\CommonDataConverter::dateIso8601Value` function to perform the conversion.

## Further Reading:

* [Drupal.org Introduction to Form API](https://www.drupal.org/docs/8/api/form-api/introduction-to-form-api)
* [Step by step method to create a custom form in Drupal 8](https://www.valuebound.com/resources/blog/step-by-step-method-to-create-a-custom-form-in-drupal-8)

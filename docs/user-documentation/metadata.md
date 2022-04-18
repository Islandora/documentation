# Metadata in Islandora

> TL;DR: In Islandora, metadata is stored in Drupal, in _fields_ attached to _entities_ (nodes or media). This allows us to interact with metadata (add, edit, remove, display, index in a search engine...) almost entirely using standard Drupal processes. If exporting this metadata to Fedora and/or a triplestore, the values are serialized to RDF using mappings that can be set for each bundle.

!!! note "Drupal 8 Terminology"
    In Drupal 8, Fields can be attached to _bundles_ (sometimes called _entity sub-types_ -- e.g. Content types, Media types, Vocabularies) or _entities_ (e.g. Users). For more on Fields, see ["2.3 Content Entities and Fields"](https://www.drupal.org/docs/user_guide/en/planning-data-types.html) and ["6.3 Adding Basic Fields to a Content Type"](https://www.drupal.org/docs/user_guide/en/structure-fields.html) in the Official Drupal Guide.

<!-- Next revision: check status of changing 'bundles' to 'entity sub-types' (https://www.drupal.org/project/drupal/issues/1380720). -->

As described in the [resource nodes section](resource-nodes.md), Islandora digital objects are comprised of _Drupal nodes_ for descriptive metadata, _Drupal media_ for technical metadata, and _Drupal files_ for the binary objects. This section describes how Islandora uses and extends Drupal fields to manage descriptive metadata.

## Content Types

In Drupal, *Nodes* come in different sub-types called *Content Types*. These let you define a type of content ("Article" and "Basic Page" are Drupal defaults and "Repository Item" is an Islandora specific example), the set of metadata fields that are attached to that content, and how those fields can be edited and displayed. Each content type is essentially a *metadata profile* that can be used for a piece of web content, or to describe a digital resource. You can create your own Content Types for your Islandora project or use a pre-defined one like *Repository Item* from the Islandora_defaults module. We will go over the metadata specific aspects of Content Types below, but for a fuller walkthrough of creating a Content Type [see here](content_types.md#create-a-content-type).

Not all Content Types in your Drupal site need be [*Islandora Resource Nodes*](resource-nodes.md). Making a Content Type a Resource Node will associate Islandora specific behaviours (such as syncing to Fedora or causing derivatives to be generated) with it. The decision to make a content an Islandora resource node is left to the discretion of the site manager. In Islandora, a "resource node" is usually considered a descriptive record for "a thing", and is conceptually similar to an "Islandora Object" in 7.x, i.e. a "Fedora Object" in Fedora 3.x and below. [Read more on configuring a content type to be treated as a Resource Node](content_types.md#create-a-content-type).

### Fields

The administrator will define the fields that are associated with a specific Content Type. The same fields can be applied to different Content Types, but the field display and editing configurations are unique to each Content Type. The names and definitions of these fields are specific to Drupal and do not have to correspond to an outside metadata schema. You will give each field a Label, Machine Name, and a specific [Field Type](#field-types), like Text, Integer, EDTF, or Entity Reference (see below). Specific to the Field Type you will then define the maximum length of the field, the number of values it can contain, and what taxonomies it might link to.

Fields can be added under **Administration** >> **Structure** >> **Content types** >> _Your Content Type's Name_ >> **Manage fields** (/admin/structure/types/your_type/fields). This tab will list all Fields, their Label, Machine Name, Field Type, and give you the option to make what edits to the definition of that field that you can.

Certain decisions must be made when fields are created, and before any content is added, because they can not be changed later. Field Type can not be changed, so you wouldn't be able to change a text field to a taxonomy field after creation. The field's machine name also can't be changed. The number of values allowed in a field or its maximum length or type of item to reference (in the case of Entity reference fields) can not be changed after content has been added. You can, however, always add new fields to a Content Type, even after content has been added.

!!! islandora "7.x Migration Note: What About My MODS XML?"
    Even when using *islandora_defaults* there is no "official" metadata schema in Islandora. Where Islandora 7.x used MODS, and took advantage of its hierarchical/extensible structure, Drupal Fields are a flat structure working with distinct, individual elements. You can base your fields on those in MODS, or any other schema, but that structure is up to you. The Metadata Interest Group has developed a sample [MODS-Drupal-RDF mapping](https://docs.google.com/spreadsheets/d/18u2qFJ014IIxlVpM3JXfDEFccwBZcoFsjbBGpvL0jJI/edit?pli=1#gid=0), which provides a structure upon which you can build your Drupal fields. It is used by the Repository Item content type in *islandora_defaults*.


!!! tip "You Cannot Change The Content Type Of A Node"
    Once a node is created, its content type cannot be changed. Just as you are unable to change many aspects of a Field once it has been created, once a node has been created it is now permanently of that content type and the fields associated with it. At that point your only option would be to create a new node of the intended content type, map the field values (programmatically or by copy-paste), and update any media or children that refer to the old node to refer to the new one.


The *islandora_defaults* module provides a **Repository Item** content type that can be used as a structure to build your collection around, or it can be used as a sample to see how fields in Content Types work. It pre-defines fields, including **Alternative Title** and **Date Issued** that could be of use in many digital repositories. The full list of fields and their field types can be seen in the screenshot below.

![Screenshot of the "Manage fields" page for the "Repository Item" content type from islandora_defaults.](../assets/metadata_content_type_screenshot.png)

!!! tip "Titles Aren't Conventionally-Configurable Fields"
    The field *title* is built-in to each Content Type by default, and can be referenced in views, templates, and indexing like other fields, but it cannot be configured like other fields. The only aspect you can change about *title* is its label. It has a built-in maximum length of 255 characters which cannot be changed. If your content requires longer titles we recommend you create a separate "long_title" field to store the full title and reserve the default title field for a display title.
    There is a contributed module called [Node Title Length](https://www.drupal.org/project/title_length), which allows an administrator to configure the length of the title field in the core node table. However, this only works on nodes (not media or other entities) and involves meddling in a core Drupal database schema, which makes some people uneasy.


### Content Entry Form/Manage Form Display

After creating the Fields for a Content Type you'll be able to manage the form used by content creators to create Nodes of that Content Type. On The **Manage form display** tab you'll be able to edit this form by arranging the order of the fields, choose what Widget will define the entry options for a field, and then set certain settings for that Widget. Fields are arranged by dragging the cross to the left of the **Label**. They can also be removed from the form, but not the Content Type, by dragging them to the bottom of the list under the **Disabled** heading. Widgets are defined by Field Type, so an Entity reference field could use auto complete, a select list, or even checkboxes, and are chosen from a drop-down list. The widget settings are accessed through the gear on the far right of a row and may allow you to set the size of an entry field, whether the field *Label* is displayed, or if you use placeholder text.


### Content Display/Manage Display

The **Manage display** tab is where you will make decisions about how to
display the metadata. Order is arranged as above, and can again be
dragged to the **Disabled** section to hide the field from display. You can
choose whether a field's label is displayed above the value, in-line, or
hidden.

## Vocabularies

See also: [MIG Presentation on Taxonomies](https://docs.google.com/presentation/d/1LfpU6H4qxXtnYQPFntwMNtsgtU30yzp2MxwKKAllUOc/edit?usp=sharing) by Kristina Spurgin, 2021-07-19

In Drupal, _Taxonomy Vocabularies_ (or simply _Vocabularies_) are also entity sub-types that define a set of fields and their configurations. Whereas instances of content types are called _nodes_, items in a vocabulary are called _taxonomy terms_ (or simply _terms_). Traditionally, taxonomy terms are used to classify content in Drupal. For instance, the Article content type includes a field `field_tags` that can refer to terms in the Tags vocabulary.

There are two ways that users can interact with taxonomies: they can be "closed," e.g. a fixed list to pick from in a dropdown, or "open," e.g. `field_tags` where users can enter new terms, which are created on the fly. This is not set on the _vocabulary_ itself, but in the configuration of the field (typically on a node). Terms within vocabularies have an ordering, and can have hierarchical structure, but do not need to.

Islandora (through the Islandora Core Feature) creates the 'Islandora Models' vocabulary which includes the terms 'Audio', 'Binary', 'Collection', 'Compound Object', 'Digital Document', 'Image', 'Newspaper', 'Page', 'Paged Content', 'Publication Issue', and 'Video'. Islandora Defaults provides contexts that cause certain actions (e.g. derivatives to happen, or blocks to appear) based on which term is used.

<!-- Is it possible to add your own terms to this vocabulary? Is it recommended? -->

<!-- field_model is a "special field" in terms of the RDF mapping, because the drupal URI gets replaced by the 'external URI' but I'm not sure if this is where to mention this.-->

The Controlled Access Terms module provides additional vocabularies:
- Corporate Body
- Country
- Family
- Form
- Genre
- Geographic Location
- Language
- Person
- Resource Types
- Subject

Each of these vocabularies has its own set of fields allowing repositories to further describe them. The Repository Item content type has fields that can reference terms in these vocabularies. See 'Entity Reference fields' in the 'Field Types' section below.

The vocabularies provided by default are a starting point, and a repository administrator can create whatever vocabularies are desired.

## Field Types

Fields are where descriptive and administrative metadata about Drupal entities is stored. There are different *types* of fields including boolean, datetime, entity reference, integer, string, text, and text_with_summary. These field types also have *widgets* (controlling how data is entered) and *formatters* (controlling how data is displayed). The [Drupal 8 documentation on FieldTypes, FieldWidgets, and FieldFormatters](https://www.drupal.org/docs/8/api/entity-api/fieldtypes-fieldwidgets-and-fieldformatters) includes a list of the core field types with brief definitions, along with a list of core widgets and formatters. [Custom field types can be created](https://www.drupal.org/docs/creating-custom-modules/creating-custom-field-types-widgets-and-formatters) to represent data in ways not provided by these core options.

More field types, formatters, and widgets are available in various modules.For example, the controlled_access_terms module provides two additional field types designed specifically for use with Islandora: ETDF, and Typed Relation. These and the Entity Reference field type are described in more detail below, since they are of particular interest for Islandora users.

### Entity Reference

Entity Reference fields are a special type of field built into Drupal core that creates relationships between entities. The field's configuration options include (but are not limited to):

- Which kind of entity can be referenced (*only one type of item to reference can be defined per field*)
- The allowed number of values (limited or unlimited)
- Whether to use Views for filtering
- Whether to allow users to create new referenced entities while inputting data, if they don't already exist

The *Repository Item* content type, provided by the *islandora_defaults* module, includes several entity reference fields that reference vocabularies defined by the *islandora* and *controlled_access_terms* modules.

#### Configurations for Entity Reference field

The screenshots below show how you can configure an entity reference field (in this case the Subject field on the Repository Item content type).

!!! tip
    Note that once the type of entity to reference has been defined, and data has been created, it cannot be changed.

Storage settings for entity reference field where you set whether the field will reference content nodes or taxonomy terms:

![Screenshot of the storage settings for an entity reference field](../assets/metadata_entity_reference_config.png)

Reference type settings for entity reference field where you select which vocabularies the autocomplete utility should query when editors are entering data:

![Screenshot of the reference type settings for an entity reference field, showing which vocabularies the autocomplete utility should query when editors are entering data.](../assets/metadata_entity_reference_config_vocabs.png)

!!! tip "Data Consistency"
    Selecting which vocabularies can be referenced from an entity reference field only affects which vocabularies will be searched when a user types into the autocomplete field in the Drupal form for adding a new item. These settings do not impose constraints on the underlying database, so it is still possible to load references to other vocabularies without being stopped or warned when ingesting data through [various migration methods](../technical-documentation/migration-overview.md).

### EDTF

The EDTF field type is defined in the *controlled_access_terms* module, and designed for recording dates in [Extended Date Time Format](https://www.loc.gov/standards/datetime/edtf.html), which is a format based off of the hyphenated form of ISO 8601 (e.g. 1991-02-03 or 1991-02-03T10:00:00), but also allows expressions of different granularity and uncertainty. The Default EDTF widget has a validator that only allows strings that conform to the EDTF standard. The Default EDTF formatter allows these date strings to be displayed in a variety of human readable ways, including big- or little-endian, and presenting months as numbers, abbreviations, or spelling month names out in full. Close review of the [EDTF Specifications](https://www.loc.gov/standards/datetime/edtf.html) is recommended when configuring this field type.

!!! tip "Endianness"
    Big-endian = year, month, day. Little-endian = day, month, year. Middle-endian = month, day, year.

!!! bug "Known EDTF Bug"
    When configuring the EDTF widget for a field in a content type, you can choose to allow date intervals (aka date ranges), but doing this prevents the widget from accepting values that include times. (The EDTF standard states that date intervals cannot contain times, but the field should be able to accept either a valid EDTF range *or* a valid EDTF datetime, so this is a bug.)

Example of valid inputs in a multi-valued EDTF Date field (including the
seasonal value 2019-22 as defined in the EDTF specification):
![Screenshot of valid dates ('2019', '2019-11', '2019-22', and '2019-02-02T02:22:22Z') in an EDTF form widget.](../assets/metadata_edtf_valid.png)

Example of the same EDTF dates displayed using little-endian format:
![Screenshot of dates displayed as '2019', 'November 2019', 'Summer 2019', and '2 February 2019 02:22:22Z'.](../assets/metadata_edtf_display.png)

EDTF field values cannot include textual representations of dates, as shown below in this example of a valid EDTF value ('1943-05') and an invalid value ('May 1943') with the corresponding error message. Use the formatter configurations detailed further below to achieve textual display of dates.
![Screenshot of both a valid ("1943-05") and an invalid ("May 1943") EDTF entry. Displays the error message "Could not parse the date 'May 1943' Years must be at least 4 characters long."](../assets/metadata_edtf_invalid.png)


#### Configuration for the Default EDTF Widget

This configuration can be set per field by clicking the **gear** icon next to any field defined with EDTF field type at **Administration** \>\> **Structure** \>\> **Content types** \>\> **Repository Item** \>\> **Manage form display** (admin/structure/types/manage/islandora_object/form-display)

![Screenshot of the gear icon on the EDTF Widget display settings](../assets/metadata_edtf_field_settings_gear.png)

Configuration options include strictness level of date validation, allowing date intervals and allowing date sets.
![Screenshot of the gear icon on the EDTF Widget display settings](../assets/metadata_edtf_widget_settings.png)

#### Configuration for the Default EDTF Formatter

This configuration can be set per field by clicking the gear icon next to any field defined with EDTF field type at **Administration** \>\> **Structure** \>\> **Content types** \>\> **Repository Item** \>\> **Manage display** (admin/structure/types/manage/islandora_object/display)

![Screenshot of the gear icon on the EDTF formatter settings](../assets/metadata_edtf_field_formatter_gear.png)

Example of how the EDTF formatter settings can change the display of an EDTF value:
![Combined screenshots displaying the EDTF default formatter settings, default on top and modified settings below, with an example formatted EDTF value displayed for each.](../assets/metadata_edtf_formatters.png)

#### Configuration for indexing and sorting EDTF fields in search results

By default, EDTF date values are indexed in Solr as string values. The entered value (not the displayed value) is indexed.

!!! Solr EDTF limitations
    The Solr string data type requires the full field value to match the query in order to count as a match. This means that searching for 2014 will not retrieve a record where the recorded date value is 2014-11-02.

EDTF date fields may be configured as sort fields in your search results Views. Currently by default, this results in a simple ordering by the literal EDTF date string.

An EDTF date field with multiple or unlimited number of allowed values may be set as a sort field. In this case, the first occurrence of the field value is used as the sorting value.

### Typed Relation

The Typed Relation field is defined in the *controlled_access_terms* module, is an extension of Drupal's Entity Reference field type, and allows the user to qualify the type of relation between the resource node and other entities, such as taxonomy terms. For example, it enables the inclusion of a resource's contributor's (assuming contributor names are modelled as taxonomy terms or some other Drupal entities) as well as their roles (such as "author", "illustrator", or "architect") in the resource node itself. Using only Drupal's Entity Reference fields, we would need individual fields for "author", "illustrator", "architect", and any other roles that may need to be made available. Using a Typed Relation field, we can have one Entity Reference field for "Contributors" and let the user pick the affiliated role from a predefined dropdown list.

!!! tip "Typed relation name"
    The parts of a field are called properties, so 'entity reference' and 'relation type' are properties of the Typed Relation field type.

#### Configurations for the Typed Relation field

The *islandora_defaults* module demonstrates a Typed Relation field labelled 'Linked Agent' as part of the Repository Item content type, and populates the available relations from the MARC relators list. ![Screenshot of adding a value into a typed relation field](../assets/metadata_typed_relation_field.png)

The list of available relations for this Linked Agent field is configurable at '/admin/structure/types/manage/islandora_object/fields/node.islandora_object.field_linked_agent'.


!!! islandora "Typed relation tradeoffs"
    - If you apply this field to another content type, you can define unique relations available for that instance of the field.
    - However, multiple instances of this field means administrative overhead to maintain the separate lists of relations defined for each instance.

Relations are defined in the format *key\|value*, and the key is used in the RDF mapping (see below).

![Screenshot of the 'Available Relations' configuration text box for the 'Linked Agent' field.](../assets/metadata_available_relations_config.png)

By default, facets can be created for typed relation fields that will facet based on the linked entity alone, not separating references based on the relationship type.

## Getting Metadata into Fedora and a Triple-store

Depending on the needs at your institution, you may or may not be using Fedora with your Islandora installation. You also may or may not be hoping to publish your metadata as RDF triples that can be queried in a triplestore. Both of these functionalities are driven by the JSON-LD module (written for Islandora), which provides a JSON-LD serialization of your content nodes, media nodes, as well as your taxonomy terms. This JSON-LD is what gets ingested by Fedora, and is also what is used to add RDF triples to the [blazegraph triplestore](https://islandora.github.io/documentation/installation/manual/installing_fedora_syn_and_blazegraph/) if you choose to use that service.

The JSON-LD serialization for an entity is available by appending `_format=jsonld` to the entity's URL. Below is an example JSON-LD document representing the RDF serialization of a Repository item node created in a standard islandora-playbook based vagrant VM:

```
{
  "@graph":[
    {
      "@id":"http://localhost:8000/node/1",
      "@type":[
        "http://pcdm.org/models#Object"
      ],
      "http://purl.org/dc/terms/title":[
        {
          "@value":"New York, New York. A large lobster brought in by the New England fishing boat [Fulton Fish Market]",
          "@language":"en"
        }
      ],
      "http://schema.org/author":[
        {
          "@id":"http://localhost:8000/user/1"
        }
      ],
      "http://schema.org/dateCreated":[
        {
          "@value":"2019-03-14T19:05:24+00:00",
          "@type":"http://www.w3.org/2001/XMLSchema#dateTime"
        }
      ],
      "http://schema.org/dateModified":[
        {
          "@value":"2019-03-14T19:20:51+00:00",
          "@type":"http://www.w3.org/2001/XMLSchema#dateTime"
        }
      ],
      "http://purl.org/dc/terms/date":[
        {
          "@value":"1943-05",
          "@type":"http://www.w3.org/2001/XMLSchema#string"
        },
        {
          "@value":"1943-05",
          "@type":"http://www.w3.org/2001/XMLSchema#gYearMonth"
        }
      ],
      "http://purl.org/dc/terms/extent":[
        {
          "@value":"1 negative",
          "@type":"http://www.w3.org/2001/XMLSchema#string"
        }
      ],
      "http://purl.org/dc/terms/identifier":[
        {
          "@value":"D 630714",
          "@type":"http://www.w3.org/2001/XMLSchema#string"
        }
      ],
      "http://purl.org/dc/terms/type":[
        {
          "@id":"http://localhost:8000/taxonomy/term/11"
        }
      ],
      "http://purl.org/dc/terms/rights":[
        {
          "@value":"No known restrictions. For information, see U.S. Farm Security Administration/Office of War Information Black & White Photographs(http://www.loc.gov/rr/print/res/071_fsab.html)",
          "@type":"http://www.w3.org/2001/XMLSchema#string"
        }
      ],
      "http://purl.org/dc/terms/subject":[
        {
          "@id":"http://localhost:8000/taxonomy/term/26"
        }
      ],
      "http://schema.org/sameAs":[
        {
          "@value":"http://localhost:8000/node/1"
        }
      ]
    },
    {
      "@id":"http://localhost:8000/user/1",
      "@type":[
        "http://schema.org/Person"
      ]
    },
    {
      "@id":"http://localhost:8000/taxonomy/term/11",
      "@type":[
        "http://schema.org/Thing"
      ]
    },
    {
      "@id":"http://localhost:8000/taxonomy/term/26",
      "@type":[
        "http://schema.org/Thing"
      ]
    }
  ]
}
```

The RDF mapping for a content type, media type, or vocabulary defines how fields in Drupal are mapped to properties in the JSON-LD serialization. The mapping defines the RDF predicates that should be used for each field. You reference Drupal fields via their Machine Name, and the RDF predicate by using the conventional syntax `namespace:predicate`. In this example, the `dc` prefix stands for `http://purl.org/dc/terms/`, so when concatenated the final RDF predicate is `http://purl.org/dc/terms/title`.

To show a small example, the RDF mapping:

```
title:
  properties:
    - dc:title
```


will map the Repository item's *title* field to `http://purl.org/dc/terms/title`. As a result, the node's title value appears like this in the JSON-LD output:

```
"http://purl.org/dc/terms/title":[
  {
    "@value":"New York, New York. A large lobster brought in by the New England fishing boat [Fulton Fish Market]",
    "@language":"en"
  }
],
```

!!! tip Adding RDF prefixes and namespaces
    To set up prefixes for namespaces and see a list of available predefined namespaces, see the ["RDF Generation" page](https://islandora.github.io/documentation/islandora/rdf-mapping/).

### Typed Relation fields in RDF

Unlike other fields, which can be assigned RDF predicates in RDF Mapping YAML files, a typed relation field uses a different predicate depending on the chosen type. These predicates are assigned using the 'keys' in the key\|value configuration. The key must be formatted `namespace:predicate`, e.g. `relators:act`.

!!! bug Current RDF limitations
    The Drupal RDF module is currently limited in the complexity of graph you can generate. All fields must be mapped directly to either a literal value, or a reference to another content type instance, media type instance, or taxonomy term instance. It is not currently possible to create [blank nodes](https://en.wikipedia.org/wiki/Blank_node) or [skolemized nodes](https://www.w3.org/2011/rdf-wg/wiki/Skolemisation) for nesting fields under more complex structures.

## Batch editing metadata in fields

If you are editing multiple resources in order for them to have the same metadata value, the Views Bulk Edit module can help. Here is a video of [creating a view using Views Bulk Operations](https://www.youtube.com/watch?v=ZMp0lPelOZw) to apply a subject term to multiple resources simultaneously.

For more complex changes, or when the values need to differ for each value, an export-modify-reimport method may be needed. Use a view to export CSV or other structured data (including an identifier such as a node id), modify the values as necessary, then use [migrate csv](../technical-documentation/migrate-csv.md) or [Workbench](../technical-documentation/migration-overview.md) to re-import and update the values.

### Exporting Data

One common approach for exporting your content and/or taxonomy data out of Islandora is to use Drupal's [Views Data Export](https://www.drupal.org/project/views_data_export) module. The module has extensions that can allow you to configure exports as CSV, XML, text files, and other formats based on your local needs.

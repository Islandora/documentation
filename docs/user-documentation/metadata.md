# Metadata in Islandora

!!! note "See also: Fields in the Starter Site"
    This page describes technical details about how metadata is handled as 
fields in Drupal and provides a deeper understanding of, and tools for 
modifying, your metadata configuration. If you want to learn about what 
metadata fields come out-of-the-box with Islandora, see [Starter Site Metadata 
Configuration](starter-site-metadata-configuration.md). 

> 1-minute synopsis: In Islandora, metadata is stored natively in Drupal, in 
> _fields_.
> Drupal fields are configured per _content type_ (and per _media type_ for 
> media and per 
> _vocabulary_ for taxonomy terms ). Fields have different _field types_ which 
> determine how you can interact with them (e.g. what content is allowed, how the values 
> can be displayed). Almost all of our interactions with metadata use standard 
> Drupalisms, though Islandora does add a few specialized field types and 
> methods of serialization. 

!!! note "Drupal Terminology"
    In Drupal, a generic term for things that can have fields is 
_bundle_ (synonymously, _entity sub-type_). For example, "Repository 
Item" (a content type), "Image" (a Media type), and "Genre" (a Vocabulary) 
are bundles. Rarely, fields are attached directly to _entity types_ when the 
entity type does not have bundles (e.g. Users). For more on Fields, see 
["2.3 Content Entities and Fields"](https://www.drupal.org/docs/user_guide/en/planning-data-types.html), ["6.3 Adding Basic Fields 
to a Content Type"](https://www.drupal.org/docs/user_guide/en/structure-fields.html), and ["Introduction to Entity API in Drupal 8"](https://www.drupal.org/docs/drupal-apis/entity-api/introduction-to-entity-api-in-drupal-8) in the Official Drupal 
Guide.

<!-- Next revision: check status of changing 'bundles' to 'entity sub-types' (https://www.drupal.org/project/drupal/issues/1380720). -->

!!! note Metadata - Structural, Descriptive and Technical
    As described in the [resource nodes section](content-models.
md#resource-nodes), Islandora digital objects are comprised of _Drupal 
nodes_ for structural and descriptive metadata, _Drupal media_ for technical 
metadata, 
and _Drupal files_ for the binary objects. This section focuses on 
structural and descriptive metadata on nodes, but the same concepts apply to 
technical metadata fields on Media types.

## Metadata profiles in Drupal: Content Types, Media Types, and Vocabularies
### Content Types

When we create a piece of content in Drupal (such as via the "Add Content" 
links), the thing we create is a _node_. Even before creating a node, we must 
select a _content 
type_.  A content type, generically 
known as a _bundle_ or _node sub-type_, includes: 
* basic properties like name (display name and machine name)
* field definitions
* form configurations
* display configurations. 
Each content type determines what fields are available, how (meta)data can be 
  entered and validated, and how (meta)data is displayed, so it is essentially a metadata profile. 

!!! tip "You cannot change an entity's bundle."
Once a node is created, its content type cannot be changed. The same
holds for media and taxonomy terms and their respective bundles.
However, you can create a new node of the
intended content type, map the field values (programmatically or by
copy-and-paste), and update any media or children that refer to the old
node to refer to the new one.

The content types 
available out-of-the-box in Islandora are "Article", "Page", and "Repository 
Item" - the first two are part of Drupal's Standard Install and contain 
fields for generic web content, while "Repository 
Item" was created by the Islandora community as a starting point for you to 
customize your repository content.

!!! note "Do I need to make a custom content type?"
    The Repository Item content type was designed to be flexible and 
    extensible. It is possible to have an entire repository (with 
    heterogenous content) using only Repository Item - and may sites do. To 
    assist with this, the Repository Item content type has a field, "Model", 
    that 
    can be used to configure custom behaviours (i.e. view modes, viewers, etc) 
    based on the type of item: for instance, whether it is a collection, a 
    digital document, or an audio file. You can also extend Repository Item 
    with whatever fields you like, or remove the fields that are present 
    (except the fields that are structural). 
    You can also create a custom content type (or multiple). As long as they
    have the 
    field with machine name `field_member_of` then they can be used 
    to hold Islandora content. If you're using multiple content types with 
    Islandora, consider that sharing fields across content types makes it 
    easier to use field-based features in search and Views. See [our tutorial 
for a fuller walk-through of creating a content type](content-types.md#create-a-content-type).

### Media Types

Media types are bundles (like content types, but) for Media. A media type is a 
collection of fields (along with one special field, usually a file field, 
that is known as the "storage"). We use fields on media types to store 
technical 
metadata such as the height and width of images, mimetype, and file size. If 
you like, you can add additional metadata fields to media. 

### Vocabularies

Vocabularies are bundles for taxonomy terms (sometimes just called "terms"). 
Unlike nodes or media, terms within vocabularies have an ordering, and can have hierarchical structure, but do not need to. 

Vocabularies allow you to
add additional descriptive metadata fields to the controlled terms in your 
repository (e.g. People, Genres, Subjects, ...). One example is the Person 
vocabulary, which out-of-the-box lets you store and display birth dates, death 
dates, and alternate names.

There are two ways that users can interact with taxonomies: they can be "closed," e.g. a fixed list to pick from in a dropdown, or "open," e.g. `field_tags` where users can enter new terms, which are created on the fly. This is not set on the _vocabulary_ itself, but in the configuration of the field (typically on a node).

!!! warning "Large Taxonomy Vocabularies"
The Drupal Taxonomy UI is known to break down when your vocabularies get large (e.g. over 20,000 terms). Jonathan Hunt created the [CCA Taxonomy Manager](https://github.com/catalyst/cca_taxonomy_manager) module for SFU to solve this problem.

!!! tip
    See also: [MIG Presentation on Taxonomies](https://docs.google.com/presentation/d/1LfpU6H4qxXtnYQPFntwMNtsgtU30yzp2MxwKKAllUOc/edit?usp=sharing) by Kristina Spurgin, 2021-07-19

## Fields

### What are fields?

Fields are places where you can store (meta)data. Each content type (or 
media type, or vocabulary) defines and configures a set of fields. These are 
usually configured by a high-permissioned user, such as a site administrator, 
or a manager responsible for metadata. This 
section will go deeper into these fields.

Each field has a machine name, such as `field_identifier`, and a [Field Type](#field-types), such 
as 'text'. These properties, along with any type-specific storage options 
(such as field length for text-type fields), 
are integral to the field, cannot be changed once created, and are constant 
across any instances where that field is used.

It is possible to re-use the same field across multiple bundles (but they 
must be of the same entity type - for example, you cannot re-use a node 
field on a media or taxonomy term). One example of a field that is re-used 
is the "Media of" field (`field_media_of`), which is used by all media types 
that come with Islandora.

Fields also have a display name (such as "Identifier") and display and form configurations; these are configured per bundle, so that a field with the 
same machine name may have a different display name on different content 
types, and may display differently.

The names and definitions of fields are entirely within Drupal and do not 
have to correspond to an outside metadata schema. 

#### Fields vs. XML and migrating from MODS

While you can store MODS as an XML file in a media in Islandora, to get 
the most out of Islandora we recommend extracting your metadata values into 
Drupal fields. This allows rich editing, indexing, and views-building 
features. The [fields configured on the Repository Item content type]
(starter-site-metadata-configuration.md) that 
comes with the Islandora Starter Site 
were designed with a [mapping from MODS](https://docs.google.com/spreadsheets/d/18u2qFJ014IIxlVpM3JXfDEFccwBZcoFsjbBGpvL0jJI/edit?pli=1#gid=0) in mind. You're free to use it as-is or 
modify it as desired.

Drupal fields aren't themselves part of any XML schema, so 
it's no longer viable to say that your Islandora data is "stored in MODS."
However, you can map fields into a variety of XML and RDF formats 
including MODS - see 
["Serialization"](#serialization). These mappings are lossy.

MODS has a lot of features that are not part of the Starter Site 
metadata configuration, and some are difficult to implement:
* Use of `type` and `displayLabel` attributes: this can be replicated for 
individual fields using Paragraphs. Additional theming would be required to 
display them in a useful way.
* Hierarchical nesting of `<relatedItem>`: This can be modelled with 
related items as their own nodes. However, this inflates the repository 
item count.
* Custom ordering of fields: The ordering of elements in XML is flexible 
and can be meaningful. In Drupal fields, you can retain the ordering within 
one field but the presentation order of fields is set for all nodes of a 
  content 
type.
* Human readability and sense-making: It is important to have a good 
data dictionary/metadata profile that explains what your fields are used 
for. The _description_ property of fields can help with this.

### Adding, Editing, or Deleting Fields

Fields can be modified under **Administration** >> **Structure** >> **Content 
types** >> _Your Content Type's Name_ >> **Manage fields** 
(`/admin/structure/types/your_type/fields`). This tab will list all Fields, 
their Label, Machine Name, Field Type. From this page you can make what 
edits to the existing fields that you can.

#### Adding fields
To make a new field, you will need to give it a Label, Machine name (usually 
automatically generated), and choose a [Field Type](#field-types). 

!!! note "Useful field types are under 'Reference'"
    Common field types such as *Taxonomy Term*, *Media*, and *Node* fields 
    are found under the generic term "Reference" (formerly "Entity Reference". 
Once selected, you will be able to choose the type of entity to reference 
(such as nodes, media, or taxonomy terms)

Next, depending on the field type, you will then define the 
maximum length of the field, the number of values it can contain, and/or what 
taxonomies it might link to.

!!! tip "You cannot change intrinsic properties of fields."
    As soon as you have created a field, you cannot change the machine name or 
    type. Additionally, once content has been added, you cannot change 
    additional properties such as the 
    number of values allowed in the field or the field 
    length. However, you can add new 
    fields to a content type, even after content is added. 

#### Deleting Fields

It is possible to delete fields. Deleting a field from a bundle instantly 
deletes all content in that 
field (unless the content is a reference to an entity in its own right - the 
referenced entity will persist). This means that:
* if you have a text field and delete it, all content in that text field is 
  gone including from all revisions.
* if you have a reference field such as a taxonomy term field, and you 
  delete the field, then you may have now-unused taxonomy terms that still  exist in a vocabulary even though no enties (or revisions) make reference to them.


### Form Display ("Manage form display")

Bundles let you configure how fields display in the edit form, which 
may be used by content editors for data entry (though you may use Islandora 
Workbench or Migrate to do data entry). This configuration happens on the 
bundle's 
**Manage form display** tab. Here, you can arrange the order of fields, make 
fields not display on the form, choose what Widget will define the 
entry options for a field, and then set certain settings for that Widget.

!!! note "Widgets"
    "Widget" is the name of a configurable editable form element in Drupal. 
Compare: 
**formatter**, which is for display.

Widgets are defined by Field Type, so an Entity reference field could use 
autocomplete, a select list, or even checkboxes. The widget used is chosen 
from a 
drop-down list. The widget settings are accessed through the gear on the far 
right of a row and may allow you to set the size of an entry field, whether 
the field *Label* is displayed, or if you use placeholder text, for example.

!!! tip "Using the drag-and-drop interface"
    Drupal uses drag-and-drop interfaces in many places. Clicking to the
    left of the label will allow you to drag the item into a new position. There
    is usually a section at the bottom for elements that are not enabled. If you
    don't want to drag, there is a link at the top right of the drag-and-drop
    table to "Show row weights". This allows you to enter digits instead of
    clicking and dragging.

!!! tip "Configuring who can edit specific fields"
    With the **Field Permissions** module (enabled with the Starter Site), 
    you can configure that only certain roles may edit a specific field.

### Content Display ("Manage display")

The **Manage display** tab for a content type (media type, vocabulary) is where 
you will make decisions about how to
display the metadata. Order is arranged using another drag-and-drop 
interface (see note "Using the drag-and-drop interface", above), and fields can 
again be
dragged to the **Disabled** section to hide the field from display. You can
choose the formatter, applicable options, and whether a field's label is 
displayed above the value, in-line, or hidden.

!!! note "Formatters"
"Formatter" is the name of a configurable field display element in Drupal. 
Compare: **widget**, which is for editable forms.

!!! tip "Configuring who can view specific fields"
    With the **Field Permissions** module (enabled with the Starter Site),
    you can configure that only certain roles may view a specific field.

## Field Types

Field types in Drupal determine what kind of data can be stored, and what 
widgets (see note "Widgets", above) and formatters (see note "Formatters", 
above) are available. 

Drupal comes with a number of built-in field types including boolean, 
datetime, entity reference, integer, string, text, and text_with_summary. 
More field types, formatters, and widgets are available in various modules. 
The [Drupal 8 documentation on FieldTypes, FieldWidgets, and FieldFormatters]
(https://www.drupal.org/docs/8/api/entity-api/fieldtypes-fieldwidgets-and
-fieldformatters) includes a list of the core field types with brief 
definitions, along with a list of core widgets and formatters. There is also 
documentation for [creating custom field types, widgets, and formatters](https://www.drupal.org/docs/creating-custom-modules/creating-custom-field-types-widgets-and-formatters)

Here we will describe a selection of field types of particular interest for 
Islandora users:
* Entity Reference (from Drupal Core)
* Authority Link (from the **Controlled Access Terms** module)
* EDTF (from the **Controlled Access Terms** module)
* Typed Relation (from the **Controlled Access Terms** module)

### Entity Reference

Entity Reference fields are a special type of field built into Drupal Core 
that creates relationships between entities such as nodes, media, and 
taxonomy terms. The field's configuration options include (but are not limited to):

- Which kind of entity can be referenced (*only one type of entity to reference 
  can be defined per field, and this cannot be changed once data has been 
  created*)
- The allowed number of values (limited or unlimited)
- Whether to use Views for filtering
- Whether to allow users to create new referenced entities while inputting data, if they don't already exist

The *Repository Item* content type, provided by the Islandora Starter Site, 
includes several entity reference fields that reference vocabularies defined 
by the Islandora Core Feature and Controlled Access Terms Defaults modules.

#### Configurations for Entity Reference fields

The screenshots below show how you can configure an entity reference field (in this case the Subject field on the Repository Item content type).

![Screenshot of the storage settings for an entity reference field](../assets/metadata_entity_reference_config.png)

Fig. 1: "Field Storage" settings for an entity reference field where you set 
whether the 
field 
will reference "Content" (nodes), taxonomy terms, or any other type of 
Drupal entity

![Screenshot of the reference type settings for an entity reference field, showing which vocabularies the autocomplete utility should query when editors are entering data.](../assets/metadata_entity_reference_config_vocabs.png)
Fig. 2: Reference type settings for an entity reference field where you select which vocabularies can be referenced

!!! tip "Data Consistency"
Selecting which vocabularies can be referenced by an entity reference field 
does not impose constraints on the underlying database, so it is 
possible to load references to other vocabularies without being stopped or 
warned when ingesting data through [various migration methods](../technical-documentation/migration-overview.md). However, this will result 
in content that cannot be edited/saved in the GUI without removing the 
offending term. 


### Authority Link

The Authority Link field type is defined in the *Controlled Access Terms* 
module and is a field that holds two associated values:

- An external authority source (selected from a configurable list of 
  external authority options),
- A link (URI) to a specific term from the selected external authority source

![Screenshot of filling out an Authority Sources field.](https://user-images.githubusercontent.com/32551917/182199562-46b6cc29-1a49-425c-8332-fbfff5eb44c6.png)

Within the Islandora Starter Site, this field type is used by the "Authority 
Sources" (`field_authority_link`) field, which is used in various 
Vocabularies including Person, Family, Subject, and Geographic Location. It 
is multivalued so can hold multiple URIs that you believe to be equivalent 
to the same concept.

!!! tip
    The term **external authority source** refers to both controlled 
vocabularies like Art & Architecture Thesaurus or FAST as well as Name Authority Files like Library of Congress Name Authority File or VIAF.

For instance, if you are creating a term called "Red squirrels" within the 
"Subject"  Vocabulary, you may want to include the URI for 
"Tamiasciurus" from the FAST (Faceted Application of Subject Terminology) 
vocabulary. If you configured the field Authority Sources to list FAST 
(Faceted Application of Subject Terminology) as an external authority source 
option, you can select this source and add the associated URI (http://id.worldcat.org/fast/1142424).


#### Configurations for Authority Link fields

Each instance of an Authority Link field can have 
different external authority source options. To configure an Authority Link 
field to change these options, navigate to the Manage Fields screen (e.g. 
Administration>>Structure>>Taxonomy>>*Taxonomy Vocabulary Name*>>Manage Fields) 
and select "Edit" for the Authority Link field (such as 
"Authority Sources"). 
Then enter your 
options in the text box, entering one value per line in the format `key|label`.
The key is the stored value (typically an abbreviation representing the authority source). The label will be used in displayed values and editing forms.

![Screenshot of the Authority Sources text box shown when editing the Authority Sources field.](https://user-images.githubusercontent.com/32551917/182200917-9d29fa07-3e4f-4850-a9c5-9bc270cc0c85.png)

By default, the Authority Sources field is repeatable. To change this, edit the 
"Field 
settings" and change Allowed numbers of values from "Unlimited" to "Limited" 
and enter the number of allowable values. If you are re-using the same field 
in multiple vocabularies, this will apply across all instances of this field.
You cannot restrict the repeatability of a field in a way that would 
disallow existing data in the field.


### EDTF

The EDTF field type is defined in the *Controlled Access Terms* module, and 
designed for recording dates in [Extended Date Time Format](https://www.loc.
gov/standards/datetime/edtf.html), which is a format based off of the 
hyphenated form of ISO 8601 (e.g. `1991-02-03` or `1991-02-03T10:00:00Z`), but 
also allows expressions of different granularity and uncertainty. The 
Default EDTF widget has an optional validator that only allows strings that 
conform 
to the EDTF standard. The Default EDTF formatter allows these date strings 
to be displayed in a variety of human-readable ways, including big- or 
little-endian, and presenting months as numbers, abbreviations, or spelling month names out in full. Close review of the [EDTF Specifications](https://www.loc.gov/standards/datetime/edtf.html) is recommended when configuring this field type.

!!! tip "Endianness"
    * Big-endian = year, month, day (e.g. 1988-02-03 or 1988 Feb 3). 
    * Little-endian = day, month, year (e.g. 03-02-1988 or 3 February 1988). 
    * Middle-endian = month, day, year (2/3/1988 or Feb. 3, 1988).

!!! note "Validation"
    The EDTF Widget always validates whether the input 
    string is valid EDTF
    format. As part of this, the components must be within appropriate ranges
    (years are four digits unless prefixed with Y; months must be within 1-12 or
    21-41, and day values must be within 1-31. However, a date that falls
    outside what we actually consider valid dates, such as 1999-02-31, will pass this basic validation.
    There is a second, strict validation option in the widget that can be
    enabled and ensures that dates provided are strictly valid: this would disallow 1999-02-31.

Example of valid inputs in a multivalued EDTF Date field (including the
seasonal value 2019-22 as defined in the EDTF specification):
![Screenshot of valid dates ('2019', '2019-11', '2019-22', and '2019-02-02T02:22:22Z') in an EDTF form widget.](../assets/metadata_edtf_valid.png)

Example of the same EDTF dates formatted using little-endian format:
![Screenshot of dates displayed as '2019', 'November 2019', 'Summer 2019', and '2 February 2019 02:22:22Z'.](../assets/metadata_edtf_display.png)

EDTF field values cannot include textual representations of dates, as shown below in this example of a valid EDTF value ('1943-05') and an invalid value ('May 1943') with the corresponding error message. Use the formatter configurations detailed further below to achieve textual display of dates.
![Screenshot of both a valid ("1943-05") and an invalid ("May 1943") EDTF entry. Displays the error message "Could not parse the date 'May 1943' Years must be at least 4 characters long."](../assets/metadata_edtf_invalid.png)


#### Configuration for the Default EDTF Widget

There is only one widget available for EDTF fields, the "Default EDTF 
Widget". If you create an alternate widget, please share it with the community! 

To configure the selected widget for a field on the Manage Form Display page 
for any bundle, click the gear icon at the far right. 

![Screenshot of the gear icon on the EDTF Widget display settings](../assets/metadata_edtf_field_settings_gear.png)

Configuration options include enabling strict date validation, allowing date 
intervals and allowing date sets.
![Screenshot of the configuration of the EDTF Widget](../assets/metadata_edtf_widget_settings.png)

!!! note "Ranges or Times - your choice"
    When configuring the EDTF widget, you can
    choose to allow date intervals (i.e. date ranges), but doing this prevents
    the widget from accepting any values that include times. If you'd like a 
    single field that contains date intervals as well as date-time values 
    (though not within a single value as that would not be valid EDTF), 
    your're welcome to file an improvement request (and link it here!)



#### Configuration for the Default EDTF Formatter

There is only one formatter available for EDTF fields, the "Default EDTF
Formatter". If you create an alternate formatter, please share it with the 
community!

To configure the selected formatter for a field on the Manage Display page
for any bundle, click the gear icon at the far right.

![Screenshot of the gear icon on the EDTF formatter settings](../assets/metadata_edtf_field_formatter_gear.png)

By selecting the appropriate configuration options for "Date separator", 
"Date Order", "Month Format", "Day Format", and "Year Format" you can 
configure a date to display in various ways.

![The Default EDTF Formatter settings pane](../assets/metadata_edtf_formatters.png)

#### Indexing and sorting EDTF fields in search results

When indexing EDTF date fields in Solr, the entered value (not the 
displayed value) is indexed, and it is indexed by default as a string. See 
heading ["Date Facets and the EDTF Year Processor"](#date-facets-and-the-edtf-year-processor) 
for an alternate method of indexing EDTF fields.

!!! Solr EDTF limitations
    The Solr string data type requires the full field value to match the 
    query in order to count as a match. This means that searching for 2014 will 
    not retrieve a record where the recorded date value is 2014-11-02. Changing 
    the Solr data type to fulltext will allow partial matches, but it will prevent 
    the field from being used as a facet.

Sorting on EDTF date fields may be configured in your search 
results Views. This results in a simple ordering by the literal EDTF date 
string.  A field with multiple or unlimited number of allowed 
values may be set as a sort field. In this case, the first occurrence of the 
field value is used as the sorting value.

##### "EDTF Year" Processor

The Controlled Access Terms module provides a search API processor, called 
"EDTF Year", that allows you to index the year values of one or more EDTF 
fields as integers. This allows for year-only date facets. Multiple fields 
(such as 
Date Created, Date Issued, 
etc) can be indexed together into a single field. 
Only the 
year, not month or day information, will be 
indexed. Multiple years will be indexed for a single EDTF value if the value 
is a range (that spans years) or if some year digits are unspecified. 

In the Islandora Starter Site, this field comes enabled 
and configured. To configure this field anew, first enable the processor by 
selecting the "EDTF Year" checkbox on the 
"Processors" tab of your search index (e.g. 
dmin/config/search/search-api/index/default_solr_index/processors) scroll 
down to the bottom of the Processors page to the "Processor Settings" and 
under the "EDTF Year" vertical tab, select the fields you would like indexed 
as well as various options.  , and then 
"Add field" on the 
"Fields" tab and select 

### Typed Relation

The Typed Relation field is defined in the *controlled_access_terms* module, is an extension of Drupal's Entity Reference field type, and allows the user to qualify the type of relation between the resource node and other entities, such as taxonomy terms. For example, it enables the inclusion of a resource's contributor's (assuming contributor names are modelled as taxonomy terms or some other Drupal entities) as well as their roles (such as "author", "illustrator", or "architect") in the resource node itself. Using only Drupal's Entity Reference fields, we would need individual fields for "author", "illustrator", "architect", and any other roles that may need to be made available. Using a Typed Relation field, we can have one Entity Reference field for "Contributors" and let the user pick the affiliated role from a predefined dropdown list.

!!! tip "Typed relation name"
    The parts of a field are called properties, so 'entity reference' and 'relation type' are properties of the Typed Relation field type.

#### Configurations for the Typed Relation field

The Islandora Starter Site includes a Typed Relation field labelled 'Contributors' as part of the Repository Item content type, and populates the available relations from the MARC relators list. This field was formerly called "Linked Agent". ![Screenshot of adding a value into a typed relation field](../assets/metadata_typed_relation_field.png)

The list of available relations for this Contributors field is configurable at '/admin/structure/types/manage/islandora_object/fields/node.islandora_object.field_linked_agent'.


!!! islandora "Typed relation tradeoffs"
    - If you apply this field to another content type, you can define unique relations available for that instance of the field.
    - However, multiple instances of this field means administrative overhead to maintain the separate lists of relations defined for each instance.

!!! Note "Publishers"
    Until Mar 2024, the Islandora Starter Site included publishers in the Contributors (`field_linked_agent`) field. The MIG made the decision to make publisher
    its own text field, in order to make it easier to separate publishers from other contributors, and to prevent clutter in the linked taxonomies. Publishers
    are often recorded by transcribing what is on the item, rather than formatting the name per Authority rules, so variations on a single name are expected. 

Relations are defined in the format *key\|value*, and the key is used in the RDF mapping (see below).

![Screenshot of the 'Available Relations' configuration text box for the 'Contributor' field.](../assets/metadata_available_relations_config.png)

By default, facets can be created for typed relation fields that will facet based on the linked entity alone, not separating references based on the relationship type.

## Structural Metadata
### Vocabularies

Islandora (through the Islandora Core Feature) creates the 'Islandora Models' vocabulary which includes the terms 'Audio', 'Binary', 'Collection', 'Compound Object', 'Digital Document', 'Image', 'Newspaper', 'Page', 'Paged Content', 'Publication Issue', and 'Video'. Islandora Starter Site provides contexts that cause certain actions (e.g. derivatives to happen, or blocks to appear) based on which term is used.

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

## Configure Field Integrations

Fields integrate with many other parts of Drupal. Consider the following 
when you add/edit/delete a field:

* **Form Display** - set a field to display (or not) in your form, and set 
  its widget, at  **Administration** \>\> **Structure** \>\> **Content types** \>\> [Your Content Type] \>\> **Manage form display**. You can also set it to display (or not) to certain users based on Field permissions at **Administration** \>\> **Structure** \>\> **Content types** \>\> [Your Content Type] \>\> **Manage fields** \>\> [Your new field].
* **Display** - set the field to display (or not) to the public at **Administration** \>\> **Structure** \>\> **Content types** \>\> [Your Content Type] \>\> **Manage display**. You can also set it to display or not to certain users based on Field permissions at **Administration** \>\> **Structure** \>\> **Content types** \>\> [Your Content Type] \>\>  **Manage fields** \>\> [Your new field].
* **Solr** - Solr configuration is set at **Administration** \>\> **Configuration** \>\> **Search and metadata** \>\> **Search API** \>\> [Default Solr content index] \>\> **Fields**.
By default, Solr indexes the "Rendered Item" using the display mode "Search index". By default, the "Search index" display mode is not configured separately, so it renders using the "Default" display mode which is the default (or only) tab when configuring Display, above. This means that if you configured your new field to display, then it will be automatically available to fulltext search. But if you want to make a facet, or a fielded search (using Advanced Search), then you need to index the field separately, as either String (for a facet) or fulltext (for a fielded search). To do this, use the "**+ Add field**" button in the Solr configuration and select your field, under the "Content" section. If it is a reference field (such as a taxonomy term field or a related item field) then you may want to "dive down" using the "(+)" buttons to index the name or title of the referenced entity.
* **Facet** - to make a facet, index the field as a string field (above). Then, create the facet at  **Administration** \>\> **Configuration** \>\> **Search and metadata** \>\> **Facets**. Finally, configure the facet to display by placing the corresponding block in the desired region.
* **Fielded Search** - to make your new field one of the drop-downs in the Advanced Search block, first index it in Solr as fulltext (as above). Find the Advanced Search block and click "Configure" and then drag and drop your new field into the non-hidden section. Save the block config.
* **RDF** - In the following section ("Getting Metadata into Fedora and a Triple-Store") we discuss how to index a field using Drupal's RDF mappings. You will want to do this if you are using Fedora or a Triple-store, or if you are using RDF to generate OAI-PMH DC data.
* **OAI-PMH MODS** - if you are using OAI-PMH to generate MODS for each object, you can configure how this field displays by setting it in the appropriate View, by default "OAI PMH Item Data". After adding the field, give it a label that matches the `rest_oai_pmh` module's [`templates/mods.html.twig`](https://git.drupalcode.org/project/rest_oai_pmh/-/blob/2.0.x/templates/mods.html.twig). For example, that template file includes `{{ elements.publisher }}` so the label in views needs to be exactly `publisher`. Save the View.
* **Citation CSL Mapping** - To let a new field be used in the creation of citations with Citation Select, navigate to the Citation CSL Settings (as of Citation Select version 2.0, this is at  **Administration** \>\> **Configuration** \>\> **Citation Select Settings** \>\> **CSL Mappings**. Select your new field where applicable.

An example of this, in terms of config changes, is visible in the [changeset of the Starter Site's 1.6.0 version](https://github.com/Islandora-Devops/islandora-starter-site/compare/1.5.0...1.6.0) for the Publisher field. However, pulling such a changeset is unlikely to work smoothly and it is recommended to set these configurations manually.


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

## Drupal Bundle Configurations

In Islandora, [content models](https://github.com/Islandora/islandora/wiki/Content-Models) are primarily created using content types (also known as node bundles) and media bundles. [Bundles](https://www.drupal.org/docs/8/api/entity-api/bundles) are defined by [YAML](http://befused.com/drupal/yaml) configuration files. [To create new content models](https://www.drupal.org/docs/8/api/entity-api/creating-a-custom-content-type-in-drupal-8), one would create the needed content types and media bundles via UI, then export the yml files related for those bundles using Configuration Synchronization (`http://localhost:8000/admin/config/development/configuration`) or [Features](https://www.drupal.org/project/features). An understanding about the structure of a bundle and various configuration files used to define it helps in creating and updating it.

Content types and media bundles can be thought of as web [forms](https://www.drupal.org/docs/user_guide/en/structure-widgets.html) consisting of fields. Drupal provides [widgets](https://www.drupal.org/docs/8/creating-custom-modules/create-a-custom-field-widget) to define the behavior of a field and field storage to define how the data is stored in the database. Drupal provides various [display modes](https://www.drupal.org/docs/8/api/entity-api/display-modes-view-modes-and-form-modes) to show the forms to user when they are editing (Manage form display) or viewing (Manage display).

A content model is packaged as a module for installation. All yml files are put in `config/install` folder of the module. Note that not all content models would contain media bundles.

The following files define the bundles themselves. It contains some metadata about the bundle and lists its dependencies.
```
node.type.your_content_type.yml
media_entity.bundle.your_media_bundle.yml
```

The following files define the fields attached to the bundle forms. There must be one config file for each field in your bundle, except for the default drupal fields.
```
field.field.node.your_content_type.field_name1.yml
field.field.node.your_content_type.field_name2.yml
...
field.field.media.your_media_bundle.field_name1.yml
field.field.media.your_media_bundle.field_name2.yml
```

If the new bundle contains new fields, then field storage configurations for the newly created fields would be needed as well. Note that if you reused existing fields, storage definitions should not be defined again. Storage config contains information about the number of values allowed for that field (cardinality).
```
field.storage.node.field_new_name3.yml
field.storage.media.field__new_name3.yml
```

There is a configuration file for each combination of bundle / display mode when managing form displays. Usually, form displays will have `default` and `inline` modes.
```
core.entity_form_display.media.your_media_bundle.default.yml
core.entity_form_display.media.your_media_bundle.inline.yml
---
core.entity_form_display.node.your_content_type.default.yml
core.entity_form_display.node.your_content_type.inline.yml
```

There is a configuration file for each combination of bundle / display mode when managing displays. Usually, displays will have `default` and `teaser` modes for content types and `default` and `content` modes for media bundles.
```
core.entity_view_display.media.your_media_bundle.default.yml
core.entity_view_display.media.your_media_bundle.content.yml
---
core.entity_view_display.node.your_content_type.default.yml
core.entity_view_display.node.your_content_type.teaser.yml
```

In addition, Islandora needs an RDF mapping to express the content in RDF and to sync to fedora. There will be one RDF mapping per bundle.
```
rdf.mapping.media.your_media_bundle.yml
rdf.mapping.node.your_content_type.yml
```

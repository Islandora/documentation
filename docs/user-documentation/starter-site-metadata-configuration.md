# Islandora Starter Site Metadata Configuration


## Introduction

As described in [Metadata In Islandora](metadata.md), in Islandora metadata is stored in Drupal, in fields attached to entities. Provided by the Islandora Starter Site, the “Repository Item” content type contains a set of default fields to describe the digital objects an Islandora repository might contain. This set of fields is based on MODS fields commonly used in Islandora 7, and is not intended to be "the standard" metadata profile, rather, a starting point for institutions designing their own repository.

This page presents the primary descriptive and administrative metadata fields found in the Islandora Starter site. We will define each field and give its basic configuration. Most of these configurations can be customized after installation, and new fields can be added as per the needs of an individual institution. Fields are grouped in standard MODS order. 

Further information on the metadata configuration can be found in a Google Spreadsheet [Islandora Starter Site Metadata Configuration (Google Sheets)](https://docs.google.com/spreadsheets/d/1N37GSwiDl_DSH9-n3BhWLUtjZohOg2udGJJlnZ8BmWQ/edit?usp=sharing) which parallels the information on this page. It goes into further detail about the configurations connected to each field, provides information on taxonomies and mappings, and can be filtered or sorted in a variety of ways. The document, or the spreadsheet above, can be copied and used as a basis for planning your own configuration customizations as you work on your Islandora site.


## Administrative/System Fields


<table>
  <tr>
    <td colspan="2" >
      <h3>Title</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >The name given to the resource. Title is a system field and as such its configurations can not be adjusted. It is the only field that is required by the system.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>title
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td><strong>yes</strong>
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255<em> (character length is set by system but can be changed with a contrib module, see below)</em>
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>title
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:title
    </td>
  </tr>
  <tr>
    <td>
      XPath MODS
    </td>
    <td>mods/titleInfo/title
    </td>
  </tr>
  <tr>
    <td>
      Transformation
    </td>
    <td>
      To create a single string out of the subelements of &lt;titleInfo>, we suggest to use the &lt;titleInfo> section of the LOC MODS-DC transform<a href="https://www.loc.gov/standards/mods/v3/MODS3-5_DC_XSLT1-0.xsl"> https://www.loc.gov/standards/mods/v3/MODS3-5_DC_XSLT1-0.xsl</a>. In words, it says to:
      <ul>
        <li>Take the value of &lt;nonSort>
        <li>If there was a &lt;nonSort>, add a space
        <li>Add the value of &lt;title>
        <li>If there is a &lt;subtitle>, add a space-colon-space and the value of (the first) &lt;subtitle>
        <li>If there is a &lt;partNumber>, add a period-space and the value of (the first) &lt;partNumber>
        <li>
          If there is a &lt;partName>, add a period-space and the value of (the first) &lt;partName>.
          <p>
            It is worth noting that the subtitle, partNumber, and partName elements are technically repeatable, but this transform only uses the first value encountered. If you have repeating subelements, information will be lost.
        </li>
      </ul>
    </td>
  </tr>
  <tr>
    <td rowspan="2" >
      Drupal Module Integration
    </td>
    <td>The <a href="https://www.drupal.org/project/title_length">Title Length</a> module allows you to extend the length of the title to 500 characters (possibly more).
    </td>
  </tr>
  <tr>
    <td>The <a href="https://www.drupal.org/project/views_natural_sort">Views Natural Sort</a> module allows you to sort a view by titles while skipping a configurable list of non-sorting characters ("A", "The", "L'", etc.). It's not as precise as nonSort, but does most of the job. 
    </td>
  </tr>
  <tr>
    <td>
      Alternatives
    </td>
    <td><a href="https://www.drupal.org/project/paragraphs">Paragraphs</a> are a way to model a multi-part title (nonSort, title, subtitle, partName, partNumber, etc). Combine paragraphs with (automatic entity label? automatic nodetitles?) to not have to enter your title information twice. (Who to contact with experience about this?)
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Member of</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >This item's parent item in Islandora. Usually this will be a collection, book ("Paged Content"),or compound object.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_member_of
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no <em>(will default to empty if no value is entered)</em>
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>no <em>(can only be connected to existing entities)</em>
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Member Of
    </td>
  </tr>
  <tr>
    <td>
      Alternatives
    </td>
    <td>You could arrange your content with <a href="https://www.drupal.org/project/entity_hierarchy">Entity Reference Hierarchy</a>, which is a very scalable way of representing large hierarchy trees. However, you'd have to re-work a number of hard-coded elements in the islandora module.
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Model</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >
      The internal-to-Islandora category of the resource. Affects how the item is displayed/viewed.
      <p>
        This field is actionable by Islandora and was designed to trigger derivatives and view modes through the use of <a href="https://islandora.github.io/documentation/user-documentation/context/">Contexts</a>. It is a controlled list and new values should only be added when required (i.e. new behaviors are made available and these values are created to trigger them).
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_model
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td><strong>yes</strong>
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>
      Islandora Models <em>(pre-populated/controlled)</em>
      <ul>
        <li>Audio
        <li>Binary
        <li>Collection
        <li>Compound Object
        <li>Digital Document
        <li>Image
        <li>Newspaper
        <li>Page
        <li>Paged Content
        <li>Publication Issue
        <li>Video</li>
      </ul>
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>type
    </td>
  </tr>
  <tr>
    <td>
      Alternatives
    </td>
    <td>You could model your different types as separate content types. This way, they could have different sets of metadata fields.
    </td>
  </tr>
</table>
<table>
  <tr>
    <td colspan="2" >
      <h3>Representative Image</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Optionally, use this field to link to a Media that will be used as the node's thumbnail. This will be displayed when the DGI Image Discovery module is used. This field takes precedence over the node's "Islandora thumbnail" (i.e. a related media with the "Thumbnail" media use term). See the module's <a href="https://github.com/discoverygarden/dgi_image_discovery">README</a> for the full scope of how images are "discovered". This is a good way to assign a "default" icon for many nodes that don't have actual thumbnail images, as this field allows you to re-use Media, which you can't do with normal Islandora media. 
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_representative_image
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference (Media)
    </td>
  </tr>
  <tr>
    <td>
      Required?
    </td>
    <td>no <em>(will default to using the Islandora thumbnail if no value is entered)</em>
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes <em>(you can upload files to create and link to a new Media)</em>
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>none
    </td>
  </tr>
  <tr>
    <td>
      Alternatives
    </td>
    <td>This field is itself an alternative to the "normal" Islandora thumbnail, and does not need to be used if the Islandora thumbnail suffices.
    </td>
  </tr>
</table>

## Other Title(s)

<table>
  <tr>
    <td colspan="2" >
      <h3>Full Title</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >If the full resource title is longer than 255 characters and you truncated it in the required Title field above, record the full title here.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_full_title
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain, long)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>rdau:P60515
    </td>
  </tr>
  <tr>
    <td>
      XPath MODS
    </td>
    <td>mods/titleInfo/title[1] + subfields[1] where [1] means first of their name (<em>see possible concatenation under Title</em>)
    </td>
  </tr>
  <tr>
    <td>
      Alternatives
    </td>
    <td>as mentioned under Title, the <a href="https://www.drupal.org/project/title_length">Title Length</a> module extends the length of the title and could make this field irrelevant.
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Alternative Title</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >
      Varying form of the title if it contributes to the further identification of the item.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_alt_title
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:alternative
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/titleInfo[@type="alternative" or @type="abbreviated" or @type="uniform"]
    </td>
  </tr>
  <tr>
    <td>Alternatives
    </td>
    <td>If you need to keep the information about what type of alternative title is being recorded, you could create multiple specific alternative title fields, or create a compound element using Paragraphs.
    </td>
  </tr>
</table>


## Contributors

<table>
  <tr>
    <td colspan="2" >
      <h3>Contributors<em> (formerly Linked Agent)</em></h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >
      Names of entities having some relationship to the resource, and, optionally, the relationship to the resource. If a relationship is not specified, it will be recorded as <a href="https://id.loc.gov/vocabulary/relators/asn.html">Associated Name</a> in the system's linked data representation.
      <p>
        This field does not allow creating names of entities on the fly. First create a term in either the person, family, or corporate body taxonomies, then link it here.
      <p>
        Field name changed from "Linked Agent" 09/23.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_linked_agent
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Typed Relation [link to https://islandora.github.io/documentation/user-documentation/metadata/#typed-relation] 
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Corporate Body; Family; Person
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Relators
    </td>
    <td>Starter Site includes 269 terms from the <a href="https://www.loc.gov/marc/relators/relaterm.html">MARC Relators</a> list. Further terms from the list, or custom terms, can be added in the configuration for this field. Publisher has been removed from this list to encourage use of the Publisher field.
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>author ; contributor ; editor
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Creators and Contributors
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:contributor
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>
      mods/name/namePart + role/roleTerm > relator
      <p>
        mods/name[@type="personal"]/namePart > "Person" taxonomy
      <p>
        mods/name[@type="corporate"]/namePart > "Corporate Body" taxonomy
      <p>
        mods/name[@type="family"]/namePart > "Family" taxonomy
    </td>
  </tr>
  <tr>
    <td>Alternatives
    </td>
    <td>Create custom facets using the "Typed Relation filtered by type" Search API processor. This would allow you to separate out, for instance, creator|author|photographer from the other types of relators as a Search API field/facet.
    </td>
  </tr>
</table>

## Type and Genre

<table>
  <tr>
    <td colspan="2" >
      <h3>Type</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >The general nature or genre of the content of the resource. To describe the digital or physical format of the resource, use Form instead. In contrast with Model, which is system-actionable, this field is designed to record the Type purely as a metadata element.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_resource_type
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference [link to <a href="https://islandora.github.io/documentation/user-documentation/metadata/#entity-reference">https://islandora.github.io/documentation/user-documentation/metadata/#entity-reference</a>
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Resource Types <em>(pre-populated with hierarchical <a href="https://id.loc.gov/vocabulary/resourceTypes.html">LOC Resource Type Scheme</a> terms)</em>
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Resource Type
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:type
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/typeOfResource, mods/typeOfResource[@collection=yes]
    </td>
  </tr>
  <tr>
    <td>Alternatives
    </td>
    <td>You may be required to use the Dublin Core types (for compatibility/harvesting); in that case you can replace the current Type vocabulary contents with the desired values.
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Genre</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >A term or terms that designate a category characterizing a particular style, form, or content, such as artistic, musical, literary composition, etc.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_genre
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Genre
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>genre
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:type
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/genre; mods/subject/genre
    </td>
  </tr>
</table>

## Origin Information

<table>
  <tr>
    <td colspan="2" >
      <h3>Place Published</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Plain text field to describe the place of publication in full or transcribed from an item \
      \
      See also the entity reference field Place Published Country.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_place_published
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>publisher-place
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>relators:pup
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/place/placeTerm [@type = 'text'] OR [not(@type)]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Country of Publication</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Entity reference field to singularly describe the country (or jurisdiction) of publication. Connected to the unpopulated Country taxonomy by default, which can be used for terms or codes from <a href="https://www.loc.gov/marc/countries/countries_code.html">MARC Code List for Countries</a> or elsewhere. Disabled, by default, on Metadata Display and Metadata Form.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_place_published_country
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Country
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>relators:pup
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/place/placeTerm[@type="code"]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Publisher</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >New field added 04/2024. Previously the Contributors/Linked agent field (with a relator value of Publisher) was used for this content.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_publisher
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>500
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>publisher
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/publisher
    </td>
  </tr>
</table>

### Date Fields
All date fields use the Drupal EDTF field type and must follow [EDTF](https://www.loc.gov/standards/datetime/) formatting. For more information on this format and how it works with Islandora see [https://islandora.github.io/documentation/user-documentation/metadata/#edtf](https://islandora.github.io/documentation/user-documentation/metadata/#edtf) 

<table>
  <tr>
    <td colspan="2" >
      <h3>Date Issued</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Date of formal issuance of the resource. This includes publication dates.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_edtf_date_issued
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>issued
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Year<em> (string converted to a simple year value for field_edtf_year and then used for Year facet. A range is converted to multiple years.)</em>
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:issued
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/dateIssued
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Date Created</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Date of creation of the resource
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_edtf_date_created
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Year<em> (string converted to a simple year value for field_edtf_year and then used for Year facet. A range is converted to multiple years.)</em>
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:created
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/dateCreated
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Date (Unspecified)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >A date without a type or relationship to the resource specified
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_edtf_date
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Year<em> (string converted to a simple year value for field_edtf_year and then used for Year facet. A range is converted to multiple years.)</em>
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:date
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/dateOther
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Copyright Date</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Date of copyright of the resource.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_copyright_date
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Year<em> (string converted to a simple year value for field_edtf_year and then used for Year facet. A range is converted to multiple years.)</em>
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:dateCopyrighted
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/copyrightDate
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Date Valid</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Date (often a range) of validity of a resource.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_date_valid
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:valid
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/dateValid
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Date Captured</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >The date on which the resource was digitized or a subsequent snapshot was taken.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_date_captured
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>premis:creation
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/dateCaptured
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Date Modified</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Date on which the original resource being represented in Islandora was changed. <em>Typically modification dates of digital representations of the resource stored in Islandora will be recorded on the relevant Media instead of here. This field is not populated automatically by any Drupal functionality.</em>
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_date_modified
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>EDTF
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>128
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:modified
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/dateModified
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Edition Statement</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Information identifying the version of the resource.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_edition
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>rdau:P60329
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/edition
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Mode of Issuance</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >A term that designates how the resource is issued. MODS standards limit the values of this field to monographic, single unit, multipart monograph, continuing, serial, and integrating resource, but there are no such limitations in Islandora and the “Issuance Mode” vocabulary is empty on initial installation.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_mode_of_issuance
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Issuance Mode
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>rdau:P60051
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/issuance
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Frequency</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >The publication frequency, in textual form. Can be based on <a href="https://www.loc.gov/standards/valuelist/marcfrequency.html">MARC Frequency</a> terms.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_frequency
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Frequency
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>rdau:P60538
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/originInfo/frequency
    </td>
  </tr>
</table>

## Language

<table>
  <tr>
    <td colspan="2" >
      <h3>Language</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >
      Language of the resource content. Can be based on a languages vocabulary such as ISO 639-2.<a href="https://www.loc.gov/marc/languages/">MARC Code List for Languages</a>.
      <p>
        <em>This field is not connected to Drupal system language fields or related to Drupal translation functionality.</em>
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_language
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Language
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>language
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:language
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/language/languageTerm[@type = 'text'] or [@type = 'code'] mapped to text per iso639-2b
    </td>
  </tr>
</table>

## Physical Description

<table>
  <tr>
    <td colspan="2" >
      <h3>Form</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >The physical format of the original resource being described. If the resource is a physical object, the physical form is recorded here. If the resource is born-digital, the original digital form is recorded here. Details of the formats of resource representations stored in Islandora should be recorded on the relevant Media files. 
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_physical_form
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Physical Form
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Physical Form
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:format
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/physicalDescription/form
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Extent</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >The size or duration of the resource in its original form. Extent of representations of the resource stored in Islandora should be recorded on the relevant Media files.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_extent
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:extent
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/physicalDescription/extent
    </td>
  </tr>
</table>

## Description(s)

<table>
  <tr>
    <td colspan="2" >
      <h3>Description</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Description of the resource. Description may include but is not limited to: an abstract, a table of contents (alternately its own field), a graphical representation, or a free-text account of the resource.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_description
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain, long)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>abstract
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:description
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/abstract<em> (xpath could also map to Abstract; do not map same values to both)</em>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Abstract</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Supplementary to the Description field, for including descriptions that more fit the definition of Abstract as it refers to ETDs, journal articles, or other scholarly publications.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_abstract
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (formatted, long)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:abstract
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/abstract<em> (xpath could also map to Description; do not map same values to both)</em>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Table of Contents</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Supplemental to Description, for breaking out specific Table of Contents information separate from a summary description of the object. Allows formatting.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_table_of_contents
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (formatted, long)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:tableOfContents
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/tableOfContents
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Note</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >General textual information relating to a resource, not described in other fields.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_note
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (formatted, long)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>skos:note
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/note (with [@displayLabel] or [@type] prepended with ': ' to the note)
    </td>
  </tr>
</table>

## Subject(s)

<table>
  <tr>
    <td colspan="2" >
      <h3>Subject</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >
      General subjects which may be constructed from topical, geographic, temporal, and genre subdivisions. If you wish to manage these types of subjects separately, use the more specific Subject fields below.
      <p>
      <p>
        This field can be used if you are storing composed subject strings rather than component headings/subdivisions in their own fields.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_subject_general
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Corporate Body; Family; Geographic Location; Person; Subject
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Store New Items In
    </td>
    <td>Subject
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Subject
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:subject
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Subject (Topical)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >For use when using separate fields to record topical, geographic, name, and temporal terms. Topical subject terms would be recorded here.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_subject
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Subject
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:subject
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject/topic
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Subject (Geographic)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >For use when using separate fields to record topical, geographic, name, and temporal terms. Geographic subject terms would be recorded here.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_geographic_subject
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Geographic Location
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:spatial
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject/geographic
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Subject (Name)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >For use when using separate fields to record topical, geographic, name, and temporal terms. Names of individuals, organizations, or families would be recorded here.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_subjects_name
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Corporate Body; Family; Person
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Store New Items In
    </td>
    <td>Person
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Subject (name)
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:subject
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject/name/namePart
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Subject (Temporal)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >For use when using separate fields to record topical, geographic, name, and temporal terms. Temporal subject terms would be recorded here.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_temporal_subject
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Entity reference
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255 (system limit for Taxonomy terms)
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      Taxonomies
    </td>
    <td>Temporal Subjects
    </td>
  </tr>
  <tr>
    <td>
      Create Referenced Entities
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      CSL Citation Mapping
    </td>
    <td>
    </td>
  </tr>
  <tr>
    <td>
      Facet
    </td>
    <td>Temporal Subject
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:temporal
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject/temporal
    </td>
  </tr>
</table>

## Coordinates

<table>
  <tr>
    <td colspan="2" >
      <h3>Coordinates</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Must be formatted either in decimal (51.47879) or sexagesimal format (51° 28' 43.644")
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_coordinates
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Geolocation
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:spatial
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject/cartographics/coordinates
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Coordinates (Text)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >A plain text field meant as an alternative to the Coordinates field, for values that do not meet the requirements of the Drupal geolocation field type.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_coordinates_text
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:spatial
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/subject/cartographics/coordinates
    </td>
  </tr>
</table>

## Classification

<table>
  <tr>
    <td colspan="2" >
      <h3>Dewey Classification</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Classification based on the Dewey Decimal system.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_dewey_classification
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dc11:subject
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/classification[@authority="ddc"]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Library of Congress Classification</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Classification based on the Library of Congress Classification system.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_lcc_classification
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dc11:subject
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/classification[@authority="lcc"]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Classification (Other)</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Classification based on any system that does not have a dedicated field.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_classification
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dc11:subject
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/classification
    </td>
  </tr>
</table>

## Identifier(s)

<table>
  <tr>
    <td colspan="2" >
      <h3>Identifier</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >A unique standard number or code that distinctively identifies a resource.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_identifier
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:identifier
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/identifier[not(@type)]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>ISBN</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >An International Standard Book Number (ISBN) identifier.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_isbn
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dbpedia:isbn
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/identifier[@type="isbn"]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>OCLC Number</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >An identifier assigned by an OCLC system, such as WorldCat.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_oclc_number
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dbpedia:oclc
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/identifier[@type="oclc"]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>Local Identifier</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Identifier from any locally managed system.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_local_identifier
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dcterms:identifier
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/identifier[@type="local"]
    </td>
  </tr>
</table>

<table>
  <tr>
    <td colspan="2" >
      <h3>PID</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >
      PID of this object in Islandora 7.x (applies to migrated objects only)
      <p>
        Disabled, by default, in Metadata Display.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_pid
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (plain)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Maximum Length
    </td>
    <td>255
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>no
    </td>
  </tr>
</table>

## Access Conditions

<table>
  <tr>
    <td colspan="2" >
      <h3>Rights</h3>
    </td>
  </tr>
  <tr>
    <td colspan="2" >Free text field for information about restrictions imposed on, and/or rights to, access to a resource, particularly a digital resource. It is not connected to any access control functionality within Drupal.
    </td>
  </tr>
  <tr>
    <td>
      Machine Name
    </td>
    <td>field_rights
    </td>
  </tr>
  <tr>
    <td>
      Drupal Field Type
    </td>
    <td>Text (formatted, long)
    </td>
  </tr>
  <tr>
    <td>
      Required
    </td>
    <td>no
    </td>
  </tr>
  <tr>
    <td>
      Repeatable
    </td>
    <td>yes
    </td>
  </tr>
  <tr>
    <td>
      RDF Mapping
    </td>
    <td>dc11:rights
    </td>
  </tr>
  <tr>
    <td>XPath For MODS
    </td>
    <td>mods/accessCondition
    </td>
  </tr>
</table>


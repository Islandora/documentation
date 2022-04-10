# Drupal Configuration Provided By Islandora Defaults

This is a list of the configuration items provided by the Islandora Defaults module.

## Entities

(see Glossary: [Entity](../user-documentation/glossary.md#entity))

### Content Types (i.e. Node bundles)

|Name  					|Description  		 		|Comments							|
|---					|---					|---								|
|Repository Item (islandora_object)   	|An item in your Islandora repository.  |Defaults provides only one node type to use with Islandora, and different behaviours are triggered by different field values. If you would like to use another content type with Islandora, it will need Islandora behaviours configured appropriately (see below). 	|


### Media Types (i.e. Media bundles)

|Name  					|Description  		 		|Comments							|
|---					|---					|---								|
|Audio (audio)   	| A locally hosted audio file.  	| This is Drupal's built-in Audio media type. Islandora Defaults overwrites the field storage so that it is not actually a locally hosted file. The description is misleading.	|
|Document (document) 	| An uploaded file or document, such as a PDF.		| This is Drupal's built-in Document media type. Islandora Defaults overwrites the field storage so that it is not actually a locally hosted file. The description is misleading.|
|File (file)		| Use local files for reusable media.			||
|Image (image)		| Use local images for reusable media.			| This is Drupal's built-in Image media type. Islandora Defaults overwrites the field storage so that it is not actually a locally hosted file. The description is misleading.	|
|Video (video)		| A locally hosted video file.				| This is Drupal's built-in Video media type. Islandora Defaults overwrites the field storage so that it is not actually a locally hosted file. The description is misleading.	|

### Vocabularies (i.e. Taxonomy bundles)

|Name			|Description		|Comments			|
|---			|---			|---				|
|Islandora Access (islandora_access)	|Terms used to limit, restrict or coordinate access	| Not sure if they actually do affect access.	|
|Islandora Display (islandora_display)	|Terms used to alter how a repository item is viewed. ie. basic image vs large image	| Check if/how this gets populated.|

### Form Modes

The following "Default" forms are configured. It is unclear how/if these include Islandora-specific choices such as ordering/grouping or form widget configuration.

|Type			|Bundle			|Name	|Comment|
|---			|---			|---	|---	|
|Node			|Islandora Object	|Default||
|Media			|Audio			|Default||
|Media			|Document		|Default||
|Media			|File			|Default||
|Media			|Image			|Default||
|Media			|Video			|Default||
|Taxonomy Term		|Islandora Display	|Default||
|Taxonomy Term		|Islandora Media Use	|Default|This vocabulary is not defined in this module.|
|Taxonomy Term		|Islandora Models	|Default|This vocabulary is not defined in this module.|


### Display Modes

The following new display modes are defined:

| Type    | View Mode      | Comment                                                              |
|---------|----------------|----------------------------------------------------------------------|
| Content | Binary         |                                                                      |
| Content | Open Seadragon |                                                                      |
| Content | PDFjs          |                                                                      |
| Content | Teaser         | This seems to be a re-definition of the built-in "Teaser" view mode. |
| Media   | Open Seadragon |                                                                      |
| Media   | PDFjs          |                                                                      |


The following display modes are configured. It is unclear how/if the "Default", "Source", and "Teaser" display modes include any Islandora-specific choices such as ordering/grouping or display widget configuration. "Media EVAs (display_media)" is a view provided by the Islandora core feature.

| Type          | Bundle              | Display Mode   |                                                                                                      |
|---------------|---------------------|----------------|------------------------------------------------------------------------------------------------------|
| node          | islandora_object    | binary         | This display mode includes the "EVA: Media EVAs - Original File - Download" as well as metadata fields. |
| node          | islandora_object    | default        | This display mode includes "EVA: Media EVAs - Service File" as well as metadata fields.                 |
| node          | islandora_object    | open_seadragon | This display mode includes "EVA: OpenSeadragon Media EVAs - Original File" as well as metadata fields. See OpenSeadragon Media EVAs under Views.  |
| node          | islandora_object    | pdfjs          | This display mode includes "EVA: PDFjs Media EVAs - Original File" as well as metadata fields. See PDFjs Media EVAs under Views.        |
| node          | islandora_object    | teaser         |                                                                                                      |
| media         | audio               | default        |                                                                                                      |
| media         | audio               | source         |                                                                                                      |
| media         | document            | default        |                                                                                                      |
| media         | document            | pdfjs          | This display mode shows only the "Document" file field, formatted as "PDF: Default viewer of PDF.js".   |
| media         | document            | source         |                                                                                                      |
| media         | file                | default        |                                                                                                      |
| media         | file                | open_seadragon | This display mode shows only the "File" file field, formatted as "OpenSeadragon."                       |
| media         | file                | pdfjs          | This display mode shows only the "File" file field, formatted as  "PDF: Default viewer of PDF.js".      |
| media         | file                | source         |                                                                                                      |
| media         | image               | default        |                                                                                                      |
| media         | image               | open_seadragon | This display mode shows only the "Image" file field, formatted as "OpenSeadragon."                      |
| media         | image               | source         |                                                                                                      |
| media         | video               | default        |                                                                                                      |
| media         | video               | source         |                                                                                                      |
| taxonomy_term | islandora_display   | default        |                                                                                                      |
| taxonomy_term | islandora_media_use | default        |                                                                                                      |
| taxonomy_term | islandora_models    | default        |                                                                                                      |


### RDF Mappings

The following RDF mappings for the RDF module are included:

| Type          | Bundle              | Comments |
|---------------|---------------------|----------|
| node          | islandora_object    |          |
| media         | audio               |          |
| media         | document            |          |
| media         | file                |          |
| media         | image               |          |
| media         | video               |          |
| taxonomy_term | islandora_access    |          |
| taxonomy_term | islandora_display   |          |
| taxonomy_term | islandora_media_use |          |
| taxonomy_term | islandora_models    |          |


### Language

These configuration files enable content translation on their bundles.

| Type          | Bundle              | Comments |
|---------------|---------------------|----------|
| node          | islandora_object    | Do not hide untranslatable fields.          |
| media         | audio               | Do not hide untranslatable fields.          |
| media         | file                | Do not hide untranslatable fields.          |
| media         | image               | Do not hide untranslatable fields.          |
| media         | video               | Do not hide untranslatable fields.          |
| taxonomy_term | islandora_access    | Language alterable is false.         |
| taxonomy_term | islandora_media_use | Language alterable is false.         |
| taxonomy_term | islandora_models    | Language alterable is false.         |

## Views

(see Glossary: [View](../user-documentation/glossary.md#view))

| Name                                                | Description                                    | Displays                                                                                     | Comments                                               |                                                                                                                                  |
|-----------------------------------------------------|------------------------------------------------|----------------------------------------------------------------------------------------------|--------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------|
| IIIF Manifest (iiif_manifest)                       | Generates IIIF manifests for paged content     | REST Export (/node/%node/book-manifest);<br/>REST Export - Single node (/node/%node/manifest) |                                                        |                                                                                                                                  |
| Members (members)                                   | Displays members for content.                  | Members (block)                                                                              | Show an unformatted list of published nodes that are "Member of" the current node (from URL), formatted as "Teaser"s, paged to 10 items.                                                      |                                                                                                                                  |
| OpenSeadragon Media EVAs (openseadragon_media_evas) | Displays media for content as EVA's per model. | Original File;<br/>Preservation Master;<br/>Service File | Show 1 Media that is "Media of" the node (from URL) and having Media Use corresponding to each Display (Original file, etc.). The Media is rendered with display mode OpenSeadragon. See media.\*.open_seadragon under "Display Modes." The description's "per model" should read "per media use". Provides the EVA used in the islandora_object.open_seadragon display mode. The Preservation Master and Service File displays are not used. |
| PDFjs Media EVAs (pdfjs_media_evas)                 | Displays media for content as EVA's per model. |  Original File;<br/>Preservation Master;<br/>Service File | Show 1 Media that is "Media of" the node (from URL) and having Media Use corresponding to each Display (Original file, etc.). The Media is rendered with display mode PDFjs. The description's "per model" should read "per media use". Provides the EVA used in the islandora_object.pdfjs display mode. The Preservation Master and Service File displays are not used. |


## Contexts

The contexts provided by Islandora Defaults are organized into groups: **Indexing**, **Derivatives**, and **Display**.

### Contexts - Indexing

| Name                            | Description                             | Conditions (if)                                                             | Reactions (then)                                                                                                                                                                                                            | Comments |
|---------------------------------|-----------------------------------------|-----------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------|
| All Media (all_media)           | All media, regardless of where it lives | **Entity bundle in:** (audio, document, extracted_text, file, image, video) | **alter JSONld type** source field: field_media_use; <br/>**JSON-LD Self Reference:** iana:describedby <br/> **Index:** Index in Fedora; Index in Triplestore<br/> **Delete:** Delete from Fedora; Delete from triplestore. |          |
| Content (repository_content)    | All repository content                  | **Node bundle:** Repository Item                                            | **Index:** Index in Fedora; Index in Triplestore<br/> **Delete:** Delete from Fedora; Delete from triplestore. <br/> **JSON-LD Self Reference:** schema:sameAs <br/>**Alter JSON-LD Type**: field_model                     |          |
| External Files (external_files) | Files not in Fedora                     | **File uses filesystem:** public                                            | **Index:** Index file as Fedora External Content <br />**Delete**: Delete file as fedora external content                                                                                                                   |          |
| Fedora Files (files_in_fedora)  | Files in Fedora                         | **File uses filesystem:** Fedora                                            | **Index:** Index Fedora File in Gemini <br/> **Delete**: Delete fedora file from Gemini                                                                                                                                     |          |
| Taxonomy Terms (taxonomy_terms) | All taxonomy terms                      | **Content Entity Type**: Taxonomy Term                                      | **Index:** Index taxonomy term in Fedora; Index taxonomy term in the triplestore <br/> **Delete**: Delete taxonomy term in Fedora; delete taxonomy term in Triplestore                                                      |          |

### Contexts - Derivatives

| Name | Description | Conditions (if) | Reactions (then) | Comments |
|---|---|---|---|---|
| Audio Derivatives (audio_original_file) | Derivatives for Audio |**Media has term with URI:** Original File (17) **AND** **Parent node for media has term with URI**: Audio (22)|**Derivative:** Audio - Generate a service file from an original file ||
| Image Derivatives (image_original_file) | Derivatives for Images |**Media has term with URI:** Original File (17) **AND** **Parent node for media has term with URI**: Image (25)|**Derivative:** Image - generate a service file from an original file; Image - generate a thumbnail from an original file||
| PDF Derivatives (pdf_original_file) | Derivatives for PDF documents |**Media has term with URI:** Original File (17) **AND** **Parent node for media has term with URI**: Digital Document (27)|**Derivative**: Digital Document - generate a thumbnail from an original file; Extract text from PDF or image| Any document types that are not PDFs but seem intuitive to call documents, such as text files, email, MS Office files or open equivalents, will not have functioning derivatives because this assumes that all Digital Documents are PDFs.|
| Page Derivatives (pages) | Derivatives for Pages |**Parent node for media has term with URI**: Page (29) **AND** **Media has term with URI:** Original File (17)|**Derivative**: Extract Text from Image or PDF; Image - generate a service file from an original file; Image - generate a thumbnail from an original file||
| Video Derivatives (video_original_file) | Derivatives for Video |**Media has term with URI:** Original File (17) **AND** **Parent node for media has term with URI**: Video (26)|**Derivative**: Video - generate a service file from an original file; Video - generate a thumbnail from an original file||

### Contexts - Display


| Name | Description | Conditions (if) | Reactions (then) | Comments |
|---|---|---|---|---|
| Binary (binary) | Binary Object Display |**Node has term with URI**: Binary (23)|**Change view mode**: Binary|See node.islandora_object.binary under Display Modes||
| Collection (collection) | Display block of children |**Node has term with URI**: Collection (24)|**Blocks**: "Members" placed in Content Suffix in Carapace||
| Open Seadragon (open_seadragon) | Open Seadragon viewer for Images |**Node has term with URI**:Open Seadragon (2)|**Change view mode**: Open Seadragon |See node.islandora_object.open_seadragon under Display Modes||
| PDFjs Viewer (pdfjs) | Displays file using the PDFjs viewer |**Node has term with URI**: PDFjs (3)|**Change view mode:** PDFjs|See node.islandora_object.pdfjs under Display Modes|
| Paged Content - Openseadragon (openseadragon_block) | Display a block for Paged Content |**Node has term with URI:** Open Seadragon (2) AND Paged Content (28)|**Blocks** Openseadragon block placed in Main Content in Carapace||


## Actions
| Name | Plugin | Comments|
|---|---|---|
| Audio - Generate a service file from an original file (audio_generate_a_service_file_from_an_original_file) | generate_audio_derivative |use the Homarus queue to generate an "audio" media tagged #ServiceFile from the #OriginalFile with mimetype audio/mpeg, using the libmp3lame codec and audio quality 5, and save in the public filesystem in a folder for the year and month.  |
| Digital Document - Generate a thumbnail from an original file (digital_document_generate_a_thumbnail_from_an_original_file) | generate_image_derivative | use the houdini queue to generate an "image" media of type thumbnail, mimetype image/**png**, size 100x100 and save to the public filesystem in a folder for the year and month.  |
| Image - Generate a thumbnail from an original file (image_generate_a_thumbnail_from_an_original_file) | generate_image_derivative |use the houdini queue to generate an "image" media tagged thumbnail, mimetype image/**jpg**, size 100x100 and save to the public filesystem in a folder for the year and month.   |
| Image - Generate a service file from an original file (image_generate_a_service_file_from_an_original_file) | generate_image_derivative |use the houdini queue to generate an "image" media of type "image/jpeg" tagged with ServiceFile, from the original file, and save it in the public filesystem in a folder for the year and month.  |
| Video - Generate a thumbnail from an original file (video_generate_a_thumbnail_from_an_original_file) | generate_image_derivative | Use the **homarus** queue to generate an image media tagged Thumbnail of type image/jpeg from the original file by taking 1 frame from the 1 second timestamp, and save it to the public filesystem in a folder for the year and month. |
| Video - Generate a service file from an original file (video_generate_a_service_file_from_an_original_file) | generate_video_derivative |use the Homarus queue to generate a video media tagged service file of type video/mp4 from an original file, save it to the public filesystem in a folder for the year and month.  |


## Migrations


| Name |
|---|
|Tags migration for islandora_defaults feature (islandora_defaults_tags)|

Contents of the islandora_defaults_tags migration:

|Vocabulary|Term name|Description|External Uri|
|---|---|---|---|
|islandora_display|"Open Seadragon"|"Display using the Open Seadragon viewer"|http://openseadragon.github.io|
|islandora_display|"PDFjs"|"Display using the PDF.js viewer"|http://mozilla.github.io/pdf.js|
|resource_types|"Collection"|"An aggregation of resources"|http://purl.org/dc/dcmitype/Collection|
|resource_types|"Dataset"|"Data encoded in a defined structure"|http://purl.org/dc/dcmitype/Dataset|
|resource_types|"Image"|"A visual representation other than text"|http://purl.org/dc/dcmitype/Image|
|resource_types|"Interactive Resource"|"A resource requiring interaction from the user to be understood, executed, or experienced"|http://purl.org/dc/dcmitype/InteractiveResource|
|resource_types|"Moving Image"|"A series of visual representations imparting an impression of motion when shown in succession"|http://purl.org/dc/dcmitype/MovingImage|
|resource_types|"Physical Object"|"An inanimate, three-dimensional object or substance"|http://purl.org/dc/dcmitype/PhysicalObject|
|resource_types|"Service"|"A system that provides one or more functions"|http://purl.org/dc/dcmitype/Service|
|resource_types|"Sound"|"A resource primarily intended to be heard"|http://purl.org/dc/dcmitype/Sound|
|resource_types|"Still Image"|"A static visual representation"|http://purl.org/dc/dcmitype/StillImage|
|resource_types|"Software"|"A computer program in source or compiled form"|http://purl.org/dc/dcmitype/Software|
|resource_types|"Text"|"A resource consisting primarily of words for reading"|http://purl.org/dc/dcmitype/Text|


core.base_field_override.node.islandora_object.changed.yml
core.base_field_override.node.islandora_object.created.yml
core.base_field_override.node.islandora_object.menu_link.yml
core.base_field_override.node.islandora_object.path.yml
core.base_field_override.node.islandora_object.promote.yml
core.base_field_override.node.islandora_object.status.yml
core.base_field_override.node.islandora_object.sticky.yml


core.base_field_override.node.islandora_object.title.yml
core.base_field_override.node.islandora_object.uid.yml



## User Roles

The role fedoraadmin (machine name: `fedoraadmin`) is defined.

## Hooks, code, etc.
- Add `'relators' => 'http://id.loc.gov/vocabulary/relators/`' to the RDF namespaces.


## Fields

This is at the end of this document because it's not particularly useful yet - there's so much information it's not clear what needs to be here.
### Fields configured on bundles

- field.field.node.islandora_object.field_access_terms.yml
- field.field.node.islandora_object.field_alternative_title.yml
- field.field.node.islandora_object.field_classification.yml
- field.field.node.islandora_object.field_coordinates.yml
- field.field.node.islandora_object.field_coordinates_text.yml
- field.field.node.islandora_object.field_description.yml
- field.field.node.islandora_object.field_dewey_classification.yml
- field.field.node.islandora_object.field_display_hints.yml
- field.field.node.islandora_object.field_edition.yml
- field.field.node.islandora_object.field_edtf_date.yml
- field.field.node.islandora_object.field_edtf_date_created.yml
- field.field.node.islandora_object.field_edtf_date_issued.yml
- field.field.node.islandora_object.field_extent.yml
- field.field.node.islandora_object.field_genre.yml
- field.field.node.islandora_object.field_geographic_subject.yml
- field.field.node.islandora_object.field_identifier.yml
- field.field.node.islandora_object.field_isbn.yml
- field.field.node.islandora_object.field_language.yml
- field.field.node.islandora_object.field_lcc_classification.yml
- field.field.node.islandora_object.field_linked_agent.yml
- field.field.node.islandora_object.field_local_identifier.yml
- field.field.node.islandora_object.field_member_of.yml
- field.field.node.islandora_object.field_model.yml
- field.field.node.islandora_object.field_note.yml
- field.field.node.islandora_object.field_oclc_number.yml
- field.field.node.islandora_object.field_physical_form.yml
- field.field.node.islandora_object.field_pid.yml
- field.field.node.islandora_object.field_place_published.yml
- field.field.node.islandora_object.field_resource_type.yml
- field.field.node.islandora_object.field_rights.yml
- field.field.node.islandora_object.field_subject.yml
- field.field.node.islandora_object.field_subjects_name.yml
- field.field.node.islandora_object.field_table_of_contents.yml
- field.field.node.islandora_object.field_temporal_subject.yml
- field.field.node.islandora_object.field_weight.yml
- field.field.media.audio.field_access_terms.yml
- field.field.media.audio.field_file_size.yml
- field.field.media.audio.field_media_audio_file.yml
- field.field.media.audio.field_media_of.yml
- field.field.media.audio.field_media_use.yml
- field.field.media.audio.field_mime_type.yml
- field.field.media.audio.field_original_name.yml
- field.field.media.document.field_access_terms.yml
- field.field.media.document.field_file_size.yml
- field.field.media.document.field_media_of.yml
- field.field.media.document.field_media_use.yml
- field.field.media.document.field_mime_type.yml
- field.field.media.document.field_original_name.yml
- field.field.media.file.field_access_terms.yml
- field.field.media.file.field_file_size.yml
- field.field.media.file.field_media_file.yml
- field.field.media.file.field_media_of.yml
- field.field.media.file.field_media_use.yml
- field.field.media.file.field_mime_type.yml
- field.field.media.file.field_original_name.yml
- field.field.media.image.field_access_terms.yml
- field.field.media.image.field_file_size.yml
- field.field.media.image.field_height.yml
- field.field.media.image.field_media_image.yml
- field.field.media.image.field_media_of.yml
- field.field.media.image.field_media_use.yml
- field.field.media.image.field_mime_type.yml
- field.field.media.image.field_original_name.yml
- field.field.media.image.field_width.yml
- field.field.media.video.field_access_terms.yml
- field.field.media.video.field_file_size.yml
- field.field.media.video.field_media_of.yml
- field.field.media.video.field_media_use.yml
- field.field.media.video.field_media_video_file.yml
- field.field.media.video.field_mime_type.yml
- field.field.media.video.field_original_name.yml
- field.field.taxonomy_term.islandora_display.field_external_uri.yml


### New fields

The following are new fields we define:

- field.storage.node.field_access_terms.yml
- field.storage.node.field_alternative_title.yml
- field.storage.node.field_classification.yml
- field.storage.node.field_coordinates.yml
- field.storage.node.field_coordinates_text.yml
- field.storage.node.field_description.yml
- field.storage.node.field_dewey_classification.yml
- field.storage.node.field_display_hints.yml
- field.storage.node.field_edition.yml
- field.storage.node.field_edtf_date.yml
- field.storage.node.field_edtf_date_created.yml
- field.storage.node.field_edtf_date_issued.yml
- field.storage.node.field_extent.yml
- field.storage.node.field_genre.yml
- field.storage.node.field_geographic_subject.yml
- field.storage.node.field_identifier.yml
- field.storage.node.field_isbn.ymlg
- field.storage.node.field_language.yml
- field.storage.node.field_lcc_classification.yml
- field.storage.node.field_linked_agent.yml
- field.storage.node.field_local_identifier.yml
- field.storage.node.field_note.yml
- field.storage.node.field_oclc_number.yml
- field.storage.node.field_physical_form.yml
- field.storage.node.field_pid.yml
- field.storage.node.field_place_published.yml
- field.storage.node.field_resource_type.yml
- field.storage.node.field_rights.yml
- field.storage.node.field_subject.yml
- field.storage.node.field_subjects_name.yml
- field.storage.node.field_table_of_contents.yml
- field.storage.node.field_temporal_subject.yml
- field.storage.media.field_access_terms.yml


### Overrides of core fields (?)

- core.base_field_override.node.islandora_object.changed.yml
- core.base_field_override.node.islandora_object.created.yml
- core.base_field_override.node.islandora_object.menu_link.yml
- core.base_field_override.node.islandora_object.path.yml
- core.base_field_override.node.islandora_object.promote.yml
- core.base_field_override.node.islandora_object.status.yml
- core.base_field_override.node.islandora_object.sticky.yml
- core.base_field_override.node.islandora_object.title.yml
- core.base_field_override.node.islandora_object.uid.yml
-
- [audio]
- core.base_field_override.media.audio.changed.yml
- core.base_field_override.media.audio.created.yml
- core.base_field_override.media.audio.name.yml
- core.base_field_override.media.audio.path.yml
- core.base_field_override.media.audio.status.yml
- core.base_field_override.media.audio.thumbnail.yml
- core.base_field_override.media.audio.uid.yml
- [file]
- core.base_field_override.media.file.changed.yml
- core.base_field_override.media.file.created.yml
- core.base_field_override.media.file.name.yml
- core.base_field_override.media.file.path.yml
- core.base_field_override.media.file.status.yml
- core.base_field_override.media.file.thumbnail.yml
- core.base_field_override.media.file.uid.yml
- [image]
- core.base_field_override.media.image.changed.yml
- core.base_field_override.media.image.created.yml
- core.base_field_override.media.image.name.yml
- core.base_field_override.media.image.path.yml
- core.base_field_override.media.image.status.yml
- core.base_field_override.media.image.thumbnail.yml
- core.base_field_override.media.image.uid.yml
- [video]
- core.base_field_override.media.video.changed.yml
- core.base_field_override.media.video.created.yml
- core.base_field_override.media.video.name.yml
- core.base_field_override.media.video.path.yml
- core.base_field_override.media.video.status.yml
- core.base_field_override.media.video.thumbnail.yml
- core.base_field_override.media.video.uid.yml

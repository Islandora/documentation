# File Viewers

## What are viewers?

[Viewers](../glossary#viewer) allow site builders to display files in interactive JavaScript-based widgets, that provide functionality like zooming in/out, turning pages, playing/pausing, viewing in full screen, etc.

In Drupal, a common way to implement a viewer is through a [module](./glossary/#module) that provides the JavaScript library, and a field formatter that uses that library. The field formatter will work with specific types of Drupal fields (e.g. file fields or image fields, some may even provide their own fields).

Viewers that are known to work with Islandora include:

* [OpenSeadragon](https://openseadragon.github.io/), via the Drupal module [OpenSeadragon](https://github.com/Islandora/openseadragon) (maintained by the Islandora Foundation).
* [pdf.js](https://github.com/mozilla/pdf.js), via the Drupal contrib module [PDF](https://www.drupal.org/project/pdf)

Both are included with Islandora Defaults.


## How are viewers configured?

In the generic Islandora content modelling scenario, we often want the viewer to appear on a _"resource node"_ when the file itself is in a field on a Media that references that node. These layers of indirection make displaying viewers on nodes rather complex.

## Example - OpenSeadragon (in Islandora Defaults)

Here is how viewers are implemented in Islandora Defaults. This example uses OpenSeadragon, but pdf.js is configured with the same method. This method was chosen because it uses pure configuration and no extra code. It relies on users tagging nodes with appropriate "display hint" terms to trigger Contexts to switch the node's display mode to one that includes an EVA field, which renders the appropriate Media in a way that shows the desired file in the viewer.

<!-- Display modes vs view modes - Here we prefer the term 'display mode' to refer to the modes listed under Structure > Display Modes > View modes. The reasoning is that for the audience of this page, this concept is likely most frequently encountered under "Manage Display" of an entity, and to distinguish it from concepts relating to Views, such as Views displays. -->

### Expected behaviour for this scenario:

Create a Repository Item, and tag it with "Open Seadragon" (under Display Hints). Add a Media of type "File" to that node, upload a file such as a TIFF or JPEG 2000, and tag it with "Original File" (under Media Use). Save and publish the Media. Go to the Repository Item's page, and you should see your file displayed in the viewer.

<!-- There should be a link here, on the word "File", that points to a page explaining why large images need to be Files and not Images. That is out of scope here. -->

<!-- "a file such as TIFF or JPEG 2000" = "a file that works with Cantaloupe". OpenSeadragon relies on an IIIF image server, in our case, Cantaloupe, and that is what determines the kinds of files that this works with. This is also out of scope for this page. -->

### Components of this scenario:

1. A viewer that includes a field formatter  (e.g. the module OpenSeadragon)
1. A media display mode (e.g. the media display mode "Open Seadragon")
1. A media type (File, though Image is configured similarly)
1. A file field on that media type that can use that field formatter (e.g. field_media_file)
1. A field on that media type that points to a parent node (e.g. field_media_of)
1. Display Mode Configuration  (make that display mode, on that media type, show only the file field rendered through that viewer's field formatter. e.g. see Media Types > File > Manage Display > Open Seadragon)
1. An EVA view (which shows a node's attached media as rendered by that Media display mode. e.g. the view "OpenSeadragon Media EVAs")
1. A content display mode (e.g. the node display mode called "Open Seadragon")
1. A content type (e.g. Repository Item)
1. Display Mode Configuration (make that display mode, on that content type, display that EVA as well as whatever metadata is relevant - see Content Types > Repository Item > Manage Display > Open Seadragon )
1. A taxonomy term with a URI (e.g. "Open Seadragon" (http://openseadragon.github.io) imported by a migration in Islandora Defaults)
1. A field on that content type that can have that taxonomy term (e.g. the "Display Hints" field on Repository Item)
1. A context (which says if a node has that term, then change to the display mode. See the context "Open Seadragon")

### Logic:

- If a node has the right term
- then a context is triggered
- so the node gets shown in a custom display mode
- which you configured for that content type
- to include a custom EVA View "field"
- which displays the node's attached media rendered in a custom media display mode
- which for that media type is configured
- to display nothing but the file in a viewer.

### Relevant files:

Islandora Defaults is a Feature, and the following YAML files in `islandora_defaults/config/install` contain configuration items that are loaded when the feature is enabled. Changes to these files will not affect the live site configuration, and changes to the live site configuration will not be reflected in these files.

| Filename | Comments|
|---|---|
| `core.entity_view_mode.node.open_seadragon.yml` | (a media display mode) defines the Open Seadragon display mode as an option for any Media |
| `media.type.file.yml` | (a media type) define File media, or in this case, re-define an existing type provided by Core. This feature will override the core settings. |
| `field.field.media.file.field_media_file.yml` | (a field that can use the viewer) Defines a file field on File media called field_media_file, or in this case, re-defines it because it was already part of core. Usually you need a field.storage too but it was already defined in core and is not overridden here. |
| `field.field.media.file.field_media_of.yml` | (a field that points to a parent node) attach the the "Media Of" field to File media. In this case again, the field storage is not present because it was defined in Islandora Core Feature. |
| `core.entity_view_display.media.file.open_seadragon.yml` | (view mode configuration) configures the Open Seadragon display mode for File media, so it shows only the field_media_file using the OpenSeadragon field formatter. |
| `views.view.openseadragon_media_evas.yml` | (an EVA View) defines a view that shows a Media that is "Media Of" the current node (from URL) and is published and has "Media Use" = Original File. (there are two other EVAs defined by this view that use Preservation Master and Service File respectively.) |
| `core.entity_view_mode.node.open_seadragon.yml` | (a content display mode) defines the Open Seadragon display mode as an option for any Node |
| `node.type.islandora_object.yml` | (a content type) defines the Repository Item content type |
| `core.entity_view_display.node.islandora_object.open_seadragon.yml` | (view mode configuration) configures the Open Seadragon display mode for Repository Item, so it shows the Open Seadragon EVA for Original File as well as normal metadata |
| `taxonomy.vocabulary.islandora_display.yml` | (taxonomy vocabulary) define a vocabulary to hold display hints  |
| `migrate_plus.migration.islandora_defaults_tags.yml` | (migration) create a term in that vocabulary for "Open Seadragon." Taxonomy terms are "content" so must be entered through a migration. |
| `field.storage.node.field_display_hints.yml` | (field storage for display hints) defines the display hints field as an option for any node |
| `field.field.node.islandora_object.field_display_hints.yml` | (field for display hints) configures the display hints field on Repository Item |
| `context.context.open_seadragon.yml` | (a context) tells nodes with term "Open Seadragon" to use display mode "Open Seadragon" |

## Improvements

This is awfully complex. If you'd like to help simplify it, we have an open issue about getting rid of display hints, and instead "sniffing' the file's mime type: [Deprecate display hints in favor of Contexts? #1193](https://github.com/Islandora/documentation/issues/1193). Other suggestions and solutions are welcome in the [issue queue](https://github.com/Islandora/documentation/issues).

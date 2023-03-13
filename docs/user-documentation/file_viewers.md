# File Viewers

## What are viewers?

[Viewers](../user-documentation/glossary#viewer) allow site builders to display files in interactive JavaScript-based widgets, that provide functionality like zooming in/out, turning pages, playing/pausing, viewing in full screen, etc.

In Drupal, a common way to implement a viewer is through a [module](glossary.md#module) that provides the JavaScript library, and a field formatter that uses that library. The field formatter will work with specific types of Drupal fields (e.g. file fields or image fields, some may even provide their own fields). Some modules also provide a block, that can display appropriate files based on the context.

Viewers that are known to work with Islandora include:

* [OpenSeadragon](https://openseadragon.github.io/), via the Drupal module [OpenSeadragon](https://github.com/Islandora/openseadragon) (maintained by the Islandora community).
* [Mirador](https://projectmirador.org/), via the Drupal module [Islandora Mirador](https://github.com/Islandora/islandora_mirador/) (maintained by the Islandora community).
* [pdf.js](https://github.com/mozilla/pdf.js), via the Drupal contrib module [PDF](https://www.drupal.org/project/pdf)
* Audio with captions, via the Islandora module
* Video with captions, via the Islandora module


## Configuring Field Formatters as Viewers

The Drupal way of showing a "viewer" is to render a Media using a View Mode that shows only the desired file, displayed in a field formatter that invokes the desired viewer.

In the Starter Site:

* On all Media Types, there is a "Source" view mode which is configured to show only the main ("source") file of that Media in a reasonable desired viewer.
* In the case of OpenSeadragon and PDFjs, additional view modes are configured on the Media Types where it is possible to have files that need those viewers (Image, Document, File).
* By default, on a node's page, a Block will display an attached Media - Service File, if it's available, with a fallback to Original File - in the "Source" view mode, i.e. in its default viewer. This is done using a Context that places a Block.
* On a node-by-node-basis, you can override the viewer used by setting the "Viewer Override" field to a different viewer (such as PDF.js). This will cause a different Context to be activated instead, which will show the file in the selected view mode.

!!! note
    Formerly, this field was called "Display Hints". That field name has been retired in order to reduce confusion, since this uses a different mechanism. This mechanism no longer relies on Node View Modes, or EVA views. However, the basic EVA view still persists in the starter site as it is part of the Islandora Core Feature. Again, it will first look for a Service File, then fall back to the Original File.


### Changing a Viewer for all media of a media type

With the above configuration:

* Navigate to the "Manage Display" page for that media type
* Select the "Source" view mode (the secondary tabs along the top)
* Make sure that only the appropriate fields are being rendered
* For the "main" file field (it's named different things in different media types: `field_media_file`, `field_media_image`... as appropriate), select a different field formatter and configure it how you like it. 

### Configuring an "optional" viewer

Suppose you have a new viewer available, for example, for zip files. You could either:

* create a new media type specially for zip files, and configure this viewer in the "Source" view mode, or,
* configure an alternative viewer for the File media type.

Either would work! The choice is yours to make. They're honestly both good.

Should you choose the latter:

* create a new Display mode for media at Structure > Display Modes > View modes. Make sure you select a "Media" view mode.
* Configure the relevant (File) Media Type to display your file in your viewer.  In the File media type, go to Manage Display, and on the Default tab, enable this view mode for "Custom Display Settings" (it's all the way at the bottom). A tab for new display mode should have appeared. Go there and set up your field so that only the file field displays, and it displays using your viewer.
* in the "Media Display" view, create a new Block (or pair of Blocks) that (just for this Block) render the Media in your new view mode. If desired, create a pair with one selecting a Service File and one selecting a Original File, and use "No results behaviour" to place a fallback.
* in the Islandora Display taxonomy, add a new term, with an external URI.
* create a Context that finds Islandora Nodes that have a term with that URI.  In that context, place the block you created in the Media Display view. 
* Finally, edit the Default Media display Context to not be in effect if the node has a term with the URI that you set.


## Configuring Viewers that use Blocks

Both OpenSeadragon and Mirador provide blocks that act as multi-page viewers. To configure one of these viewers:

* Place the block on relevant pages. Usually this is a node page. In the Starter Site this is done by a Context ("Openseadragon Block - Multipaged items"). Other methods of placing blocks include the standard Block interface, and Layout Builder.
* While placing the block, it will ask you to configure the "IIIF manifest URL". In the Starter Site, we have a IIIF Manifest view configured to create a manifest based on the "original file" media attached to the pages (children) of a given node. In the view, it is configured with path `node/%node/book-manifest-original`; in the block, we enter this as node/[node:nid]/book-manifest-original. When the block is rendered on a node page, such as `node/18`, then the nid (18) will be passed into the view.
* If placing a block using Contexts, make sure that "Include blocks from block layout" is selected. (If you find yourself missing normal page elements, this may be why). 

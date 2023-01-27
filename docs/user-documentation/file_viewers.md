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

The Drupal way of showing a viewer is to render a Media, using a View Mode that shows only the desired file, and which displays that file using the desired field formatter.

In the Starter Site:
* Repository Item nodes are set up to display a special view called "Media EVAs - Service File" (see Structure > Content Types > Repository Item > Manage Display)
* The "Media EVAs - Service File" view is set up to display one (1) attached media tagged "Service File" rendered using the "Source" view mode. (see Structure > Views > Media EVAs)
* In most media types, the "Source" view mode is specially configured to show just the "main" file of that media, using an appropriate viewer.
* Individual media can override the default viewer by setting the "Display mode for viewer" field. This field allows you to select a different view mode that will be displayed instead of "Source". For example, Images can render in the default image viewer, or in OpenSeadragon.


### Changing a Viewer for all media of a media type

With the above configuration:
* Navigate to the "Manage Display" page for that media type
* Select the "Source" view mode (the secondary tabs along the top)
* Make sure that only the appropriate fields are being rendered
* For the "main" file field (it's named different things in different media types: File, Image, as appropriate), select a different field formatter and configure it how you like it. 

### Configuring an "optional" viewer

Suppose you have a new viewer available, for example, for zip files. You could either:

* create a new media type specially for zip files, and configure this viewer in the "Source" view mode, or,
* configure an alternative viewer for the File media type.

Either would work! The choice is yours to make. They're honestly both good.

Should you choose the latter:
* create a new Display mode for media at Structure > Display Modes > View modes. Make sure you select a "Media" view mode.
* in the File media type, configure the "Display mode for viewer" field. If it doesn't exist yet, add a view mode switch field type. Configure it so that "View modes to switch" has "Source" selected, and "View modes allowed to switch to" includes your new view mode. 
* in the File media type, go to Manage Display, and on the Default tab, scroll all the way to the bottom. Open the collapsed "Custom display settings" section. Select your new view mode and click save.
* A tab for new display mode should have appeared. Go there and set up your fields so that the file field displays in your viewer.


## Configuring Viewers that use Blocks

Both OpenSeadragon and Mirador provide blocks that act as multi-page viewers. To configure one of these viewers:

* Place the block on relevant pages. Usually this is a node page. In the Starter Site this is done by a Context ("Openseadragon Block - Multipaged items"). Other methods of placing blocks include the standard Block interface, and Layout Builder.
* While placing the block, it will ask you to configure the "IIIF manifest URL". In the Starter Site, we have a IIIF Manifest view configured to create a manifest based on the "original file" media attached to the pages (children) of a given node. In the view, it is configured with path `node/%node/book-manifest-original`; in the block, we enter this as node/[node:nid]/book-manifest-original. When the block is rendered on a node page, such as `node/18`, then the nid (18) will be passed into the view.
* If placing a block using Contexts, make sure that "Include blocks from block layout" is selected. (If you find yourself missing normal page elements, this may be why). 

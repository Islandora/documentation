# IIIF (International Image Interoperability Framework)

[IIIF](https://iiif.io/) is a set of specifications that provides interoperability of image-based collections across platforms. What this means for a repository platform like Islandora at a general level is that image-based objects such as (still) images and paged content can be managed by Islandora but viewed in external applications, and that Islandora can bring in image-based content from elsewhere to supplement locally managed content. If this intrigues you, see the section "Looking under the hood (and beyond)" below.

At a practical level, because Islandora supports several of the IIIF specifications, we can:

- Zoom, pan, and rotate images within a IIIF-compliant [viewer](../file_viewers) like OpenSeadragon or Mirador
   - Islandora uses an IIIF-compliant image server (by default, [Cantaloupe](https://cantaloupe-project.github.io/)) that utilizes the [IIIF Image API](https://iiif.io/api/image/2.1/). This capability is similar to what Islandora 7.x users experience when they view a Large Image.
- Display thumbnails for all pages of a book or newspaper issue within image viewers
   - IIIF-compliant image viewers such as OpenSeadragon or Mirador can display a "collection" of images such as all the pages of a book or newspaper issue using the [IIIF Presentation API](https://iiif.io/api/presentation/2.1/). For example, here is a screenshot of OpenSeadragon rendering all the pages of a book:

   ![OpenSeadragon rendering book pages](../assets/osd_collection_mode.png)

## Using IIIF in Islandora

If you're not using one of our provisioning tools, you will need to:
* install and configure the Cantaloupe (or another IIIF-compliant) image server
* install a [viewer](../file-viewers) module and configure it to point to the IIIF Image server location
* if using the viewer to show single images, configure it as a file formatter in one of the display modes for your media.
* if using the viewer to show multi-paged content, install the [Islandora IIIF](https://github.com/Islandora/islandora/tree/2.x/modules/islandora_iiif) module and create a IIIF view (see the one in the Starter Site as an example), then configure the viewer's _block_ to show up where desired (see below for details on how it could be configured)


## Using IIIF in the Islandora Starter Site

The Islandora Starter Site uses a Context to automatically use the IIIF Presentation API for showing [paged content](../paged-content).


To use this Context, give your book or newspaper (or other paged content) a model of "Paged Content" or "Publication Issue". Then, in the _Paged Content - Openseadragon_ Context, make sure the term used is in the "Node has term" condition (you can register more than one term there). Now, when you view a paged content Islandora node, you will see service files of all of its child pages (assuming you have added some child pages to the object) in the OpenSeadragon viewer as illustrated above.

You can change how the paged content images are arranged in the OpenSeadragon viewport by doing the following:

1. Visit `admin/config/media/openseadragon`
1. Scroll to the bottom, where you will see the "Collection Mode" options.
1. The "Enable Collection Mode" checkbox will be unchecked. This is normal (unless you have already checked it). The Open Seadragon Context automatically, and temporarily, puts OpenSeadragon in Collection mode when rendering a Paged Content object and then puts it back to Sequence mode (which is what mode it's in when the "Enable Collection Mode" checkbox is unchecked). _This means that in order for you to change options that apply to Collection Mode, you will need to check the "Enable Collection Mode" checkbox, change its options, save the form, then uncheck the "Enable Collection Mode" checkbox again._ Just follow these steps, and it will work!
  1. Check the "Enable Collection Mode" checkbox. The Collection Mode options will appear.
  1. Adjust the options to what you want.
  1. Click the "Save Configuration" button.
  1. After the form is saved, navigate back down to the "Collection Mode" options and _uncheck_ the "Enable Collection Mode" checkbox.
  1. Click the "Save Configuration" button.

## Looking under the hood (and beyond)

  If you want to see the raw output of the IIIF API implementations in Islandora, visit a node that is displaying the OpenSeadragon viewer (doesn't matter if it's a single image or a paged content node like a book), and tack "manifest" onto the end of the URL, like `http://myrepo.org/node/23/manifest` and hit enter. You will see the raw JSON that IIIF-compliant viewers use to render the content.

  The really neat thing is, IIIF-compliant viewers don't need to be embedded in Islandora websites. If a viewer on another website knows the URL of a IIIF manifest like the ones that Islandora can produce, that viewer can display the content described in the manifest. Some implementations of IIIF viewers that show off the potential to combine content from multiple IIIF servers include:

  * [The Biblissima Project](http://demos.biblissima-condorcet.fr/mirador/) - brings together related content from different repositories.
  * [diva.js](https://ddmal.music.mcgill.ca/diva.js/try/iiif-external.html) - select a source from the drop-down list at the top.

These two examples have nothing to do with Islandora, but illustrate the potential for IIIF to build tools that extend beyond a given repository platform.

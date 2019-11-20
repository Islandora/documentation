# IIIF (International Image Interoperability Framework)

[IIIF](https://iiif.io/) is a set of specifications that provides interoperability of image-based collections across platforms. What this means for a repository platform like Islandora is that image-based objects such as (still) images and paged content can be managed by Islandora but viewed in external applications, and that Islandora can bring in image-based content from elsewhere to supplement locally managed content.

At a practial level, because Islandora supports several of the IIIF specifications, we can:

- Zoom, pan, and rotate images within OpenSeadragon
   - Islandora uses an IIIF-compliant image server (by default, [Cantaloupe](https://cantaloupe-project.github.io/)) that utilizes the [IIIF Image API](https://iiif.io/api/image/2.1/). This capablity is similar to what Islandora 7.x users experience.
- Display thumbnails for all pages of a book or newspaper issue within image viewers
   - IIIF-compliant image viewers such as OpenSeadragon or Mirador can display a "collection" of images such as all the pages of a book or newspaper issue using the [IIIF Presentation API](https://iiif.io/api/presentation/2.1/). For example, here is a screenshot of OpenSeadragon rendering all the pages of a book:
   
   ![OpenSeadragon rendering book pages](../assets/osd_collection_mode.png)

## Using IIIF in Islandora 8

Implementation of the IIIF Presentation API is new in Islandora 8, and using it is as simple as configuring a Context (in fact, you don't even need to configure it, the Context, Openseadragon Block, already exists by default).

To use this Context, all that is required is for your book or newspaper (or other paged content) to be given a model of "Paged Content" or "Publication Issue". Then, in the Openseadragon Block Context, make sure the term used in the "Node has term" condition (you can register more than one term there). Now, when you view a paged content Islndora object, you will see thumbnails of all of its child pages (assuming you have added some child pages to the object) in the OpenSeadragon viewer as illustrated above. 

You can change how the paged content thumbnails are arranged in the OpenSeadragon viewport by doing the following:

1. Visit `admin/config/media/openseadragon`
1. Scroll to the bottom, where you will see the "Collection Mode" options.
1. The "Enable Collection Mode" checkbox will be unchecked. This is normal (unless you have already checked it). The Openseadragon Context automatically, and temporarily, puts OpenSeadragon in Collection mode when rendering a Paged Content object and then puts it back to Sequence mode (which is what mode it's in when the "Enable Collection Mode" checkbox is unchecked). _This means that in order for you to change options that apply to Collection Mode, you will need to check the "Enable Collection Mode" checkbox, change its options, save the form, then uncheck the "Enable Collection Mode" checkbox again._ Just follow these steps, and it will work!
  1. Check the "Enable Collection Mode" checkbox. The Collection Mode options will appear.
  1. Adjust the options to what you want.
  1. Click the "Save Configuration" button.
  1. After the form is saved, navigate back down to the "Collection Mode" options and _uncheck_ the "Enable Collection Mode" checkbox.
  1. Click the "Save Configuration" button.
  
  ## Looking under the hood (and beyond)
  
  If you want to see the raw output of the IIIF API implementations in Islandora 8, visit a node that is displaying the OpenSeadragon viewer (doesn't matter if its a single image or a paged content node like a book), and tack "manifest" onto the end of the URL, like `http://myrepo.org/node/23/manifest` and hit enter. You will see the raw JSON that IIIF-compliant viewers use to render the content.
  
  The really neat thing is, IIIF-compliant viewers don't need to be embedded in Islandora websites. If a viewer on another website knows the URL of a IIIF manifest like the ones that Islandora can produce, that viewer can display the content described in the manifest. Some implementations of IIIF viewers tha show off the potential to combine content from multiple IIIF servers include:
  
  * [The Biblissima Project](http://demos.biblissima-condorcet.fr/mirador/)
  * [diva.js](https://ddmal.music.mcgill.ca/diva.js/try/iiif-external.html) - select a source from the drop-down list at the top.
  
These two examples have nothing to do with Islandora, but illustrate the potential for IIIF to build tools that extend beyond a given repository platoform.

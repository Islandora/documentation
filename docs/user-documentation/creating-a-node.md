## Overview

A resource node holds the descriptive metadata for content, as well as grouping together an original file and
all of the derivatives files generated from it.   

The model for exactly what constitutes an object in Islandora 8 is flexible and can be adapted to the needs of specific users. For example, the Islandora Defaults configuration considers an object as a resource node of the type "Repository Item"  which contains descriptive metadata about the object. Attached to that Node are one or more Media, each representing a file that is part of this object, such as "Original File", "Thumbnail", "Preservation Master", etc. With this model, every original file uploaded into Islandora has its own resource node.

Multi-file Media configurations also attach Media to a parent node, but allow for that node to be represened by multiple "Original File"s. In this model, a Media contains the original file as well as any derivative files created from it (thumbnail, service file, etc.).

For an example of where these two different approaches could apply, the basic configuration might make sense for a book that has rich page-level metadata, so that each page would be its own Node with its own metadata record; the multi-file media configuration might be a better fit for a book that does not have page-level metadata (except an ordering or page numbers), so that each Media would represent one page, and all pages (Media) would be attached to a single parent Node/metadata record for the entire book. 

## Before you start

- The following How-To assumes that you are using the optional [islandora_defaults](https://github.com/Islandora/islandora_defaults) configuration. This configuration is deployed automatically if you build your Islandora site using the [Ansible Playbook](https://islandora.github.io/documentation/installation/playbook/), ISLE (documentation pending), or are using the [sandbox or a Virtual Machine Image](https://islandora.ca/try)
- This How-To assumes familiarity with Drupal terms such as [Node](https://www.drupal.org/docs/7/nodes-content-types-and-fields/about-nodes) and [Media](https://www.drupal.org/docs/8/core/modules/media).

## How to add an item to your repository

### Create the Node

To create a new resource node, go to Content >> Add content or click on **Add content** under _Tools_.

![Click on add content](../assets/add-content-loading-media.jpg)

Then click on **Repository Item**. This will assign the default metadata profile to your item.

![Click on repository item](../assets/repository-item.jpg)

Fill out the form. We're going to create an image, so under _System_, select **Image** from the _Model_
drop down box. Selecting different models will impact how Islandora handles content, dictating
important behaviours such as display and derivative generation.

![Under system select appropriate model, or format](../assets/under-system-select-format.jpg)

When done, click **Save**.

### Upload an Original File

Congratulations, you have created a resource node! But alas, it has no files. To upload a file, click on the
node's _Media_ tab.

![When done, click on Media](../assets/click-media.jpg)

Then click on **Add Media**.

![Click on Add Media](../assets/add-media.jpg)

We want to add an image, so clicking on _Image_ is appropriate in most circumstances. Drupal considers
any type of image that can be viewed natively in the browser as an _Image_. For other image types that
require special viewers, such as Tiffs, you have to choose _File_

![Click on image option](../assets/image-option.png)

You are now presented with the form for the technical metadata of the file. There are three required
parts of the form:

1. The media's name.
1. The file to upload.
1. The usage of the file, which dictates how Islandora interprets the file. To trigger derivative
generation, select _Original File_ from the drop down box.

![You are now presented with the form for the technical metadata of the file.](../assets/adding-image.jpg)

Click **Save** when done, and the file will be uploaded (to Fedora by default). Now return to the node
you created and you should see the image along with its descriptive metadata.

![The file is now loaded, return to the main site to view](../assets/final-loaded-image.jpg)

!!! Tip "Islandora Quick Lessons"
    Learn more with this video on [Adding Content](https://youtu.be/G52is7iFkG4).


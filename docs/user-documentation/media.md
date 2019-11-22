Drupal 8 recognizes files (such as images, audio files, video files, etc.) but wraps each file in an intermediate structure called a
"media" to allow us to attach fields to files. Drupal uses a media's fields to store information about the media's file, such as file
size, width and height (for images), alt text (for images), creation date, and so on.

!!! note "Compared to Islandora 7"
    In Islandora 7, this sort of technical metadata would have been stored as datastream properties or as additional metadata-specific datastreams.
    In Islandora 8, each datastream holds its technical metadata using an associated media entity.
    
Fedora will store the media's file as a binary resource and use the media's properties as the binary resource's description.

For example, creating a media with a file named `test.jpg` (in November 2019) will create 
a Fedora binary resource of the file accessible at `/fcrepo/rest/2019-11/test.jpg` 
with the media's fields accessible at `/fcrepo/rest/2019-11/test.jpg/fcr:metadata`.


## Media revisions

The metadata associated with a file can be updated by clicking the _Edit_ tab while on the media's page and clicking **Save**.

The _Create new revision_ checkbox is selected by default which will prompt Fedora to make a new version of the media's metadata
before updating its resource record. A message can be added to the revision which is stored in Drupal but is not currently saved in Fedora.

### Do not replace a media's file

The media edit form allows a user to remove a file and replace it with a new one. 
However, because of the relationship Islandora creates between a file and its media, the effects of removing a file and uploading a new one are not obvious.

First, the _remove_ button removes the file's reference on the media but does not delete the file. This leaves the file in Fedora. 

Second, because Drupal does not want users to reuse a file path uploading a file, even with the same name in the same month, Drupal will rename the file for you resulting in a new binary resource in Fedora.

Third, because the new file is now associated with the media, the media will now be used to represent the metadata for the new file and will no longer be used for the existing one.

!!! note "Replacing Media via REST"
    REST can circumvent Drupal form behavior making it possible to add and replace media and files via [Islandora's REST interface](../technical-documentation/using-rest-endpoints.md).	

## Media Ownership

Islandora 8 objects can have any number of media associated with them. Media store a reference to the resource node they belong to using a special field,
"Media Of". By changing this field's value, you can change which resource node owns the media, and therefore, where it gets displayed or managed.

!!! note "Compared to Islandora 7"
    The direction of the relationship between objects and datastreams is reversed when compared to Islandora 7.  Generally speaking,
    objects are unaware of their datastreams, and it's a Drupal view that lists datastreams for an object.

## Media Use

Islandora 8 media express their intended use with a special "Media Use" field, which accepts taxonomy terms from the "Media Usage"
vocabulary. Because the Media Usage vocabulary is an ordinary Drupal vocabulary, Islandora 8 site administrators can create additional
terms, and in turn, these local terms can be used to identify media that have some custom local purpose. However, most of the 
default set of "Media Use" terms are taken from the [PCDM Use Extension](https://pcdm.org/2015/05/12/use) vocabulary:

![Media tab](../assets/media_use_vocabulary_media_form.png)

!!! note "Compared to Islandora 7"
    Terms from the Media Usage vocabulary are very similar to DSIDs in Islandora 7.  The only difference is that a DSID is immutable,
    but a media's usage can be changed at any time through the media's edit form.

## Derivatives

Islandora generates derivatives based on the Media Use term selected for a Media and the Model of the node that owns it.  All of this is configurable
using Context.

By default, derivatives are generated from Media with the "Original Files" term selected. When an Original File is uploaded, if the node that
owns it has an "Image" model, image derivatives are created.  If it's a "Video", then video derivatives are generated, etc...

By default, derivatives are created with a path determined by the current year, current month, the associated resource node' identifier 
assigned by Drupal, and the type of derivative. For example, a resource node
found at `node/2` with a service file media generated in October 2019 will have the path "2019-10/2-Service File.jpg".
Naming conventions can be configured by editing each derivative action's _File path_ settings.

![File path setting for the Image Service File derivative action.](../assets/media_action_file_path_setting.png)

Within a node's media tab, you can see all of its media, including derivatives, listed along with their usage. For example, from the
Original File, a lower quality "Service File" and a smaller "Thumbnail Image" file were generated.

![Media tab](../assets/resource_nodes_media_tab.png)

For more information on how to configure derivatives, see the section on [Context](context.md).

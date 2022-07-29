# Media in Islandora

In Islandora, Media are created to hold the various files that pertain
to the descriptive metadata provided by a Node. Islandora provides some
fields and features to make this possible. 

## Media are wrappers for files

Drupal Media are said to be wrappers around files, that allow that file
to have fields attached. These fields may contain technical metadata
about that file, such as its mimetype or size. This is how Islandora
uses Media (though our approach to [technical metadata] differs). 


## Media may store files in different locations.

Configuration on Media types determines where uploaded files will be 
stored - for example, the Drupal public or private filesystem, or through
a tool called [Flysystem] to another data store such as Fedora. Islandora
does not dictate where you put your files. Islandora Defaults, as a
full-featured example, sets all media types to store their files in Fedora. 
This can be overridden when creating media programmatically, such as for 
[derivatives](derivatives.md). 

## Media belong to nodes

While in Drupal, media are said to be "reusable files" that can be 
referred to by any number of content items, in Islandora there is a
special relationship, the "Media Of" (`media_of`) field, that links 
media to a single node that describes that file and how it fits into the repository.
The field "Media Of" must be on all Islandora media types for media
of those types to behave as Islandora media. 

Then, the media "of" an Islandora node (if any) will appear in that 
node's "Media" tab. From this tab, there are also links 
to add new Media, individually or in batch. When media are created this way,
their "Media of" field is automatically populated with the current node. 

## Media have different uses

In a repository, a node may have several media that belong to it, which
represent the originally uploaded file, a smaller service file, a 
thumbnail, and perhaps a file transformed into a specific format for
preservation. All of these media can be distinguished using the 
Islandora-provided field, "Media Use" (`field_media_use`). This is a 
taxonomy reference field that points to the vocabulary, Islandora 
Media Use.

The Islandora module provides, through a migration, the following values:

| Media Use term           	| External URI          			|
|--------------------------	|--------------------------------------------	|
| Extracted Text           	| http://pcdm.org/use#ExtractedText          	|
| Intermediate File        	| http://pcdm.org/use#IntermediateFile       	|
| Original File            	| http://pcdm.org/use#OriginalFile           	|
| Preservation Master File 	| http://pcdm.org/use#PreservationMasterFile 	|
| Service File             	| http://pcdm.org/use#ServiceFile            	|
| Thumbnail Image          	| http://pcdm.org/use#ThumbnailImage         	|
| Transcript               	| http://pcdm.org/use#Transcript             	|

This migration is performed by all installation methods, so these values should
be available "out of the box". These values were provided by the PCDM data model
(see [RDF in Islandora](rdf.md)),
and not all values are associated with behaviours you might expect - for instance, 
Islandora is not configured out-of-the box to display a transcript if you add a 
Media (file) and tag it as "Transcript". See [Audio](../models/audio.md) and
[Video](../models/video.md) for setting up transcripts.

!!! note "Standard vs Multi-file media model"
    This describes the stadard file-media relationship in Islandora. 
    There is an alternative method of arranging files and their derivatives
    which we call the ["Multi-file media"](../user-documentation/media/#multi-file-media) method.



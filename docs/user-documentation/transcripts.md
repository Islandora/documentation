# Transcripts

Transcripts, captions, or subtitles may be added to audio and video media.
This functionality is available because of a field type, "Media track", 
provided by the Islandora module, and the "Audio with Captions" and "Video 
with Captions" field formatters from the Islandora Audio and Video modules, 
respectively. 

## Using Transcripts with the Islandora Starter Site

!!! note "Sandbox"
    On the public sandbox, or other sites using the Islandora Install Profile Demo,
    you will first need to make the "Track" field visible in the media form, at
    Structure > Media > Audio|Video > Manage Display.

To add transcripts (or subtitles, or captions) to a Repository Item,
navigate to the Service File media (or whichever one
will be playing), open the edit form, and in the "Track" field, add a WebVTT file 
containing the text you'd like displayed. Select "captions" or "subtitles" 
as a type, and set a label. The label will be displayed when selecting the 
caption/subtitle track. Save the media and refresh it, and you should see the text
display in the viewer (or above the viewer, in the case of audio.) The track text 
appears during the time specified in the WebVTT file. We have not yet implemented
a scrolling, clickable, or otherwise interactive transcript. 

Note that if you add the transcript file to the Original File, but you have configured
the site to play the Service File, then you will not see the transcript.

This feature was added to the 2.0 release of Islandora. Read the [captions changelog here](https://github.com/Islandora/documentation/blob/main/docs/release_notes/8.x-2.0.md#adding-captions).

!!! info "Languages"
    While you can add subtitles in different languages, you may only choose from
    the site's installed languages.

!!! tip "Types"
    The five options: **captions**; **subtitles**; **descriptions**; **chapters**; and **metadata** come from
    the [HTML standard's `<track>` element](https://html.spec.whatwg.org/multipage/media.html#the-track-element). 
    As per their definitions, captions and subtitles will be displayed as optional text over the video,
    available through the usual [cc] icon in the viewer controls. Descriptions, chapters, and metadata
    will not be displayed as they are intended for programmatic use.


## Tracing the components of this feature

| Feature Component | Source |
|---|---|
| Define Field type "Media track" | Islandora module |
| Add "Media track" fields to audio and video media types | Islandora Defaults |
| Define IslandoraFileMediaFormatterBase, which finds Media Track fields on media, and adds them as playable tracks | Islandora module | 
| Define "Audio with Captions" field formatter, extending IslandoraFileMediaFormatterBase | Islandora Audio |
| Define "Video with Captions" field formatter, extending IslandoraFileMediaFormatterBase | Islandora Video |
| Select these field formatters for the Default and Source display modes for audio and video media | Islandora Defaults |


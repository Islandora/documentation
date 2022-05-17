# Transcripts

Transcripts, captions, or subtitles may be added to audio and video media.
 Islandora provides a field type called "Media track" and Islandora Defaults puts 
a field of this type on the Audio and Video media types. Islandora Audio and Islandora Video
provide respective field formatters (viewers) that allow for the transcript track to be played
with the audio or video content. Islandora Defaults makes this field formatter the
selected formatter for the Default and Source display modes. 

To add transcripts (or subtitles, or captions) to a Repository Item 
using Islandora Defaults, navigate to the Service File media (or whichever one
will be playing), open the edit form, and add a WebVTT file containing the text
you'd like displayed. Select "captions" or "subtitles" as a type, and set a label.
The label will be displayed when selecting the caption/subtitle track.
Save the media and refresh it, and you should see the text
display in the viewer (or above, in the case of audio.)

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




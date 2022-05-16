# Transcripts

Transcripts, captions, or subtitles may be added to audio and video media.
 Islandora provides a field type called "Media track" and Islandora Defaults puts 
a field of this type on the Audio and Video media types. Islandora Audio and Islandora Video
provide respective field formatters (viewers) that allow for the transcript track to be played
with the audio or video content. Islandora Defaults makes this field formatter the
selected formatter for the Default and Source display modes. 

In short, to add transcripts (or subtitles, or captions) to a Repository Item 
using Islandora defaults, navigate to the Service File media (or whichever one
will be playing), open the edit form, and add a WebVTT file containing the text
you'd like displayed. Save the media and refresh it, and you should see the text
display in the viewer (or above, in the case of audio.)

Changelog: https://github.com/Islandora/documentation/blob/main/docs/release_notes/8.x-2.0.md#adding-captions 

Note: while you can add subtitles in different languages, you may only choose from
the site's installed languages.

Note: The five options, captions; subtitles; descriptions; chapters; and metadata come from
the [HTML standard's <track> element](https://html.spec.whatwg.org/multipage/media.html#the-track-element). 
As per their definitions, captions and subtitles will be displayed as optional text over the video,
available through the usual [cc] icon in the viewer controls. Descriptions, chapters, and metadata
 will not be displayed. 




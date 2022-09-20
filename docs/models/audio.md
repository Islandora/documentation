# Audio

## Media and File Types

Islandora Defaults uses the built-in Drupal "Audio" media type for video. Islandora Defaults configures it to accept files of type `mp3`, `wav`, and `aac`.

## Derivatives

Islandora uses Homarus (ffmpeg as a microservice) to create audio derivatives. Islandora Defaults sets you up to create:

* "Service file" from the original file, with parameters `-codec:a libmp3lame -q:a 5`, stored in the public filesystem

These parameters can be changed in the configuration for the Drupal Action that Islandora uses to generate an Audio derivative.

Islandora Defaults sets up a context to automatically create this derivative when:

* The Audio media is tagged with the "Original File" term (a term with External URI `http://pcdm.org/use#OriginalFile`)
* The media's parent node is tagged with the "Audio" model (a term with External URI `http://purl.org/coar/resource_type/c_18cc`)

The mimetype formats allowed by Homarus are configured in Homarus itself - see [Installing Crayfish](../installation/manual/installing_crayfish.md#homarus-audiovideo-derivatives)

## Display

Drupal provides an "Audio" field formatter for file fields that displays a simple playable audio widget. It works but does not support captions/subtitles. Islandora provides an "Audio with captions" formatter that allows for captions.

To use the captions feature out of the box, add the captions track as a WEBVTT file (`.vtt`) in the Audio media's "Track" field (see below regarding which Audio media to use). If you don't have the "Track" field (provided by Islandora Defaults), create a field of type "Media Track" (a type provided by Islandora) on the same Media (or more broadly, same entity) as your audio file.  Then use the Manage Display page to set your audio file field to render using the "Audio with captions" field formatter.

If you're using Islandora Defaults, you can expect to see an audio player on your node. This is done with a special view (an EVA view) that displays service files, which is configured to show on Repository Item's default display mode (on the "Manage Display" page). The EVA view renders the service file media using the "Source" view mode. For Audio media, this is configured to show only the audio file using the "Audio with Captions" widget. Note that captions, to be displayed, must be on the media that is playing. Thus, when the Service file media is being played, captions on the Original File media are ignored. 
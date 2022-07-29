# Video

## Media and file types

Islandora Defaults uses the built-in Drupal "Video" media type for videos. Islandora Defaults configures it to accept files of type `mp4`.

The Install Profile also makes available the "Remote video" media type, but its behaviour has not been publically documented.

## Derivatives

Islandora uses Homarus (ffmpeg as a microservice) to create video derivatives. Islandora Defaults sets you up to create:

* Service file with mimetype video/mp4 [yes, this is the same as the input, i'm not sure if it downsamples]
* Thumbnail with mimetype image/jpeg, derived from one second into the video and scaled down to ~100 px using the command `-ss 00:00:01.000 -frames 1 -vf scale=100:-2`. 

These parameters can be changed in the configuration for the Drupal Actions that Islandora uses to create video derivatives.

Islandora Defaults sets up a context to automatically create this derivative when:

* The Video media is tagged with the "Original File" term (a term with External URI `http://pcdm.org/use#OriginalFile`)
* The media's parent node is tagged with the "Video" model (a term with External URI `http://purl.org/coar/resource_type/c_18cc`)

The mimetype formats allowed by Homarus are configured in Homarus itself - see [Installing Crayfish](../installation/manual/installing_crayfish/#homarus-audiovideo-derivatives)

## Display

Drupal provides a "Video" field formatter for file fields that displays a simple playable video widget. It works but does not support captions/subtitles. Islandora provides a "Video with Captions" formatter that allows for captions.

To use the Captions feature out of the box, add the captions track as a  WEBVTT file (`.vtt`) in the Video media's "Track" field (see below regarding which Video media to use). If you don't have the "Track" field (provided by Islandora Defaults), create a field of type "Media Track" (a type provided by Islandora) on the same Media (or more broadly, same entity) as your audio file.  Then use the Manage Display page to set your audio file field to render using the "Audio with captions" field formatter.

If you're using Islandora Defaults, you can expect to see a video player on your node. This is done with a special view (an EVA view) that displays service files, which is configured to show on the default view of Repository Items (see the "Manage Display" page). The EVA view renders the service file media using the "Source" view mode. For Video media, this is configured to show only the video file using the "Video with Captions" formatter. Thus when the Service file media is being played, captions on the Original File media are ignored. 

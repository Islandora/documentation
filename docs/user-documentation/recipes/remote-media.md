# Remotely-hosted Media in Islandora

Two different ways to support remotely-hosted media, via oEmbed providers and HTML embed code.

## Ingredients

* To support embed code: [Entity Media Embed Code](https://www.drupal.org/project/entity_media_embed_code)
* To expand the oEmbed providers: 
    * [oEmbed Providers](https://www.drupal.org/project/oembed_providers)
	* New Media Sources, configured below
* New Media Types, configured below

## Instructions

### Embed code

The Embed Code media type allows any HTML media embedding (video, audio, etc.), as well as HTML5 complex items if properly written/configured.

1. Install and enable the [Entity Media Embed Code](https://www.drupal.org/project/entity_media_embed_code) module.
2. Create a new Media Type: "Embed Code"
    * Name: Embed Code
    * Description: HTML embed code to display remotely-hosted content.
    * Media source: Embed code
    * Field with source information:
        * If a field to contain embed code does not already exist (i.e. `field_media_embed_code`), choose "Create"
        * Otherwise, choose the appropriate existing field
3. Add and configure the appropriate fields:
    * "Insert embed code" should already be there
    * Re-use the existing "Media Of" and "Media Use" fields

### oEmbed remote endpoints

oEmbed allows supported remote content to be displayed using just a link. Out of the box, Islandora supports Vimeo and YouTube links via the Remote Video meda source. This recipe expands these options.

1. Install and enable the [oEmbed Providers](https://www.drupal.org/project/oembed_providers) module.
2. Configure the module:
    * Allow external fetch of providers from `https://oembed.com/providers.json`
    * Add custom providers if yours is not in the default list
    * Create a Provider Bucket, selecting all the providers you wish to make available
3. Create a new Media Type: "oEmbed Remote Media"
    * Name: oEmbed Remote Media
    * Description: Remotely-hosted media from an oEmbed provider
    * Media source: oEmbed Providers
4. Add and configure the appropriate fields:
    * New field: 
        * "oEmbed Providers URL" (`field_media_oembed_video`)
        * Type: text (plain)
    * Re-use the existing "Media Of" and "Media Use" fields
    
### Adding media

When creating your Repository Items and adding media, select the media type of your choice (Embed Code, or oEmbed Remote Media). Fill in the fields as appropriate.

* If entering Embed Code, acquire the appropriate embed code from your source, and paste the full HTML into the embed code field.
* If entering an oEmbed link, just paste the link into the URL field.

Under "Media Use", choose **Service File**. This will allow your media to be displayed on the Repository Item node page.

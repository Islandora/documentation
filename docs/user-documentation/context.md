## Context in Islandora 8

[Context](https://www.drupal.org/project/context) is a Drupal contrib module that allows configuration of "if this, then that" logic using an administrative user interface. Context defines "conditions" and "reactions" to enact the "if this [condition], then that [reaction]" logic. Some Islandora 7.x repositories use the community-contributed [Islandora Context](https://github.com/SFULibrary/islandora_context) module to apply this sort of logic to Islandora objects. An example Context from Islandora 7.x is

> If an object's namespace is 'customnamespace', render the block containing the rights statement "Please contact Special Collections before reusing this item".

The Context project page on drupal.org says "You can think of each context as representing a 'section' of your site", but that doesn't tell the whole story, since it gives the impression that Context is all about defining subsites. Context in Drupal 8 is much more powerful in Drupal 8 than in Drupal 7 due to how it interacts with the rest of Drupal (specifically, through Drupal 8's new plugin API). Because of this increased flexibility and power, and because Context provides a ready-made user interface for site administrators to combine conditions and reactions, Islandora 8 uses Context to drive the logic underlying many important aspects of Islandora, including which derivatives get generated and how objects are displayed. In Islandora 7, many of these things were managed (and often hard-coded) within solution packs and utility modules.

Let's look at the example of how Context can determine how an object is diplayed. Drupa 8 has the idea of "view modes", which allow site builders to choose what happens when an object is viewed by the user (it has nothing to do with Drupal Views). In the node edit form for Islandora objects, there is a checkbox that, if checked, tells Drupal to render the image using the OpenSeadragon viewer:

![Display Hints field in node edit form](../assets/context_display_hints.png)

 This  functionality is accomplished via the "Open Seadragon" Context, which, as its configuration suggests, checks as its condition whether the node as the "Open Seadragon" tag and if so, reacts by using the view mode "Open Seadragon":

![Open Seadragon Context configuration](../assets/context_openseadragon_configuration.png)

One important group of functionality in Islandora 8 repositories that admins might want to control is what types of [media](datastreams.md) get persisted to Fedora. Unlike in Islandora 7.x, Islandora 8 allows administrators to choose what types of media get persisted to Fedora. For example, you may only want the "Preservation Master" and "Original File" media persisted to Feodra, but not "Thumbnails". This is dertermined using Contexts.

[add the example of not persisting thumbnails to Fedora here]

Most Islandora 8 repository administrators will not need to alter or configure the Contexts that perform these operations. But since much of Islandora 8's underlying functionality is governed by Context, administrators should be comfortable using it to customize their repositories.

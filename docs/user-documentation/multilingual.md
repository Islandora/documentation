Islandora 8 enables you to build full fledged multilingual repositories leveraging the multilingual support provided by Drupal core modules. The multilingual content gets indexed into Fedora repository as well as the Triplestore (blazegraph), and can be queried using the SPARQL endpoint. In this guide, we will describe the steps needed to setup a basic multilingual Islandora 8 site.  

## Drupal 8 Configurations
Drupal 8 comes with four modules for [multilingual support](https://www.drupal.org/docs/8/multilingual/choosing-and-installing-multilingual-modules). Islandora 8 enables Language and Content Translation modules by default.

### Adding Languages
From the top menu, go to Configuration >> Regional and language >> Languages (`http://localhost:8000/admin/config/regional/language`). Add a language. 

You can place the default language selector block to switch between languages. To create the language switcher block go to Structure >> Block layout. Click Place block in a region of your choice.  Search for `Language switcher` block and click `Place block`.

### Adding Multilingual Menus
From the top menu, go to Configuration >> Regional and language >> Content language and translation. Check `Custom menu link` under `Custom language settings`. Scroll down to `Custom menu link` section and check all the relevant fields and Save the configurations. Clear the cache (Configuration >> Development >> Performance).  

From the top menu, go to Structure >> Menu. Edit "Main navigation" menu. Default home menu item cannot be translated due to [this issue](https://www.drupal.org/project/drupal/issues/2838106). Disable that menu item. Click `Add link` to create a new menu item. Provide a menu title (i.e Home) and input `<front>` for the link field. Save. Right click on the Operations beside the new menu link and click the Translate button. Translate the menu link title for the language added above and save.

Go back to home. The language switcher will enable you to switch the language/content of the menu and content.

### Adding a Multilingual Repository Item
From the top menu, go to Content >> Add content >> Repository item. Provide the required fields and save the object. Click the Translate tab of the object, provide a title in the second language and fill any translatable fields (i.e description). Add the media for the object. Media object can be translated similar to the repository item node.

Go back to home, you should be able to view content in the language selected in the language switcher.  

### Field Label Translations
If you need the field labels of the repository Item displayed in a different language, additional configuration is needed. `Configuration translation` module in the core needs to be enabled. Note that this will enable User Interface translation as well. 

Each field label needs to be translated. Alternatively, you can import existing translations. For instance, go to `Interface translation import` at `http://localhost:8000/admin/config/regional/translate/import`, select islandora translations from `https://github.com/Islandora-CLAW/islandora_defaults/tree/8.x-1.x/translations`, set the ` Treat imported strings as custom translations` option and import. This will provide translation for Islandora 8 repository item field labels. Cache should be cleared to see the changes. An example second language display is shown below. 

![Multilingual Content Representation in Drupal](../assets/multilingual_repository_item_in_drupal.png)

## Fedora Representation
Islandora 8 indexes the multilingual values using language tags. An example representation is shown below.

![Multilingual Content Representation in Fedora](../assets/multilingual_repository_item_in_fedora.png)

## Triplestore Representation
Islandora 8 indexes the multilingual content using language tags to Fedora repository. An example representation is shown below. 

![Multilingual Content Representation in Triplestore](../assets/multilingual_repository_item_in_triplestore.png)

You can query the result and filter the results by a specific language. For example, to get all titles and filter by language, the following query can be used:

```
PREFIX dcterm: <http://purl.org/dc/terms/>
select ?s ?o {
  ?s dcterm:title ?o
FILTER (lang(?o) = 'ta')     
}
```
## Further Reading
* [Multilingual guide](https://www.drupal.org/docs/8/multilingual)

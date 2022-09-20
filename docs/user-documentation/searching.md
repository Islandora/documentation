# Setup and Configure Search

Islandora comes with the Drupal 8 [Search API](https://www.drupal.org/project/search_api) and [SOLR](https://www.drupal.org/project/search_api_solr) modules enabled with a corresponding SOLR instance. This guide gives an overview to the setup provided by the islandora-playbook. Much more detail is available in the [Search API documentation](https://www.drupal.org/docs/8/modules/search-api). Another helpful resource is [Adam Fuch's "Drupal 8 Custom Site Search with Search API"](https://www.electriccitizen.com/blog/drupal-8-custom-site-search-search-api) (https://www.electriccitizen.com, 2018-01-10; last accessed 2019-03-08).

## Indexing Islandora with SOLR

To access the search indexing settings, log in as an administrator and navigate to  '/admin/config/search/search-api' or click **Configuration** and then **Search API**.

![Screenshot of the search-api configuration page.](../assets/search-settings-page.png)

### SOLR Server Configuration

On the _Search API_ page, Use the **Solr Server** link to view the SOLR server's configuration and the **Default Solr content index** link to view or update the index settings. In most cases, where the site was built using Ansible, the _Solr Server_ settings that were configured during installation should be left alone.

### SOLR Index Configuration

The _Default Solr content index_ user interface is divided into four tabs: **View**, **Edit**, **Fields**, and **Processors**.

#### View Tab

The 'View' tab gives an overview of the index and its status:

- _Index status_ is a progress bar that indicates how many items have been indexed.
- _Status_ allows you to enable/disable the index from being used.
- _Datasource_ indicates what types of entities are being indexed and how many have.
- _Tracker_ states which item tracker is being used.
- _Server_ provides a link to the SOLR server's configuration page.
- _Server index status_ is a count of the total number of items returned from an unfiltered empty search. Generally speaking, this should match the total number of items indexed. See the [Drupal 8 Search API FAQ](https://www.drupal.org/docs/8/modules/search-api/getting-started/frequently-asked-questions#server-index-status) for more details.
- _Cron batch size_ displays how many items will be indexed when Drupal's cron runs.

The _View_ tab also provides links to some common actions. **Start Indexing Now** allows you to start an indexing job for a specified number of items (default is 'all'). You can also specify how many items should be indexed in each batch (default is '50'). The other links allow a repository manager to queue all objects for reindexing, clear the index, or rebuild tracking information.

#### Edit Tab

The _Edit_ tab allows repository managers to configure how the index works as a whole, including the Index name, the data sources — entity types — it can index (including which specific content types or taxonomies will be indexed), which server it is connected to, and other SOLR-specific options.

_Content_ (types) is the only data source enabled by default. Selecting _Taxonomy term_ will enable searching taxonomies which is recommended if the repository uses taxonomies for subjects or other discovery points. Once the data sources are enabled a configuration box for each of them will appear in a section just below the list of data sources. This allows repository managers to select which content types (or taxonomy vocabularies) will be included in the index. By default, all the content types, and vocabularies if the taxonomy data source is enabled, are indexed.


!!! note "Defaults"
    The defaults assume a repository is adding content using the web interface. If a repository manager plans on bulk-loading content they should disable the **Index items immediately** option in the expandable _Index Options_ box and increase the 'Cron batch size' option.

#### Fields Tab

The _Fields_ tab allows repository managers to select which fields will be indexed. The default set of fields enabled come from a standard Drupal installation and do not reflect the fields Islandora adds for 'Repository Item'. **Repository managers need to add the fields necessary for their Islandora instance.**

To add a field, click the **+ Add fields** button. A shadow-box will appear with a list of the fields available for the index.

Some fields, such as the _Body_ ('body') field provided by Drupal, have multiple properties which can be completely different values or variations on the same value. Click on the plus-sign next to the field to show the properties available to index. In most cases repository managers can ignore the properties list and click the **Add** button by the field to index the default property ('value'). Only select a different field property if you understand how it will impact user searching. Entity reference fields, such as Tags ('field_tags'), allow you to select fields or their properties from the referenced entity for indexing, such as a referenced taxonomy term's name field.

Once the fields are added they can be configured further on the _Fields_ tab, although the label, machine name, and type usually don't need to be changed. The 'Type' dropdown has several Full-text processing options available, which may be of interest. Each is described in the expandable _Data Types_ box at the bottom of the page. The _Boost_ setting allows repository managers to increase the weight of particular fields when calculating search relevancy.

#### Processors Tab

The _Processors_ tab allows repository managers to adjust how data and search queries are processed to adjust results. The defaults are acceptable in most cases.

## Indexing EDTF Dates

EDTF date fields can only be successfully indexed directly as strings. However, there is a way to include a custom "edtf year" field containing the facets-friendly year (or year ranges) from one or more EDTF fields. This must first be enabled by checking "EDTF Year" under the Processors tab, then can be added on the Field tab (it'll be called "EDTF Creation Date Year (edtf_year)"), and finally can be configured as a facet. See "How should this be tested" on [this pull request](https://github.com/Islandora/controlled_access_terms/pull/68) for instructions on setting it up.

## Searching Islandora

Searching using Search API in Drupal is done using Drupal Views. The Islandora Defaults module comes with a search page pre-configured (accessible at '/solr-search/content'). To edit the search page, navigate to '/admin/structure/views/view/solr_search_content'.

Repository managers may want to change the URL used to access the page, add it to the site navigation, or add a search box. In the 'Page Settings' box in the middle of the page, click on the existing path to open a shadow-box with an edit field. Change the URL as desired, for example, to 'search' and click **Apply**. Then, click the **No menu** link just below it to open the menu settings shadow-box. Selecting 'Normal menu entry' will allow a repository manager to add a menu link text, description, and place it within the site menu tree (the default, `<Main navigation>` works for most sites). A search box can be added by expanding the _Advanced_ options and changing the _Exposed form in block_ setting and then use the _Block Layout_ interface (found at '/admin/structure/block') to place the block where desired. After making changes to the View's settings, click the **Save** button to ensure the changes are not lost.

Islandora's Repository Items are displayed in the search results as a fully rendered entity by default. Repository managers can choose which view mode should be used for each search datasource by clicking the **Settings** link next to the _Show:_ setting under the _Format_ section of the search view configuration page (shown in a red box in the screenshot below). The _Teaser_ and _Search result highlighting input_ are the two most likely options. Alternatively, repository managers can select specific fields to display instead by clicking the **Rendered Entity** link and changing it to _Fields_ and then choosing which fields will be displayed in the _Fields_ section underneath.

!!! note "Thumbnails"
    thumbnails will not immediately be available using the _Fields_ display option without more advanced configurations.

![Screenshot of the default SOLR search view settings page with the format's type settings links highlighted. ](../assets/search-view-format-settings-highlighted.png)
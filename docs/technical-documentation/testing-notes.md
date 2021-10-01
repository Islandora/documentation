## How to find things in Fedora?

For any Drupal URI, the corresponding Fedora URIs is computed in a service called Milliner. In Drupal, a "pseudo-field" is available that will display the corresponding Fedora URI on the page of a node, taxonomy term, or media object. The presence of this URI does NOT guarantee the existence of the corresponding entity in Fedora. 

### Enabling the Fedora URI Pseudo-field

To display the Fedora URI pseudo-field on a Drupal node, media, or taxonomy term, go to Manage > Configuration > Islandora and select all the bundles for which you would like the Fedora URI displayed. Once you have selected the bundles, and cleared the cache, the new pseudo-field will appear at the bottom of _all display modes_. You can alter where in the display the Fedora URI field appears, by going to the "Manage Display" page for the bundle. For example, for a Repository Item, you'd go to Manage > Structure > Content Types, and under the dropdown for "Repository Item, select "Manage Display". In that list you should see `Fedora URI` which you can move around (or hide) as desired. This will need to be repeated in each Display mode (tab). Clearing cache may be necessary to refresh the node display.

Note: This information used to be stored in a service called Gemini, which kept track of corresponding minted Fedora URIs and their minted equivalents. Gemini was removed for Islandora 2.0.0.


## How do I search for a object in the Solr?
* Go to `http://localhost:8983/solr/#/islandora/query`
* Issue a Solr query.

Example
```
ss_search_api_id:"entity:node/4:en"
```

## Sample Triplestore queries
* Go to `http://localhost:8080/bigdata/#query`
* Under namespaces (`http://localhost:8080/bigdata/#namespaces`), make sure `islandora` is selected.  

### Find all triples with given object as the subject
```
select ?p ?o  where { <drupal_url> ?p ?o }
```

Example:

```
select ?p ?o  where { <http://localhost:8000/media/8?_format=jsonld> ?p ?o }
```

### Getting objects in a collection
```
select ?s where { ?s <http://pcdm.org/models#memberOf> <drual_url_of_the_collection?_format=jsonld> }
```

Example:

```
select ?s where { ?s <http://pcdm.org/models#memberOf> <http://localhost:8000/node/7?_format=jsonld> }
```

### Find all media/files belonging to a node

```
select ?s where { ?s <http://pcdm.org/models#fileOf> <drupal_url_of_the_object?_format=jsonld> }
```

Example:

```
select ?s where { ?s <http://pcdm.org/models#fileOf> <http://localhost:8000/node/4?_format=jsonld> }
```

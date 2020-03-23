## How to find things in Fedora?

The map between Drupal URIs and their corresponding Fedora URIs is stored in a service called Gemini. An Islandora "pseudo-field" can be enabled that will display the corresponding Fedora URI on the page of a node, taxonomy term, or media object.  You can also query Gemini directly. Instructions for doing that are provided below.

For more information about the Gemini service, see the [Gemini README](https://github.com/Islandora-CLAW/Crayfish/tree/master/Gemini).

## Fedora URI Pseudo-field

To display the Fedora URI  pseudo-field on a Drupal node, media, or taxonomy term, go to Manage > Configuration > Islandora and select all the bundles for which you would like the Fedora URI displayed. A valid Gemini URL is required. On the [playbook](https://github.com/Islandora-Devops/claw-playbook), Gemini is at http://localhost:8080/gemini. Once you have selected the bundles, and cleared the cache, the new pseudo-field will appear at the bottom of all display modes. You can alter where in the display the Fedora URI field appears, by going to the "Manage Display" page for the bundle. For example, for a Repository Item, you'd go to http://localhost:8000/admin/structure/types/manage/islandora_object/display. In that list you should see `Fedora URI` which you can move around (or hide) as desired. Clearing cache may be necessary to refresh the node display.


## How to look up a Fedora URI through Gemini (nodes and taxonomy terms)

In Gemini, each entry is keyed on the UUID assigned by Drupal (the long one, not the node id). To get the UUID of a node, or taxonomy term in Drupal, you can look in the Devel tab or in the JSON representation, e.g.: `http://localhost:8000/node/2?_format=json`.  

Then, query the Gemini REST service (using a REST client such as POSTman, or a command-line tool such as `curl`) for that UUID: 

```
curl -H "Authorization:Bearer islandora" http://localhost:8000/gemini/[uuid_value]
```

Alternatively, you can login to SQL, choose the gemini database, and issue a query. With the playbook's default setup:

`
vagrant ssh
mysql -uroot -pislandora gemini
select fedora_uri from Gemini where drupal_uri = 'http://localhost:8000/node/[nid]?_format=jsonld';
`

## How to look up a Fedora URI through Gemini (files)?
Files in Fedora have the file URI in Gemini, and can be queried based on the UUID (accessible from the Media's json) or their URI.

`
select fedora_uri from Gemini where drupal_uri = 'http://localhost:8000/_flysystem/fedora/2019-05/639647.jpg';
`

## Media and Gemini
Media objects aren't indexed in Gemini. Their Fedora counterparts are located at the Fedora URI of the file, appended with `/fcr:metadata`. This is an RDF document in Fedora that describes _the binary file itself_, and it contains all the information describing the file that is present in the fields of the Media. 

When you enable the pseudo-field on a media object, you see the URI of its Fedora counterpart, `file_uri + /fcr:metadata`,  constructed using the Gemini mapping of its attached file.  


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

## How to look up a Gemini entry for a node?

Gemini is a service that maps Drupal URIs to their corresponding Fedora URIs. Each entry is keyed on the UUID assigned by Drupal. 

To get the UUID of a node, taxonomy term, or media entity (see Note below) in Drupal, you can look in the Devel tab or in the JSON representation, e.g.: `http://localhost:8000/node/2?_format=json`.  

Then, you can look up the entry for that UUID using the Gemini service. It's a REST interface, so you can use a REST client such as POSTman, or a command-line tool such as `curl`: 

```
curl -H "Authorization:Bearer islandora" http://localhost:8000/gemini/[uuid_value]
```

Alternatively, you can login to your SQL database, choose the gemini database, and issue the following query:

`
select * from Gemini where uuid="uuid_value";
`

Note: Media objects for files in Fedora aren't indexed in Gemini because they are not reliable. See [Github Issue #1079](https://github.com/Islandora-CLAW/CLAW/issues/1079).

## How to look up a file metadata in Fedora?
Files in Fedora have the file URI in Gemini. However, Media for files in Fedora aren't indexed in Gemini because they are not reliable. To look up file metadata, go to `file_uri + /fcr:metadata`.


## How do I search for a object in the Solr?
* Go to `http://localhost:8983/solr/#/CLAW/query`
* Issue a solr query.

Example
```
ss_search_api_id:"entity:node/4:en"
```

## Sample Tripestore queries
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

Files in Fedora have their file URIs in Gemini. To see file metadata in Fedora, go to `file_uri + /fcr:metadata`. 

For more information about the Gemini service, see the [Gemini README](https://github.com/Islandora-CLAW/Crayfish/tree/master/Gemini).

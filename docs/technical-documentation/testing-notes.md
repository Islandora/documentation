## How to look up a [Gemini](https://github.com/Islandora-CLAW/Crayfish/tree/master/Gemini) entry for a node?
To locate a particular Gemini entry for a node, you need to know the uuid of the node. You can get the uuid for the node by looking at the devel tab in Drupal or you can get it from json representation: `http://localhost:8000/node/2?_format=json`.  

You can look up Gemini entries for node  using the Gemini service via curl or a REST client such as POSTman. 

```
curl -H "Authorization:Bearer islandora" http://localhost:8000/gemini/uuid_value
```

Alternatively, you can login to mysql database, select the gemini database and issue the following query:
`
select * from Gemini where uuid="uuid_value";
`

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



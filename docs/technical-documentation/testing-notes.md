## How to look up a Gemini entry for a node?
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

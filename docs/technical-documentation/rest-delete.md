# Removing resources, media and files - DELETE Requests 

Deleting is as easy as [getting](./rest-get.md) resources and more difficult than [creating](./rest-create.md) resources.

If you can perform a GET request then you have the information required to perform a DELETE request.

Check out the examples below.

* [Authorization](#authorization)
* [Content](#content-nodes)
* [Media](#media)
* [Files](#files)

## Authorization

If you have restricted access to view your content, you will need to use one of the configured authorization methods to access your content, media and/or files.

These are defined under [Authorization](./using-rest-endpoints.md#authorization) on the overview.

## Content (Nodes)

You will need your _node id_, you can find more information in the [GET](./rest-get.md#content-nodes) documentation.

A delete is simply the same request as a `GET` but sending a `DELETE` http verb.

Our example node has a _node id_ of 2

```
> curl -i -u admin:islandora -X GET 'http://localhost:8000/node/2?_format=json'

HTTP/1.1 200 OK
Date: Fri, 15 Mar 2019 15:02:00 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
Link: <http://localhost:8000/node/2>; rel="canonical"
Link: <http://localhost:8000/node/2/delete>; rel="https://drupal.org/link-relations/delete-form"
Link: <http://localhost:8000/admin/content/node/delete?node=2>; rel="https://drupal.org/link-relations/delete-multiple-form"
Link: <http://localhost:8000/node/2/edit>; rel="edit-form"
Link: <http://localhost:8000/node/2/revisions>; rel="version-history"
Link: <http://localhost:8000/node/2>; rel="https://drupal.org/link-relations/revision"
Link: <http://localhost:8000/node?node=2>; rel="https://drupal.org/link-relations/create"
Link: <http://purl.org/dc/dcmitype/Collection>; rel="tag"; title="Collection"
Link: <http://localhost:8000/node/2?_format=jsonld>; rel="alternate"; type="application/ld+json"
X-Drupal-Dynamic-Cache: MISS
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 1564
Content-Type: application/json

{"nid":[{"value":2}],"uuid":[{"value":"413135a6-0bd1-4d6b-8bcb-059cf7784d83"}],"vid":[{"value":2}],"langcode":[{"value":"en"}],"type":[{"target_id":"islandora_object","target_type":"node_type","target_uuid":"62189bec-3ef3-4196-b847-b17e5ce61fd5"}],"revision_timestamp":[{"value":"2019-03-05T18:04:43+00:00","format":"Y-m-d\\TH:i:sP"}],"revision_uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"revision_log":[],"status":[{"value":true}],"title":[{"value":"Created a collection with POST"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"created":[{"value":"2019-03-05T18:04:43+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-05T18:04:43+00:00","format":"Y-m-d\\TH:i:sP"}],"promote":[{"value":true}],"sticky":[{"value":false}],"default_langcode":[{"value":true}],"revision_translation_affected":[{"value":true}],"content_translation_source":[{"value":"und"}],"content_translation_outdated":[{"value":false}],"field_access_terms":[],"field_alternative_title":[],"field_description":[],"field_display_hints":[],"field_edtf_date":[],"field_edtf_date_created":[],"field_edtf_date_issued":[],"field_extent":[{"value":"1 item"}],"field_identifier":[],"field_linked_agent":[],"field_member_of":[],"field_model":[{"target_id":23,"target_type":"taxonomy_term","target_uuid":"6a3b293d-4617-417b-99d2-23d75b57f7c2","url":"\/taxonomy\/term\/23"}],"field_pid":[],"field_resource_type":[],"field_rights":[],"field_subject":[]}%
```

Then we switch `GET` to `DELETE`

```
> curl -i -u admin:islandora -X DELETE 'http://localhost:8000/node/2?_format=json'

HTTP/1.1 204 No Content
Date: Fri, 15 Mar 2019 15:02:30 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Type: text/html; charset=UTF-8
```

All subsequent requests to the above URI will return a `404 Not Found` status code.

```
> curl -i -u admin:islandora -X GET 'http://localhost:8000/node/2?_format=json'

HTTP/1.1 404 Not Found
Date: Fri, 15 Mar 2019 15:12:58 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 149
Content-Type: application/json

{"message":"The \u0022node\u0022 parameter was not converted for the path \u0022\/node\/{node}\u0022 (route name: \u0022rest.entity.node.GET\u0022)"}
```

## Media

You will need a media id as used in [GET](./rest-get.md#media) documentation.

A delete is simply the same request as a `GET` but sending a `DELETE` http verb.

With a media id of 1 for our example, I'll perform a `GET`

```
> curl -i -u admin:islandora -X GET 'http://localhost:8000/media/1?_format=json' 

HTTP/1.1 200 OK
Date: Fri, 15 Mar 2019 14:53:54 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
Link: <http://localhost:8000/media/add>; rel="https://drupal.org/link-relations/add-page"
Link: <http://localhost:8000/media/add/image>; rel="https://drupal.org/link-relations/add-form"
Link: <http://localhost:8000/media/1>; rel="canonical"
Link: <http://localhost:8000/admin/content/media>; rel="collection"
Link: <http://localhost:8000/media/1/delete>; rel="https://drupal.org/link-relations/delete-form"
Link: <http://localhost:8000/media/delete?media=1>; rel="https://drupal.org/link-relations/delete-multiple-form"
Link: <http://localhost:8000/media/1/edit>; rel="edit-form"
Link: <http://localhost:8000/media/1>; rel="https://drupal.org/link-relations/revision"
Link: <http://localhost:8000/node/1>; rel="related"; title="Media of"
Link: <http://pcdm.org/use#OriginalFile>; rel="tag"; title="Original File"
Link: <http://localhost:8000/media/1?_format=jsonld>; rel="alternate"; type="application/ld+json"
Link: <http://localhost:8000/media/1/source>; rel="edit-media"
Link: <http://localhost:8000/_flysystem/fedora/2019-03/Louis_Riel.jpg>; rel="describes"; type="image/jpeg"
X-Drupal-Dynamic-Cache: HIT
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 1937
Content-Type: application/json

{"mid":[{"value":1}],"uuid":[{"value":"d8893926-ddb7-4125-b2da-30428af0fe3d"}],"vid":[{"value":1}],"langcode":[{"value":"en"}],"bundle":[{"target_id":"image","target_type":"media_type","target_uuid":"3860e653-201b-4509-89dd-628c446d81cb"}],"revision_created":[{"value":"2019-03-01T19:43:46+00:00","format":"Y-m-d\\TH:i:sP"}],"revision_user":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"revision_log_message":[],"status":[{"value":true}],"name":[{"value":"An image"}],"thumbnail":[{"target_id":2,"alt":"A portrait of Louis Riel","title":null,"width":800,"height":1333,"target_type":"file","target_uuid":"b0625129-c592-463a-93c3-3eff7cd3567e","url":"http:\/\/localhost:8000\/_flysystem\/fedora\/2019-03\/Louis_Riel.jpg"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"created":[{"value":"2019-03-01T19:43:22+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-01T19:43:46+00:00","format":"Y-m-d\\TH:i:sP"}],"default_langcode":[{"value":true}],"revision_translation_affected":[{"value":true}],"content_translation_source":[{"value":"und"}],"content_translation_outdated":[{"value":false}],"field_access_terms":[],"field_file_size":[{"value":166613}],"field_height":[{"value":1333}],"field_media_image":[{"target_id":2,"alt":"A portrait of Louis Riel","title":"","width":800,"height":1333,"target_type":"file","target_uuid":"b0625129-c592-463a-93c3-3eff7cd3567e","url":"http:\/\/localhost:8000\/_flysystem\/fedora\/2019-03\/Louis_Riel.jpg"}],"field_media_of":[{"target_id":1,"target_type":"node","target_uuid":"8322e36e-f8ec-4fd9-919d-52aed7b17a52","url":"\/node\/1"}],"field_media_use":[{"target_id":16,"target_type":"taxonomy_term","target_uuid":"08e01ff9-eb72-42f5-ae3a-8b21ba0c0bc3","url":"\/taxonomy\/term\/16"}],"field_mime_type":[{"value":"image\/jpeg"}],"field_width":[{"value":800}]}
```

Then we replace `GET` with `DELETE`.

```
> curl -i -u admin:islandora -X DELETE 'http://localhost:8000/media/1?_format=json'

HTTP/1.1 204 No Content
Date: Fri, 15 Mar 2019 14:54:55 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Type: text/html; charset=UTF-8
```

Subsequent requests to the media return `404 Not Found` statuses.

## Files

You'll need the file id, there is more information at the top of the [GET requests for files](./rest-get.md#files) documentation.

A delete is simply the same request as a `GET` but sending a `DELETE` http verb.

With a file id of 2 for our example, I can perform a test `GET`

```
> curl -i -u admin:islandora 'http://localhost:8000/entity/file/2?_format=json'

HTTP/1.1 200 OK
Date: Fri, 15 Mar 2019 14:40:40 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
X-Drupal-Dynamic-Cache: MISS
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 617
Content-Type: application/json

{"fid":[{"value":2}],"uuid":[{"value":"b0625129-c592-463a-93c3-3eff7cd3567e"}],"langcode":[{"value":"en"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"filename":[{"value":"Louis_Riel.jpg"}],"uri":[{"value":"fedora:\/\/2019-03\/Louis_Riel.jpg","url":"\/_flysystem\/fedora\/2019-03\/Louis_Riel.jpg"}],"filemime":[{"value":"image\/jpeg"}],"filesize":[{"value":166613}],"status":[{"value":true}],"created":[{"value":"2019-03-01T19:43:35+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-01T19:43:46+00:00","format":"Y-m-d\\TH:i:sP"}]}
```

If this is the correct file, I can delete it.

```
> curl -i -u admin:islandora -X DELETE 'http://localhost:8000/entity/file/2?_format=json'

HTTP/1.1 204 No Content
Date: Fri, 15 Mar 2019 14:43:22 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Type: text/html; charset=UTF-8
```

Subsequent requests to the URI now return a `404 Not Found` status.

```
> curl -i -u admin:islandora 'http://localhost:8000/entity/file/2?_format=json'

HTTP/1.1 404 Not Found
Date: Fri, 15 Mar 2019 14:43:33 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 157
Content-Type: application/json

{"message":"The \u0022file\u0022 parameter was not converted for the path \u0022\/entity\/file\/{file}\u0022 (route name: \u0022rest.entity.file.GET\u0022)"}
```

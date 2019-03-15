# Updating resources - PATCH request

PATCH requests allow you to update resources in place via a REST call. In this case you send a few required fields and then any changed fields. PATCH requests are available for nodes and media entities, but _not_ for files. PATCH requests are very consistent between node and media entities so we will just show one set of examples here.

Our example node is at `http://localhost:8000/node/3`.

If you perform a [GET](./rest-get.md) against another node or media you can see all of the fields, some of these are calculated (change based on others, ie. "changed"), some are references to other entities (nodes, media, files in Drupal. ie. "field_model") and others are pure text fields (ie. "title").

The fields will be different between different node types and media types, but the syntax to update them is the same.

* [Authorization](#authorization)
* [Change a text field](#change-a-text-field)
* [Change an entity reference](#change-an-entity-reference-field)

## Authorization

If you have restricted access to view your content, you will need to use one of the configured authorization methods to access your content, media and/or files.

These are defined under [Authorization](./using-rest-endpoints.md#authorization) on the overview.

This with assume you have already created a [node](./rest-create.md#content-nodes) or [media](./rest-create.md#files-and-media) at some location.

## Change a text field

To change the title of a node, you need to supply the new title and the node_type. The node_type is required as this defines what fields are available to the node.

A successful PATCH request will return a 200 status code and the body will contain the newly updated body.

```
curl -i -u admin:islandora -H"Content-type: application/json" -X PATCH -d '{ "type": [{"target_id": "islandora_object"}], "title": [{"value":"Updated with a PATCH request"}]}' 'http://localhost:8000/node/3?_format=json'

HTTP/1.1 200 OK
Date: Mon, 11 Mar 2019 17:01:23 GMT
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
Content-Length: 1562
Content-Type: application/json

{"nid":[{"value":3}],"uuid":[{"value":"3f618cdc-3f2a-4e77-b932-9ff1d461a57a"}],"vid":[{"value":3}],"langcode":[{"value":"en"}],"type":[{"target_id":"islandora_object","target_type":"node_type","target_uuid":"62189bec-3ef3-4196-b847-b17e5ce61fd5"}],"revision_timestamp":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"revision_uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"revision_log":[],"status":[{"value":true}],"title":[{"value":"Updated with a PATCH request"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"created":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-11T17:01:23+00:00","format":"Y-m-d\\TH:i:sP"}],"promote":[{"value":true}],"sticky":[{"value":false}],"default_langcode":[{"value":true}],"revision_translation_affected":[{"value":true}],"content_translation_source":[{"value":"und"}],"content_translation_outdated":[{"value":false}],"field_access_terms":[],"field_alternative_title":[],"field_description":[],"field_display_hints":[],"field_edtf_date":[],"field_edtf_date_created":[],"field_edtf_date_issued":[],"field_extent":[{"value":"1 item"}],"field_identifier":[],"field_linked_agent":[],"field_member_of":[],"field_model":[{"target_id":23,"target_type":"taxonomy_term","target_uuid":"6a3b293d-4617-417b-99d2-23d75b57f7c2","url":"\/taxonomy\/term\/23"}],"field_pid":[],"field_resource_type":[],"field_rights":[],"field_subject":[]}
```

## Change an entity reference field

This example is how to change a field that references some other entity. For this example we will use the `field_model` field, this is a reference to the taxonomy term that holds the "model" of the resource (ie. Image, Collection, Audio, Video, etc)

On our example installation taxonomy term 22 is "Binary", so to change a node from what it was to a Binary you would do.

```
> curl -i -u admin:islandora -H"Content-type: application/json" -X PATCH -d '{ "type": [{"target_id": "islandora_object"}], "field_model": [{"target_id": 22, "target_type": "taxonomy_term"}]}' 'http://localhost:8000/node/3?_format=json'

HTTP/1.1 200 OK
Date: Mon, 11 Mar 2019 17:51:47 GMT
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
Content-Length: 1562
Content-Type: application/json

{"nid":[{"value":3}],"uuid":[{"value":"3f618cdc-3f2a-4e77-b932-9ff1d461a57a"}],"vid":[{"value":4}],"langcode":[{"value":"en"}],"type":[{"target_id":"islandora_object","target_type":"node_type","target_uuid":"62189bec-3ef3-4196-b847-b17e5ce61fd5"}],"revision_timestamp":[{"value":"2019-03-11T17:36:10+00:00","format":"Y-m-d\\TH:i:sP"}],"revision_uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"revision_log":[],"status":[{"value":true}],"title":[{"value":"Updated with a PATCH request"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"created":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-11T17:51:47+00:00","format":"Y-m-d\\TH:i:sP"}],"promote":[{"value":true}],"sticky":[{"value":false}],"default_langcode":[{"value":true}],"revision_translation_affected":[{"value":true}],"content_translation_source":[{"value":"und"}],"content_translation_outdated":[{"value":false}],"field_access_terms":[],"field_alternative_title":[],"field_description":[],"field_display_hints":[],"field_edtf_date":[],"field_edtf_date_created":[],"field_edtf_date_issued":[],"field_extent":[{"value":"1 item"}],"field_identifier":[],"field_linked_agent":[],"field_member_of":[],"field_model":[{"target_id":22,"target_type":"taxonomy_term","target_uuid":"e1f167e1-124d-4db4-96ab-30641ca4e21b","url":"\/taxonomy\/term\/22"}],"field_pid":[],"field_resource_type":[],"field_rights":[],"field_subject":[]}
```

To patch an object and make it part of a collection, you need the id number of the collection object. In this example node 2 will be our collection.

`target_type` can be a confusing one, if you are ever unsure have a look at the returned values for an existing object.

```
> curl -i -u admin:islandora -H"Content-type: application/json" -X PATCH -d '{ "type": [{"target_id": "islandora_object"}], "field_member_of": [{"target_id": 2, "target_type": "node_type"}]}' 'http://localhost:8000/node/3?_format=json'

HTTP/1.1 200 OK
Date: Mon, 11 Mar 2019 18:01:40 GMT
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
Content-Length: 1669
Content-Type: application/json

{"nid":[{"value":3}],"uuid":[{"value":"3f618cdc-3f2a-4e77-b932-9ff1d461a57a"}],"vid":[{"value":4}],"langcode":[{"value":"en"}],"type":[{"target_id":"islandora_object","target_type":"node_type","target_uuid":"62189bec-3ef3-4196-b847-b17e5ce61fd5"}],"revision_timestamp":[{"value":"2019-03-11T17:36:10+00:00","format":"Y-m-d\\TH:i:sP"}],"revision_uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"revision_log":[],"status":[{"value":true}],"title":[{"value":"Updated with a PATCH request"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"created":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-11T18:01:40+00:00","format":"Y-m-d\\TH:i:sP"}],"promote":[{"value":true}],"sticky":[{"value":false}],"default_langcode":[{"value":true}],"revision_translation_affected":[{"value":true}],"content_translation_source":[{"value":"und"}],"content_translation_outdated":[{"value":false}],"field_access_terms":[],"field_alternative_title":[],"field_description":[],"field_display_hints":[],"field_edtf_date":[],"field_edtf_date_created":[],"field_edtf_date_issued":[],"field_extent":[{"value":"1 item"}],"field_identifier":[],"field_linked_agent":[],"field_member_of":[{"target_id":2,"target_type":"node","target_uuid":"413135a6-0bd1-4d6b-8bcb-059cf7784d83","url":"\/node\/2"}],"field_model":[{"target_id":22,"target_type":"taxonomy_term","target_uuid":"e1f167e1-124d-4db4-96ab-30641ca4e21b","url":"\/taxonomy\/term\/22"}],"field_pid":[],"field_resource_type":[],"field_rights":[],"field_subject":[]}
```
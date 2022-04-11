## Creating resources, media and files - POST/PUT Requests

* [Authorization](#authorization)
* [Content](#content-nodes)
* [Files and Media](#files-and-media)

## Authorization

You will need to use one of the configured authorization methods to create content, media and/or files.

These are defined under [Authorization](./using-rest-endpoints.md#authorization) on the overview.

## Content (Nodes)

![REST configuration](../assets/rest-node-configuration.png)

The above setup shows a configuration where the JSON format is enabled for GET, PATCH, DELETE, and POST, with auththentication types "basic_auth" and "jwt_auth" enabled for each method. Thus, with this configuration, you can perform a POST request against a node at the `/node` endpoint with a body in the JSON format.

To create a node, you need to provide two elements in your message body: the node type and any required field values.

For the islandora_defaults included Repository Item these are:

1. A type - this tells Drupal what content type we are creating
1. A title - this is a required field of all nodes.
1. A model - this is a required by Islandora to tell the type of object (i.e. Image, Audio, Collection)

A good way to make your first POST request is to perform a GET request against an existing node and erase all the extra content.

You can find more information about [GET requests here](./rest-get.md).

Again we are using the json format.

&#x1F34E; For example `curl -X GET 'http://localhost:8000/node/3?_format=json`

Look for the **type** element

```
"type" : [
  {
    "target_id" : "islandora_object",
    "target_type" : "node_type",
    "target_uuid" : "62189bec-3ef3-4196-b847-b17e5ce61fd5"
  }
]
```

In our example "islandora_object" is the machine name of the content type "Repository Item". If you have created a new type you will have a different target_id.

You will not need the `target_uuid`.

Next look for the **title** element

```
"title" : [
  {
    "value" : "An example Islandora object"
  }
]
```

Lastly look for the **field_model** element

```
"field_model": [
  {
    "target_id": 24,
    "target_type": "taxonomy_term",
    "target_uuid": "e7560b68-e95a-4e76-9671-2a3041cd9800",
    "url": "\\/taxonomy\\/term\\/24"
  }
]
```

You can find the models by browsing the taxonomy terms available at `http://localhost:8000/admin/structure/taxonomy/manage/islandora_models/overview`

In my example installation, term 24 is an "Image", but let's create a collection which is term 23.

**Note**: Taxonomy terms may vary between instances and you should verify the correct number for your installation.

So the body of the request will be:
```
{
  "type": [
    {
      "target_id": "islandora_object",
      "target_type": "node_type"
    }
  ],
  "title": [
    {
      "value": "Created a collection with POST"
    }
  ],
  "field_model": [
    {
      "target_id": 23,
      "target_type": "taxonomy_term"
    }
  ]
}
```

**Note**: You **must** include an **appropriate** Content-type header for the format you're requesting

**Other Note**: You **must** include some authentication credentials to say who you are and so Drupal can check if you are allowed to create this object. Otherwise you will receive a `401 Unauthorized` response.

If you do provide credentials but don't have permission, you will receive a `403 Forbidden` response.

You can find more information about [Authorization here](./using-rest-endpoints.md#authorization)

&#x1F34E; For example:

```
curl -i -X POST -u admin:islandora -H"Content-type: application/json" --data '{"type":[{"target_id":"islandora_object","target_type":"node_type"}],"title":[{"value":"Created a collection with POST"}],"field_model":[{"target_id":23,"target_type":"taxonomy_term"}]}' 'http://localhost:8000/node?_format=json'

HTTP/1.1 201 Created
Date: Tue, 05 Mar 2019 18:07:00 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Location: http://localhost:8000/node/3
Cache-Control: must-revalidate, no-cache, private
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary:
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 1564
Content-Type: application/json

{"nid":[{"value":3}],"uuid":[{"value":"3f618cdc-3f2a-4e77-b932-9ff1d461a57a"}],"vid":[{"value":3}],"langcode":[{"value":"en"}],"type":[{"target_id":"islandora_object","target_type":"node_type","target_uuid":"62189bec-3ef3-4196-b847-b17e5ce61fd5"}],"revision_timestamp":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"revision_uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"revision_log":[],"status":[{"value":true}],"title":[{"value":"Created a collection with POST"}],"uid":[{"target_id":1,"target_type":"user","target_uuid":"46a47057-de2d-4ce2-87ae-dbe4551209b8","url":"\/user\/1"}],"created":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"changed":[{"value":"2019-03-05T18:07:00+00:00","format":"Y-m-d\\TH:i:sP"}],"promote":[{"value":true}],"sticky":[{"value":false}],"default_langcode":[{"value":true}],"revision_translation_affected":[{"value":true}],"content_translation_source":[{"value":"und"}],"content_translation_outdated":[{"value":false}],"field_access_terms":[],"field_alternative_title":[],"field_description":[],"field_display_hints":[],"field_edtf_date":[],"field_edtf_date_created":[],"field_edtf_date_issued":[],"field_extent":[{"value":"1 item"}],"field_identifier":[],"field_linked_agent":[],"field_member_of":[],"field_model":[{"target_id":23,"target_type":"taxonomy_term","target_uuid":"6a3b293d-4617-417b-99d2-23d75b57f7c2","url":"\/taxonomy\/term\/23"}],"field_pid":[],"field_resource_type":[],"field_rights":[],"field_subject":[]}
```

The parts of the above request are:

1. `-i` - return the response headers
1. `-X POST` - send a POST request
1. `-u admin:islandora` - use these basic authentication credentials
1. `-H"Content-type: application/json"` - send the content-type header
1. `--data '{...}'` - send the request body (seen above)
1. `'http://localhost:8000/node?_format=json'` - the endpoint of the request

## Files and Media

The Drupal REST UI is supposed to have a way to upload files, but this seems to require the use of an X-CSRF-Token, which can only be retrieved using Cookie authentication and even then does not allow you to upload.

However there is a special REST endpoint created by Islandora, which is less configurable and is not part of the above-mentioned REST UI.

This endpoint is available at `http://localhost:8000/node/{node id}/media/{media type}/{media use}`

It only accepts PUT requests. If the media and file don't exist they are created, if they exist the file is updated with the new body.

The node id and taxonomy term id are used to search (via an [entity query](https://api.drupal.org/api/drupal/core!lib!Drupal.php/function/Drupal%3A%3AentityQuery/8.6.x)) for a matching media. If this media exists the body of the file is replaced with the new content, otherwise a new file and media are created to hold the content.

The tokens to this URI are as follows:

1. node id : The numeric ID of the node you wish to link this media/file to.
1. media type : The media type name you wish to create (i.e. image, file, audio)
1. media use : The numeric ID of the media use taxonomy term to set for this media

You can find the media use taxonomy terms at `http://localhost:8000/admin/structure/taxonomy/manage/islandora_media_use/overview`

The body of the request is the actual binary file to upload.

&#x1F34E; For example:

With a local file called `my-image.png` that I wanted to link to a node with ID `3`.

I am using the taxonomy term "Original file", which on my machine is `16`

```
> curl -i -X PUT -u admin:islandora -H"Content-type: image/png" --data-binary "@my-image.png" -H"Content-Location: public://images/my-image.png" 'http://localhost:8000/node/3/media/image/16'

HTTP/1.1 100 Continue

HTTP/1.1 201 Created
Date: Tue, 05 Mar 2019 22:01:39 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
Location: http://localhost:8000/media/4
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary:
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 0
Content-Type: text/html; charset=UTF-8
```

The parts of the above request are:

1. `-i` - return the response headers
1. `-X PUT` - send a PUT request
1. `-u admin:islandora` - use these basic authentication credentials
1. `-H"Content-type: image/png"` - send the content-type header
1. `--data-binary "@my-image.png"` - send the contents of the file located at my-image.png as binary
1. `-H"Content-Location: public://images/my-image.png"` - store the file in the public scheme (ie. in Drupal) at the path `images/my-image.png`, to store the file in Fedora use the `fedora//` scheme (ie. fedora://images/my-image.png)
1. `'http://localhost:8000/node/3/media/image/16'` - the endpoint of the request specifying the node, media type and taxonomy term.

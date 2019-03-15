
# Islandora 8 via REST


Each node, media and file in Drupal 8 has its own URI and we can GET the resources, some in a variety of formats.

We can also create nodes, media and files in Drupal by using PUT and/or POST requests.

We can update the field content by using a PATCH request and lastly we can DELETE a node, media or file resource.

To perform these actions, you will need the `RESTful Web Services` module enabled.

To configure your setup via the UI, you also need the `RESTful UI` module enabled.

Then you can configure your REST services at `https://<yourmachine>/admin/config/services/rest`

This screenshot shows the setup for resources, you can see the various HTTP methods and what formats they will respond in and what authentication methods they support.

![REST configuration](../assets/rest-node-configuration.png)

1. [Authorization](#authorization)
1. [Getting resources - GET](./rest-get.md)
1. [Creating resources - POST/PUT](./rest-create.md)
1. [Updating resources - PATCH](./rest-patch.md)
1. [Deleting resources - DELETE](./rest-delete.md)

## Authorization

If your resources are restricted (hidden) you will need to have authorization to access them.

You can specify which types of authentication are allowed for which HTTP methods.

These are common to all HTTP methods against the REST API.

In the above screenshot we have 3 allowed methods.
1. basic_auth
1. jwt_auth
1. cookie

### Basic authentication (basic_auth)
To use basic authentication with a client like cURL use the `-u username:password` argument.

For example:
```
curl -u admin:islandora http://localhost:8000/node/3
```

### JWT authentication (jwt_auth)

By default JWTs are passed internally from Drupal to various microservices and Fedora. 

To use a JWT yourself you need to enable the `JWT Authentication Issuer` module.

Once enabled this module makes a `/jwt/token` endpoint. You can perform a `GET` against this endpoint as an authenticated user to receive a JWT.

For example:
```
curl -i -u admin:islandora http://localhost:8000/jwt/token

HTTP/1.1 200 OK
Date: Mon, 04 Mar 2019 22:08:37 GMT
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
Content-Length: 620
Content-Type: application/json

{
  "token" : "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJpYXQiOjE1NTE3MzczMTcsImV4cCI6MTU1MTc0NDUxNywiZHJ1cGFsIjp7InVpZCI6IjEifSwid2ViaWQiOiIxIiwiaXNzIjoiaHR0cHM6XC9cL2xvY2FsaG9zdDo4MDAwIiwic3ViIjoiYWRtaW4iLCJyb2xlcyI6WyJhdXRoZW50aWNhdGVkIiwiYWRtaW5pc3RyYXRvciIsImZlZG9yYWFkbWluIl19.QUTrMiK_DyBxqQY4LnibLYtieEW3-MyjjQO9NSFI7bPylNm1S5ZY0uvzjDob3ckYgRN4uCyMFZO4BPytpQVA_jyeSuZyUA_10v33ItpoKyjrJ_S057iykNd_rWmxe8tT8T1fPypq_-Z7Th_PkyZWrYBqoBBVO1iVQt5txxfGWMqhxd2FgsXw6N-aR9sYOSc4UrLmFRmPP5Zk_CNIZP6NtBaM9JNr8CnWyPEFSAR75xfyH3ge5qjBqLlDS389k07pyJFB5rOT59txzLE9WLvpp9JK3oQv821Q1Bp-PMiASghXc0dBCHxM8o41BzLE88UstRA7agBAkUqG3hMpoNZqfA"
}
```

You can then take the same token and re-use it.

```
curl -H"Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJpYXQiOjE1NTE3MzczMTcsImV4cCI6MTU1MTc0NDUxNywiZHJ1cGFsIjp7InVpZCI6IjEifSwid2ViaWQiOiIxIiwiaXNzIjoiaHR0cHM6XC9cL2xvY2FsaG9zdDo4MDAwIiwic3ViIjoiYWRtaW4iLCJyb2xlcyI6WyJhdXRoZW50aWNhdGVkIiwiYWRtaW5pc3RyYXRvciIsImZlZG9yYWFkbWluIl19.QUTrMiK_DyBxqQY4LnibLYtieEW3-MyjjQO9NSFI7bPylNm1S5ZY0uvzjDob3ckYgRN4uCyMFZO4BPytpQVA_jyeSuZyUA_10v33ItpoKyjrJ_S057iykNd_rWmxe8tT8T1fPypq_-Z7Th_PkyZWrYBqoBBVO1iVQt5txxfGWMqhxd2FgsXw6N-aR9sYOSc4UrLmFRmPP5Zk_CNIZP6NtBaM9JNr8CnWyPEFSAR75xfyH3ge5qjBqLlDS389k07pyJFB5rOT59txzLE9WLvpp9JK3oQv821Q1Bp-PMiASghXc0dBCHxM8o41BzLE88UstRA7agBAkUqG3hMpoNZqfA" http://localhost:8000/node/3?_format=jsonld

HTTP/1.1 200 OK
Date: Mon, 04 Mar 2019 22:10:02 GMT
Server: Apache/2.4.18 (Ubuntu)
X-Powered-By: PHP/7.1.26-1+ubuntu16.04.1+deb.sury.org+1
Cache-Control: must-revalidate, no-cache, private
Link: <http://localhost:8000/node/3>; rel="canonical"
Link: <http://localhost:8000/node/3/delete>; rel="https://drupal.org/link-relations/delete-form"
Link: <http://localhost:8000/admin/content/node/delete?node=3>; rel="https://drupal.org/link-relations/delete-multiple-form"
Link: <http://localhost:8000/node/3/edit>; rel="edit-form"
Link: <http://localhost:8000/node/3/revisions>; rel="version-history"
Link: <http://localhost:8000/node/3>; rel="https://drupal.org/link-relations/revision"
Link: <http://localhost:8000/node?node=3>; rel="https://drupal.org/link-relations/create"
Link: <http://purl.org/coar/resource_type/c_c513>; rel="tag"; title="Image"
Link: <http://localhost:8000/media/1>; rel="related"; title="Original File"
Link: <http://localhost:8000/media/2>; rel="related"; title="Service File"
Link: <http://localhost:8000/media/3>; rel="related"; title="Thumbnail Image"
Link: <http://localhost:8000/node/3?_format=json>; rel="alternate"; type="application/json"
X-Drupal-Dynamic-Cache: HIT
X-UA-Compatible: IE=edge
Content-language: en
X-Content-Type-Options: nosniff
X-Frame-Options: SAMEORIGIN
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Vary: 
X-Generator: Drupal 8 (https://www.drupal.org)
Content-Length: 858
Content-Type: application/ld+json

{
  "@graph": [
    {
      "@id": "http:\\/\\/localhost:8000\\/node\\/3?_format=jsonld",
      "@type": [
        "http:\\/\\/pcdm.org\\/models#Object"
      ],
      "http:\\/\\/purl.org\\/dc\\/terms\\/title": [
        {
          "@value": "Custom item",
          "@language": "en"
        }
      ],
      "http:\\/\\/schema.org\\/author": [
        {
          "@id": "http:\\/\\/localhost:8000\\/user\\/1?_format=jsonld"
        }
      ],
      "http:\\/\\/schema.org\\/dateCreated": [
        {
          "@value": "2019-03-01T19:42:54+00:00",
          "@type": "http:\\/\\/www.w3.org\\/2001\\/XMLSchema#dateTime"
        }
      ],
      "http:\\/\\/schema.org\\/dateModified": [
        {
          "@value": "2019-03-01T19:43:12+00:00",
          "@type": "http:\\/\\/www.w3.org\\/2001\\/XMLSchema#dateTime"
        }
      ],
      "http:\\/\\/purl.org\\/dc\\/terms\\/extent": [
        {
          "@value": "1 item",
          "@type": "http:\\/\\/www.w3.org\\/2001\\/XMLSchema#string"
        }
      ],
      "http:\\/\\/schema.org\\/sameAs": [
        {
          "@value": "http:\\/\\/localhost:8000\\/node\\/1?_format=jsonld"
        }
      ]
    },
    {
      "@id": "http:\\/\\/localhost:8000\\/user\\/1?_format=jsonld",
      "@type": [
        "http:\\/\\/schema.org\\/Person"
      ]
    }
  ]
}
```

### Cookie authentication (cookie)
This allows you to use a cookie stored in your web browser when you log in to Drupal to access these REST endpoint pages.

This is what allows you to access the URIs like `http://localhost:8000/node/1?_format=json` with your web browser.



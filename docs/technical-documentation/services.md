#Islandora 7.x 2.x Middleware Services Doc

Islandora middleware services utilize the Fedora 4 REST API and the Drupal Services module to create an API to be used for the majority of interactions between the two systems.  The Drupal node id will be used to identify objects (and datastreams thereof) when using these services.  The breakdown of services is as follows:
- There will be a service to provide CRUD operations for objects regardless of content type.  
- There will be a service to provide advanced creational capabilities based on content model.  For example:
  - POST a tiff to create a large image object and set the tiff as the OBJ datatream.
  - POST a zip file containing book pages Islandora Book Batch format to create an entire book object (with pages and derivatives processed asynchronously, of course).
- There will be services per datastream type to perform operations based on the node ID of the parent.
  - GET me the MODS datastream for object with node id 1.
- There will be services per derivative type to perform operations based on the node ID of the parent.
  - PATCH to generate a new thumbnail for the object with node id 1.

# Object Service
Essentially wraps the Fedora 4 API requests for manipulating a resource identified by Drupal node id.  This service is limited to objects (e.g. Containers in Fedora 4), and does not work for datastreams since they do not have their own Drupal node id.
#### Endpoint: http://localhost:8080/islandora/services/object/
#### Actions:
#####GET http://localhost:8080/islandora/services/object/{id}
Retrieves metadata from Fedora for the object associated with the provided Drupal node id.
#####POST http://localhost:8080/islandora/services/object/{path}
Creates an object in Fedora and a node in Drupal, associating the newly minted path and node id.  Will require the content model to be supplied in the POST data or the Drupal node will not be able to be created.  The object in Fedora is created as a child to the supplied path.
#####PUT http://localhost:8080/islandora/services/object/{id}
Updates metadata in Fedora for the object associated with the provided Drupal node id.  Additionally updates all applicable fields in the Drupal node.
#####PATCH http://localhost:8080/islandora/services/object/{id}
Partially updates metadata in Feodra for the object associated with the provided Drupal node id.  Additionally updates all applicable fields in the Drupal node.
#####DELETE http://localhost:8080/islandora/services/object/{id}
Deletes the object in Fedora associated with provided Drupal node id.  Also deletes the Drupal node itself.

# Datastream Services
## OBJ Service
Provides requests for manipulating the OBJ datastream of Fedora resources identified by Drupal node id.
#### Endpoint: http://localhost:8080/islandora/services/OBJ/
#### Actions:
#####GET http://localhost:8080/islandora/services/OBJ/{id}
Returns the OBJ datastream for the Fedora object identified by the provided Drupal node id.
#####PUT http://localhost:8080/islandora/services/OBJ/{id}
Creates or updates an OBJ datastream for the Fedora object identified by the provided Drupal node id, setting it's content to the PUT content.
#####DELETE http://localhost:8080/islandora/services/OBJ/{id}
Deletes the OBJ datastream for the Fedora object identified by the provided Drupal node id.

## MODS Service
Provides requests for manipulating the MODS datastream of Fedora resources identified by Drupal node id.
#### Endpoint: http://localhost:8080/islandora/services/MODS/
#### Actions:
#####GET http://localhost:8080/islandora/services/MODS/{id}
Returns the MODS datastream for the Fedora object identified by the provided Drupal node id.
#####PUT http://localhost:8080/islandora/services/MODS/{id}
Creates or updates a MODS datastream for the Fedora object identified by the provided Drupal node id, setting it's content to the PUT content.  If the Drupal node identified by the provided id contains field_mods, its contents will get updated as well.
#####DELETE http://localhost:8080/islandora/services/MODS/{id}
Deletes the MODS datastream for the Fedora object identified by the provided Drupal node id.  If the Drupal node identified by the provided id contains field_mods, its contents will get deleted as well.

## DC Service
Provides requests for manipulating the DC datastream of Fedora resources identified by Drupal node id.
#### Endpoint: http://localhost:8080/islandora/services/DC/
#### Actions:
#####GET http://localhost:8080/islandora/services/DC/{id}
Returns the DC datastream for the Fedora object identified by the provided Drupal node id.
#####PUT http://localhost:8080/islandora/services/DC/{id}
Creates or updates a DC datastream for the Fedora object identified by the provided Drupal node id, setting it's content to the PUT content.  If the Drupal node identified by the provided id contains field_dc, its contents will get updated as well.
#####DELETE http://localhost:8080/islandora/services/DC/{id}
Deletes the DC datastream for the Fedora object identified by the provided Drupal node id.  If the Drupal node identified by the provided id contains field_dc, its contents will get deleted as well.

## TODO:  Add services for other descriptive metadata formats.

# Derivative Services
## TN Service
Provides requests for manipulating the thumbnail for a Fedora resource identified by Drupal node id
#### Endpoint: http://localhost:8080/islandora/services/TN/
#### Actions:
#####GET http://localhost:8080/islandora/services/TN/{id}
Returns the thumbnail for the Fedora object identified by the provided Drupal node id.
#####PUT http://localhost:8080/islandora/services/TN/{id}
Sets the value of field_tn for the Drupal node identified by the provided id using the PUT content.
#####PATCH http://localhost:8080/islandora/services/TN/{id}
Requests the thumbnail for the Fedora object identified by provided Drupal node id be asynchronously generated from its OBJ datastream, with field_tn for the Drupal node getting set to the results.  
#####DELETE http://localhost:8080/islandora/services/TN/{id}
Deletes the content of field_tn for the Drupal node identified by the provided id.

## TECHMD Service
SUBJECT TO CHANGE AS MUCH OF THIS MAY GET PUSHED INTO RDF!!!!
Provides requests for manipulating the FITS generated technical metadata for a Fedora resource identified by Drupal node id
#### Endpoint: http://localhost:8080/islandora/services/TECHMD/
#### Actions:
#####GET http://localhost:8080/islandora/services/TECHMD/{id}
Returns the technical metadata for the Fedora object identified by the provided Drupal node id.
#####PUT http://localhost:8080/islandora/services/TECHMD/{id}
Sets the value of field_techmd for the Drupal node identified by the provided id using the PUT content.
#####PATCH http://localhost:8080/islandora/services/TECHMD/{id}
Requests the technical metadata for the Fedora object identified by provided Drupal node id be asynchronously generated from its OBJ datastream, with field_techmd for the Drupal node getting set to the results.  
#####DELETE http://localhost:8080/islandora/services/TECHMD/{id}
Deletes the content of field_techmd for the Drupal node identified by the provided id.

## TODO:  Add other derivatives, such As MEDIUM_SIZE, JP2, MP3, MP4, MKV, etc...

# Ingest Services
## Basic Image Service
Convienence service for generating basic image objects.
#### Endpoint http://localhost:8080/islandora/services/basic_image/
#### Actions:
#####POST http://localhost:8080/islandora/services/basic_image/
Creates a new Fedora object giving it the basic_image content model.  Also creates a Drupal node, and associates the newly minted node id and path.  Behavior depends on the mimetype of the POST content.  If it is an image mimetype, it sets the content of the OBJ datastream to the POST data.  If it is a zip mimetype, POST content assumed to be in Islandora Zip Importer format and the service will use it to construct the entire object, datastreams and all.

## Large Image Service
Convienence service for generating large image objects.
#### Endpoint http://localhost:8080/islandora/services/large_image/
#### Actions:
#####POST http://localhost:8080/islandora/services/large_image/
Creates a new Fedora object giving it the large_image content model.  Also creates a Drupal node, and associates the newly minted node id and path.  Behavior depends on the mimetype of the POST content.  If it is a large image mimetype, it sets the content of the OBJ datastream to the POST data.  If it is a zip mimetype, POST content assumed to be in Islandora Zip Importer format and the service will use it to construct the entire object, datastreams and all.

## Audio Service
Convienence service for generating audio objects.
#### Endpoint http://localhost:8080/islandora/services/audio/
#### Actions:
#####POST http://localhost:8080/islandora/services/audio/
Creates a new Fedora object giving it the audio content model.  Also creates a Drupal node, and associates the newly minted node id and path.  Behavior depends on the mimetype of the POST content.  If it is an audio mimetype, it sets the content of the OBJ datastream to the POST data.  If it is a zip mimetype, POST content assumed to be in Islandora Zip Importer format and the service will use it to construct the entire object, datastreams and all.

## Video Service
Convienence service for generating video objects.
#### Endpoint http://localhost:8080/islandora/services/video/
#### Actions:
#####POST http://localhost:8080/islandora/services/video/
Creates a new Fedora object giving it the video content model.  Also creates a Drupal node, and associates the newly minted node id and path.  Behavior depends on the mimetype of the POST content.  If it is a video mimetype, it sets the content of the OBJ datastream to the POST data.  If it is a zip mimetype, POST content assumed to be in Islandora Zip Importer format and the service will use it to construct the entire object, datastreams and all.

## Book Service
Convienence service for generating book objects.
#### Endpoint http://localhost:8080/islandora/services/book/
#### Actions:
#####POST http://localhost:8080/islandora/services/book/
Creates a new Fedora object giving it the book content model.  Also creates a Drupal node, and associates the newly minted node id and path.  The POST data is assumed to be a zip file containing Islandora Book Batch content, and pages and derivatives are then generated from it asynchronously.  

## Newspaper Service
Convienence service for generating newspaper objects.
#### Endpoint http://localhost:8080/islandora/services/newspaper/
#### Actions:
#####POST http://localhost:8080/islandora/services/newspaper/
Creates a new Fedora object giving it the newspaper content model.  Also creates a Drupal node, and associates the newly minted node id and path.  The POST data is assumed to be a zip file in Islandora Newspaper Batch format, and pages and derivatives are then generated from it asynchronously.  

## TODO:  Add other content model specific ingest services.
## TODO:  Consider accepting form data in addition to zip in order to create entire obejcts. 

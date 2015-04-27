# Middleware Services

Islandora middleware services utilize the Fedora 4 REST API and the Drupal Services module to create an API for the majority of interactions between the two systems. The Drupal uuid will be used to identify `pcdm:Objects` (and `pcdm:Files` thereof) when using these services.  The breakdown of services is as follows:

* There will be a services to provide CRUD operations for `pcdm:Collections`, `pcdm:Objects` and `pcdm:Files`.  
* There will be a service to provide advanced creational capabilities based on content model.  For example:
  * `POST` a tiff to create a large image object and set the tiff as the OBJ datatream.
  * `POST` a zip file containing book pages Islandora Book Batch format to create an entire book object (with pages and derivatives processed asynchronously, of course).
* There will be services per derivative type to perform operations based on the node ID of the parent.
  * `PATCH` to generate a new thumbnail for the object with uuid 1.

## Collection Service

Used for manipulating `pcdm:Collections`. Essentially wraps the Fedora 4 API requests for manipulating a Container identified by Drupal uuid.

**Endpoint**: http://localhost:8080/islandora/services/collection/

**Actions**:

* `GET` http://localhost:8080/islandora/services/collection/{id}
Retrieves metadata from Fedora for the `pcdm:Collection` associated with the provided Drupal uuid.

* `POST` http://localhost:8080/islandora/services/collection/
Creates a `pcdm:Collection` in Fedora and a node in Drupal, associating the newly minted path and uuid. The `fedora:hasParent` can optionally be supplied in `POST` data to provide Fedora with a location to create the new object.

* `PUT` http://localhost:8080/islandora/services/collection/{id}
Updates metadata in Fedora for the `pcdm:Collection` associated with the provided Drupal uuid. Additionally updates all applicable fields in the Drupal node.

* `PATCH` http://localhost:8080/islandora/services/collection/{id}
Partially updates metadata in Feodra for the `pcdm:Collection` associated with the provided Drupal uuid. Additionally updates all applicable fields in the Drupal node.

* `DELETE` http://localhost:8080/islandora/services/collection/{id}
Deletes the `pcdm:Collection` in Fedora associated with provided Drupal uuid. Also deletes the Drupal node itself.

## Object Service

Used for manipulating `pcdm:Objects`. Essentially wraps the Fedora 4 API requests for manipulating a Container identified by Drupal uuid. 

**Endpoint**: http://localhost:8080/islandora/services/object/

**Actions**:

* `GET` http://localhost:8080/islandora/services/object/{id}
Retrieves metadata from Fedora for the `pcdm:Object` associated with the provided Drupal uuid.

* `POST` http://localhost:8080/islandora/services/object/
Creates a `pcdm:Object` in Fedora and a node in Drupal, associating the newly minted path and uuid. Will require the content model to be provided as `rdf:type` in `POST` data so the Drupal node can be created. The `fedora:hasParent` can optionally be supplied in `POST` data to provide Fedora with a location to create the new object.

* `PUT` http://localhost:8080/islandora/services/object/{id}
Updates metadata in Fedora for the `pcdm:Object` associated with the provided Drupal uuid. Additionally updates all applicable fields in the Drupal node.

* `PATCH` http://localhost:8080/islandora/services/object/{id}
Partially updates metadata in Feodra for the `pcdm:Object` associated with the provided Drupal uuid. Additionally updates all applicable fields in the Drupal node.

* `DELETE` http://localhost:8080/islandora/services/object/{id}
Deletes the `pcdm:Object` in Fedora associated with provided Drupal uuid. Also deletes the Drupal node itself.

## File Services

Used for manipulating `pcdm:Files` associated with a `pcdm:Object` or `pcdm:Collection`. They will essentially wrap the Fedora 4 API requests for manipulating a `NonRDFSourceDescription` identified by `dcterms:title` (obj, mods, dc, etc...) and its parent's Drupal uuid. There will be seperate implementations for each applicable `dcterms:title`, though each service will abide by a common conventions and behavior. This allows implementations to vary independently based on file type (technical metadata, descriptive metadata, archival binary), while allowing for new services to be easily added over time.

### General Convention

**Endpoint**: http://localhost:8080/islandora/services/file/{dcterms:title}/{id}/
**Actions**:

* `GET` http://localhost:8080/islandora/services/file/{dcterms:title}/{id}/
Retrieves the `pcdm:Files` content belonging to the `pcdm:Object` or `pcdm:Collection`. The `pcdm:File` is identified by `dcterms:title` and its parents Drupal uuid.

* `POST` http://localhost:8080/islandora/services/file/{dcterms:title}/{id}/
Adds a `pcdm:File` to the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The file's content will be set using the `POST` content, and the provided `dcterms:title` will be given to the newly created `pcdm:File`. If the Drupal node has applicable field(s) pertaining to this file, they will be updated as well.

* `PUT` http://localhost:8080/islandora/services/file/{dcterms:title}/{id}/
Updates the `pcdm:File` with `dcterms:title` owned by the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The file's content will be set using the `PUT` content. If the Drupal node has applicable field(s) pertaining to this file, they will be updated as well.

* `DELETE` http://localhost:8080/islandora/services/file/{dcterms:title}/{id}/
Deletes the `pcdm:File` with `dcterms:title` owned by the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. If the Drupal node has applicable field(s) pertaining to this file, their content will be deleted as well.

### Example Services

Some example services would include:

* http://localhost:8080/islandora/services/file/obj/{id}/
* http://localhost:8080/islandora/services/file/dc/{id}/
* http://localhost:8080/islandora/services/file/mods/{id}/
* http://localhost:8080/islandora/services/file/fits/{id}/

Implementations would differ in the sense that perhaps xml metadata would be transformed to RDF differently and different Drupal fields would have to be updated. Accept headers could be different, etc... 

## Derivative Services

Derivatives (while generally dealt with through Islandora Sync) may need to be dealt with manually, so services will be provided to handle all the common derivatives types an Islandora installation may require. Much like the file services, a general convention on naming and behavior will be enforced, but seperate implementations will be provided. This will allow for maximum flexibility and encourage new services to be easily added and contributed. 

### General Convention

**Endpoint**: http://localhost:8080/islandora/services/derivative/{type}/{id}/
**Actions**:

* `GET` http://localhost:8080/islandora/services/derivative/{type}/{id}/
Retrieves the derivative belonging to the `pcdm:Object` or `pcdm:Collection`. The derivative is identified by its type and its parents Drupal uuid.

* `POST` http://localhost:8080/islandora/services/derivative/{type}/{id}/
Adds a derivative to the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The derivative's content will be set using the `POST` content. The result will reside in either Fedora or Drupal, depending on the derivative.

* `PUT` http://localhost:8080/islandora/services/derivative/{type}/{id}/
Updates the derviative for a `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The derivative's content will be set using the `PUT` content. The result will reside in either Fedora or Drupal, depending on the derivative.

* `PATCH` http://localhost:8080/islandora/services/derivative/{type}/{id}/
Asynchronously regenerates the derivative for a `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The result will reside in either Fedora or Drupal, depending on the derivative.

* `DELETE` http://localhost:8080/islandora/services/derivative/{type}/{id}/
Deletes the derivative for the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The derivative will be removed from either Fedora or Drupal, depending on the derivative.

### Example Services
Some example services would include:

* http://localhost:8080/islandora/services/derivative/tn/{id}/
* http://localhost:8080/islandora/services/derivative/medium_size/{id}/
* http://localhost:8080/islandora/services/derivative/jp2/{id}/
* http://localhost:8080/islandora/services/derivative/techmd/{id}/
* http://localhost:8080/islandora/services/derivative/mp3/{id}/
* http://localhost:8080/islandora/services/derivative/mp4/{id}/
* http://localhost:8080/islandora/services/derivative/mkv/{id}/

Implementations would differ in the sense that different derivatives will be created from varying sources using varying programs. Also, display derivatives should reside in Drupal (TN, JP2, etc...) while technical metadata should reside in Fedora.  Within Drupal, different derivatives will reside as different field types.

## Ingest Services

Several services will exist to encapsulate the process of constructing a fully-formed pcdm:Object based on content model in an Islandora repository within a single transaction. They will accept multipart/form-data and use the information to construct the pcdm:Object with RDF metadata and any additionally provided files (obj, mods, dc, etc...).  

### General Convention

**Endpoint**: http://localhost:8080/islandora/services/ingest/{content_model}/

**Actions**:

* `POST` http://localhost:8080/islandora/services/ingest/{content_model}/
Accepts multipart/form-data requests and uses the provided metadata and files to completely construct an object of the specified content-model.

### Example Services
Some example services would include:

* http://localhost:8080/islandora/services/ingest/basic_image/
* http://localhost:8080/islandora/services/ingest/large_image/
* http://localhost:8080/islandora/services/ingest/audio/
* http://localhost:8080/islandora/services/ingest/video/

## Zip Ingest Services

Instead of multipart/form-data reqeusts, application/zip requests can be accepted to construct a pcdm:Object based on content model.  While this is convienent for basic object types, it's required for more complicated content models such as Book, Newspaper, and Compound.  Message bodies are assumed to be in Islandora Zip Importer format.

### General Convention
**Endpoint**: http://localhost:8080/islandora/services/zip/ingest/{content_model}/
**Actions**:

* `POST` http://localhost:8080/islandora/services/zip/ingest/{content_model}/
Accepts a zip file, extracts its contents, and uses it to completely construct an object of the specified content-model.

### Example services
All of the Ingest Services above, plus:

* http://localhost:8080/islandora/services/zip/ingest/book/
* http://localhost:8080/islandora/services/zip/ingest/newspaper/
* http://localhost:8080/islandora/services/zip/ingest/compound/

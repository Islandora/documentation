# Middleware Services

Islandora middleware services utilize the Fedora 4 REST API and the Drupal Services module to create an API for the majority of interactions between the two systems. The Drupal uuid will be used to identify `pcdm:Objects` (and `pcdm:Files` thereof) when using these services.  The breakdown of services is as follows:

* There will be a services to provide CRUD operations for `pcdm:Collections`, `pcdm:Objects` and `pcdm:Files`.
* There will be a service to provide creational operations using zip content based on content model.  For example:
  * `POST` a zip file containing book pages Islandora Book Batch format to create an entire book object (with pages and derivatives processed asynchronously, of course).
* There will be services per derivative type to perform operations based on the node UUID of the parent.
  * `PATCH` to generate a new thumbnail for a `pcdm:Object`.

For more information on PCDM, please see [this](https://wiki.duraspace.org/display/FF/Portland+Common+Data+Model) page.

## Collection Service

Used for manipulating `pcdm:Collections`. Essentially wraps the Fedora 4 API requests for manipulating a Container identified by Drupal uuid.  Node content in `application/json` format is massaged into `application/sparql-update` format in order to interact with Fedora.

**Endpoint**: http://localhost:8080/islandora-services/collection/

**Actions**:

* `GET` http://localhost:8080/islandora-services/collection/{uuid}
    * Returns `application/ld+json` RDF metadata for the `pcdm:Collection` identified by the supplied Drupal uuid.
    
* `POST` http://localhost:8080/islandora-services/collection/
    * Accepts `application/json` serialized node data.
    * Creates a `pcdm:Collection` in Fedora using the Drupal node data, associating the newly minted path with the node's uuid. If `fedora:hasParent` is supplied supplied in `POST` data, that location will be used to create the new object as a child.

* `PUT` http://localhost:8080/islandora-services/collection/{uuid}
    * Accepts `application/json` serialized node data.
    * Updates metadata in Fedora for the `pcdm:Collection` associated with the provided Drupal uuid.

* `DELETE` http://localhost:8080/islandora-services/collection/{uuid}
    * Deletes the `pcdm:Collection` in Fedora associated with provided Drupal uuid.

## Object Service

Used for manipulating `pcdm:Objects`. Essentially wraps the Fedora 4 API requests for manipulating a Container identified by Drupal uuid.  Node content in `application/json` format is massaged into `application/sparql-update` format in order to interact with Fedora.  Certain field values, as well as other files sent along in the message will be converted into `pcdm:Files` and attached to the newly created `pcdm:Object`.

**Endpoint**: http://localhost:8080/islandora-services/object/

**Actions**:

* `GET` http://localhost:8080/islandora-services/object/{uuid}
    * Returns `application/ld+json` RDF metadata for the `pcdm:Object` identified by the supplied Drupal uuid.

* `POST` http://localhost:8080/islandora-services/object/
    * Accepts `multipart/form-data` messages.  One "part" of the message will be the JSON serialized Drupal node content.  Other "parts" will be files used to create `pcdm:Files` for the `pcdm:Object`.
    * Creates a `pcdm:Object` in Fedora using the Drupal node data, associating the newly minted path with the node's uuid. If `fedora:hasParent` is supplied supplied in `POST` data, that location will be used to create the new object as a child.  Certain field values as well as any other files contained in the message will be used to create `pcdm:Files` associated with this `pcdm:Object`.

* `PUT` http://localhost:8080/islandora-services/object/{uuid}
    * Accepts `multipart/form-data` messages.  One "part" of the message will be the JSON serialized Drupal node content.  Other "parts" will be files used to create `pcdm:Files` for the `pcdm:Object`.
    * Updates metadata in Fedora for the `pcdm:Object` associated with the provided Drupal uuid. Certain field values as well as any other files contained in the message will be used to update `pcdm:Files` associated with this `pcdm:Object`.

* `DELETE` http://localhost:8080/islandora-services/object/{uuid}
    * Deletes the `pcdm:Object` in Fedora associated with provided Drupal uuid. Also deletes any `pcdm:Files` directly contained by the `pcdm:Object`.

## File Services

Used for manipulating `pcdm:Files` associated with a `pcdm:Object`. They will essentially wrap the Fedora 4 API requests for manipulating a `NonRDFSourceDescription` identified by `dcterms:title` (obj, mods, dc, etc...) and its parent's Drupal uuid. There will be seperate implementations for each applicable `dcterms:title`, though each service will abide by a common conventions and behavior. This allows implementations to vary independently based on file type (technical metadata, descriptive metadata, archival binary), while allowing for new services to be easily added over time.

### General Convention

**Endpoint**: http://localhost:8080/islandora-services/file/{uuid}/{dcterms:title}
**Actions**:

* `GET` http://localhost:8080/islandora-services/file/{uuid}/{dcterms:title}
    * Retrieves the `pcdm:File` content belonging to the `pcdm:Object`. The `pcdm:File` is identified by `dcterms:title` and its parents Drupal uuid.

* `POST` http://localhost:8080/islandora-services/file/{uuid}/{dcterms:title}
    * Adds a `pcdm:File` to the `pcdm:Object` identified by the provided Drupal uuid. The file's content will be set using the `POST` content, and the provided `dcterms:title` will be given to the newly created `pcdm:File`.

* `PUT` http://localhost:8080/islandora-services/file/{uuid}/{dcterms:title}
    * Updates the `pcdm:File` with `dcterms:title` owned by the `pcdm:Object` identified by the provided Drupal uuid. The file's content will be set using the `PUT` content.

* `DELETE` http://localhost:8080/islandora-services/file/{uuid}/{dcterms:title}
    * Deletes the `pcdm:File` with `dcterms:title` owned by the `pcdm:Object` identified by the provided Drupal uuid.

### Example Services

Some example services would include:

* http://localhost:8080/islandora/services/file/{uuid}/obj
* http://localhost:8080/islandora/services/file/{uuid}/dc
* http://localhost:8080/islandora/services/file/{uuid}/mods
* http://localhost:8080/islandora/services/file/{uuid}/fits

Implementations would differ in the sense that accept headers would be different, and data may have to be transformed differently before being inserted as the `pcdm:File`.

## Derivative Services

Derivatives (while generally dealt with through Islandora Sync) may need to be dealt with manually, so services will be provided to handle all the common derivatives types an Islandora installation may require. Much like the file services, a general convention on naming and behavior will be enforced, but seperate implementations will be provided. This will allow for maximum flexibility and encourage new services to be easily added and contributed. 

### General Convention

**Endpoint**: http://localhost:8080/islandora-services/derivative/{uuid}/{type}
**Actions**:

* `GET` http://localhost:8080/islandora-services/derivative/{uuid}/{type}
    * Retrieves the derivative belonging to the `pcdm:Object` or `pcdm:Collection`. The derivative is identified by its type and its parents Drupal uuid.

* `POST` http://localhost:8080/islandora-services/derivative/{uuid}/{type}
    * Adds a derivative to the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The derivative's content will be set using the `POST` content. The result will reside in either Fedora or Drupal, depending on the derivative.

* `PUT` http://localhost:8080/islandora-services/derivative/{uuid}/{type}
    * Updates the derviative for a `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The derivative's content will be set using the `PUT` content. The result will reside in either Fedora or Drupal, depending on the derivative.

* `PATCH` http://localhost:8080/islandora-services/derivative/{uuid}/{type}
    * Asynchronously regenerates the derivative for a `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The result will reside in either Fedora or Drupal, depending on the derivative.

* `DELETE` http://localhost:8080/islandora-services/derivative/{uuid}/{type}
    * Deletes the derivative for the `pcdm:Object` or `pcdm:Collection` identified by the provided Drupal uuid. The derivative will be removed from either Fedora or Drupal, depending on the derivative.

### Example Services
Some example services would include:

* http://localhost:8080/islandora/services/derivative/{uuid}/tn
* http://localhost:8080/islandora/services/derivative/{uuid}/medium_size
* http://localhost:8080/islandora/services/derivative/{uuid}/jp2
* http://localhost:8080/islandora/services/derivative/{uuid}/techmd
* http://localhost:8080/islandora/services/derivative/{uuid}/mp3
* http://localhost:8080/islandora/services/derivative/{uuid}/mp4
* http://localhost:8080/islandora/services/derivative/{uuid}/mkv

Implementations would differ in the sense that different derivatives will be created from varying sources using varying programs. Also, display derivatives should reside in Drupal (TN, JP2, etc...) while technical metadata should reside in Fedora.  Within Drupal, different derivatives will reside as different field types.

## Zip Ingest Services

Instead of multipart/form-data reqeusts, application/zip requests can be accepted to construct a pcdm:Object based on content model.  While this is convienent for basic object types, it's required for more complicated content models such as Book, Newspaper, and Compound.  Message bodies are assumed to be in Islandora Zip Importer format.

### General Convention
**Endpoint**: http://localhost:8080/islandora/services/zip/ingest/{content_model}/
**Actions**:

* `POST` http://localhost:8080/islandora/services/zip/ingest/{content_model}/
    * Accepts `application/zip` messages
    * Takes the supplied zip file, extracts its contents, and uses it to completely construct an object of the specified content-model.

### Example services
* http://localhost:8080/islandora/services/zip/large_image
* http://localhost:8080/islandora/services/zip/audio
* http://localhost:8080/islandora/services/zip/video
* http://localhost:8080/islandora/services/zip/book
* http://localhost:8080/islandora/services/zip/newspaper
* http://localhost:8080/islandora/services/zip/compound

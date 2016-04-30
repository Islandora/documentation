## Transaction Service (done)

Used to start, commit, and rollback transactions.

**Endpoint**: http://localhost:8080/islandora/transaction/

**Actions**:

* `POST` http://localhost:8080/islandora-services/transaction/

    * Creates a transaction in fedora, returning the id of the newly made transaction

* `POST` http://localhost:8080/islandora-services/transaction/{tx_id}/extend

    * Extends a transaction

    * Returns 204 - No Content

* `POST` http://localhost:8080/islandora-services/transaction/{tx_id}/commit

    * Commits a transaction

    * Returns 204 - No Content

* `POST` http://localhost:8080/islandora-services/transaction/{tx_id}/rollback

    * Rolls back a transaction

    * Returns 204 - No Content

## Resource Service (done)

Exposes basic CRUD operations on repository resources identified by a UUID.

**Endpoint**: http://localhost:8080/islandora/resource/

**Actions**:

* `GET` http://localhost:8080/islandora/resource/{uuid}?tx={tx_id}

    * Returns RDF metadata for the resource identified by the provided UUID.  Respects all headers the Fedora 4 API respects.

    * Optional transaction id will ensure the returned the RDF represents the current status of the resource within said transaction.

* `POST` http://localhost:8080/islandora/resource/?tx={tx_id}

    * Creates a resource in Fedora 4.  Respects all headers the Fedora 4 API respects.

    * Optional transaction id will ensure the resource is created within said transaction.

* `PUT` http://localhost:8080/islandora/resource/{uuid}?tx={tx_id}

    * Updates a resource in Fedora 4.  Respects all headers the Fedora 4 API respects.

    * Optional transaction id will ensure the resource is updated within said transaction.

* `PATCH` http://localhost:8080/islandora/resource/{uuid}?tx={tx_id}

    * Applies a SPARQL/Update query against a resource in Fedora 4.  Respects all headers the Fedora 4 API respects.

    * Optional transaction id will ensure the resource is updated within said transaction.

* `DELETE` http://localhost:8080/islandora-services/resource/{uuid}?tx={tx_id}

    * Deletes the resource in Fedora 4.

    * Optional transaction id will ensure the resource is deleted within said transaction.

## Collection Service (Sprint 003)

Convenience operations for pcdm:Collections in Fedora 4.

**Endpoint**: http://localhost:8080/islandora/collection/

**Actions**:

* `POST` http://localhost:8080/islandora/collection/?tx={tx_id}

    * Creates a pcdm:Collection in Fedora 4.  Respects all headers the Fedora 4 API respects.  Creates the appropriate indirect containers, with slug "members", to manage the pcdm:hasMember relationship or its inverse.

    * Optional transaction id will ensure the resources are created within said transaction.

## Object Service

Convenience operations for pcdm:Objects in Fedora 4.

**Endpoint**: http://localhost:8080/islandora/object/

**Actions**:

* `POST` http://localhost:8080/islandora/object/?tx={tx_id}

    * Creates a pcdm:Object in Fedora 4.  Respects all headers the Fedora 4 API respects.  Adds the appropriate containers to manage the pcdm:hasMember and pcdm:hasFile relationships and their inverses.

    * Optional transaction id will ensure the resources are created within said transaction.

## Membership Service

Operations to add/remove members from pcdm:Objects and pcdm:Collections.

**Endpoints**:

http://localhost:8080/islandora/object/{uuid}/members

http://localhost:8080/islandora/collection/{uuid}/members

**Actions**:

For brevity, only one of the two endpoints is described in the following section.

* `GET` http://localhost:8080/islandora/object/{parent_uuid}/members?tx={tx_id}

    * Retrieves a list of members associated with object identified by parent_uuid.

    * Optional transaction id will return the state of the list of members within said transaction.

* `POST` http://localhost:8080/islandora/object/{parent_uuid}/members/{child_uuid}?tx={tx_id}

    * Adds the resource identified by child_uuid to the object/collection identified by parent_uuid

    * Optional transaction id will ensure the operation is performed within said transaction.

* `DELETE` http://localhost:8080/islandora/object/{parent_uuid}/members/{child_uuid}?tx={tx_id}

    * Removes the resource identified by child_uuid from the object/collection identified by parent_uuid

    * Optional transaction id will ensure the operation is performed within said transaction.

## File Service

Lists all files for a pcdm:Object.

**Endpoint**:  http://localhost:8080/islandora/object/{uuid}/files

**Actions**:

* `GET` http://localhost:8080/islandora/object/{parent_uuid}/files?tx={tx_id}

    * Retrieves a list of files associated with object identified by parent_uuid.

    * Optional transaction id will return the state of the list of files within said transaction.

## Thumbnail Service

CRUD operations for thumbnails

**Endpoint**:  http://localhost:8080/islandora/object/{uuid}/thumbnail

**Actions**:

* `GET` http://localhost:8080/islandora/object/{uuid}/thumbnail?tx={tx_id}

    * Retrieves the thumbnail for the object identified by the provided uuid.  Setting the accept header to an RDF mimetype will return the RDF for the file.  Setting it to a binary mimetype will return the contents of the file.

    * Optional transaction id will return the thumbnail within said transaction.

* `PUT` http://localhost:8080/islandora/object/{uuid}/thumbnail?tx={tx_id}

    * Saves the thumbnail provided in the message body to the object identified by the provided uuid.  

    * Optional transaction id will save the thumbnail within said transaction.

* `DELETE` http://localhost:8080/islandora/object/{uuid}/thumbnail?tx={tx_id}

    * Removes the thumbnail for the object identified by the provided uuid.  

    * Optional transaction id will delete the thumbnail within said transaction.

## Preservation Master Service

CRUD operations for preservation masters

**Endpoint**:  http://localhost:8080/islandora/object/{uuid}/preservationMaster

**Actions**:

* `GET` http://localhost:8080/islandora/object/{uuid}/preservationMaster?tx={tx_id}

    * Retrieves the preservation master for the object identified by the provided uuid.  Setting the accept header to an RDF mimetype will return the RDF for the file.  Setting it to a binary mimetype will return the contents of the file.

    * Optional transaction id will return the file within said transaction.

* `PUT` http://localhost:8080/islandora/object/{uuid}/preservationMaster?tx={tx_id}

    * Saves the preservation master provided in the message body to the object identified by the provided uuid.  

    * Optional transaction id will save the file within said transaction.

* `DELETE` http://localhost:8080/islandora/object/{uuid}/preservationMaster?tx={tx_id}

    * Removes the preservation master for the object identified by the provided uuid.  

    * Optional transaction id will delete the file within said transaction.


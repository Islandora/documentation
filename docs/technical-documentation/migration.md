## fcrepo3 Object properties to fcrepo4

| fcrepo 3         | fcrepo4             | Example                  |
|------------------|---------------------|--------------------------|
| PID              | dcterms:identifier  | yul:328697               |
| state            | fedora:status       | Active                   |
| label            | dcterms:title       | Elvis Presley            |
| createDate       | fedora:created      | 2015-03-16T20:11:06.683Z |
| lastModifiedDate | fedora:lastModified | 2015-03-16T20:11:06.683Z |
| ownerId          |                     | nruest                   |

Notes:

* fcrepo4 will create a new createDate and lastModifiedDate, we will need to have a stragtegy for presevering the original createdDate and lastModifiedDate since this are not alterable.
* We will need to figure out a strategy for ownerID
* state will change with [FCREPO-1363](https://jira.duraspace.org/browse/FCREPO-1363)

## fcrepo3 Datastream properties to fcrepo4

| fcrepo3       | fcrepo4                                    | Example                                        |
|---------------|--------------------------------------------|------------------------------------------------|
| DSID          | dcterms:identifier                         | OBJ                                            |
| Label         | dcterms:title                              | ASC19109.tif                                   |
| MIME Type     | fedora:mimeType                            | image/tiff                                     |
| State         | fedora:status                              | Active                                         |
| Created       | fedora:created                             | 2015-03-16T20:11:06.683Z                       |
|               | fedora:lastModified                        |                                                |
| Versionable   | fedora:hasVersions                         | true                                           |
| Format URI    |                                            |                                                |
| Alternate IDs |                                            |                                                |
| Access URL    |                                            |                                                |
| Checksum      | fedora:hasFixityService + computedChecksum | SHA-1 c91342b705b15cb4f6ac5362cc6a47d9425aec86 |

Notes:

* Inline vs. Managed XML
* fcrepo4 will create a new createDate and lastModifiedDate, we will need to have a stragtegy for presevering the original createdDate and lastModifiedDate since this are not alterable.
* Do we have a use for or need to migrate State?
* Discuss fcrepo4 versioning
* * state will change with [FCREPO-1363](https://jira.duraspace.org/browse/FCREPO-1363)
* Verify proposed use of hasFixityService and computedChecksum
* Discuss whether or not we need:
  * Format URI
  * Alternate IDs
  * Access URL

## Diagram

Example Islandora Solution Pack Large Image object Fedora 4 Modeling

![Layer Interaction](https://raw.githubusercontent.com/wiki/Islandora-Labs/islandora/images/Islandora-SP-Large-Image-Fedora4.jpg)

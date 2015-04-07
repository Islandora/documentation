## fcrepo3 Object properties to fcrepo4

| fcrepo 3         | fcrepo4              | Example                  |
|------------------|----------------------|--------------------------|
| PID              | dcterms:identifier   | yul:328697               |
| state            | objState*            | Active                   |
| label            | dcterms:title        | Elvis Presley            |
| createDate       | fedora:created*      | 2015-03-16T20:11:06.683Z |
| lastModifiedDate | fedora:lastModified* | 2015-03-16T20:11:06.683Z |
| ownerId          | fedora:createdBy*    | nruest                   |

*immutable

Notes:

* Penn State ([fedora-migrate](https://github.com/projecthydra-labs/fedora-migrate))
  * createDate -> dc:dateSubmitted
  * lastModifiedDate -> dc:dateModified
* createDate -> premis:hasDateCreatedByApplication
* fedora:lastModified -> premis:hasEventDateTime + hasEventType == fcrepo3 migration
* ownerId -> premis:hasAgentName + premis:hasAgentNote == fcrepo3 migration

## fcrepo3 Datastream properties to fcrepo4

| fcrepo3       | fcrepo4                                                    | Example                                        |
|---------------|------------------------------------------------------------|------------------------------------------------|
| DSID          | dcterms:identifier                                         | OBJ                                            |
| Label         | dcterms:title                                              | ASC19109.tif                                   |
| MIME Type     | fedora:mimeType*                                           | image/tiff                                     |
| State         | objState                                                   | Active                                         |
| Created       | fedora:created*                                            | 2015-03-16T20:11:06.683Z                       |
|               | fedora:lastModified*                                       |                                                |
| Versionable   | fedora:hasVersions*                                        | true                                           |
| Format URI    | premis:formatDesignation                                   | info:pronom/fmt/156                            |
| Alternate IDs | dcterms:identifier                                         |                                                |
| Access URL    | dcterms:identifier                                         |                                                |
| Checksum      | premis:hasMessageDigestAlgorithm + premis:hasMessageDigest | SHA1, c91342b705b15cb4f6ac5362cc6a47d9425aec86 |

*immutable

Notes:

* Do we have a use for or need to migrate State?
* Discuss fcrepo4 versioning
* Discuss whether or not we need:
  * Alternate IDs
  * Access URL

## fcrepo3 RELS-EXT to fcrepo4 Mapping

| fcrepo3                                | Example                                                    | fcrepo4          | Example              |
|----------------------------------------|------------------------------------------------------------|------------------|----------------------|
| fedora:isMemberOfCollection            | rdf:resource="info:fedora/yul:F0433"                       | fedora:hasParent |                      |
| fedora-model:hasModel                  | rdf:resource="info:fedora/islandora:sp_large_image_cmodel" | rdf:type         | islandora:largeImage | 
| islandora:inheritXacmlFrom             | rdf:resource="info:fedora/yul:F0433"                       |                  |                      |
| islandora:hasLanguage                  | fra                                                        | dcterms:language | fra                  |
| islandora:isPageOf                     | rdf:resource="info:fedora/yul:336566"                      |                  |                      |
| islandora:isSequenceNumber             | 213                                                        |                  |                      |
| islandora:isPageNumber                 | 213                                                        |                  |                      |
| islandora:isSection                    | 1                                                          |                  |                      |
| fedora:isConstituentOf                 | rdf:resource="info:fedora/yul:271119"                      | fedora:hasParent |                      |
| islandora:isSequenceNumberOfyul_271119 | 1                                                          |                  |                      |
| islandora:dateIssued                   | 1945-10-31                                                 |                  |                      |
| islandora:isSequenceNumber             | 2023                                                       |                  |                      |

**Samples**

Large Image object
* * *
```xml
<rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:fedora-model="info:fedora/fedora-system:def/model#" xmlns:islandora="http://islandora.ca/ontology/relsext#">
  <rdf:Description rdf:about="info:fedora/yul:328697">
    <fedora:isMemberOfCollection rdf:resource="info:fedora/yul:F0433"></fedora:isMemberOfCollection>
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:sp_large_image_cmodel"></fedora-model:hasModel>
    <islandora:inheritXacmlFrom rdf:resource="info:fedora/yul:F0433"></islandora:inheritXacmlFrom>
  </rdf:Description>
</rdf:RDF>
```

Page object
* * *
```xml
<rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:fedora-model="info:fedora/fedora-system:def/model#" xmlns:islandora="http://islandora.ca/ontology/relsext#">
  <rdf:Description rdf:about="info:fedora/yul:336779">
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:pageCModel"></fedora-model:hasModel>
    <islandora:hasLanguage>fra</islandora:hasLanguage>
    <islandora:isPageOf rdf:resource="info:fedora/yul:336566"></islandora:isPageOf>
    <islandora:isSequenceNumber>213</islandora:isSequenceNumber>
    <islandora:isPageNumber>213</islandora:isPageNumber>
    <islandora:isSection>1</islandora:isSection>
    <fedora:isMemberOf rdf:resource="info:fedora/yul:336566"></fedora:isMemberOf>
    <islandora:inheritXacmlFrom rdf:resource="info:fedora/yul:336566"></islandora:inheritXacmlFrom>
  </rdf:Description>
</rdf:RDF>
```

Compound object (child)
* * *
```xml
<rdf:RDF xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:fedora-model="info:fedora/fedora-system:def/model#" xmlns:islandora="http://islandora.ca/ontology/relsext#" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="info:fedora/yul:271117">
    <fedora:isMemberOfCollection rdf:resource="info:fedora/yul:F0375"></fedora:isMemberOfCollection>
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:sp_large_image_cmodel"></fedora-model:hasModel>
    <islandora:inheritXacmlFrom rdf:resource="info:fedora/yul:F0375"></islandora:inheritXacmlFrom>
    <fedora:isConstituentOf rdf:resource="info:fedora/yul:271119"></fedora:isConstituentOf>
    <islandora:isSequenceNumberOfyul_271119>1</islandora:isSequenceNumberOfyul_271119>
  </rdf:Description>
</rdf:RDF>
```

Newspaper object
* * *
```xml
<rdf:RDF>
  <rdf:Description rdf:about="info:fedora/uofm:1243378">
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:newspaperCModel"/>
    <fedora:isMemberOfCollection rdf:resource="info:fedora/uofm:libraries"/>
  </rdf:Description>
</rdf:RDF>
```

Newspaper issue object
* * *
```xml
<rdf:RDF>
  <rdf:Description rdf:about="info:fedora/uofm:1351347">
    <fedora:isMemberOf rdf:resource="info:fedora/uofm:1243378"/>
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:newspaperIssueCModel"/>
    <islandora:dateIssued>1945-10-31</islandora:dateIssued>
    <islandora:isSequenceNumber>2023</islandora:isSequenceNumber>
  </rdf:Description>
</rdf:RDF>
```

Newspaper page object
* * *
```xml
<rdf:RDF>
  <rdf:Description rdf:about="info:fedora/uofm:1351348">
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:newspaperPageCModel"/>
    <fedora:isMemberOf rdf:resource="info:fedora/uofm:1351347"/>
    <islandora:isPageOf rdf:resource="info:fedora/uofm:1351347"/>
    <islandora:isSequenceNumber>1</islandora:isSequenceNumber>
    <islandora:isPageNumber>1</islandora:isPageNumber>
  </rdf:Description>
</rdf:RDF>
```

## Audit log migration

TODO: Document known audit log events, and provide examples.

## Diagram

Example Islandora Solution Pack Large Image object Fedora 4 Modeling

![Layer Interaction](https://raw.githubusercontent.com/wiki/Islandora-Labs/islandora/images/Islandora-SP-Large-Image-Fedora4.jpg)

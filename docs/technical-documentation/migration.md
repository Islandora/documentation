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
* fedora:lastModified -> premis:hasEventDateTime + hasEventType == [migration](http://id.loc.gov/vocabulary/preservation/eventType/mig.html)
* ownerId -> premis:hasAgentName + premis:hasAgentNote == [migration](http://id.loc.gov/vocabulary/preservation/eventType/mig.html)
* Can we nest the combined events, or do they have to flat?

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
| islandora:isMemberOf                   | islandora:sp_large_image_cmodel                            | fedora:hasParent |                      |
| fedora:isAnnotationOf                  | rdf:resource="info:fedora/islandora:96                     |                  |                      |
| islandora:targetedBy                   | admin                                                      |                  |                      |
| islandora:isAnnotationType             | my type                                                    |                  |                      |
| islandora:hasURN                       | urn:uuid:C691142D-FCC0-0001-F6B3-1390128014A5              |                  |                      |
| islandora:targets                      | info:fedora/islandora:96                                   |                  |                      |
| islandora:isViewableByUser             | nruest                                                     |                  |                      |
| islandora:isViewableByRole             | islandora creator                                          |                  |                      |
| islandora:isManageableByUser           | nruest                                                     |                  |                      |
| islandora:isManageableByRole           | islandora administrator                                    |                  |                      |

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

**Examples**:

addDatastream

```xml
<audit:record ID="AUDREC1">
<audit:process type="Fedora API-M"/>
<audit:action>addDatastream</audit:action>
<audit:componentID>TECHMD_FITS</audit:componentID>
<audit:responsibility>joanna</audit:responsibility>
<audit:date>2013-06-21T14:54:27.396Z</audit:date>
<audit:justification>Copied datastream from yul:61282.</audit:justification>
</audit:record>
```

modifyDatastreamByReference

```xml
<audit:record ID="AUDREC4">
<audit:process type="Fedora API-M"/>
<audit:action>modifyDatastreamByReference</audit:action>
<audit:componentID>TN</audit:componentID>
<audit:responsibility>fedoraAdmin</audit:responsibility>
<audit:date>2013-06-22T05:14:34.443Z</audit:date>
<audit:justification></audit:justification>
</audit:record>
```

modifyObject

```xml
<audit:record ID="AUDREC5">
<audit:process type="Fedora API-M"/>
<audit:action>modifyObject</audit:action>
<audit:componentID></audit:componentID>
<audit:responsibility>joanna</audit:responsibility>
<audit:date>2013-07-02T14:31:59.699Z</audit:date>
<audit:justification></audit:justification>
</audit:record>
```

modifyObject (checksum validation)

```xml
<audit:record ID="AUDREC23">
<audit:process type="Fedora API-M"/>
<audit:action>modifyObject</audit:action>
<audit:componentID></audit:componentID>
<audit:responsibility>anonymous</audit:responsibility>
<audit:date>2014-01-22T21:07:43.502Z</audit:date>
<audit:justification>PREMIS:file=yul:96031+FULL_TEXT+FULL_TEXT.0; PREMIS:eventType=fixity check; PREMIS:eventOutcome=SHA-1 checksum validated.</audit:justification>
</audit:record>
```

modifyDatastreamByValue

```xml
<audit:record ID="AUDREC16">
<audit:process type="Fedora API-M"/>
<audit:action>modifyDatastreamByValue</audit:action>
<audit:componentID>RELS-EXT</audit:componentID>
<audit:responsibility>nruest</audit:responsibility>
<audit:date>2013-11-27T15:42:08.823Z</audit:date>
<audit:justification></audit:justification>
</audit:record>
```
purgeDatastream

```xml
<audit:record ID="AUDREC15">
<audit:process type="Fedora API-M"/>
<audit:action>purgeDatastream</audit:action>
<audit:componentID>MKV</audit:componentID>
<audit:responsibility>fedoraAdmin</audit:responsibility>
<audit:date>2015-04-08T14:37:54.963Z</audit:date>
<audit:justification>Purged datastream (ID=MKV), versions ranging from the beginning of time to the end of time.  This resulted in the permanent removal of 1 datastream version(s) (2015-02-19T21:01:56.235Z) and all associated audit records.</audit:justification>
```

## Diagram

Example Islandora Solution Pack Large Image object Fedora 4 Modeling

![Layer Interaction](https://raw.githubusercontent.com/wiki/Islandora-Labs/islandora/images/Islandora-SP-Large-Image-Fedora4.jpg)

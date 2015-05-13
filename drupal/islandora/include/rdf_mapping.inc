<?php

/**
 * @file
 * RDF mapping utility functions.
 */

/**
 * Returns the default mapping for Islandora content types.
 *
 * @param string $bundle
 *   The bundle to associate with the RDF mapping.
 * @param string $rdf_type
 *   The RDF type to associate with the bundle.
 *
 * return array
 *   The default RDF mapping for Islandora content types.
 */
function islandora_get_default_rdf_mapping($bundle, $rdf_type) {
  return array(
    array(
      'type' => 'node',
      'bundle' => $bundle,
      'mapping' => array(
        'rdftype' => array($rdf_type),
        ISLANDORA_DCTERMS_CONTRIBUTOR_FIELD => array(
          'predicates' => array('dc:contributor'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_COVERAGE_FIELD => array(
          'predicates' => array('dc:coverage'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_CREATOR_FIELD => array(
          'predicates' => array('dc:creator'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_DATE_FIELD => array(
          'predicates' => array('dc:date'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_DESCRIPTION_FIELD => array(
          'predicates' => array('dc:description'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_FORMAT_FIELD => array(
          'predicates' => array('dc:format'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_IDENTIFIER_FIELD => array(
          'predicates' => array('dc:identifier'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_LANGUAGE_FIELD => array(
          'predicates' => array('dc:language'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_PUBLISHER_FIELD => array(
          'predicates' => array('dc:publisher'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_RELATION_FIELD => array(
          'predicates' => array('dc:relation'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_RIGHTS_FIELD => array(
          'predicates' => array('dc:rights'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_SOURCE_FIELD => array(
          'predicates' => array('dc:source'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_SUBJECT_FIELD => array(
          'predicates' => array('dc:subject'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_TITLE_FIELD => array(
          'predicates' => array('dc:title'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DCTERMS_TYPE_FIELD => array(
          'predicates' => array('dc:type'),
          'datatype' => 'xsd:string',
        ),
      ),
    ),
  );
}

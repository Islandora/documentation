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
        'uuid' => array(
          'predicates' => array('dc11:identifier'),
          'datatype' => 'xsd:ID'
        ),
        ISLANDORA_FEDORA_HAS_PARENT => array(
          'predicates' => array('fedora:hasParent'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_PCDM_HAS_MEMBER => array(
          'predicates' => array('pcdm:hasMember'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_CONTRIBUTOR_FIELD => array(
          'predicates' => array('dc11:contributor'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_COVERAGE_FIELD => array(
          'predicates' => array('dc11:coverage'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_CREATOR_FIELD => array(
          'predicates' => array('dc11:creator'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_DATE_FIELD => array(
          'predicates' => array('dc11:date'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_DESCRIPTION_FIELD => array(
          'predicates' => array('dc11:description'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_FORMAT_FIELD => array(
          'predicates' => array('dc11:format'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_IDENTIFIER_FIELD => array(
          'predicates' => array('dc11:identifier'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_LANGUAGE_FIELD => array(
          'predicates' => array('dc11:language'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_PUBLISHER_FIELD => array(
          'predicates' => array('dc11:publisher'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_RELATION_FIELD => array(
          'predicates' => array('dc11:relation'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_RIGHTS_FIELD => array(
          'predicates' => array('dc11:rights'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_SOURCE_FIELD => array(
          'predicates' => array('dc11:source'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_SUBJECT_FIELD => array(
          'predicates' => array('dc11:subject'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_TITLE_FIELD => array(
          'predicates' => array('dc11:title'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_DC_TYPE_FIELD => array(
          'predicates' => array('dc11:type'),
          'datatype' => 'xsd:string',
        ),
      ),
    ),
  );
}

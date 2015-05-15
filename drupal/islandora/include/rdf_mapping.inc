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
 * @return array
 *   The default RDF mapping for Islandora content types.
 */
function islandora_get_default_rdf_mapping($bundle, $rdf_types) {
  return array(
    array(
      'type' => 'node',
      'bundle' => $bundle,
      'mapping' => array(
        'rdftype' => $rdf_types,
        'uuid' => array(
          'predicates' => array('fedora:uuid'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_FEDORA_HAS_PARENT_FIELD => array(
          'predicates' => array('fedora:hasParent'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_PCDM_HAS_MEMBER_FIELD => array(
          'predicates' => array('pcdm:hasMember'),
          'datatype' => 'xsd:string',
        ),
      ),
    ),
  );
}

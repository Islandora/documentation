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
 * @param array $rdf_types
 *   RDF types to apply on top of the Fedora defaults.
 *
 * @return array
 *   The default RDF mapping for Islandora content types.
 */
function islandora_get_default_rdf_mapping($bundle, array $rdf_types) {
  $default_rdf_types = array(
    'ldp:RDFSource',
    'ldp:Container',
    'fedora:Resource',
    'fedora:Container',
  );

  $rdf_types = array_merge($default_rdf_types, $rdf_types);

  return array(
    array(
      'type' => 'node',
      'bundle' => $bundle,
      'mapping' => array(
        'rdftype' => $rdf_types,
        'uuid' => array(
          'predicates' => array('nfo:uuid'),
          'datatype' => 'xsd:string',
        ),
        ISLANDORA_FEDORA_HAS_PARENT_FIELD => array(
          'predicates' => array('fedora:hasParent'),
          'type' => 'rel',
        ),
        ISLANDORA_PCDM_HAS_MEMBER_FIELD => array(
          'predicates' => array('pcdm:hasMember'),
          'type' => 'rel',
        ),
      ),
    ),
  );
}

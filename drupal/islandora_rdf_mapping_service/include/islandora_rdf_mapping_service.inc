<?php

/**
 * @file
 * Provides the callback implementations for the service.
 */

/**
 * The GET callback for the RDF mapping service.
 *
 * @param string $type
 *   Entity type to which the RDF mapping belongs.
 * @param string $bundle
 *   Bundle to which the RDF mapping belongs.
 *
 * @return array
 *   The RDF mapping.
 */
function islandora_rdf_mapping_service_retrieve($type, $bundle) {
  return array(
    'rdf_mapping' => rdf_mapping_load($type, $bundle),
    'rdf_namespaces' => rdf_get_namespaces(),
    'pcdm_file_mapping' => module_invoke_all('pcdm_file_mapping'),
  );
}

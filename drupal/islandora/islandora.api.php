<?php

/**
 * @file
 * Hooks provided by Islandora.
 */

/**
 * Hook to collect pcdm:File to field mappings.
 *
 * @return array
 *   An associative array, where keys are the name of the field and the values
 *   are dc:identifier of the associated pcdm:File (e.g. DSID for those using
 *   the old vocabulary).
 */
function hook_pcdm_file_mapping() {
  return array(
    'field_mods' => array(
      'id' => 'MODS',
      'mimetype' => 'application/xml',
    ),
  );
}

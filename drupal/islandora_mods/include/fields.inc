<?php

/**
 * @file
 * Utility functions for working with MODS fields.
 */

/**
 * Adds all the MODS fields to a bundle.
 *
 * @param string $bundle_name
 *   The name of the bundle to give the fields.
 */
function islandora_mods_add_fields_to_bundle($bundle_name) {
  module_load_include('inc', 'islandora', 'include/fields');
  islandora_add_field_to_bundle($bundle_name, ISLANDORA_MODS_FIELD, "MODS XML", "A MODS record for the Fedora resource");
}

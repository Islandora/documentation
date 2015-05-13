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
  // If this gets called from an install hook, we can't guarantee the t's
  // existance.
  $t = get_t();

  if (field_info_field(ISLANDORA_MODS_FIELD)) {
    $mods_field_instance = array(
      'field_name' => ISLANDORA_MODS_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("MODS XML"),
      'description' => $t("A MODS record for the Fedora resource"),
      'required' => FALSE,
    );
    field_create_instance($mods_field_instance);
    $message = $t('Field @name was successfully added to @bundle',
                  array(
                    '@name' => ISLANDORA_MODS_FIELD,
                    '@bundle' => $bundle_name,
                  ));
    drupal_set_message($message);
  }
}

<?php

/**
 * @file
 * Utility functions for attaching default fields to collections.
 */

/**
 * Attaches default fields for this module's content type.
 */
function islandora_collection_attach_default_fields() {
  module_load_include('inc', 'islandora_dcterms', 'include/fields');

  $t = get_t();

  // Add thumbnail field.
  if (field_info_field(ISLANDORA_TN_FIELD)) {
    $tn_field_instance = array(
      'field_name' => ISLANDORA_TN_FIELD,
      'entity_type' => 'node',
      'bundle' => ISLANDORA_COLLECTION_CONTENT_TYPE,
      'label' => $t("Thumbnail"),
      'description' => $t("A thumbnail for the Fedora resource"),
      'required' => FALSE,
    );
    field_create_instance($tn_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_TN_FIELD)));
  }

  // Add MODS field.
  if (field_info_field(ISLANDORA_MODS_FIELD)) {
    $mods_field_instance = array(
      'field_name' => ISLANDORA_MODS_FIELD,
      'entity_type' => 'node',
      'bundle' => ISLANDORA_COLLECTION_CONTENT_TYPE,
      'label' => $t("MODS"),
      'description' => $t("A MODS record for the Fedora resource"),
      'required' => FALSE,
    );
    field_create_instance($mods_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_MODS_FIELD)));
  }

  // Add dcterms fields.
  islandora_dcterms_attach_fields_to_bundle(ISLANDORA_COLLECTION_CONTENT_TYPE);
}

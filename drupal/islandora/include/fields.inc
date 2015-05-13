<?php

/**
 * @file
 * Utility functions for working Islandora core fields.
 */

/**
 * Adds all the core Islandora fields to a bundle.
 *
 * @param string $bundle_name
 *   The name of the bundle to give the fields.
 */
function islandora_add_fields_to_bundle($bundle_name) {
  // If this gets called from an install hook, we can't guarantee the t's
  // existance.
  $t = get_t();

  // Big list of field names/labels/descriptions.
  $field_data = array(
    ISLANDORA_TN_FIELD => array(
      'label' => 'Thumbnail',
      'description' => 'Thumbnail image',
    ),
    ISLANDORA_FEDORA_HAS_PARENT_FIELD => array(
      'label' => 'fedora:hasParent',
      'description' => 'Parent resource in Fedora 4',
    ),
    ISLANDORA_PCDM_HAS_MEMBER_FIELD => array(
      'label' => 'pcdm:hasMember',
      'description' => 'Child resources in Fedora 4',
    ),
  );

  // Iterate over fields and add each to the specified bundle.
  foreach ($field_data as $field_name => $data) {
    $field_label = $data['label'];
    $field_description = $data['description'];

    if (field_info_field($field_name)) {
      $field_instance = array(
        'field_name' => $field_name,
        'entity_type' => 'node',
        'bundle' => $bundle_name,
        'label' => $t("%label", array("%description" => $field_label)),
        'description' => $t("%description", array("%description" => $field_description)),
        'required' => FALSE,
      );
      field_create_instance($field_instance);
      $message = $t('Field %name was added to %bundle successfully',
                    array(
                      '%name' => $field_name,
                      '%bundle' => $bundle_name,
                    ));
      drupal_set_message($message);
    }
  }
}

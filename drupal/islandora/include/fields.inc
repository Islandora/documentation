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
  // Big list of field names/labels/descriptions.
  $field_data = array(
    ISLANDORA_TN_FIELD => array(
      'label' => 'Thumbnail',
      'description' => 'Thumbnail image',
    ),
    ISLANDORA_FEDORA_PATH_FIELD => array(
      'label' => 'Fedora Path',
      'description' => 'Path to resource in Fedora 4',
    ),
    ISLANDORA_FEDORA_HAS_PARENT_FIELD => array(
      'label' => 'fedora:hasParent',
      'description' => 'Parent resource in Fedora 4',
    ),
    ISLANDORA_PCDM_HAS_MEMBER_FIELD => array(
      'label' => 'pcdm:hasMember',
      'description' => 'Child resources in Fedora 4',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    ),
  );

  // Iterate over fields and add each to the specified bundle.
  foreach ($field_data as $field_name => $data) {
    $field_label = $data['label'];
    $field_description = $data['description'];

    islandora_add_field_to_bundle($bundle_name, $field_name, $field_label, $field_description);
  }
}

/**
 * Adds a field to a bundle.
 *
 * @param string $bundle_name
 *   The machine name of the bundle.
 * @param string $field_name
 *   The machine name of the field.
 * @param string $field_label
 *   The human-readable label for the field.
 * @param string $field_description
 *   The human-readable description for the field.
 */
function islandora_add_field_to_bundle($bundle_name, $field_name, $field_label, $field_description) {
  // If this gets called from an install hook, we can't guarantee the t's
  // existance.
  $t = get_t();

  if (field_info_field($field_name)) {
    $field_instance = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("@label", array("@label" => $field_label)),
      'description' => $t("@description", array("@description" => $field_description)),
      'required' => FALSE,
    );
    field_create_instance($field_instance);
    $message = $t('Field @name was added to @bundle successfully',
                  array(
                    '@name' => $field_name,
                    '@bundle' => $bundle_name,
                  ));
    drupal_set_message($message);
  }
}

/**
 * Creates a field from a field definition.
 *
 * @param array $field_definition
 *   Field definition for the desired field.
 */
function islandora_create_field($field_definition) {
  // If this gets called from an install hook, we can't guarantee the t's
  // existance.
  $t = get_t();

  field_create_field($field_definition);
  drupal_set_message($t("Created field @field",
                     array("@field" => $field_definition['field_name'])));
}

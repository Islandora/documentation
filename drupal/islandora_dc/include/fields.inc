<?php

/**
 * @file
 * Utility functions for working with DC fields.
 */

/**
 * Adds all the DC fields to a bundle.
 *
 * @param string $bundle_name
 *   The name of the bundle to give the fields.
 */
function islandora_dc_add_fields_to_bundle($bundle_name) {

  // If this gets called from an install hook, we can't guarantee the t's
  // existance.
  $t = get_t();

  // Big list of field names/labels/descriptions.
  $field_data = array(
    ISLANDORA_DC_CONTRIBUTOR_FIELD => array(
      'label' => 'DC Contributor',
      'description' => 'Dublin Core 1.1 Contributor Metadata',
    ),
    ISLANDORA_DC_COVERAGE_FIELD => array(
      'label' => 'DC Coverage',
      'description' => 'Dublin Core 1.1 Coverage Metadata',
    ),
    ISLANDORA_DC_CREATOR_FIELD => array(
      'label' => 'DC Creator',
      'description' => 'Dublin Core 1.1 Creator Metadata',
    ),
    ISLANDORA_DC_DATE_FIELD => array(
      'label' => 'DC Date',
      'description' => 'Dublin Core 1.1 Date Metadata',
    ),
    ISLANDORA_DC_DESCRIPTION_FIELD => array(
      'label' => 'DC Description',
      'description' => 'Dublin Core 1.1 Description Metadata',
    ),
    ISLANDORA_DC_FORMAT_FIELD => array(
      'label' => 'DC Format',
      'description' => 'Dublin Core 1.1 Format Metadata',
    ),
    ISLANDORA_DC_IDENTIFIER_FIELD => array(
      'label' => 'DC Identifier',
      'description' => 'Dublin Core 1.1 Identifier Metadata',
    ),
    ISLANDORA_DC_LANGUAGE_FIELD => array(
      'label' => 'DC Language',
      'description' => 'Dublin Core 1.1 Language Metadata',
    ),
    ISLANDORA_DC_PUBLISHER_FIELD => array(
      'label' => 'DC Publisher',
      'description' => 'Dublin Core 1.1 Publisher Metadata',
    ),
    ISLANDORA_DC_RELATION_FIELD => array(
      'label' => 'DC Relation',
      'description' => 'Dublin Core 1.1 Relation Metadata',
    ),
    ISLANDORA_DC_RIGHTS_FIELD => array(
      'label' => 'DC Rights',
      'description' => 'Dublin Core 1.1 Rights Metadata',
    ),
    ISLANDORA_DC_SOURCE_FIELD => array(
      'label' => 'DC Source',
      'description' => 'Dublin Core 1.1 Source Metadata',
    ),
    ISLANDORA_DC_SUBJECT_FIELD => array(
      'label' => 'DC Subject',
      'description' => 'Dublin Core 1.1 Subject Metadata',
    ),
    ISLANDORA_DC_TITLE_FIELD => array(
      'label' => 'DC Title',
      'description' => 'Dublin Core 1.1 Title Metadata',
    ),
    ISLANDORA_DC_TYPE_FIELD => array(
      'label' => 'DC Type',
      'description' => 'Dublin Core 1.1 Type Metadata',
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
                      '%bundle' => $bundle,
                    ));
      drupal_set_message($message);
    }
  }
}

<?php

/**
 * @file
 * Utility functions for working with dcterms fields.
 */

/**
 * Attaches all the dcterms fields to a bundle.
 *
 * @param string $bundle_name
 *   The name of the bundle to give the fields.
 */
function islandora_dcterms_attach_fields_to_bundle($bundle_name) {
  $t = get_t();

  if (field_info_field(ISLANDORA_DCTERMS_CONTRIBUTOR_FIELD)) {
    $dcterms_contributor_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_CONTRIBUTOR_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Contributor"),
      'description' => $t("Dublin Core Contributor Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_contributor_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_CONTRIBUTOR_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_COVERAGE_FIELD)) {
    $dcterms_coverage_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_COVERAGE_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Coverage"),
      'description' => $t("Dublin Core Coverage Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_coverage_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_COVERAGE_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_CREATOR_FIELD)) {
    $dcterms_creator_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_CREATOR_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Creator"),
      'description' => $t("Dublin Core Creator Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_creator_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_CREATOR_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_DATE_FIELD)) {
    $dcterms_date_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_DATE_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Date"),
      'description' => $t("Dublin Core Date Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_date_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_DATE_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_DESCRIPTION_FIELD)) {
    $dcterms_description_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_DESCRIPTION_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Description"),
      'description' => $t("Dublin Core Description Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_description_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_DESCRIPTION_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_FORMAT_FIELD)) {
    $dcterms_format_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_FORMAT_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Format"),
      'description' => $t("Dublin Core Format Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_format_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_FORMAT_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_IDENTIFIER_FIELD)) {
    $dcterms_identifier_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_IDENTIFIER_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Identfier"),
      'description' => $t("Dublin Core Identifier Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_identifier_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_IDENTIFIER_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_LANGUAGE_FIELD)) {
    $dcterms_language_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_LANGUAGE_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Language"),
      'description' => $t("Dublin Core Language Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_language_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_LANGUAGE_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_PUBLISHER_FIELD)) {
    $dcterms_publisher_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_PUBLISHER_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Publisher"),
      'description' => $t("Dublin Core Publisher Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_publisher_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_PUBLISHER_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_RELATION_FIELD)) {
    $dcterms_relation_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_RELATION_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Relation"),
      'description' => $t("Dublin Core Relation Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_relation_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_RELATION_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_RIGHTS_FIELD)) {
    $dcterms_rights_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_RIGHTS_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Rights"),
      'description' => $t("Dublin Core Rights Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_rights_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_RIGHTS_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_SOURCE_FIELD)) {
    $dcterms_source_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_SOURCE_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Source"),
      'description' => $t("Dublin Core Source Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_source_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_SOURCE_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_SUBJECT_FIELD)) {
    $dcterms_subject_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_SUBJECT_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Subject"),
      'description' => $t("Dublin Core Subject Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_subject_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_SUBJECT_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_TITLE_FIELD)) {
    $dcterms_title_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_TITLE_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Title"),
      'description' => $t("Dublin Core Title Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_title_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_TITLE_FIELD)));
  }

  if (field_info_field(ISLANDORA_DCTERMS_TYPE_FIELD)) {
    $dcterms_type_field_instance = array(
      'field_name' => ISLANDORA_DCTERMS_TYPE_FIELD,
      'entity_type' => 'node',
      'bundle' => $bundle_name,
      'label' => $t("DC Terms Type"),
      'description' => $t("Dublin Core Type Metadata"),
      'required' => FALSE,
    );
    field_create_instance($dcterms_type_field_instance);
    drupal_set_message($t('Field %name was created successfully', array('%name' => ISLANDORA_DCTERMS_TYPE_FIELD)));
  }
}

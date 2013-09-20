<?php
/**
 * @file
 * Defines functions used when viewing metadata displays on Islandora objects.
 */

/**
 * Retrieves the metadata display markup for an Islandora object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return string
 *   Markup to be rendered for display on Islandora object pages.
 */
function islandora_retrieve_metadata_markup(AbstractObject $object) {
  $viewers = module_invoke_all('islandora_metadata_display_info');
  $viewer = variable_get('islandora_metadata_display', 'dublin_core');

  if (isset($viewers[$viewer]['callback'])) {
    return call_user_func($viewers[$viewer]['callback'], $object);
  }
  else {
    return '';
  }
}

/**
 * Form used for choosing which default metadata display to use for viewing.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   An array representing the metadata display viewer form.
 */
function islandora_metadata_display_form($form, $form_state) {
  module_load_include('inc', 'islandora', 'includes/solution_packs.inc');
  $form = array();
  $defined_displays = module_invoke_all('islandora_metadata_display_info');
  if (!empty($defined_displays)) {
    $no_viewer = array();
    $no_viewer['none'] = array(
      'label' => t('None'),
      'description' => t("Don't show any metadata for displaying"),
    );
    $viewers = array_merge_recursive($no_viewer, $defined_displays);

    $form['viewers'] = array(
      '#type' => 'item',
      '#title' => t('Select a viewer'),
      '#description' => t('Preferred metadata display for Islandora. These may be provided by third-party modules.'),
      '#tree' => TRUE,
      '#theme' => 'islandora_viewers_table',
    );

    foreach ($viewers as $name => $profile) {
      $options[$name] = '';
      $form['viewers']['name'][$name] = array(
        '#type' => 'hidden',
        '#value' => $name,
      );
      $form['viewers']['label'][$name] = array(
        '#type' => 'item',
        '#markup' => $profile['label'],
      );
      $form['viewers']['description'][$name] = array(
        '#type' => 'item',
        '#markup' => $profile['description'],
      );
      $form['viewers']['configuration'][$name] = array(
        '#type' => 'item',
        '#markup' => (isset($profile['configuration']) AND $profile['configuration'] != '') ? l(t('configure'), $profile['configuration']) : '',
      );
    }
    $form['viewers']['default'] = array(
      '#type' => 'radios',
      '#options' => isset($options) ? $options : array(),
      '#default_value' => variable_get('islandora_metadata_display', 'dublin_core'),
    );
  }
  else {
    $form['viewers']['no_viewers'] = array(
      '#markup' => t('No viewers detected.'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for the metadata display form which sets the default viewer.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_metadata_display_form_submit($form, $form_state) {
  variable_set('islandora_metadata_display', $form_state['values']['viewers']['default']);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Metadata display callback for rendering Dublin Core metadata.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return string
 *   Markup representing the rendered metadata from Dublin Core.
 */
function islandora_metadata_display_callback(AbstractObject $object) {
  $elements = array(
    'islandora_object' => $object,
  );
  return theme('islandora_dublin_core_display', $elements);
}

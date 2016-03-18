<?php

/**
 * @file
 * Admin form.
 */

/**
 * Form function for module config.
 *
 * @param array $form
 *   Drupal renderable array.
 * @param array $form_state
 *   Drupal form state.
 *
 * @return array
 *   Final form to render.
 */
function islandora_config_form(array $form, array &$form_state) {
  $form = array(
    'islandora_karaf_url' => array(
      '#type' => 'textfield',
      '#title' => t('Karaf URL'),
      '#default_value' => variable_get('islandora_karaf_url', 'http://127.0.0.1:8181'),
      '#description' => t('URL to the Karaf'),
      '#element_validate' => array('islandora_config_form_validate_karaf_url'),
      '#required' => TRUE,
    ),
    'islandora_stomp_url' => array(
      '#type' => 'textfield',
      '#title' => t('Stomp URL'),
      '#default_value' => variable_get('islandora_stomp_url', 'tcp://localhost:61613'),
      '#description' => t('URL to the Stomp broker for async messaging'),
      '#element_validate' => array('islandora_config_form_validate_stomp_url'),
      '#required' => TRUE,
    ),
    'islandora_triplestore_index_queue' => array(
      '#type' => 'textfield',
      '#title' => t('Triplestore Index Queue'),
      '#default_value' => variable_get('islandora_triplestore_index_queue', 'islandora/triplestore/index'),
      '#description' => t('Queue to send triplestore indexing messages'),
      '#required' => TRUE,
    ),
  );

  return system_settings_form($form);
}

/**
 * Stomp broker url validator.
 *
 * @param array $element
 *   Form element for stomp broker url.
 * @param array $form_state
 *   Drupal form state.
 * @param array $form
 *   Drupal renderable array.
 */
function islandora_config_form_validate_stomp_url(array $element, array &$form_state, array $form) {
  // Try to connect to the server.
  try {
    $url = $element['#value'];
    $stomp = new Stomp($url);
    unset($stomp);
  }
  catch (StompException $e) {
    form_error($element, t('Connection failed for @url: @message', array('@url' => $url, '@message' => $e->getMessage())));
  }
}

/**
 * Karaf url validator.
 *
 * @param array $element
 *   Form element for stomp broker url.
 * @param array $form_state
 *   Drupal form state.
 * @param array $form
 *   Drupal renderable array.
 */
function islandora_config_form_validate_karaf_url(array $element, array &$form_state, array $form) {
  // Try to connect to the server.
  $url = $element['#value'] . '/cxf';
  $handle = curl_init($url);
  curl_setopt($handle,  CURLOPT_RETURNTRANSFER, TRUE);
  $response = curl_exec($handle);
  $http_code = curl_getinfo($handle, CURLINFO_HTTP_CODE);
  if ($http_code != 200) {
    form_error($element, t('Connection failed for @url', array('@url' => $url)));
  }
  curl_close($handle);
}

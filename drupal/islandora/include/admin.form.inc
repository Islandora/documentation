<?php

/**
 * @file
 * Admin form
 */

function islandora_config_form(array $form, array &$form_state) {
  $form = array(
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

function islandora_config_form_validate_stomp_url(array $element, array &$form_state, array $form) {
  // Try to connect to the server.
  try {
    $url = $element['#value'];
    $stomp = new Stomp($url);
    unset($stomp);
  } catch(StompException $e) {
    form_error($element, t('Connection failed for @url: @message', array('@url' => $url, '@message' => $e->getMessage())));
  }
}

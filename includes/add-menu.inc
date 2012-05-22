<?php

function islandora_add_callback($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $user = new stdClass();
  $user->name = 'fedoraAdmin';
  $user->pass = 'password';
  $connection = new RestConnection($user);
  $object = $connection->repository->getObject($pid);
  $models = $object->models;

  return drupal_get_form('islandora_add_form', $models, $object);
}

function islandora_add_form($object) {
  $form = array ();
  return $form;
}

function islandora_form_alter(&$form, &$form_state, $form_id) {
  if($form_id == 'islandora_add_form') {
    $form['islandora_repository_pid'] = array(
      '#type' => 'textfield',
      '#title' => t('Root Collection PID'),
      '#default_value' => variable_get('islandora_repository_pid', 'islandora:root'),
      '#description' => t('The PID of the Root Collection Object'),
      '#required' => TRUE,
      '#weight' => -18
    );
  }
}
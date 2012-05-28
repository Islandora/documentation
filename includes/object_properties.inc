<?php

function islandora_edit_properties_form ($form, &$form_state, $object_id){
  $form = array();
  $islandora_object = islandora_get_object($object_id);
  if(!isset($islandora_object)){
    return NULL;
  }
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $object_id,
  );

  $form['object_label'] = array(
    '#title' => t('Item Label'),
    '#default_value' => $islandora_object->label,
    '#required' => 'TRUE',
    '#description' => t('A Human readable label'),
    '#type' => 'textfield'
  );
  $form['object_owner'] = array(
    '#title' => t('Owner'),
    '#default_value' => $islandora_object->owner,
    '#required' => FALSE,
    '#description' => t('The owner id'),
    '#type' => 'textfield',
  );
  $form['object_state'] = array(
    '#title' => t('State'),
    '#default_value' => $islandora_object->state,
    '#required' => TRUE,
    '#description' => t('The items state, either active, inactive or deleted'),
    '#type' => 'select',
    '#options' => array ('A' => 'Active', 'I' => 'Inactive', 'D'=>'Deleted'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update Properties',
  );
  return $form;
}
?>

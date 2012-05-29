<?php

function islandora_edit_properties_form_validate($form, &$form_state){
  $islandora_object = islandora_get_object($form_state['values']['pid']);
  if(!isset($islandora_object)){
    form_set_error('', t('Could not update properties object not found.'));
    return FALSE;
  }
}

/**
 *
 * @param array $form
 * @param array $form_state 
 */
function islandora_edit_properties_form_submit($form, &$form_state){
  $islandora_object = islandora_get_object($form_state['values']['pid']);
  $owner = $form_state['values']['object_owner'];
  $state = $form_state['values']['object_state'];
  $label = $form_state['values']['object_label'];
  $submit = FALSE;
  $params = array();
  if(isset($owner) && $owner != $islandora_object->owner){
    $params['ownerId'] = check_plain($owner);
    $submit = TRUE;
  }
  if(isset($state) && $state != $islandora_object->state){
    $params['state'] = check_plain($state);
    $submit = TRUE;
  }
  if(isset($label) && $label != $islandora_object->label){
    $params['label'] = check_plain($label);
    $submit = TRUE;
  }
  if($submit){
    $islandora_object->modifyObject($params);
  }
}

/**
 *
 * @param array $form
 * @param array $form_state
 * @param string $object_id
 *   an object id
 * @return array 
 */
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
    '#description' => t('The items state one of active, inactive or deleted'),
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

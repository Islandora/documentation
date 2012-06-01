<?php

/**
 * @file
 * ChildCollection.inc
 */

/**
 * Create the child collection creation form
 * 
 * @param array $form
 * @param array $form_state
 * @param string $this_collection_pid
 * @return array
 */
function islandora_create_child_collection_form($form, &$form_state, $collection_object) {
  module_load_include('inc', 'islandora_basic_collection', 'includes/CollectionPolicy');

  $policy_datastream = $collection_object->getDatastream(variable_get('Islandora_Collection_Policy_DSID', 'COLLECTION_POLICY'));
  $collection_policy = new CollectionPolicy($policy_datastream->content);
  $current_content_models = $collection_policy->getContentModels();
  
  $collection_content_model_exists = FALSE;
  
  foreach ($current_content_models as $current_content_model) {
    if ($current_content_model['pid'] == 'islandora:collectionCModel') {
      $collection_content_model_exists = TRUE;
    }
  }
  
  if (!$collection_content_model_exists) {
    $form['not_collection'] = array(
      '#type' => 'item',
      '#title' => t("This object can't contain child collections. To fix this add the islandora:collectionCModel to this object's collection policy"),
    );
    return $form;
  }
  $restricted = FALSE;
  if (variable_get('islandora_namespace_restriction_enforced', FALSE)) {
    $restricted = TRUE;
    $allowed_string = variable_get('islandora_pids_allowed', 'default: demo: changeme: islandora:');
    $namespaces = explode(':', $allowed_string);
    foreach ($namespaces as $namespace) {
      if ($namespace) {
        $allowed[trim($namespace)] = trim($namespace);
      }
    }
  }
  $collection_namespace = substr($collection_object->id, 0, strpos($collection_object->id, ":"));

  $content_models = get_content_models_as_option_array();
  unset($content_models['islandora:collectionCModel']);

  $form['child_creation']['titlebox'] = array(
    '#type' => 'item',
    '#title' => t("Create new child collection within @collection", array('@collection' => $collection_object->id)),
  );

  $form['child_creation']['collection_name'] = array(
    '#title' => "Collection name",
    '#type' => 'textfield',
    '#size' => 25,
    '#description' => t("Human readable name for this collection"),
  );

  $form['child_creation']['new_collection_pid'] = array(
    '#title' => "Collection PID",
    '#type' => 'textfield',
    '#size' => 15,
    '#default_value' => '',
    '#description' => t("Unique PID for this collection. Leave blank for default. <br />Pids take the general form of namespace:collection (eg. islandora:pamphlets)"),
  );

  if (!$restricted) {
    $form['child_creation']['collection_namespace'] = array(
      '#title' => "Collection namespace",
      '#type' => 'textfield',
      '#size' => 15,
      '#default_value' => $collection_namespace,
      '#description' => t("Namespace for objects in this collection."),
    );
  }
  else {
    $form['child_creation']['collection_namespace'] = array(
      '#title' => "Collection namespace",
      '#type' => 'select',
      '#options' => $allowed,
      '#default_value' => 'default',
      '#description' => t("Namespace for objects in this collection."),
    );
  }

  $form['current'] = array(
    '#type' => 'hidden',
    '#value' => $collection_object->id,
  );

  $form['child_creation']['content_models'] = array(
    '#title' => "Choose allowable content models for this collection",
    '#type' => 'checkboxes',
    '#options' => $content_models,
    '#description' => t("Content models describe the behaviours of objects with which they are associated."),
  );

  $form['child_creation']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create collection'),
    '#id' => 'create_child'
  );
  return $form;
}

function islandora_create_child_collection_form_validate($form, &$form_state) {
  
}

/**
 * Submit handler for child collection creation form
 * 
 * @global type $base_url
 * @param array $form
 * @param array $form_state 
 */
function islandora_create_child_collection_form_submit($form, &$form_state) {
  global $base_url;
  module_load_include('inc', 'islandora', '/includes/islandora.ingest');
  $thumbnail = $base_url . '/' . drupal_get_path('module', 'islandora_basic_collection') . '/Crystal_Clear_filesystem_folder_grey.png';
  $new_collection_pid = $form_state['values']['new_collection_pid'];
  $this_collection_pid = $form_state['values']['current'];
  if(empty($new_collection_pid)) {
    $collection_namespace = substr($this_collection_pid, 0, strpos($this_collection_pid, ":"));
  }
  $new_collection_label = $form_state['values']['collection_name'];
  $namespace = $form_state['values']['collection_namespace'];
  $all_cModels = get_content_models_as_option_array();
  $collection_content_models = array(array('pid' => 'islandora:collectionCModel'));
  $relationship = 'isMemberOfCollection';

  $collection_policy = '<?xml version="1.0" encoding="UTF-8"?>
<collection_policy xmlns="http://www.islandora.ca" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" name="" xsi:schemaLocation="http://www.islandora.ca http://syn.lib.umanitoba.ca/collection_policy.xsd">
    <content_models>
    <content_model dsid="ISLANDORACM" name="Islandora Collection Model ~ islandora:collectionCModel" namespace="islandora:1" pid="islandora:collectionCModel"/>
    </content_models>
    <search_terms>
    </search_terms>
    <staging_area></staging_area>
    <relationship>isMemberOfCollection</relationship>
</collection_policy>';

  $content_models = $form_state['values']['content_models'];

  $collection_policy_xml = new DOMDocument();
  $collection_policy_xml->loadXML($collection_policy);
  
  $content_models_element = $collection_policy_xml->getElementsByTagName('content_models');
  $content_model_element = $content_models_element->item(0)->getElementsByTagName('content_model');
  foreach ($content_models as $content_model) {
    if ($content_model) {
      $content_model_element = $collection_policy_xml->createElement('content_model');
      $content_model_element->setAttribute('name', $all_cModels[$content_model]);
      $content_model_element->setAttribute('dsid', variable_get('Islandora_Content_Model_DSID', 'ISLANDORACM'));
      $content_model_element->setAttribute('namespace', $namespace . ':1');
      $content_model_element->setAttribute('pid', $content_model);
      $content_models_element->item(0)->appendChild($content_model_element);
    }
  }
  $fedora_object = islandora_ingest_get_object($collection_content_models, $form_state['values']['current'], $relationship, $new_collection_pid);
  $fedora_object->label = $new_collection_label;
  $thumbnail_datastream = $fedora_object->constructDatastream('TN');
  $thumbnail_datastream->setContentFromUrl($thumbnail);
  $thumbnail_datastream->label = 'Thumbnail';
  $thumbnail_datastream->mimetype = 'image/png';
  $fedora_object->ingestDatastream($thumbnail_datastream);
  $policy_datastream = $fedora_object->constructDatastream(variable_get('Islandora_Collection_Policy_DSID', 'COLLECTION_POLICY'), 'X');
  $policy_datastream->setContentFromString($collection_policy_xml->saveXML());
  $policy_datastream->label = 'Collection policy';
  $fedora_object->ingestDatastream($policy_datastream);
  $new_fedora_object = islandora_ingest_add_object($fedora_object);

  drupal_goto('/islandora/object/' . $new_collection_pid);
}
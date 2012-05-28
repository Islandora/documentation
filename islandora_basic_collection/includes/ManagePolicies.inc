<?php

function islandora_manage_policies_form($form, &$form_state, $collection_pid) {
  module_load_include('inc', 'islandora_basic_collection', 'includes/CollectionPolicy');
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();

  $content_models = get_content_models_list($collection_pid);
  $cm_options = array();
  $name_mappings = array();
  foreach ($content_models as $content_model) {
    if ($content_model != "islandora:collectionCModel") {
      $item = new FedoraObject($content_model, $restConnection->repository);
      $cm_name = $item->Label;
      $cm_options[$content_model] = $cm_name;
    }
  }

  $new_options = array();
  if (is_array($content_models) && is_array($cm_options)) {
    $new_options = array_diff_key($content_models, $cm_options);
  }

  $namespace = substr($collection_pid, 0, strpos($collection_pid, ":"));

  $collection_policy_dsid = variable_get('Islandora_Collection_Policy_DSID', 'COLLECTION_POLICY');
  $collection_object = new FedoraObject($collection_pid, $restConnection->repository);
  $collection_policy_string = $collection_object->getDatastream($collection_policy_dsid);

  $supported_collection_models = array();
  if ($collection_policy_string) {
    $collection_policy = new CollectionPolicy($collection_policy_string->content);
    $supported_collection_models = $collection_policy->getContentModels();
  }
  $collection_namespace = substr($collection_pid, 0, strpos($collection_pid, ":"));

  $represented_content_models = get_represented_content_models($collection_pid);
  $collection_name = $collection_object->label;
  $new_content_models = get_content_models_as_option_array();
  $current_models_in_policy = array();

  if ($supported_collection_models) {
    foreach ($supported_collection_models as $supported_model) {
      $current_models_in_policy[$supported_model['pid']] = $supported_model['pid'];
    }
  }

  $new_options = array();
  if (is_array($new_content_models) && is_array($current_models_in_policy)) {
    $new_options = array_diff_key($new_content_models, $current_models_in_policy);
  }

  $form ['manage_collection_policy']['add']['content_model_to_add'] = array(
    '#title' => "Choose Content Model",
    '#type' => 'select',
    '#options' => $new_options,
    '#description' => t("Choose content model to add to this collection policy."),
  );

  $form ['manage_collection_policy']['add']['new_cp_namespace'] = array(
    '#title' => "Choose Namespace",
    '#type' => 'textfield',
    '#size' => 15,
    '#default_value' => $namespace,
    '#description' => t("Choose namespace for objects in this collection associated with this content model"),
  );

  $form['parent_collection'] = array(
    '#type' => 'hidden',
    '#value' => $collection_pid,
  );

  $form['collection_pid'] = array(
    '#type' => 'hidden',
    '#value' => $collection_pid,
  );

  $form['manage_collection_policy']['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Content Model to Collection Policy'),
    '#id' => 'add_cm'
  );

  if (count($current_models_in_policy) > 0) {
    $form['manage_collection_policy']['remove'] = array(
      '#title' => "Delete Content Model from Collection Policy",
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form ['manage_collection_policy']['remove']['content_models_to_remove'] = array(
      '#title' => "Choose Content Model to Remove",
      '#type' => 'checkboxes',
      '#options' => $current_models_in_policy,
      '#description' => t("Choose content models to remove from this collection policy."),
    );


    $form['manage_collection_policy']['remove']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Remove Content Model From Collection Policy'),
      '#id' => 'remove_cm'
    );
  }
  return $form;
}

function islandora_manage_policies_form_validate($form, &$form_state) {
  
}

function islandora_manage_policies_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_basic_collection', 'CollectionPolicy');
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();

  $collection_pid = $form_state['values']['parent_collection'];
  $collection_object = new FedoraObject($collection_pid, $restConnection->repository);
  $policy = $collection_object->getDatastream(variable_get('Islandora_Collection_Policy_DSID', 'COLLECTION_POLICY'));
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

  $collection_policy_xml = new DOMDocument();
  $collection_policy_xml->loadXML($policy->content);

  if ($form_state['clicked_button']['#id'] == 'add_cm') {
    if (empty($policy->content)) {
      $collection_policy_datastream = $collection_object->constructDatastream('COLLECTION_POLICY', 'X');
      $collection_policy_datastream->setContentFromString($collection_policy);
      $collection_policy_datastream->label = 'COLLECTION_POLICY';
      $collection_object->ingestDatastream($collection_policy_datastream);
      $policy = $collection_object->getDatastream(variable_get('Islandora_Collection_Policy_DSID', 'COLLECTION_POLICY'));
    }

    $cp_namespace = $form_state['values']['new_cp_namespace'];
    $cp_content_model = $form_state['values']['content_model_to_add'];
    $content_model_object = new FedoraObject($cp_content_model, $restConnection->repository);
    $content_model_datastream = $content_model_object->getDatastream(variable_get('Islandora_Content_Model_DSID', 'ISLANDORACM'));

    $content_models_element = $collection_policy_xml->getElementsByTagName('content_models');
    $content_model_element = $content_models_element->item(0)->getElementsByTagName('content_model');

    $content_model_element = $collection_policy_xml->createElement('content_model');
    $content_model_element->setAttribute('name', $content_model_object->label);
    $content_model_element->setAttribute('dsid', variable_get('Islandora_Content_Model_DSID', 'ISLANDORACM'));
    $content_model_element->setAttribute('namespace', $cp_namespace);
    $content_model_element->setAttribute('pid', $content_model_object->id);
    $content_models_element->item(0)->appendChild($content_model_element);

    $new_collection_policy_datastream = $collection_object->getDatastream('COLLECTION_POLICY');
    $new_collection_policy_datastream->setContentFromString($collection_policy_xml->saveXML());
    $new_collection_policy_datastream->label = 'COLLECTION_POLICY';
    $collection_object->ingestDatastream($new_collection_policy_datastream);
    drupal_set_message("Collection model successfully added");
  }

  //remove content model from collection policy
  if ($form_state['clicked_button']['#id'] == 'remove_cm') {
    $candidates = $form_state['values']['content_models_to_remove'];
    $count = 0;
    $s = '';
    foreach ($candidates as $candidate) {
      if (is_string($candidate)) {

        $content_models_element = $collection_policy_xml->getElementsByTagName('content_models');
        $models = $content_models_element->item(0)->getElementsByTagName('content_model');
        $found = FALSE;
        for ($i = 0; $found === FALSE && $i < $models->length; $i++) {
          if ($models->item($i)->getAttribute('pid') == $candidate) {
            $found = $models->item($i);
          }
        }

        if ($found !== FALSE && $models->length > 1) {
          $content_models_element->item(0)->removeChild($found);
          $ret = TRUE;
        }
        $count++;
      }
    }
    if ($count > 0) {
      $new_collection_policy_datastream = $collection_object->getDatastream('COLLECTION_POLICY');
      $new_collection_policy_datastream->setContentFromString($collection_policy_xml->saveXML());
      $new_collection_policy_datastream->label = 'COLLECTION_POLICY';
      $collection_object->ingestDatastream($new_collection_policy_datastream);
      if ($count > 1) {
        $s = 's';
      }
      drupal_set_message("$count collection model$s removed");
    }
  }
}
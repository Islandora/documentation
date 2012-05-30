<?php

/**
 * @file
 * ChildCollection.inc
 */

/**
 *
 * @param type $form
 * @param type $form_state
 * @param type $this_collection_pid
 * @return string
 */
function islandora_create_child_collection_form($form, &$form_state, $this_collection_pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $rest_connection = new RestConnection();

  $restricted = FALSE;
  if (variable_get('fedora_namespace_restriction_enforced', TRUE)) {
    $restricted = TRUE;
    $allowed_string = variable_get('fedora_pids_allowed', 'default: demo: changeme: islandora:');
    $namespaces = explode(':', $allowed_string);
    foreach ($namespaces as $namespace) {
      if ($namespace) {
        $allowed[trim($namespace)] = trim($namespace);
      }
    }
  }
  $collection_namespace = substr($this_collection_pid, 0, strpos($this_collection_pid, ":"));

  $form['child_creation']['titlebox'] = array(
    '#type' => 'item',
    '#title' => t("Create New Child Collection within @collection", array('@collection' => $this_collection_pid)),
  );

  $form['child_creation']['collection_name'] = array(
    '#title' => "Collection Name",
    '#type' => 'textfield',
    '#size' => 25,
    '#description' => t("Human readable name for this collection"),
  );

  $form['child_creation']['new_collection_pid'] = array(
    '#title' => "Collection PID",
    '#type' => 'textfield',
    '#size' => 15,
    '#default_value' => $rest_connection->repository->api->m->getNextPid($collection_namespace),
    '#description' => t("Unique PID for this collection.  <br />Pids take the general form of namespace:collection (eg. islandora:pamphlets)"),
  );

  if (!$restricted) {
    $form['child_creation']['collection_namespace'] = array(
      '#title' => "Collection Namespace",
      '#type' => 'textfield',
      '#size' => 15,
      '#default_value' => $collection_namespace,
      '#description' => t("Namespace for objects in this collection."),
    );
  }
  else {
    $form['child_creation']['collection_namespace'] = array(
      '#title' => "Collection Namespace",
      '#type' => 'select',
      '#options' => $allowed,
      '#default_value' => 'default',
      '#description' => t("Namespace for objects in this collection."),
    );
  }

  $form['current'] = array(
    '#type' => 'hidden',
    '#value' => $this_collection_pid,
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

function islandora_create_child_collection_form_submit($form, &$form_state) {
  global $base_root;
  module_load_include('inc', 'islandora', '/includes/islandora.ingest');
    $thumbnail = $base_root . '/' . drupal_get_path('module', 'islandora_basic_collection') . '/Crystal_Clear_filesystem_folder_grey.png';
    $new_collection_pid = $form_state['values']['new_collection_pid'];
    $new_collection_label = $form_state['values']['collection_name'];
    $namespace = $form_state['values']['collection_namespace'];
//    $all_cModels = get_content_models_as_option_array();
    $content_models = array('islandora:collectionCModel');
    $relationship = array(
      'uri' => FEDORA_RELS_EXT_URI,
      'value' => 'isMemberOfCollection',
    );

    $fedora_object = islandora_ingest_get_object($content_models, $form_state['values']['current'], $relationship, $new_collection_pid);
    $fedora_object->label = $new_collection_label;
    $thumbnail_datastream = $fedora_object->constructDatastream('TN');
    $thumbnail_datastream->setContentFromUrl($thumbnail);
    $thumbnail_datastream->label = 'Thumbnail';
    $thumbnail_datastream->mimetype = 'image/png';
    $fedora_object->ingestDatastream($thumbnail_datastream);
    $new_fedora_object = islandora_ingest_add_object($fedora_object);

//    $content_models = $form_state['values']['content_models'];
//    $collection_policy_xml = simplexml_load_string($collection_policy);
//    foreach ($content_models as $content_model) {
//      if ($content_model) {
//        $node = $collection_policy_xml->content_models->addChild('content_model');
//        $node->addAttribute('dsid', 'ISLANDORACM');
//        $node->addAttribute('name', $all_cModels[$content_model]);
//        $node->addAttribute('namespace', $pid_namespace . ':1');
//        $node->addAttribute('pid', $content_model);
//      }
//    }
    drupal_goto('/islandora/object/' . $new_collection_pid);
}
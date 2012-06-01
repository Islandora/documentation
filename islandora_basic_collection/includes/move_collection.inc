<?php

/**
 * @file
 * MoveCollection.inc
 */

/**
 * Create object migration form for the collection manager
 * 
 * @param array $form_state
 * @param string $pid
 *
 * @return string
 */
function islandora_collection_migrate_form($form, &$form_state, $object) {
  module_load_include('inc', 'islandora_basic_collection', 'collection_manager_table');
  $potential_collections = get_collections_as_option_array();
  $table = islandora_collection_table($object);
  if (!$table) {
    $form['no_objects'] = array(
      '#type' => 'item',
      '#title' => t('No objects found in this collection'),
    );
    return $form;
  }
  $form = array();

  $form['migrate']['titlebox'] = array(
    '#type' => 'item',
    '#title' => t("Move objects from @collection_pid", array('@collection_pid' => $object->id)),
  );  

  $form['migrate']['new_collection'] = array(
    '#title' => t('New collection'),
    '#description' => t("All content will be migrated from @pid to the selected collection", array('@pid' => $object->id)),
    '#type' => 'select',
    '#options' => $potential_collections,
  );

  $form['migrate']['table'] = $table;

  $form['current'] = array(
    '#type' => 'hidden',
    '#value' => $object->id,
  );

  $form['migrate']['message'] = array(
    '#type' => 'item',
    '#value' => t(""),
  );

  $form['migrate']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate selected objects'),
  );
  return $form;
}

/**
 * Submit handler for object migration form
 * 
 * @param array $form
 * @param array $form_state 
 */
function islandora_collection_migrate_form_submit($form, &$form_state) {

  $pids = array_filter($form_state['values']['table']);
  $new_collection = $form_state['values']['new_collection'];
  $current = $form_state['values']['current'];
  foreach ($pids as $pid) {
    $fedora_object = islandora_object_load($pid);
    $fedora_object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $current);
    $fedora_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $new_collection);
  }
}
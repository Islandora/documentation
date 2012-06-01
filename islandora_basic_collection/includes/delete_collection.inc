<?php

/**
 * @file
 * DeleteCollection.inc
 */

/**
 *
 * @param array $form_state
 * @param string $pid
 *
 * @return string
 */
function islandora_collection_deletion_form($form, &$form_state, $object) {
  module_load_include('inc', 'islandora_basic_collection', 'collection_manager_table');
  $pid = $object->id;
  $potential_collections = get_collections_as_option_array();
  $table = islandora_collection_table($object);
  $deletion_message = ($table) ? "Delete Members of this Collection" : "Delete Collection";
  $submit_text_message = ($table) ? "Delete selected objects" : "Delete collection";

  $form = array();

  $form['collection_delete']['titlebox'] = array(
    '#type' => 'item',
    '#title' => t("Delete objects from @collection_pid", array('@collection_pid' => $pid)),
  );

  if ($table) {
    $form['collection_delete']['table'] = array(
      'table' => $table,
    );
  }
  else {
    $form['collection_delete']['delete_root'] = array(
      '#type' => 'checkbox',
      '#title' => "Remove this empty collection?",
      '#id' => 'delete_collection',
    );
  }
  $form['current'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );

  $form['collection_delete']['message'] = array(
    '#type' => 'item',
    '#value' => t("This action is permanant and cannot be undone."),
  );
  $form['collection_delete']['submit'] = array(
    '#type' => 'submit',
    '#description' => t("This action is permanant and cannot be undone."),
    '#value' => t('@message', array('@message' => $submit_text_message)),
  );

  return $form;
}

/**
 * Submit handler for object deletion form in the collection manager.
 * 
 * @global type $user
 * @param array $form
 * @param array $form_state 
 */
function islandora_collection_deletion_form_submit($form, &$form_state) {

  $collection_pid = $form_state['values']['current'];
  $fedora_object = islandora_object_load($collection_pid);

  $parents = $fedora_object->relationships->get(NULL, 'isMemberOfCollection');
  $parents = islandora_collections_get_collection_from_pid($fedora_object);
  $collection_pid = $form_state['values']['current'];
  if (isset($form_state['values']['delete_root']) && $form_state['values']['delete_root'] == 1) {
    delete_root_collection($fedora_object);
    drupal_goto("islandora/object/" . $parents[0]['parent']['value']);
  }

  $child_collections = get_child_collections($fedora_object);

  $populated_child_collections = array();
  $pids = @array_filter($form_state['values']['table']);

  if (!empty($child_collections)) {
    foreach ($child_collections as $child) {
      $child_pids = get_related_items_as_array(islandora_object_load($child['object']['value']), 'isMemberOfCollection');
      if (!empty($child_pids)) {
        $populated_child_collections[] = $child;
      }
    }
  }
  if (!empty($populated_child_collections)) {
    $conflict = FALSE;
    foreach ($populated_child_collections as $collection) {
      if (in_array($collection, $pids)) {
        $conflict = TRUE;
        drupal_set_message(t("Populated child collections were not deleted."));
      }
    }
  }
  $pids_to_delete = array_diff($pids, $populated_child_collections);

  foreach ($pids_to_delete as $pid_to_delete) {
    $fedora_object->repository->purgeObject($pid_to_delete);
  }
  drupal_goto("islandora/object/" . $collection_pid);
}

/**
 * Deletes the collection object
 * 
 * @param string $pid
 */
function delete_root_collection($object) {
  $pid = $object->id;
  try {
    $object->repository->purgeObject($object->id);
  } catch (RepositoryException $e) {
    drupal_set_message(t("Collection '@pid' could not be deleted!", array('@pid' => $pid)), 'error');
    return;
  }
  drupal_set_message(t("Collection '@pid' deleted.", array('@pid' => $pid)));
}

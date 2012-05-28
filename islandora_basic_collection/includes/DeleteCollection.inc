<?php

/**
 *
 * @param array $form_state
 * @param string $pid
 *
 * @return string
 */
function islandora_collection_deletion_form($form, &$form_state, $pid) {
  module_load_include('inc', 'islandora_basic_collection', 'CollectionManagerTable');
  $potential_collections = get_collections_as_option_array();
  $table = islandora_collection_table($pid);
  $deletion_message = ($table) ? "Delete Members of this Collection" : "Delete Collection";
  $submit_text_message = ($table) ? "Delete Selected Objects" : "Delete Collection";

  $form = array();

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

function islandora_collection_deletion_form_submit($form, &$form_state) {
  global $user;
//  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
//  module_load_include('inc', 'fedora_repository', 'api/fedora_collection');
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  
  $collection_pid = $form_state['values']['current'];
  $fedora_object = new FedoraObject($collection_pid, $restConnection->repository);
  $parents = $fedora_object->relationships->get(NULL, 'isMemberOfCollection');
  var_dump($parents);
//  exit;
//  $parents = Islandora_collections_get_collection_from_pid($collection_pid);
//  $collection_pid = $form_state['values']['current'];
  if ($form_state['values']['delete_root'] == 1) {
    delete_root_collection($collection_pid);
    drupal_goto("islandora/" . $parents[0]);
  }

  $child_collections = get_child_collections($collection_pid);

  $populated_child_collections = array();
  $pids = @array_filter($form_state['values']['table']['selections']);

  if (!empty($child_collections)) {
    foreach ($child_collections as $child) {
      $child_pids = get_related_items_as_array($child, 'isMemberOfCollection');
      if (!empty($child_pids)) {
        $populated_child_collections[] = $child;
      }
    }
  }
  if (!empty($populated_child_collections)) {
    $conflict = false;
    foreach ($populated_child_collections as $collection) {
      if (in_array($collection, $pids)) {
        $conflict = true;
        drupal_set_message("Populated child collections were not deleted.");
      }
    }
  }
  $pids_to_delete = array_diff($pids, $populated_child_collections);

  foreach ($pids_to_delete as $pid_to_delete) {
    $restConnection->repository->purgeObject($pid_to_delete);
  }
  drupal_goto("islandora/" . $parents[0]);
}

function delete_root_collection($pid) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $item = new Fedora_Item($pid);
  $item->purge();
  drupal_set_message("$pid deleted.");
}

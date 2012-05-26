<?php

/**
 *
 * @param array $form_state
 * @param string $pid
 *
 * @return string
 */
function islandora_collection_migrate_form(&$form_state, $pid) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
  module_load_include('inc', 'islandora_collection_manager', 'CollectionManagerTable');
  $potential_collections = get_collections_as_option_array();
  $table = islandora_collection_table($pid);
  if (!$table) {
    return;
  }
  $form = array();
  $form['migrate'] = array(
    '#title' => "Migrate Members",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['migrate']['new_collection'] = array(
    '#title' => t('New Collection'),
    '#description' => t("All content will be migrated from $pid to the selected collection"),
    '#type' => 'select',
    '#options' => $potential_collections,
  );

  $form['migrate']['table'] = array(
    'table' => $table,
  );

  $form['current'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
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

function islandora_collection_migrate_form_submit($form, &$form_state) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $pids = array_filter($form_state['values']['table']['selections']);
  $new_collection = $form_state['values']['new_collection'];
  $current = $form_state['values']['current'];
  foreach ($pids as $pid) {
    $item = new Fedora_Item($pid);
    $item->add_relationship('isMemberOfCollection', $new_collection, RELS_EXT_URI);
    $item->purge_relationship('isMemberOfCollection', $current);
  }
}
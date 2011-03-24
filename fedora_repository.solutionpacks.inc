<?php


/**
 * Invokes a hook to any dependent modules asking them if their installations require
 * any fedora objects to be present. Modules implementing this hook should return an array
 * of arrays of the form:
 *
 * array( 'pid', 'path-to-foxml-file', 'dsid', 'path-to-datastream-file', int dsversion)
 *
 * where the last three options are optional. A module can either point to a simple
 * foxml file to install, or can specify a datastreamstream to check for, with a
 * path to load the datastream from if it isn't there. Optionally a version number
 * can be included, to enable updating of content model or collection policy streams
 * that may have been updated. THis is a simple whole number that should be incremented
 * when changed.  This value appears in as an attribute of the topmost element of the stream,
 * e.g.,:
 *
 * <?xml version="1.0" encoding="utf-8"?> <content_model name="Collection" version="2" ...
 *
 * Datastreams which don't have this element are assumed to be at version 0.
 */
function fedora_repository_solution_packs_page() {
  $enabled_solution_packs = module_invoke_all('required_fedora_objects');
  $output = '';
  foreach ($enabled_solution_packs as $solution_pack_module => $solution_pack_info) {
    $objects = array();
    foreach ($solution_pack_info as $field => $value) {
      switch ($field) {
        case 'title':
          $solution_pack_name = $value;
          break;
        case 'objects':
          $objects = $value;
          break;
      }
    }
    $output .= drupal_get_form('fedora_repository_solution_pack_form', $solution_pack_module, $solution_pack_name, $objects);
  }

  return $output;
}

/**
 * Check for installed objects and add a 'Update' or 'Install' button if some objects are missing.
 * @param array $solution_pack
 */
function fedora_repository_solution_pack_form(&$form_state, $solution_pack_module, $solution_pack_name, $objects = array()) {
  // Check each object to see if it is in the repository.
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');

  $needs_update = FALSE;
  $needs_install = FALSE;
  $form = array();
  $form['solution_pack_module'] = array(
    '#type' => 'hidden',
    '#value' => $solution_pack_module,
  );

  if (!$form_state['submitted']) {

    $form['objects'] = array(
      '#type' => 'fieldset',
      '#title' => "Objects",
      '#weight' => 10,
      '#attributes' => array('class' => 'collapsed'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $table_header = array('PID', 'Status');
    $table_rows = array();


    foreach($objects as $object) {
      if (isset($object['pid'])) {
        $pid = $object['pid'];

        $item = new Fedora_Item($pid);
        $table_row = array($object['pid']);
        $object_status = t('Up-to-date');
        if (!$item->exists()) {
          $object_status = 'Missing';
          $needs_install = TRUE;
        }
        elseif (isset($object['dsid']) && isset($object['datastream_file']) && isset($object['dsversion'])) {
          // Check if the datastream is versioned and needs updating.
          $installed_version = fedora_repository_get_islandora_datastream_version($item, $object['dsid']);
          $available_version = fedora_repository_get_islandora_datastream_version(NULL, NULL, $object['datastream_file']);
          if ($available_version > $installed_version) {
            $needs_update = TRUE;
            $object_status = 'Out of date';
          }
        }
        array_push($table_row, $object_status);

        $table_rows[] = $table_row;

      }
    }
    $form['objects']['table'] = array(
      '#type' => 'markup',
      '#value' => theme_table($table_header, $table_rows),
    );
  }
  $form['submit'] = array(
    '#value' => t('Install'),
    '#disabled' => !$needs_install,
    '#type' => 'submit',
    '#name' => 'ingest',
  );

  return $form;
}

function fedora_repository_solution_pack_form_submit($form, &$form_state) {
  $what = $form_state;
  $module_name = $form_state['values']['solution_pack_module'];
  $solution_pack_info = call_user_func($module_name . '_required_fedora_objects');
  $batch = array(
    'title' => t('Installing / updating solution pack objects'),
    'file' => drupal_get_path('module', 'fedora_repository') . '/fedora_repository.module',
    'operations' => array(),
  );


  foreach ($solution_pack_info[$module_name]['objects'] as $object) {
    // Add this object to the batch job queue.
    $batch['operations'][] = array('fedora_repository_batch_reingest_object', array($object));
  }
  batch_set($batch);
}

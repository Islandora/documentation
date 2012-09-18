<?php

/**
 * @file
 * This file contains all admin and callback functions for solution pack management.
 */

function islandora_solution_packs_admin() {
  // set variables
  $enabled_solution_packs = module_invoke_all('islandora_required_objects');

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
    // get form
    $form_array = drupal_get_form('islandora_solution_pack_form_' . $solution_pack_module, $solution_pack_module, $solution_pack_name, $objects);
    // render form
    $output .= drupal_render($form_array);
  }

  return $output;
}


function islandora_solution_pack_form($form, &$form_state,  $solution_pack_module, $solution_pack_name, $objects = array()) {

  // set variables
  global $base_path;
  $needs_update = FALSE;
  $needs_install = FALSE;
  $form = array();

  $form['solution_pack'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('islandora-solution-pack-fieldset')),
  );

  // adding values
  $form['solution_pack']['solution_pack_module'] = array(
    '#type' => 'value',
    '#value' => $solution_pack_module,
  );
  $form['solution_pack']['solution_pack_name'] = array(
    '#type' => 'value',
    '#value' => $solution_pack_name,
  );
  $form['solution_pack']['objects'] = array(
    '#type' => 'value',
    '#value' => $objects,
  );

  // table
  // header
  $table_header = array('PID', 'Status');
  $table_rows = array();

  // loop over defined objects
  foreach ($objects as $object) {
    $datastreams = NULL;
    if (isset($object['pid'])) {
      $pid = $object['pid'];

      // load object
      $item = islandora_object_load($pid);

      $table_row = array($object['pid']);
      $object_status = t('Up-to-date');
      if (!$item) {
        $object_status = t('Missing');
        $needs_install = TRUE;
      }
      else {
        if (isset($object['dsid']) && isset($object['datastream_file']) && isset($object['dsversion'])) {
          $datastreams = array(
            array(
              'dsid' => $object['dsid'],
              'datastream_file' => $object['datastream_file'],
              'dsversion' => $object['dsversion'],
            ),
          );
        }
        elseif (!empty($object['datastreams'])) {
          $datastreams = $object['datastreams'];
        }
        if (!empty($datastreams) && is_array($datastreams)) {
          foreach ($datastreams as $ds) {

            $ds_id = $ds['dsid'];
            // check if defined datastream exists in the object
            if (!$item[$ds_id]) {
              $needs_update = TRUE;
              $object_status = t('Missing datastream');
              break;
            }
            elseif (isset($ds['dsversion'])) {
              // Check if the datastream is versioned and needs updating.
              $installed_version = islandora_get_islandora_datastream_version($item, $ds['dsid']);
              $available_version = islandora_get_islandora_datastream_version(NULL, NULL, $ds['datastream_file']);

              if ($available_version > $installed_version) {
                $needs_update = TRUE;
                $object_status = t('Out of date');
                break;
              }
            }
          }
        }
      }
      array_push($table_row, $object_status);
      $table_rows[] = $table_row;
    }
  }

  // title
  if (!$form_state['submitted']) {
    $form['solution_pack']['solution_pack_label'] = array(
      '#markup' => filter_xss($solution_pack_name),
      '#prefix' => '<h3>',
      '#suffix' => '</h3>',
    );

    $form['solution_pack']['install_status'] = array(
      '#markup' => '<strong>' . t('Object status:') . '&nbsp;</strong>',
      '#prefix' => '<div class="islandora-solution-pack-install-status">',
      '#suffix' => '</div>',
    );
    if (!$needs_install && !$needs_update) {
      $form['solution_pack']['install_status']['#markup'] .= ' ' . theme('image', array('path' => 'misc/watchdog-ok.png')) . ' ' . t('All required objects are installed and up-to-date.');
      $submit_button_text = t("Force reinstallation of objects");
    }
    else {
      $form['solution_pack']['install_status']['#markup'] .= ' ' . theme('image', array('path' => 'misc/watchdog-warning.png')) . ' ' . t('Some objects must be re-ingested. See objects list for details.');
      $submit_button_text = t("Install objects");

    }

    $form['solution_pack']['table'] = array(
      '#type' => 'item',
      '#markup' => theme('table', array('header' => $table_header, 'rows' => $table_rows)),
    );
  }

  $form['solution_pack']['submit'] = array(
    '#value' => $submit_button_text,
    '#type' => 'submit',
    '#name' => $solution_pack_module,
    '#attributes' => array('class' => array('islandora-solution-pack-submit')),
    '#weight' => 40,
  );

  $form['solution_pack']['#submit'] = array(
    'islandora_solution_pack_form_submit',
  );

  return $form;
}

/**
 * Submit handler for solution pack form.
 *
 * @param array $form
 *   The form submitted.
 * @param array_reference $form_state
 *   The state of the form submited.
 */
function islandora_solution_pack_form_submit($form, &$form_state) {

  // get variables
  $solution_pack_module = $form_state['values']['solution_pack_module'];
  $solution_pack_name = $form_state['values']['solution_pack_name'];
  $objects = $form_state['values']['objects'];

  $batch = array(
    'title' => t('Installing / updating solution pack objects'),
    'file' => drupal_get_path('module', 'islandora') . '/includes/solution_packs.inc',
    'operations' => array(),
  );

  foreach ($objects as $object) {
    // Add this object to the batch job queue.
    $batch['operations'][] = array('islandora_batch_reingest_object', array($object));
  }

  batch_set($batch);

  // Hook to let solution pack objects be modified.
  // Not using module_invoke so solution packs can be expanded by other modules.
  module_invoke_all('islandora_postprocess_solution_pack', $solution_pack_module);

}




/**
 * Batch reingest object
 *
 * @param type $object
 * @param type $context
 * @return type
 */
function islandora_batch_reingest_object($object, &$context) {

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/islandora.ingest');
  // include Tuque library
  module_load_include('inc', 'islandora', 'includes/tuque');
  global $user;
  // new connection
  try {
    $connection = new IslandoraTuque($user);
  } catch (Exception $e) {
    drupal_set_message(t('Unable to connect to the repository %e', array('%e' => $e)), 'error');
    return;
  }

  if (!empty($object) && is_array($object)) {
    // set and validate PID
    $pid = $object['pid'];
    if (!islandora_validate_pid($pid)) {
      return NULL;
    }

    // purge object
    // check if object already exits
    $object_query = $connection->api->a->findObjects('query', 'pid=' . $pid);
    if (!empty($object_query['results'])) {
      islandora_object_purge($pid);
    }
    else {
      drupal_set_message(t('Content models for the basic image module already exist!'), 'warning');
    }

    // build and ingest new object
    islandora_ingest_new_object($object);
  }
}








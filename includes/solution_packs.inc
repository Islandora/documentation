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
  global $base_url;
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
  $table_header = array(t('Label'), t('PID'), t('Status'));
  $table_rows = array();

  // loop over defined objects
  foreach ($objects as $object) {
    $datastreams = NULL;
    if (isset($object['pid'])) {
      // set variables
      $pid = $object['pid'];

      // load object
      $item = islandora_object_load($pid);

      // table row
      $table_row = array();

      // check out object status
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
      // label (prepend)
      if ($needs_install) {
        $label = $object['label'] ? $object['label'] : '';
      }
      else {
        $label = $object['label'] ? l($object['label'], $base_url . '/islandora/object/' . $pid) : '';
      }
      $table_row[] = $label;      
      // pid
      $table_row[] = $pid;
      // object status
      $table_row[] = $object_status;
      // add row
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
function islandora_batch_reingest_object($object_model, &$context) {

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/islandora.ingest');
  // include Tuque library
  module_load_include('inc', 'islandora', 'includes/tuque');
  global $user;
  global $base_url;
  // new connection
  try {
    $connection = new IslandoraTuque($user);
  }
  catch (Exception $e) {
    drupal_set_message(t('Unable to connect to the repository %e', array('%e' => $e)), 'error');
    return;
  }

  if (!empty($object_model) && is_array($object_model)) {
    // set and validate PID
    $pid = $object_model['pid'];
    if (!islandora_validate_pid($pid)) {
      return NULL;
    }

    // purge object
    // check if object already exits
    $object_query = $connection->api->a->findObjects('query', 'pid=' . $pid);
    $reinstall = FALSE;
    if (!empty($object_query['results'])) {
      islandora_object_purge($pid);
      $reinstall = TRUE;
    }

    // build and ingest new object
    try {
      $object = islandora_ingest_new_object($object_model);
      $object_name = $object->label;
      if ($reinstall) {
        drupal_set_message(t('Successfully re-installed <em>@object_name</em>.', array('@object_name' => $object_name, '@pid' => $pid)));
      }
      else {
        drupal_set_message(t('Successfully installed <em>@object_name</em>.', array('@object_name' => $object_name, '@pid' => $pid)));
      }
    }
    catch (Exception $e) {
      drupal_set_message(t('Installation of object @pid failed', array('@pid' => $pid)), 'error');
    }
  }
}


/**
 * Callback function that can be called from the solution pack's hook_install() and hook_uninstall() functions.
 *
 * @TODO: add documentation
 */
function islandora_install_solution_pack($module_name = NULL, $op = 'install') {
  if (!empty($module_name)) {
    module_load_include('inc', 'islandora', 'includes/tuque');
    module_load_include('module', 'islandora', 'islandora');
    module_load_include('inc', 'islandora', 'includes/islandora.ingest');
    module_load_include('module', $module_name, $module_name);
    global $base_url;
    global $user;
  
    // get module info
    $info_file = drupal_get_path('module', $module_name) . '/' . $module_name . '.info';
    $info_array = drupal_parse_info_file($info_file);
    $module_label = $info_array['name'];
  
    // create new connection
    try {
      $connection = new IslandoraTuque($user);
    } catch (Exception $e) {
      drupal_set_message(st('Unable to connect to the repository %e', array('%e' => $e)), 'error');
      return;
    }
  
    // get object models
    $enabled_solution_packs = module_invoke_all('islandora_required_objects');
    $islandora_required_objects = $module_name . '_islandora_required_objects';
    $required_objects = $islandora_required_objects();
    $objects = $required_objects[$module_name]['objects'];
  
    // loop over object models
    foreach ($objects as $object) {
      // set variables
      $pid = $object['pid'];
      $label = isset($object['label']) ? $object['label'] : st('Object');
      // check if object already exists
      $query = $connection->api->a->findObjects('query', 'pid=' . $pid);
  
      // operation: install or uninstall
      switch ($op) {
        case 'install':
          // if object exists, don't re-ingest
          if (!empty($query['results'])) {
            $object_url = url($base_url . '/islandora/object/' . $pid);
            drupal_set_message(st('@module_label: did not install <a href="!url" title="@pid">@label</a> because the object already exists.', array('@module_label' => $module_label, '@label' => $label, '@pid' => $pid, '!url' => $object_url)), 'warning');
          }
          else {
            // build and ingest new object
            islandora_ingest_new_object($object);
            // set message
            drupal_set_message(st('@module_label: installed <a href="!url" title="@pid">@label</a> object.', array('@module_label' => $module_label, '@label' => $label, '@pid' => $pid)));
          }
          break;
  
        case 'uninstall':
          // if object exists, set message
          if (!empty($query['results'])) {
            $object_url = url($base_url . '/islandora/object/' . $pid);
            drupal_set_message(st('@module_label: Did not remove <a href="!object_url" title="@pid">@label</a>. It may be used by other sites.', array('@pid' => $pid, '!object_url' => $object_url, '@label' => $label, '@module_label' => $module_label)), 'warning');
          }
          break;
      }
    }    
  }
}

<?php

/**
 * @file
 * Handles the management of deleted objects.
 */

/**
 * Manages the two deletion management forms.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_deleted_objects_form($form, &$form_state) {
  if (!isset($form_state['stage'])) {
    $form_state['stage'] = 'get_content_models';
  }
  $form = array();
  switch ($form_state['stage']) {
    case 'get_content_models':
      $form =  islandora_delete_form_contentmodels($form, $form_state);

    break;

    case 'get_objects':
      $form = islandora_delete_form_select_objects($form, $form_state);

    break;
  }
  return $form;
}

/**
 * Submit handler for deletion management.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function islandora_deleted_objects_form_submit($form, &$form_state) {
  if (isset($form_state['clicked_button']['#attributes']['source']) && $form_state['clicked_button']['#attributes']['source'] == 'previous') {
    $form_state['rebuild'] = TRUE;
    $form_state['stage'] = 'get_content_models';
    return;
  }

  module_load_include('inc', 'islandora', 'includes/utilities');
  if ($form_state['stage'] == 'get_content_models') {
    $form_state['rebuild'] = TRUE;
    $form_state['stage'] = 'get_objects';
    return;
  }

  $objects_to_process = array_filter($form_state['values']['objects_to_process']);
  if ($form_state['clicked_button']['#attributes']['source'] == 'restore') {
    $descriptor = "Restoring";
    $action = 'islandora_restore_object';
  }
  if ($form_state['clicked_button']['#attributes']['source'] == 'purge') {
    $descriptor = "Purging";
    $action = 'islandora_purge_object';
  }
  $pids_to_restore = $objects_to_process;
  if ($form_state['values']['propogate']) {
    foreach ($objects_to_process as $pid) {
      $fedora_object = islandora_object_load($pid);
      $hooks = islandora_build_hook_list(ISLANDORA_UPDATE_RELATED_OBJECTS_PROPERTIES_HOOK, $fedora_object->models);
      foreach ($hooks as $hook) {
        $temp = module_invoke_all($hook, $fedora_object);
        if (!empty($temp)) {
          $pids_to_restore = array_merge_recursive($pids_to_restore, $temp);
        }
      }
    }
  }
  $batch = array(
    'title' => t('@descriptor selected objects', array('@descriptor' => $descriptor)),
    'file' => drupal_get_path('module', 'islandora') . '/includes/manage_deleted_objects.inc',
    'operations' => array(),
  );

  foreach ($pids_to_restore as $pid) {
    $batch['operations'][] = array(
      $action,
      array($pid),
    );
  }
  batch_set($batch);
}

/**
 * The first form in the deletion managemnt process.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_delete_form_select_objects(array $form, array &$form_state) {
  $form['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
    '#attributes' => array('source' => 'previous'),
  );
  $content_models = $form_state['values']['content_models'];
  $chosen_contentmodels = array_filter($content_models);
  $options = islandora_get_deleted_objects($chosen_contentmodels);
  if (empty($options)) {
    $form['message'] = array(
      '#type' => 'markup',
      '#markup' => t("There are no deleted objects with the selected content models in this repository."),
    );
    return $form;
  }
  $form['propogate'] = array(
    '#title' => t('Apply changes to related objects?'),
    '#default_value' => TRUE,
    '#description' => t("Objects associated with selected objects will also be restored. ie page objects associated with a book object."),
    '#type' => 'checkbox',
  );
  $form['chosen'] = array(
    '#type' => 'hidden',
    '#value' => $chosen_contentmodels,
  );
  $form['objects_to_process'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'title' => t('Name'),
      'pid' => t('PID'),
      'content_model' => t('Content Model'),
    ),
    '#multiple' => TRUE,
    '#options' => $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Restore selected objects'),
    '#attributes' => array('source' => 'restore'),
  );
  if (user_access(FEDORA_PURGE)) {
    $form['purge'] = array(
      '#type' => 'submit',
      '#value' => t('Irrevocably purge selected objects'),
      '#attributes' => array('source' => 'purge'),
    );
  }

  return $form;
}

/**
 * The first form in the deletion management process.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_delete_form_contentmodels(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $contentmodels_with_deleted_members = islandora_get_contentmodels_with_deleted_members();
  if (empty($contentmodels_with_deleted_members)) {
    $form['message'] = array(
      '#type' => 'markup',
      '#markup' => t("There are no deleted objects in this repository."),
    );
    return $form;
  }
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => t("Select content models of deleted objects."),
  );
  $table_element = islandora_content_model_select_table_form_element(NULL);

  foreach ($table_element['#options'] as $option) {
    if (!in_array($option['pid'], $contentmodels_with_deleted_members)) {
      unset($table_element['#options'][$option['pid']]);
    }
  }
  if (isset($form_state['values']['chosen'])) {
    $chosen_contentmodels = $form_state['values']['chosen'];
    foreach ($table_element['#options'] as $option) {
      if (in_array($option['pid'], $chosen_contentmodels)) {
        $table_element['#default_value'][$option['pid']] = TRUE;
      }
    }
  }
  $form['content_models'] = $table_element;
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Gets PIDS of all deleted objects.
 *
 * @return array
 *   array of pids
 */
function islandora_get_deleted_objects($content_models) {
  $qualifier = '';
  $last_content_model = key(array_slice($content_models, -1, 1, TRUE));
  foreach ($content_models as $content_model) {
    $qualifier .= "\$subject<info:fedora/fedora-system:def/model#hasModel> <info:fedora/$content_model>";
    if ($content_model != $last_content_model) {
      $qualifier .= ' or ';
    }
  }
  $tuque = new IslandoraTuque();
  $repository = $tuque->repository;
  $query = <<<STRING
select \$subject \$title \$object from <#ri>
where ($qualifier)     
and \$subject <info:fedora/fedora-system:def/model#state> <info:fedora/fedora-system:def/model#Deleted>
and \$subject<info:fedora/fedora-system:def/model#hasModel> \$object
and \$subject <dc:title> \$title
STRING;
  $objects = $repository->ri->itqlQuery($query, 'unlimited');
  $deleted_objects = array();
  foreach ($objects as $object) {
    if ($object['object']['value'] != "fedora-system:FedoraObject-3.0") {
      $pid = $object['subject']['value'];
      $title = $object['title']['value'];
      $content_model = islandora_object_load($object['object']['value']);
      $deleted_objects[$pid] = array(
        'title' => $title, 'pid' => $pid,
        'content_model' => $content_model->label,
      );
    }
  }
  return $deleted_objects;
}

/**
 * Gets PIDS of all content models associated with deleted objects.
 *
 * @return array
 *   array of content model pids
 */
function islandora_get_contentmodels_with_deleted_members() {

  $tuque = new IslandoraTuque();
  $repository = $tuque->repository;
  $query = <<<STRING
select \$object from <#ri>
where \$subject <info:fedora/fedora-system:def/model#state> <info:fedora/fedora-system:def/model#Deleted>
and \$subject<info:fedora/fedora-system:def/model#hasModel> \$object

STRING;
  $objects = $repository->ri->itqlQuery($query, 'unlimited');
  $content_models = array();
  foreach ($objects as $object) {
    if ($object['object']['value'] != "fedora-system:FedoraObject-3.0") {
      $content_models[] = $object['object']['value'];
    }
  }
  return $content_models;
}

/**
 * Restores deleted object.
 *
 * @param String $pid
 *   PID of object to be restored
 */
function islandora_restore_object($pid) {
  $fedora_object = islandora_object_load($pid);
  $fedora_object->state = 'A';
}

/**
 * Purges deleted object.
 *
 * @param String $pid
 *   PID of object to be restored
 */
function islandora_purge_object($pid) {
  $fedora_object = islandora_object_load($pid);
  $fedora_object->repository->purgeObject($pid);
}

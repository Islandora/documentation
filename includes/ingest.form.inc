<?php

/**
 * @file
 *
 * Defines the multi-page ingest form and any relevant hooks and functions for defining the multi-page ingest forms.
 */

/**
 * Ingest form build function. Initializes the form state, and builds the initial list of steps,
 * excutes the current step.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param array $configuration
 *   A list of key value pairs that are used to build the list of steps to be executed.
 *
 * @return array
 *   The form definition of the current step.
 */
function islandora_ingest_form(array $form, array &$form_state, array $configuration) {
  islandora_ingest_form_init_form_state($form_state, $configuration);
  return islandora_ingest_form_execute_step($form, $form_state);
}

/**
 * Prepares/Initializes the form state for use.
 *
 * Also cleans up or loads any data required.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param array $configuration
 *   A list of key value pairs that are used to build the list of steps to be executed.
 */
function islandora_ingest_form_init_form_state(array &$form_state, array $configuration) {
  islandora_ingest_form_init_form_state_storage($form_state, $configuration); // First time initialization of storage.
  $steps = &islandora_ingest_form_get_steps($form_state);
  usort($steps, 'drupal_sort_weight'); // Always re-sort the steps just incase any build/submit handlers have appended new steps.
  islandora_ingest_form_step_form_load_include($form_state); // Load any required files for the current step.
}

/**
 * Initializes the form_state storage for use in the ingest multi-page forms.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param array $configuration
 *   A list of key value pairs that are used to build the list of steps to be executed.
 */
function islandora_ingest_form_init_form_state_storage(array &$form_state, array $configuration) {
  if (empty($form_state['islandora'])) {
    $id = isset($configuration['id']) ? $configuration['id'] : NULL; // Use ID if given.
    $namespace = isset($configuration['namespace']) && !isset($id) ? $configuration['namespace'] : $id; // Use namespace if ID not given.
    $label = isset($configuration['label']) ? $configuration['label'] : 'New Object';
    $relationship_map = function($o) { return array('relationship' => 'isMemberOfCollection', 'pid' => $o); };
    $relationships = empty($configuration['collections']) ? array() : array_map($relationship_map, $configuration['collections']);
    $object = islandora_prepare_new_object($namespace, $label, array(), array(), $relationships);
    $form_state['islandora'] = array(
      'step' => 0,
      'steps' => islandora_ingest_get_steps($configuration),
      'objects' => array($object),
      'configuration' => $configuration
    );
  }
}

/**
 * Executes the current step, building the form definition and appending on any additonal elements
 * required for the step to function.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The form definition of the current step.
 */
function islandora_ingest_form_execute_step(array $form, array &$form_state) {
  $step_info = islandora_ingest_form_get_step_info($form_state);
  switch ($step_info['type']) {
    case 'form':
      $args = array($form, &$form_state);
      $args = isset($step_info['args']) ? array_merge($args, $step_info['args']) : $args;
      $form = call_user_func_array($step_info['form_id'], $args);
      return islandora_ingest_form_stepify($form, $form_state, $step_info);
    case 'batch': // @todo Implement if possible.
      break;
  }
  return array();
}

/**
 * Append Prev/Next buttons submit/validation handlers etc.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param array $step_info
 *   The info for building the current form step.
 *
 * @return array
 *   The stepified drupal form definition for the given step.
 */
function islandora_ingest_form_stepify(array $form, array &$form_state, array $step_info) {
  $step = islandora_ingest_form_get_step($form_state);
  $first_step = $step > 0;
  $last_step = $step == (islandora_ingest_form_get_step_count($form_state) - 1);
  $form['prev'] = $first_step ? islandora_ingest_form_previous_button() : NULL;
  $form['next'] = $last_step ? islandora_ingest_form_ingest_button($step_info) : islandora_ingest_form_next_button($step_info);
  return $form;
}

/**
 * Defines the previous button for the ingest form.
 *
 * @return array
 *   The previous button for the ingest form.
 */
function islandora_ingest_form_previous_button() {
  return array(
    '#type' => 'submit',
    '#value' => t('Previous'),
    '#name' => 'prev',
    '#submit' => array('islandora_ingest_form_previous_submit'),
    /**
     * #limit_validation_errors, is why when the previous button is pressed no values persisted in the form_state, but its also what allows us to go back when validation errors occur.
     * To have a better solution going forward we can either limit validation only on required fields, or we can convert all required fields to use #element_validation functions, and
     * Remove the need for #limit_validation_errors. Or maybe there is some other solution, regardless of what it is, it won't be standard.
     */
    '#limit_validation_errors' => array()
  );
}

/**
 * The submit handler for the ingest form previous button.
 *
 * Stores the current form steps values in the form storage.
 * Moves the focus of the multi-page ingest form back one step.
 * Restores the form values for the previous step.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_ingest_form_previous_submit(array $form, array &$form_state) {
  $step = &islandora_ingest_form_get_step($form_state);
  $step_info = &islandora_ingest_form_get_step_info($form_state, $step);
  $step_info['values'] = $form_state['values']; // Store values
  $step--; // Goto previous step
  $step_info = &islandora_ingest_form_get_step_info($form_state, $step); // Get values
  $form_state['values'] = isset($step_info['values']) ? $step_info['values'] : NULL; // Use values
  $form_state['rebuild'] = TRUE;
}

/**
 * Defines the next button for the ingest form.
 *
 * Adds submit/validate handlers for the form step if they exist.
 *
 * @return array
 *   The next button for the ingest form.
 */
function islandora_ingest_form_next_button(array $step_info) {
  $form_id = $step_info['form_id'];
  $validate_callback = $form_id . '_validate';
  $validate = function_exists($validate_callback) ? array($validate_callback) : NULL;
  $submit_callback = $form_id . '_submit';
  $submit = function_exists($submit_callback) ? array($submit_callback, 'islandora_ingest_form_next_submit') : array('islandora_ingest_form_next_submit');
  return array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#name' => 'next',
    '#validate' => $validate,
    '#submit' => $submit
  );
}

/**
 * The submit handler for the ingest form next button.
 *
 * Stores the current form steps values in the form storage.
 * Moves the focus of the multi-page ingest form forward one step.
 * Restores the form values for the next step if present.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_ingest_form_next_submit(array $form, array &$form_state) {
  $step = &islandora_ingest_form_get_step($form_state);
  $step_info = &islandora_ingest_form_get_step_info($form_state, $step);
  $step_info['values'] = $form_state['values']; // Store Values
  $step++; // Goto Next Step
  $step_info = &islandora_ingest_form_get_step_info($form_state, $step);
  $form_state['values'] = isset($step_info['values']) ? $step_info['values'] : array(); // Restore Values if Stored.
  $form_state['rebuild'] = TRUE;
}

/**
 * Defines the ingest button for the ingest form.
 *
 * This button is only shown on the last page of the multi-page ingest form.
 *
 * @return array
 *   The ingest button for the ingest form.
 */
function islandora_ingest_form_ingest_button(array $step_info) {
  $form_id = $step_info['form_id'];
  $validate_callback = $form_id . '_validate';
  $validate = function_exists($validate_callback) ? array($validate_callback) : NULL;
  $submit_callback = $form_id . '_submit';
  $submit = function_exists($submit_callback) ? array($submit_callback, 'islandora_ingest_form_submit') : array('islandora_ingest_form_submit');
  return array(
    '#type' => 'submit',
    '#name' => 'ingest',
    '#value' => t('Ingest'),
    '#validate' => $validate,
    '#submit' => $submit
  );
}

/**
 * The submit handler for the ingest form.
 *
 * Attempts to ingest every object built by the previous steps.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_ingest_form_submit(array $form, array &$form_state) {
  foreach ($form_state['islandora']['objects'] as $object) {
    try {
      islandora_add_object($object);
    } catch (Exception $e) {
      watchdog('islandora', $e->getMessage(), NULL, WATCHDOG_ERROR);  // If post hooks throws it may already exist at this point but may be invalid, so don't say failed :P
      drupal_set_message(t('A problems occured while ingesting "@label" (ID: @pid), please notifiy the administrator.', array('@label' => $object->label, '@pid' => $object->id)), 'error');
    }
  }
}

/**
 * Gets the configuration.
 *
 * @param array $form_state
 *   The drupal form state.
 *
 * @return int
 *   The current step index.
 */
function islandora_ingest_form_get_configuration(array $form_state) {
  return $form_state['islandora']['configuration'];
}

/**
 * Gets the stored objects.
 *
 * @param array $form_state
 *   The drupal form state.
 *
 * @return int
 *   The current step index.
 */
function &islandora_ingest_form_get_objects(array $form_state) {
  return $form_state['islandora']['objects'];
}

/**
 * Gets a single object.
 *
 * @param array $form_state
 *   The drupal form state.
 *
 * @return int
 *   The current step index.
 */
function islandora_ingest_form_get_object(array $form_state) {
  $objects = &islandora_ingest_form_get_objects($form_state);
  return current($objects);
}

/**
 * Get a reference to the current step index.
 *
 * @param array $form_state
 *   The drupal form state.
 *
 * @return int
 *   The current step index.
 */
function &islandora_ingest_form_get_step(array &$form_state) {
  return $form_state['islandora']['step'];
}

/**
 * Get a reference to the step info of the given step or the current step if none is given.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param int $step
 *   The index of the step to get.
 *
 * @return integer
 *   The step info of the requested step if found, NULL otherwise.
 */
function &islandora_ingest_form_get_step_info(array &$form_state, $step = NULL) {
  $step = isset($step) ? $step : islandora_ingest_form_get_step($form_state);
  $steps = &islandora_ingest_form_get_steps($form_state);
  if (!empty($steps[$step])) {
    return $steps[$step];
  }
  return NULL; // @todo deal with unknown case.
}

/**
 * Get a reference to the steps stored in the form state.
 *
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   All the steps to be used in the ingest process.
 */
function &islandora_ingest_form_get_steps(array &$form_state) {
  return $form_state['islandora']['steps'];
}

/**
 * Call form_load_include, for the current step if it defines a required file.
 *
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_ingest_form_step_form_load_include(array &$form_state) {
  form_load_include($form_state, 'inc', 'islandora', 'includes/ingest.form'); // Always load this file.
  $step_info = islandora_ingest_form_get_step_info($form_state);
  if (isset($step_info['file']) && isset($step_info['module'])) {
    $matches = array();
    preg_match('/^(.*)\.(.*)$/', $step_info['file'], $matches);
    list($file, $name, $type) = $matches;
    form_load_include($form_state, $type, $step_info['module'], $name); // Load include files.
  }
}

/**
 * Get the number of steps.
 *
 * @param array $form_state
 *   The drupal form state.
 *
 * @return int
 *   The number of steps.
 */
function islandora_ingest_form_get_step_count(array $form_state) {
  $steps = islandora_ingest_form_get_steps($form_state);
  return count($steps);
}

/**
 * Buildes the initial list of ingest steps. Sorted by weight expected range between -50 to 50.
 *
 * The sort order is undefined for steps which have the same weight.
 *
 * @param array $configuration
 *   The configuration options used to build the multi-paged ingest process.
 *
 * @return array
 *   The initial list of sorted ingest steps as defined by all implementers of ISLANDORA_INGEST_STEP_HOOK.
 */
function islandora_ingest_get_steps(array $configuration) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $steps = array();
  $models = isset($configuration['models']) ? $configuration['models'] : array();
  foreach (islandora_build_hook_list(ISLANDORA_INGEST_STEP_HOOK, $models) as $hook) {
    $steps = array_merge($steps, module_invoke_all($hook, $configuration));
  }
  usort($steps, 'drupal_sort_weight');
  return $steps;
}

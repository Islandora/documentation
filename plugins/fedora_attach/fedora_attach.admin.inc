<?php
// $Id$                                                                                                                                                  

/** 
 * @file 
 * Administration page callbacks for the redmine_issuer module.
 */ 


/**
 * Form builder.  Configure Redmine parameters.
 *
 * @ingroup forms
 * @see system_settings_form().                                           
 */
function fedora_attach_admin() {
  $options = drupal_map_assoc(explode(" ", variable_get('fedora_pids_allowed', 'default: demo: changeme: Islandora: ilives: ')));
  $default_value = variable_get('fedora_attach_pid_namespace', 'default:');

  if (!in_array($default_value, $options)) {
    drupal_set_message( "The value last set here ($default_value) is not in the list of available PID namespaces (perhaps it has changed?).  Please choose a valid option from the list.", 'warning' );
  }

  $form = array();
  $form['fedora_attach_pid_namespace'] = array(
    '#type' => 'select',
    '#title' => t('Fedora Attach PID namespace'),
    '#options' => $options,
    '#default_value' => $default_value,
    '#description' => t('The PID namespace into which the Fedora Attach module will ingest objects.'),
  );
  return (system_settings_form($form));
}


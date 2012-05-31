<?php

/**
 * @file islandora.admin.inc
 * Create admin form
 * @return array 
 */

  function islandora_repository_admin() {
    if (!user_access('administer site configuration')) {
      drupal_set_message(t('You must be a site administrator to edit the Fedora collections list.'), 'error');
      return;
    }
   
    $form = array();
    $form['islandora_repository_pid'] = array(
      '#type' => 'textfield',
      '#title' => t('Root Collection PID'),
      '#default_value' => variable_get('islandora_repository_pid', 'islandora:root'),
      '#description' => t('The PID of the Root Collection Object'),
      '#required' => TRUE,
      '#weight' => -18
    );

    $form['islandora_base_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Fedora base URL'),
      '#default_value' => variable_get('islandora_base_url', 'http://localhost:8080/fedora'),
      '#description' => t('The URL to use for REST connections'),
      '#required' => TRUE,
      '#weight' => -16,
    );
        
// will allow admin user to remove namepsace restrictions if not explicitly disallowed  in settings.php
    $form['islandora_namespace'] = array(
      '#type' => 'fieldset',
    );

    $form['islandora_namespace']['islandora_namespace_restriction_enforced'] = array(
      '#weight' => -1,
      '#type' => 'radios',
      '#title' => t('Enforce namespace restrictions'),
      '#options' => array(
        TRUE => t('Enabled'),
        FALSE => t('Disabled')
      ),
      '#description' => t('Allow administrator to restrict user\'s access to the PID namepaces listed below'),
      '#default_value' => variable_get('islandora_namespace_restriction_enforced', FALSE),
    );

    $form['islandora_namespace']['islandora_pids_allowed'] = array(
      '#type' => 'textfield',
      '#title' => t('PID namespaces allowed in this Drupal install'),
      '#default_value' => variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '),
      '#description' => t('A space separated list of PID namespaces that users are permitted to access from this Drupal installation. <br /> This could be more than a simple namespace ie demo:mydemos. <br> islandora: is reserved and is always allowed.'),
      '#weight' => 0,
    );


    return system_settings_form($form);
  }
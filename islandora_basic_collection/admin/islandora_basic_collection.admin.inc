<?php

function islandora_basic_collection_admin() {
  $form = array();
  $form['islandora_basic_collection_use_for_default_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide the view for the default tab'),
    '#default_value' => variable_get('islandora_basic_collection_use_for_default_tab', 'TRUE'),
    '#description' => t('Should this module provide the view for the default view tab.  If you are seeing unexpected content
      on a view tab you may have multiple modules configured to provide content for the default tab.'),
    '#weight' => -10
  );
  $form['islandora_basic_collection_page_size'] = array(
    '#type' => 'textfield',
    '#title' => t('The default number of objects to show for a collection'),
    '#default_value' => variable_get('islandora_basic_collection_page_size', '10'),
    '#description' => t('The default number of object to show in a collection view.'),
    '#weight' => -10
  );

  return system_settings_form($form);
}


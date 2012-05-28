<?php

function islandora_manage_policies_form($form, &$form_state, $collection_pid) {

  $new_options = array();
  if (is_array($content_models) && is_array($cm_options)) {
    $new_options = array_diff_key($content_models, $cm_options);
  }

  $form ['manage_collection_policy']['add']['content_model_to_add'] = array(
    '#title' => "Choose Content Model",
    '#type' => 'select',
    '#options' => $new_options,
    '#description' => t("Choose content model to add to this collection policy."),
  );

  $form ['manage_collection_policy']['add']['new_cp_namespace'] = array(
    '#title' => "Choose Namespace",
    '#type' => 'textfield',
    '#size' => 15,
    '#default_value' => $collection_namespace,
    '#description' => t("Choose namespace for objects in this collection associated with this content model"),
  );
  $form['manage_collection_policy']['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Content Model to Collection Policy'),
    '#id' => 'add_cm'
  );

  if (count($current_models_in_policy) > 0) {
    $form['manage_collection_policy']['remove'] = array(
      '#title' => "Delete Content Model from Collection Policy",
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form ['manage_collection_policy']['remove']['content_models_to_remove'] = array(
      '#title' => "Choose Content Model to Remove",
      '#type' => 'checkboxes',
      '#options' => $current_models_in_policy,
      '#description' => t("Choose content models to remove from this collection policy."),
    );


    $form['manage_collection_policy']['remove']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Remove Content Model From Collection Policy'),
      '#id' => 'remove_cm'
    );
  }
  return $form;
}

function islandora_manage_policies_form_validate($form, &$form_state) {
  
}

function islandora_manage_policies_form_submit($form, &$form_state) {
  
}
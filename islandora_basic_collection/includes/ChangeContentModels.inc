<?php

/**
 * @file
 * ChangeContentModels.inc
 */

function islandora_change_content_models_form($form, &$form_state, $collection_pid) {

  $represented_content_models = get_represented_content_models($collection_pid);
  var_dump($represented_content_models);

  $form['change_cmodel']['titlebox'] = array(
    '#type' => 'item',
    '#title' => t("Change Content Models within @collection_pid", array('@collection_pid' => $collection_pid)),
  );

  $form['change_cmodel']['current_content_model'] = array(
    '#title' => "Choose content model to be changed",
    '#type' => 'select',
    '#options' => $represented_content_models,
    '#description' => t("All objects in this collection with the selected content model will be changed."),
  );

  $form['change_cmodel']['new_content_model'] = array(
    '#title' => "Choose new content model",
    '#type' => 'select',
    '#options' => $new_content_models,
    '#description' => t("The new content model to be assigned to selected objects."),
  );

  $form['change_cmodel']['collection_pid'] = array(
    '#type' => 'hidden',
    '#value' => $this_collection_pid,
  );

  $form['change_cmodel']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change Content Model Associations'),
    '#id' => 'change_model',
  );

  return $form;
}

function islandora_change_content_models_form_validate($form, &$form_state) {

}

function islandora_change_content_models_form_submit($form, &$form_state) {

}
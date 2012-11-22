<?php

/**
 * @file
 *
 * This file contains the admin (confirmation) form and callback functions to purge an object.
 */

/**
 * The admin delete object form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param FedoraObject $object
 *   The object to be deleted.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_delete_object_form(array $form, array &$form_state, FedoraObject $object) {
  $form_state['object'] = $object;
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $object->label)),
    "islandora/object/$object->id",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Gives the option of deleting or purging and object.
 *
 * @note The description below probably applies to islanodra_object_purge() function...
 *
 * The default behaviour is to purge the object to reduce maintenance.
 * If a solution pack wants to change this behaviour and have the object set to deleted then
 * it can respond to the 'islandora_pre_purge_object' hook with an array containing the pair
 * 'delete' => TRUE.
 * Once the object has been deleted/purged then a second call lets the solution packs know that
 * the object has been dealt with. In this call the object id and content models are sent out so
 * that the solution packs can act on this news. There is no guarantee that the object still exists
 * and so the object object isn't sent.
 *
 * @todo Clean Up!
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_delete_object_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  $object = $form_state['object'];
  $parents = islandora_get_parents_from_rels_ext($object);
  $parent = array_pop($parents);
  $form_state['redirect'] = isset($parent) ? "islandora/object/{$parent->id}" : 'islandora';
  islandora_delete_object($object);
}

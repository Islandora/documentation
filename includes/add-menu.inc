<?php

function islandora_add_callback($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  global $user;
  $connection = new RestConnection($user);
  $object = $connection->repository->getObject($pid);

  dsm($object->id);

  /**

  $ingest_registry = module_invoke_all('islandora_ingest_registry', $collection_pid);

  $registry_count = count($ingest_registry);

  if ($registry_count == 0) {
    // No ingest implementations.
    drupal_set_message(t('There are no ingest methods specified for this collection.'));
    drupal_goto('islandora/object/' . $collection_pid);
  }
  elseif ($registry_count == 1) {
    // One registry implementation, go there
    drupal_goto($ingest_registry[0]['url']);
  }
  else {
    // Multiple ingest routes registered
    return islandora_ingest_registry_render($ingest_registry);
  }
   *
   */
}
<?php

/**
 * @file
 * Provides the callback implementations for the service.
 */

/**
 * The POST callback for the deletion by Fedora URI service.
 *
 * @param array $args
 *   JSON encoded arguments to delete function.
 *
 * @return array
 *   JSON encoded response message.
 */
function islandora_delete_by_fedora_uri_service_delete(array $args) {
  // Hack the Fedora path out of the POST data.
  $fedora_uri = $args['fedoraUri'];

  // Get entities identified by Fedora path.
  $query = new EntityFieldQuery();
  $results = $query->fieldCondition('field_fedora_path', 'value', $fedora_uri)
    ->execute();

  // Delete 'em all.
  foreach ($results as $entity_type => $entities) {
    $ids = array_keys($entities);
    if (entity_delete_multiple($entity_type, $ids) === FALSE) {
      $response[] = "Could not delete any $entity_type identified by $fedora_uri.";
    }
    else {
      $response[] = "Deleted entities of type $entity_type with ids " . implode(', ', $ids) . ".";
    }
  }

  if (!isset($response)) {
    $response[] = "Could not find any entities identified by $fedora_uri.";
  }

  return $response;
}

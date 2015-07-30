<?php

/**
 * @file
 * Provides the callback implementations for the service.
 */

/**
 * The POST callback for the TN service.
 *
 * @param string $type
 *   Entity type to which the RDF mapping belongs.
 * @param string $bundle
 *   Bundle to which the RDF mapping belongs.
 *
 * @return array
 *   The RDF mapping.
 */
function islandora_tn_service_create($args) {
  $uuid = $args['uuid'];
  $tn_b64 = $args['file'];
  $mimetype = $args['mimetype'];
  $exploded = explode("/", $mimetype);
  $extension = $exploded[1];

  $entities = entity_uuid_load('node', array($uuid));
  
  if (empty($entities)) {
    return "Could not add thumbnail because there is no entity identified by $uuid"; 
  }

  $node = array_pop($entities);

  $file = file_save_data(base64_decode($tn_b64), "public://" . $uuid . "_TN." . $extension, FILE_EXISTS_REPLACE);
 
  $node->field_tn[LANGUAGE_NONE][] = array(
    'fid' => $file->fid,
    'width' => 100,
    'height' => 100,
    'alt' => "Thumbnail for node $uuid",
    'title' => "Thumbnail for node $uuid",
  );
 
  node_save($node);

  return "Successfully added thumbnail to node $uuid";
}

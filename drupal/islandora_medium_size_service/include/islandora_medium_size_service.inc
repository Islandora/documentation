<?php

/**
 * @file
 * Provides the callback implementations for the service.
 */

/**
 * The POST callback for the MEDIUM_SIZE service.
 *
 * @param array $args
 *   JSON encoded arguments.
 *
 * @return string 
 *   Response message.
 */
function islandora_medium_size_service_create($args) {
  $uuid = $args['uuid'];
  $tn_b64 = $args['file'];
  $mimetype = $args['mimetype'];
  $exploded = explode("/", $mimetype);
  $extension = $exploded[1];

  $entities = entity_uuid_load('node', array($uuid));
  
  if (empty($entities)) {
    return "Could not add medium sized image because there is no entity identified by $uuid"; 
  }

  $node = array_pop($entities);

  $file = file_save_data(base64_decode($tn_b64), "public://" . $uuid . "_MEDIUM_SIZE." . $extension, FILE_EXISTS_REPLACE);
 
  $node->field_medium_size[LANGUAGE_NONE][] = array(
    'fid' => $file->fid,
    'alt' => "Medium sized image for node $uuid",
    'title' => "Medium sized image for node $uuid",
  );
   $node->field_tn[LANGUAGE_NONE][] = array(
    'fid' => $file->fid,
    'width' => 100,
    'height' => 100,
    'alt' => "Thumbnail for node $uuid",
    'title' => "Thumbnail for node $uuid",
  );

  node_save($node);

  return "Successfully added medium sized image to node $uuid";
}

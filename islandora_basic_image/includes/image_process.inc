<?php


/**
 *
 * @param object $file 
 *   stdclass
 */
function islandora_basic_image_create_derivative($file, $width, $height) {
  $real_path = drupal_realpath($file->uri);
  $image = image_load($real_path);
  if (!empty($image)) {
    $scale = image_scale($image, $width, $height, TRUE);
    if($scale){
      return image_save($image);
    }
  }
  return FALSE;
}

/**
 * adds a datastream and deletes the tmp file from local file system
 * @param object $object
 * @param string $dsid
 * @param object $file 
 */
function islandora_basic_image_add_datastream($object, $dsid, $file) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = $object['OBJ']->mimeType;
    $ds->setContentFromFile(drupal_realpath($file->uri));
    $object->ingestDatastream($ds);
    file_delete($file);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}


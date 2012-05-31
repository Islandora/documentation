<?php

function islandora_basic_image_create_all_derivatives($object){
  module_load_include('inc', 'islandora', 'includes/MimeClass');
  module_load_include('inc', 'islandora_basic_image', 'includes/image_process');
  $mime_class = new MimeClass();
  if(!isset($object['OBJ'])){
    drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.',array('%s' => $object->id)),'error');
    return "";
  }
  $ext = $mime_class->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);
  $original_file = file_save_data($object['OBJ']->content, 'temporary://' . $file_name . 'OBJ.' . $ext);
  $tn_file = file_copy($original_file, 'temporary://' . $file_name . 'TN.' . $ext);
  if (islandora_basic_image_create_derivative($tn_file, 200, 200)) {
    islandora_basic_image_add_datastream($object, 'TN', $tn_file);
  }
  $medium_file = file_copy($original_file, 'temporary://' . $file_name . 'MEDIUM.' . $ext);
  if (islandora_basic_image_create_derivative($medium_file, 500, 700)) {
    islandora_basic_image_add_datastream($object, 'MEDIUM_SIZE', $medium_file);
  }
  file_delete($original_file);
}


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


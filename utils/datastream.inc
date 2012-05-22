<?php
/**
 * @file datastream.inc 
 */

/**
 *
 * @global object $user
 * @param string $object_id
 * @param string $dsid
 * @return stream 
 *  printsdatastream to browser
 */
function islandora_datastream_as_attachment($object_id, $dsid){
  module_load_include('inc', 'islandora', 'RestConnection');
  global $user;
  try {
    $restConnection = new RestConnection($user);
    $fedora_object = new FedoraObject($object_id, $restConnection->repository);
  } catch (Exception $e) {
    drupal_set_message(t('Error getting Islanndora datastream $d for object %s', array('%s' => $object_id, '%d' => $dsid)), 'error');
    return"";
  }
  header('Content-type: '.$fedora_object[$dsid]->mimetype);
  header('Content-length: '.$fedora_object[$dsid]->size);
  header("Cache-control: private");
  $method = arg(5);
  if($method =='download'){
    header("Content-Disposition: attachment; filename=\"".$fedora_object[$dsid]->label);
  }
  print ($fedora_object[$dsid]->content);
  exit();
}
?>

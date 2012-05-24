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
 *  prints datastream to browser
 */

define('DS_COMP_STREAM', 'DS-COMPOSITE-MODEL');

function islandora_datastream_as_attachment($object_id, $dsid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  global $user;
  try {
    $restConnection = new RestConnection($user);
    $fedora_object = new FedoraObject($object_id, $restConnection->repository);
  } catch (Exception $e) {
    drupal_set_message(t('Error getting Islanndora datastream $d for object %s', array('%s' => $object_id, '%d' => $dsid)), 'error');
    return"";
  }
  header('Content-type: ' . $fedora_object[$dsid]->mimetype);
  header('Content-length: ' . $fedora_object[$dsid]->size);
  header("Cache-control: private");
  $method = arg(5);
  if (isset($method) && $method == 'download') {
    header("Content-Disposition: attachment; filename=\"" . $fedora_object[$dsid]->label);
  }
  print($fedora_object[$dsid]->content);
  exit();
}


/**
 *
 * @param array $arr
 *   an array of dsids that are defined by this objects cmodels
 * @param string $ds_comp_stream 
 *   the dscomposite stream as xml
 */
function islandora_update_available_dsids_array(&$arr, $ds_comp_stream){
  $sxml = new SimpleXMLElement($ds_comp_stream);
  foreach($sxml->dsTypeModel as $ds){
    //$arr[$ds['ID']] 
    $mimes = array();
    foreach($ds->form as $form){
      $mimetype = (string)$form['MIME'];
      $mimes[] = $mimetype;
    }
    $dsid = (string)$ds['ID'];
    if($dsid != 'AUDIT'){
      $arr[(string)$ds['ID']] = $mimes;
    }
  }  
}

/**
 * this function may not be being used
 * @param type $pid
 * @param type $form_state
 * @return string 
 */
function islandora_add_datastream_form($object_id, &$form_state) {
  //dump_vars($form_state);
  // Populate the list of datastream IDs.
  module_load_include('inc', 'islandora', 'RestConnection');
  global $user;
  try {
    $restConnection = new RestConnection($user);
    $fedora_object = new FedoraObject($object_id, $restConnection->repository);
  } catch (Exception $e) {
    drupal_set_message(t('Error getting Islandora object %s ', array('%s' => $object_id)), 'error');
    return "";
  }
  if (!isset($fedora_object)) {
    drupal_set_message(t('Could not create add datastream form for %s'), array('%s' => $object_id));
    return;
  }
  $models = $fedora_object->models;
  $available_dsids = array();
  if (isset($models)) {
    foreach ($models as $model) {
      try {
        $model_object = new FedoraObject($model, $restConnection->repository);
        $dscomposite_stream = $model_object[DS_COMP_STREAM]->content;
        islandora_update_available_dsids_array($available_dsids, $dscomposite_stream);
      } catch (Exception $e) {
        //do nothing as other objects may have a dscompsite stream
      }
      //$model_ds_comp = 
    }
  }
  
  /*if (!empty($content_models)) {
    foreach ($content_models as $content_model) {


      $newElements = $content_model->listDatastreams();
      if (!empty($newElements)) {
        $available_dsids = array_merge($available_dsids, $newElements);
      }
    }
  }

  $item = new Fedora_Item($pid);
  $used_datastreams = $item->get_datastreams_list_as_SimpleXML();
  $used_datastream_ids = array();
  foreach ($used_datastreams->datastreamDef as $used_datastream) {
    array_push($used_datastream_ids, $used_datastream->ID);
  }
  $unused_dsids = array();

  if ($form_state['submitted'] && $form_state['clicked_button']['#value'] != 'OK') {
    $form['add_datastream_label'] = array(
      '#value' => t('<br /><h3>The datastream has been uploaded.</h3>'),
      '#weight' => -10,
    );
    $form['#redirect'] = "fedora/repository/$pid/";
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('OK')
    );
    return $form;
  }
  if (!empty($available_dsids)) {
    $unused_dsids = array_diff($available_dsids, $used_datastream_ids);
    if (empty($unused_dsids)) {
      return;
    }
  }

  $form['add_datastream_label'] = array(
    '#value' => t('<br /><h3>Add Datastream:</h3>'),
    '#weight' => -10,
  );

  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => "$pid"
  );

  $form['stream_label'] = array(
    '#title' => 'Datastream Label',
    '#required' => 'TRUE',
    '#description' => t('A Human readable label'),
    '#type' => 'textfield'
  );

  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['add-stream-file-location'] = array(
    '#type' => 'file',
    '#title' => t('Upload Document'),
    '#size' => 48,
    // '#required'=>'TRUE',
    '#description' => t('The file to upload.')
  );
  $form['#redirect'] = "fedora/repository/$pid/";
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Datastream')
  );

  if (!empty($unused_dsids)) {
    $dsidsForForm = array();
    foreach ($unused_dsids as $dsid) {
      $dsidsForForm[$dsid] = $dsid;
    }
    $form['stream_id'] = array(
      '#type' => 'select',
      '#title' => t('Datastream ID'),
      '#default_value' => variable_get('feed_item_length', 'teaser'),
      '#weight' => '-1',
      '#description' => t('Datastream IDs defined by the content model.'),
    );
    $form['stream_id']['#options'] = array_combine($unused_dsids, $unused_dsids);
  }
  else {
    $form['stream_id'] = array(
      '#title' => 'Datastream ID',
      '#required' => 'TRUE',
      '#description' => t('An ID for this stream that is unique to this object. Must start with a letter and contain only alphanumeric characters and dashes and underscores.'),
      '#type' => 'textfield',
      '#weight' => -1,
    );
  }
  return $form;*/
}

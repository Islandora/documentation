<?php

/**
 * @file
 *
 * This file contains the admin form and callback functions for datastream manipulations.
 */

/**
 * Callback to download the given datastream to the users computer.
 *
 * @param FedoraDatastream $datastream
 *   The datastream to download.
 */
function islandora_download_datastream(FedoraDatastream $datastream) {
  islandora_view_datastream($datastream, TRUE);
}

/**
 * Callback function to view or download a datastream.
 *
 * @note
 *  This function calls exit().
 *
 * @param FedoraDatastream $datastream
 *   The datastream to view/download.
 * @param boolean $download
 *   If TRUE the file is download to the user computer for viewing otherwise it will attempt to display in the browser natively.
 */
function islandora_view_datastream(FedoraDatastream $datastream, $download = FALSE) {
  header_remove('Cache-Control');
  header_remove('Expires');
  header('Content-type: ' . $datastream->mimetype);
  if ($datastream->controlGroup == 'M' || $datastream->controlGroup == 'X') {
    header('Content-length: ' . $datastream->size);
  }
  if ($download) {
    header("Content-Disposition: attachment; filename=\"{$datastream->label}\"");
  }
  drupal_page_is_cacheable(FALSE);
  // Try not to load the file into PHP memory!
  $file = drupal_tempnam(file_directory_temp(), 'islandora');
  $datastream->getContent($file);
  readfile($file);
  drupal_unlink($file);
  exit();
}

/**
 * Get the human readable size of the given datastream.
 *
 * @param FedoraDatastream $datastream
 *   The datastream to check.
 *
 * @return string
 *   A human readable size of the given datastream, or '-' if the size could not be determined.
 */
function islandora_datastream_get_human_readable_size(FedoraDatastream $datastream) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $size_is_calculatable = $datastream->controlGroup == 'M' || $datastream->controlGroup == 'X';
  return $size_is_calculatable ? islandora_convert_bytes_to_human_readable($datastream->size) : '-';
}

/**
 * Get either the 'view' or 'download' url for the given datastream if possible.
 *
 * @param FedoraDatastream $datastream
 *   The datastream to generated the url to.
 *
 * @return string
 *   either the 'view' or 'download' url for the given datastream.
 */
function islandora_datastream_get_url(FedoraDatastream $datastream, $type = 'download') {
  return $datastream->controlGroup == 'R' ? $datastream->url : "islandora/object/{$datastream->parent->id}/datastream/{$datastream->id}/$type";
}

/**
 * Gets the delete link.
 *
 * @param FedoraDatastream $datastream
 *   The datastream to generated the url to.
 */
function islandora_datastream_get_delete_link(FedoraDatastream $datastream) {
  $datastreams = module_invoke_all('islandora_undeletable_datastreams', $datastream->parent->models);
  if (in_array($datastream->id, $datastreams)) {
    return '';
  }
  else {
    return l(t('delete'), 'islandora/object/' . $datastream->parent->id . '/datastream/' . $datastream->id . '/delete');
  }
}


function islandora_datastream_edit_get_link($object, $ds_id) {
  $edit_registry = module_invoke_all('islandora_edit_datastream_registry', $object, $ds_id);
  if (count($edit_registry) > 0 && user_access(FEDORA_METADATA_EDIT)) {
    return l(t('edit'), 'islandora/object/' . $object->id . '/datastream/' . $ds_id . '/edit');
  }
  else {
    return '';
  }
}

function islandora_edit_datastream($object, $ds_id) {
  $edit_registry = module_invoke_all('islandora_edit_datastream_registry', $object, $ds_id);
  $edit_count = count($edit_registry);

  if ($edit_count == 0) {
    // No edit implementations.
    drupal_set_message(t('There are no edit methods specified for this datastream.'));
    drupal_goto('islandora/object/' . $object->id . '/manage/datastreams');
  }
  elseif ($edit_count == 1) {
    // One registry implementation, go there
    drupal_goto($edit_registry[0]['url']);
  }
  else {
    // Multiple edit routes registered
    return islandora_edit_datastream_registry_render($edit_registry);
  }
}

// @TODO: theme / preprocess
function islandora_edit_datastream_registry_render($edit_registry) {
  $output = array(
    '#type' => 'markup',
    '#markup' => '',
  );
  foreach ($edit_registry AS $edit_route) {
    $output['#markup'] .= l($edit_route['name'], $edit_route['url']) . '<br/>';
  }
  return $output;
}

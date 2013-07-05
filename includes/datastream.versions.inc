<?php

function islandora_datastream_versions_table ($datastream) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');

  drupal_set_title("$datastream->id Previous Versions");

  $header = array();
  $header[] = array ('data' => 'Created Date');
  $header[] = array('data' => t('Size'));
  $header[] = array('data' => t('Label'));
  $header[] = array('data' => t('Mime type'));
  $rows = array();

  foreach ($datastream as $version => $datastream_version) {
    $row = array();
    $row[] = array(
      'class' => 'datastream-date',
      'data' => theme('islandora_datastream_view_link', array(
        'datastream' => $datastream,
        'label' => $datastream_version->createdDate->format(DATE_RFC850),
        'version' => $version,
      )),
    );
    $row[] = array(
      'class' => 'datastream-size',
      'data' => islandora_datastream_get_human_readable_size($datastream_version),
    );
    $row[] = array(
      'class' => 'datastream-label',
      'data' => $datastream_version->label,
    );
    $row[] = array(
      'class' => 'datastream-mime',
      'data' => $datastream_version->mimeType,
    );
    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
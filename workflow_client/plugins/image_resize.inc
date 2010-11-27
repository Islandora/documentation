<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'process');

class image_resize extends Process {
  protected function process($pid, $parameters) {
    $required_params = array('dsid', 'width', 'file_ext');
    $missing_params = array();
    foreach ($required_params as $param)
      if (!isset($parameters[$param]))
        $missing_params[]=$param;
    if (count($missing_params) > 0) {
      $this->setMessage(t('Missing parameter(s) "%params" for image_resize process on "%pid"', array('%params' => join(',', $missing_params), '%pid' => $pid)));
      return FALSE;
    }

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');    
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    
    $item = new fedora_item($pid);
    $dslist = $item->get_datastreams_list_as_array();    
    if (!isset($dslist[$parameters['dsid']])) {
      $this->setMessage(t('Datastream "%dsid" could not be found for image_resize process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $ds = $dslist[$parameters['dsid']];
    $file = '/tmp/'. $ds['label'];
    $dest_ds = isset($parameters['dest_ds']) ? $parameters['dest_ds'] : 'TN';
    
    $objectHelper = new ObjectHelper();
    $objectHelper->makeObject($pid, $parameters['dsid'], FALSE, NULL, $file);
       
    
    $system = getenv('System');
    $file_suffix = '_'. $dest_ds . '.'. $parameters['file_ext'];
    $returnValue = TRUE;
    $output = array();

    $command = 'convert -resize '. $parameters['width'].(isset($parameters['height'])?'x'.$parameters['height']:'') .' -quality 85  '. $file . '[0] -strip '. $file . $file_suffix .' 2>&1 &';
    exec($command, $output, $returnValue);
    
    if (!file_exists($file . $file_suffix)) {
      $this->setMessage('command failed: '. htmlentities($command ."\n". join("\n", $output) ."\n return value: $returnValue"));
      @unlink($file);
      return FALSE;
    }
    
    if ($returnValue == '0') {
      
      if (isset($dslist[$dest_ds])) {
        $item->purge_datastream($dest_ds);
      }
      $ret = $item->add_datastream_from_file($file . $file_suffix, $dest_ds, $ds['label'] . $file_suffix);
      @unlink($file);
      @unlink($file . $file_suffix);
      
      if (!$ret) {
        $this->setMessage(t('Unable to add datastream "%dsid" to "%pid".', array('%dsid' => $dest_ds, '%pid' => $pid)));
        return FALSE;
      }

      return TRUE;
      
    }
  }
}

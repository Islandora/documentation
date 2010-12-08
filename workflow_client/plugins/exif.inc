<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'process');

class exif extends Process {
  protected function process($pid, $parameters) {
    $required_params = array('dsid');
    $missing_params = array();
    foreach ($required_params as $param)
      if (!isset($parameters[$param]))
        $missing_params[]=$param;
    if (count($missing_params) > 0) {
      $this->setMessage(t('Missing parameter(s) "%params" for exif process on "%pid"', array('%params' => join(',', $missing_params), '%pid' => $pid)));
      return FALSE;
    }

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');    
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    
    $item = new fedora_item($pid);
    $dslist = $item->get_datastreams_list_as_array();    
    if (!isset($dslist[$parameters['dsid']])) {
      $this->setMessage(t('Datastream "%dsid" could not be found for exif process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $ds = $dslist[$parameters['dsid']];
    $file = '/tmp/'. $ds['label'];
    $dest_ds = isset($parameters['dest_ds']) ? $parameters['dest_ds'] : 'EXIF';
    
    $objectHelper = new ObjectHelper();
    $objectHelper->makeObject($pid, $parameters['dsid'], FALSE, NULL, $file);
      
    if (!file_exists($file)) {
      $this->setMessage('couldnt get datastream '. $parameters['dsid'] .' as '. $file);
      return FALSE;
    }
    
    $system = getenv('System');
    $returnValue=TRUE;
    $output=array();

    $command = '/usr/local/exif/exiftool  -X '. $file;
    exec($command, $output, $returnValue);
    
    if (!file_exists($file . $file_suffix)) {
      $this->setMessage('command failed: '. htmlentities($command ."\n" . join("\n", $output) ."\n return value: $returnValue"));
      @unlink($file);
      return FALSE;
    }
    
    if ($returnValue == '0') {
      if (isset($dslist[$dest_ds])) {
        $item->purge_datastream($dest_ds);
      }
      
      $ret = $item->add_datastream_from_string(join("\n",$output), $dest_ds, $ds['label'] . $file_suffix, 'text/xml', 'X','Added by workflow process EXIF.');
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

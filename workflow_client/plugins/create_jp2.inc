<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'process');

class create_jp2 extends Process {
  protected function process($pid, $parameters) {
    $required_params = array('dsid');
    $missing_params = array();
    foreach ($required_params as $param)
      if (!isset($parameters[$param])) 
        $missing_params[] = $param;
        if (count($missing_params) > 0) {
          $this->setMessage(t('Missing parameter(s) "%params" for create_jp2 process on "%pid"', array('%params' => join(',', $missing_params), '%pid' => $pid)));
          return FALSE;
        }

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');    
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    
    $item = new fedora_item($pid);
    $dslist = $item->get_datastreams_list_as_array();    
    if (!isset($dslist[$parameters['dsid']])) {
      $this->setMessage(t('Datastream "%dsid" could not be found for create_jp2 process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $ds = $dslist[$parameters['dsid']];
    $file = '/tmp/'. $ds['label'];
    
    $objectHelper = new ObjectHelper();
    $objectHelper->makeObject($pid, $parameters['dsid'], FALSE, NULL, $file);
       
    $returnValue = TRUE;
    $output=array();
    $command='/usr/local/kakadu/kdu_compress -i "'. $file . '" -o "'. $file .'.jp2" -rate 0.5 Clayers=1 Clevels=7 "Cprecincts={256,256},{256,256},{256,256},{128,128},{128,128},{64,64},{64,64},{32,32},{16,16}" "Corder=RPCL" "ORGgen_plt=yes" "ORGtparts=R" "Cblk={32,32}" Cuse_sop=yes -num_threads 2';
    exec($command, $output, $returnValue);
   
    if (!file_exists($file .'.jp2')) {
      $this->setMessage('command failed: '. htmlentities($command ."\n" . join("\n", $output) ."\n return value: $returnValue"));
      @unlink($file);
      return FALSE;
    }

    $returnValue = TRUE;
    $output=array();
    $command='/usr/local/kakadu/kdu_compress -i "'. $file . '" -o "'. $file .'_lossless.jp2" -rate -,0.5 Clayers=2 Creversible=yes Clevels=8 "Cprecincts={256,256},{256,256},{128,128}" Corder="RPCL" ORGgen_plt="yes" ORGtparts="R" Cblk="{32,32} -num_threads 2"';
    exec($command, $output, $returnValue);

    if (!file_exists($file .'_lossless.jp2')) {
      $this->setMessage('command failed: '. htmlentities($command ."\n". join("\n", $output) ."\n return value: $returnValue"));
      @unlink($file);
      return FALSE;
    }    
    
    if (file_exists($file.'.jp2') && file_exists($file.'_lossless.jp2')) {
      
      if (isset($dslist['JP2'])) {
        $item->purge_datastream('JP2');
      }
      if (isset($dslist['LOSSLESS_JP2'])) {
        $item->purge_datastream('LOSSLESS_JP2');
      }
      
      $ret = $item->add_datastream_from_file($file .'.jp2', 'JP2' , $ds['label'] .'.jp2');
      $ret = $ret && $item->add_datastream_from_file($file .'_lossless.jp2', 'LOSSLESS_JP2', $ds['label'] .'_lossless.jp2');


      @unlink($file);
      @unlink($file .'.jp2');
      @unlink($file .'_lossless.jp2');
      
      if (!$ret) {
        $this->setMessage(t('Unable to add datastream "%dsid" to "%pid".', array('%dsid' => $dest_ds, '%pid' => $pid)));
        return FALSE;
      }

      return TRUE;
      
    }
  }
}

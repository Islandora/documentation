<?php

class FedoraObject {
  function  __construct($pid = '') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {

      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }
  
  public function showFieldSets() {
    $objHelper = new ObjectHelper();
    
    return $objHelper->getFormattedDatastreamList($this->pid, NULL);
  }
}
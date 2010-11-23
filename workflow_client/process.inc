<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'workflow');

abstract class Process {
  
  private $workflow;
  private $process;
  private $message = NULL;
  
  function __construct(&$wf, $processId) {
    $this->workflow = &$wf;
    $this->process=$wf->getProcess($processId);
  }
  
  public function run() {
    $ret = $this->process($this->workflow->pid, $this->process['params']);
    $state = ($ret === FALSE) ? 'error' : 'completed';
    $this->workflow->recordAttempt($this->process['id'], $state, $this->message);
    
    return $ret;
  }
  
  protected function setMessage($msg) {
    $this->message = $msg;
  }
  
  abstract protected function process($pid, $parameters);
}

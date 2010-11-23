<?php

// $Id$

module_load_include('inc', 'fedora_repository', 'XMLDatastream');

class Workflow extends XMLDatastream {
    static $SCHEMA_URI = 'http://localhost/workflow.xsd'; 
    static $DEFAULT_DSID = 'WORKFLOW';
    static $MAX_ATTEMPTS = 3;
    
    protected $name = 'Workflow Record';
    
  /**
   * Gets the default DSID to use for ContentModel datastreams.
   *
   * @return string $default_dsid
   */
  static function getDefaultDSID() {
    return variable_get('Islandora_Workflow_DSID', Workflow::$DEFAULT_DSID);
  }

  /**
   * Gets the max number of attempts
   *
   * @return string $default_dsid
   */
  static function getMaxAttempts() {
    return variable_get('Islandora_Workflow_max_attempts', Workflow::$MAX_ATTEMPTS);
  }

  /**
   * Constructs a new Workflow object from the specified 
   * object PID.  Returns FALSE on failure.
   *
   * @param string $pid
   * @return Workflow $ret
   */
  static function loadFromObject($pid, $dsid = NULL) {
    $ret = FALSE;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    
    try {
      if (self::validPid($pid)) {
        $fedoraItem = new Fedora_Item($pid);
        $dsid = ($dsid != NULL && self::validDsid($dsid)) ? $dsid : Workflow::getDefaultDSID();
        $datastreams = $fedoraItem->get_datastreams_list_as_array();
        if (isset($datastreams[$dsid])) {
          $ds = $fedoraItem->get_datastream_dissemination($dsid);
        }
      }
      
      if (!empty($ds)) {
        $ret=new Workflow($ds, $pid, $dsid);
      }
    } 
    catch (SOAPException $e) {
      $ret = FALSE;
    }
    return $ret;
  }
  
  function resetDependancies($dsid,$logMessage = null) {
    $ret = FALSE;
    if ($this->validate()) {
      $processes = $this->xml->getElementsByTagName('workflow')->item(0)->getElementsByTagName('process');
      for  ($i=0; $i < $processes->length; $i++) {
        $dependencies   = $processes->item($i)->getElementsByTagName('dependant');
	$update = FALSE;
	
	//check log message to make sure that this process didnt modify the datastream.  If this is the case, then dont reset it (prevents loops).
	if ($logMessage != null && !preg_match('/'.strtolower(trim($processes->item($i)->getAttribute('name'))).'/',strtolower($logMessage)))
	{
	  for ($j=0; $j < $dependencies->length; $j++) {
	    if (trim($dependencies->item($j)->nodeValue) == $dsid) {
	      $update = TRUE;
	    }
	  }
	}

       if ($update) {
         $processes->item($i)->setAttribute('state', 'waiting');
         $ret = TRUE;
       }
      }      
    }

    return $ret;
  }
  
  function getProcesses() {
    $ret = FALSE;
    
    if ($this->validate()) {
      $processes = $this->xml->getElementsByTagName('workflow')->item(0)->getElementsByTagName('process');
      $ret=array();
      for ($i=0; $i < $processes->length; $i++) {
        $id = $processes->item($i)->getAttribute('id');
        $ret[$id]=$processes->item($i)->getAttribute('name');
      }
    }
    return $ret;    
  }
  
  function getReadyProcesses() {
    $ret = FALSE;
    if ($this->validate()) {
      $processes = $this->xml->getElementsByTagName('workflow')->item(0)->getElementsByTagName('process');
      $waitingProcesses = array();
      $completeProcesses = array();
      for ($i=0; $i < $processes->length; $i++) {
        $id=$processes->item($i)->getAttribute('id');
        switch (strtolower($processes->item($i)->getAttribute('state'))) {
          case 'waiting':
            $waitingProcesses[$id]=$processes->item($i);
            break;

          case 'completed':
            $completeProcesses[$id]=$processes->item($i);
            break;

          case 'error':
            if (intval($processes->item($i)->getAttribute('attempts')) < Workflow::getMaxAttempts()) {
              $waitingProcesses[$id]=$processes->item($i);
            }
          break;
        }
      }

      $readyProcesses = array();
      foreach ($waitingProcesses as $id => $proc) {
        $prereqs = $proc->getElementsByTagName('prereq');
        $ready = TRUE;
        for ($i=0;$ready && $i<$prereqs->length;$i++) {
          if (!isset($completeProcesses[trim($prereqs->item($i)->nodeValue)])) {
            $ready = FALSE;
          }
        }
        if ($ready) {
          $readyProcesses[$id]=$proc->getAttribute('name');
        }
      }
      $ret = $readyProcesses;    
    }
    return $ret;
  }
  
  function setState($processId, $state) {
    $ret = FALSE;
    if ($this->validate()) {
      $process = FALSE;
      $processes = $this->xml->getElementsByTagName('workflow')->item(0)->getElementsByTagName('process');
      for ($i=0; $process === FALSE && $i < $processes->length; $i++) {
        if ($processId == $processes->item($i)->getAttribute('id')) {
          $process = $processes->item($i);
        }
      }
      
      if ($process !== FALSE) {
        $process->setAttribute('state', $state);
        $process->setAttribute('attempts', 0);

        $msgEl = $process->getElementsByTagName('message');
        if ($msgEl->length > 0) {
          $process->removeChild($msgEl->item(0));
        }
      }
    }
    return $ret;
  }
  
  function getProcess($processId) {
    $ret = FALSE;
    if ($this->validate()) {
      $process = FALSE;
      $processes = $this->xml->getElementsByTagName('workflow')->item(0)->getElementsByTagName('process');
      for  ($i=0; $process === FALSE && $i < $processes->length; $i++) {
        if ($processId == $processes->item($i)->getAttribute('id')) {
          $process = $processes->item($i);
        }
      }
      
      if ($process !== FALSE) {
        $attempts = $process->getAttribute('attempts');
        if (!$attempts) {
          $attempts = 0;
        }

        $msgEl = $process->getElementsByTagName('message');
        $message = ($msgEl->length > 0)? trim($msgEl->item(0)->nodeValue) : '';

        $params = array();
        $paramEls = $process->getElementsByTagName('parameters');
        if ($paramEls->length > 0) {
          $paramEls=$paramEls->item(0)->getElementsByTagName('parameter');
          for ($i=0; $i < $paramEls->length; $i++) {
           $params[$paramEls->item($i)->getAttribute('name')] = trim($paramEls->item($i)->nodeValue);
          }
        }

        $ret=array('name' => $process->getAttribute('name'),
          'id' => $process->getAttribute('id'),
          'state' => $process->getAttribute('state'),
          'timestamp' => $process->getAttribute('timestamp'),
          'attempts' => $attempts,
          'message' => $message,
          'params' => $params
        );
      }
      
    }
    return $ret;
  }
  
 function recordAttempt($processId, $state, $message) {
    $ret = FALSE;
    if ($this->validate()) {
      $process = FALSE;
      $processes = $this->xml->getElementsByTagName('workflow')->item(0)->getElementsByTagName('process');
      for ($ix= 0; $process === FALSE && $i < $processes->length; $i++) {
        if ($processId == $processes->item($i)->getAttribute('id')) {
          $process = $processes->item($i);
        }
      }

      if ($process !== FALSE) {
        $process->setAttribute('state', $state);
        $attempts = $process->getAttribute('attempts');
        if (!$attempts) {
          $attempts = 1;
        } 
        else 
        $attempts++;
        $process->setAttribute('attempts', $attempts);
        $process->setAttribute('timestamp', date(DateTime::ATOM));

        $msgEl = $process->getElementsByTagName('message');
        if (strlen(trim($message)) > 0) {
          $newMessage = $this->xml->createElement('message', $message);  
          if ($msgEl->length > 0) {
            $process->replaceChild($newMessage, $msgEl->item(0));
          } 
          elseif ($process->hasChildNodes()) {
            $process->insertBefore($newMessage, $process->childNodes->item(0));
          } 
          else {
           $process->appendChild($newMessage);
          }
        } 
        elseif ($msgEl->length > 0) {
          $process->removeChild($msgEl->item(0));
        }

        $ret = TRUE;
      }
      
    }
    return $ret;
  }
    
  function convertFromOldSchema() { return TRUE; } 
}

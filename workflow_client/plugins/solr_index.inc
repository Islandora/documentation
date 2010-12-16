<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'process');

class solr_index extends Process {
  protected function process($pid, $parameters) {
   $required_params = array('dsid');
    $missing_params = array();
    foreach ($required_params as $param)
      if (!isset($parameters[$param]))
        $missing_params[] = $param;
    if (count($missing_params) > 0) {
      $this->setMessage(t('Missing parameter(s) "%params" for solr_index process on "%pid"', array('%params' => join(',', $missing_params), '%pid' => $pid)));
      return FALSE;
    }
    
    if (!isset($parameters['xslt']) && !isset($parameters['xslt_file'])) {
      $this->setMessage(t('Must include either "xslt_file" or "xslt" parameter to specify which template to apply on "%pid" for process solr_index.', array('%pid' => $pid)));
      return FALSE;
    }
    
    if (module_load_include('php', 'islandora_solr_search', 'Solr/Service') === FAlSE) {
      $this->setMessage(t('Unable to load Solr/Service from islandora_solr_search module on "%pid" for process solr_index.', array('%pid' => $pid)));
      return FALSE;
    }
    
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');    
		module_load_include('inc', 'fedora_repository', 'ContentModel'); 

    $xsltDom = NULL;
    if (isset($parameters['xslt'])) {
      list($template_pid, $template_dsid) = explode('/', $parameters['xslt']);
      
      $template_item = new fedora_item($template_pid);
      $dslist = $template_item->get_datastreams_list_as_array();    
      if (!isset($dslist[$template_dsid])) {
        $this->setMessage(t('Datastream "%dsid" for template "%template" could not be found for xslt process on "%pid"', array('%template' => $parameters['xslt'], '%dsid' => $template_dsid, '%pid' => $pid)));
        return FALSE;
      }
      $xsltDom = DOMDocument::loadXML($template_item->get_datastream_dissemination($template_dsid));
    } 
    else {
      $xsltDom = new DOMDocument();
      $xsltDom->load(drupal_get_path('module', 'islandora_workflow_client') .'/xsl/'. trim($parameters['xslt_file']));
    }

		if ($xsdlDom === FALSE) { 
			$this->setMessage(t('Unable to load xslt.')); 
			return FALSE;
		}

    $item = new fedora_item($pid);
    $dslist = $item->get_datastreams_list_as_array();    
    if (!isset($dslist[$parameters['dsid']])) {
      $this->setMessage(t('Datastream "%dsid" could not be found for solr_index process on "%pid"',  array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $dom = DOMDocument::loadXML($item->get_datastream_dissemination($parameters['dsid']));
    if ($dom === FALSE) {
      $this->setMessage(t('Unable to load/interpret DOM Document from "%dsid" for solr_index process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $proc = new XSLTProcessor();
    $proc->importStylesheet($xsltDom);
    $solrDom = $proc->transformToDOC($dom);  
   
		if ($solrDoc === FALSE) { 
			$this->setMessage(t('Document transform failed.'));
			return FALSE;
		}
    $doc = $solrDom->getElementsByTagName('doc');
    if ($doc->length > 0) {
      $doc=$doc->item(0);
      $workflow = Workflow::loadFromObject($pid);
      $procs= $workflow->getProcesses();
      foreach ($procs as $id => $proc) {
        $field=$solrDom->createElement('field', $proc);
        $field->setAttribute('name', 'workflow_process_t');
        $doc->appendChild($field);
      }
    
			if (($cm = ContentModel::loadFromObject($pid)) !== FALSE) { 
				 $field=$solrDom->createElement('field', $cm->pid);
				 $field->setAttribute('name', 'cmodel_t');
				 $doc->appendChild($field);
			}  
		
      if (isset($dslist['DC'])) 
      {
				$dc = $item->get_datastream('DC');
				$field=$solrDom->createElement('field',$dc->createDate);
				$field->setAttribute('name','created_d');
				$doc->appendChild($field);
      }
    }
    
    
    
    $host = variable_get('islandora_solr_search_block_host', 'localhost');
    $port = variable_get('islandora_solr_search_block_port', '8080');
    $appName = variable_get('islandora_solr_search_block_app_name', 'solr');
    $solr = new Apache_Solr_Service($host, $port, '/'. $appName .'/');

    try {
      if ($solr->ping()) {
        $solr->add($solrDom->saveXML()); 
        $solr->commit();
        $solr->optimize();
        return TRUE;
      } 
      else {
        $this->setMessage(t('Unable to connect to Solr at "%solr" for solr_index process on "%pid".', array('%pid' => $pid, '%solr' => $host .':'. $port . '/'. $appName .'/')));
       return FALSE;
      }
    } 
    catch (Exception $e) {
      $this->setMessage(t('Caught exception from Solr at %solr for solr_index process on "%pid":  %msg', array('%pid' => $pid, '%solr' => $host .':'. $port .'/'. $appName .'/', '%msg' => $e->getMessage())));
      return FALSE;
    }
  }
}

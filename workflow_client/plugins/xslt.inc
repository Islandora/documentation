<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'process');

class xslt extends Process {
  protected function process($pid, $parameters) {
    $required_params = array('dest_dsid', 'dsid');
    $missing_params = array();
    foreach ($required_params as $param) {
      if (!isset($parameters[$param])) {
        $missing_params[]=$param;
      }
    }
    if (count($missing_params) > 0) {
      $this->setMessage(t('Missing parameter(s) "%params" for xslt process on "%pid"', array('%params' => join(',', $missing_params), '%pid' => $pid)));
      return FALSE;
    }
    
    if (!isset($parameters['xslt']) && !isset($parameters['xslt_file'])) {
      $this->setMessage(t('Must include either "xslt_file" or "xslt" parameter to specify which template to apply on "%pid"', array('%pid' => $pid)));
      return FALSE;
    }
    

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');    
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

    $item = new fedora_item($pid);
    $dslist = $item->get_datastreams_list_as_array();    

    if (!isset($dslist[$parameters['dsid']])) {
      $this->setMessage(t('Datastream "%dsid" could not be found for xslt process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $modsDom = DOMDocument::loadXML($item->get_datastream_dissemination($parameters['dsid']));
  
    $proc = new XSLTProcessor();
    $proc->importStylesheet($xsltDom);
    
    $dc = $proc->transformToXML($modsDom);  
    if (isset($dslist[$parameters['dest_dsid']])) {
      $item->modify_datastream_by_value( $dc, $parameters['dest_dsid'], isset($parameters['dest_label']) ? $parameters['dest_label'] : $dslist[$parameters['dest_dsid']]['label'], 'text/xml',false, 'Modified by workflow process xslt.');
    } 
    else {
      $item->add_datastream_from_string( $dc, $parameters['dest_dsid'], isset($parameters['dest_label']) ? $parameters['dest_label'] : NULL, 'text/xml', 'X','Added by workflow process xslt.');
    }
    
  }
}

<?php

// $Id$

module_load_include('inc', 'islandora_workflow_client', 'process');

class mods_extend extends Process {
  static $MODS_NS = 'http://www.loc.gov/mods/v3';
  
  protected function process($pid, $parameters) {
    $required_params = array('dsid');
    $missing_params = array();
    foreach ($required_params as $param)
      if (!isset($parameters[$param]))
        $missing_params[]=$param;
    if (count($missing_params) > 0) {
      $this->setMessage(t('Missing parameter(s) "%params" for mods_extend process on "%pid"', array('%params' => join(',', $missing_params), '%pid' => $pid)));
      return FALSE;
    }

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');    
    
    $item = new fedora_item($pid);
    $dslist = $item->get_datastreams_list_as_array();    
    if (!isset($dslist[$parameters['dsid']])) {
      $this->setMessage(t('Datastream "%dsid" could not be found for mods_extend process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    if (!isset($dslist['RELS-EXT'])) {
      $this->setMessage(t('Datastream "RELS-EXT" could not be found for mods_extend process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $modsDom = DOMDocument::loadXML($item->get_datastream_dissemination($parameters['dsid']));
    $relsExtDom = DOMDocument::loadXML($item->get_datastream_dissemination('RELS-EXT'));
  
    if ($modsDom === FALSE) {
      $this->setMessage(t('Unable to load/interpret MODS Document from "%dsid" for mods_extend process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
    
    $modsRoot = $modsDom->getElementsByTagNameNS(mods_extend::$MODS_NS, 'mods');
    
    if ($modsRoot->length > 0) {
      $modsRoot=$modsRoot->item(0);
      
      /* Remove any pre-existing relatedItems,
         physicalDescriptions (mimetypes), or HDL Identifiers
      */
      for ($i = $modsRoot->childNodes->length-1; $i >= 0; $i--) {
        $node = $modsRoot->childNodes->item($i);

        switch ($node->nodeName) {
          case 'mods:relatedItem':
          case 'mods:physicalDescription':
            $modsRoot->removeChild($node);
            break;
          case 'mods:identifier':
            if ($node->getAttribute('type') == 'hdl') {
              $modsRoot->removeChild($node);
            }
            if ($node->getAttribute('type') == 'pid') {
              $modsRoot->removeChild($node);
            }
          break;
        }
      }
      
      $relationships = array('isMemberOfCollection','isMemberOf','isPartOf');
      foreach ($relationships as $relationship) 
      {
	$collections= $relsExtDom->getElementsByTagName($relationship);
	for ($i=0; $i < $collections->length; $i++) {
	  $collection = $collections->item($i);
	  list(, $ident) = explode('/', $collection->getAttribute('rdf:resource'));

	  $collection = new fedora_item($ident);
	  $dc =  new SimpleXMLElement($collection->get_datastream_dissemination('DC'), NULL, FALSE, 'http://purl.org/dc/elements/1.1/');

	  $relatedItem = $modsDom->createElement('mods:relatedItem');
	  $relatedItem->setAttribute('type', 'host');
	  $titleInfo = $modsDom->createElement('mods:titleInfo');
	  $title = $modsDom->createElement('mods:title', $dc->title);
	  $titleInfo->appendChild($title);
	  $relatedItem->appendChild($titleInfo);

	  $identifier = $modsDom->createElement('mods:identifier', $ident);
	  $identifier->setAttribute('type', 'pid');
	  $relatedItem->appendChild($identifier);

	  $ident = preg_replace('/^.*\:/', '10719/', $ident);

	  $identifier = $modsDom->createElement('mods:identifier', $ident);
	  $identifier->setAttribute('type', 'hdl');
	  $relatedItem->appendChild($identifier);

	  $modsRoot->appendChild($relatedItem);
	}
      }
      
      $identifier = $modsDom->createElement('mods:identifier', $pid);
      $identifier->setAttribute('type', 'pid');
      $modsRoot->appendChild($identifier);
     
      $ident= preg_replace('/^.*\:/', '10719/', $pid);
      $identifier = $modsDom->createElement('mods:identifier', $ident);
      $identifier->setAttribute('type', 'hdl');
      $modsRoot->appendChild($identifier);
      
      if (isset($dslist['OBJ']['MIMEType']) && trim($dslist['OBJ']['MIMEType']) != '') {
        $physDesc =$modsDom->createElement('mods:physicalDescription');
        $internetMediaType = $modsDom->createElement('mods:internetMediaType', htmlspecialchars(trim($dslist['OBJ']['MIMEType'])));
        $physDesc->appendChild($internetMediaType);
        $modsRoot->appendChild($physDesc);
      }
      
      // add in record information type? 
      
      $item->modify_datastream_by_value( $modsDom->saveXML(), 'MODS', "MODS Record", 'text/xml',false, 'Modified by workflow process mods_extend.');
      RETURN TRUE;
    }  
    else {
      $this->setMessage(t('Could not find MODS root element in "%dsid" for mods_extend process on "%pid"', array('%dsid' => $parameters['dsid'], '%pid' => $pid)));
      return FALSE;
    }
  }
}

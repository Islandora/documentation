<?php

// $Id$

function ife_autocomplete_person($string)  {
    $string=strtolower($string);
    
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('php', 'islandora_solr_search', 'Solr/Service');
    $host = variable_get('islandora_solr_search_block_host','localhost');
    $port = variable_get('islandora_solr_search_block_port','8080');
    $appName = variable_get('islandora_solr_search_block_app_name','solr');
    $requestHandler = variable_get("islandora_solr_search_block_request_handler",t("standard"));
    
    $query = 'name_personal_t:'.$string.'* OR name_organization_t:'.$string.'* OR name_conference_t:'.$string.'*';
    $solr = new Apache_Solr_Service($host, $port, '/'.$appName.'/');
    
    $additionalParams = array(
            'facet' => 'false',
            'qt' => $requestHandler,
    );
    
    try {
      $results = $solr->search($query, 0, 10,$additionalParams);
    }catch (Exception $e) {
      drupal_set_message(t('error searching ').$e->getMessage());
    }
    if(empty($results)){
      drupal_set_message(t('Error searching solr index. Is the solr search block configured properly?'),'error');
      return ' ';
    }
    
    header('content-type: text/plain');
    $matches = array();

    foreach($results->response->docs as $doc) {
      
      $item = new fedora_item($doc->id);
      
      $mods = $item->get_datastream_dissemination('MODS');
      if (trim($mods) != '')  {
	$modsDom = new DomDocument();    
	$modsDom->loadXML($mods);
	
	$xpath = new DOMXPath($modsDom);
	$xpath->registerNamespace("php", "http://php.net/xpath");
	$xpath->registerPHPFunctions();
//  	echo $mods;
	$nodeList = $xpath->evaluate('/mods:mods/mods:name');
	foreach ($nodeList as $node) {
	  
	  $type=$node->getAttribute('type');
	  $title=null;
	  $name =null;
	  $given=null;
	  $family=null;
	  $date = null;
	  $nameParts = $node->getElementsByTagName('namePart');
	  foreach ($nameParts as $part) {
	    switch ($part->getAttribute('type')) {
	      case 'given': $given = $part->nodeValue; break;
	      case 'family': $family = $part->nodeValue; break;
	      case 'termsOfAddress': $title= $part->nodeValue; break;
	      case 'date': $date = $part->nodeValue; break;
	      default: $name = $part->nodeValue; break;
	    }
	  }
	  
	  $role =null;
	  $roleTerm = $node->getElementsByTagName('roleTerm');
	  if ($roleTerm->length > 0) {
	    $role=$roleTerm->item(0)->nodeValue;
	  }
	  
	  if (strpos(strtolower($given.$family.$name),$string)!==FALSE) {
	    switch ($type)
	    {
	      case 'personal': $display = $title.' '.$given.' '.$family; break;
	      default: $display = $name; break;
	    }
	    $matches[json_encode(array('type'=>$type,'title'=>$title,'given'=>$given,'family'=>$family,'name'=>$name,'role'=>$role,'date'=>$date))]= $display;
	  }
	}
      }
    }
    drupal_json($matches); exit();
    
}

function ife_autocomplete_marcrelator($collection, $string='') {
  $moduleRoot = drupal_get_path('module', 'islandora_form_elements');

  if ($string == '') {
     $string = $collection;
     $collection = FALSE;
   }

   $string=ucfirst(trim($string));
   
   $dom = DOMDocument::load ( $moduleRoot .'/xml/relators.rdf' );
   $rootEl = $dom->getElementsByTagName('RDF');
   if ($rootEl->length > 0) {
     $rootEl=$rootEl->item(0);
     
     $xpath = new DOMXPath($dom);
     $xpath->registerNamespace ( 'skos' , $rootEl->getAttribute('xmlns:skos') );
     $xpath->registerNamespace ( 'rdf' , $rootEl->getAttribute('xmlns:rdf') );
     
     
     $result = $xpath->query('/rdf:RDF/rdf:Description[starts-with(skos:prefLabel, "'. $string .'")]');

     $terms = array();
     foreach ($result as $term) {
       $termVal =$term->getElementsByTagName('prefLabel')->item(0)->nodeValue;
       $termVal = preg_replace('/\s+/', ' ', $termVal);
       $terms[$term->getElementsByTagName('notation')->item(0)->nodeValue]= $termVal;
     }
     echo drupal_json($terms);
   }
   
   
}

function ife_autocomplete_gacs($collection, $string='') {
  $moduleRoot = drupal_get_path('module', 'islandora_form_elements');

  if ($string == '') {
     $string = $collection;
     $collection = FALSE;
   }

   $string=ucwords($string);
   
   $dom = DOMDocument::load ( $moduleRoot .'/xml/gacs.xml' );
   $rootEl = $dom->getElementsByTagName('codelist');
   if ($rootEl->length > 0) {
     $rootEl=$rootEl->item(0);
     
     $xpath = new DOMXPath($dom);
     $xpath->registerNamespace ( 'a' , $rootEl->getAttribute('xmlns') );
     
     $result = $xpath->query('/a:codelist/a:gacs/a:gac[starts-with(a:name, "'. $string .'")]');

     $gacs = array();
     foreach ($result as $gac) {
       $gacs[$gac->getElementsByTagName('name')->item(0)->nodeValue]= $gac->getElementsByTagName('name')->item(0)->nodeValue;
     }
     echo drupal_json($gacs);
   }
   
   
}

function ife_autocomplete_language($collection, $string='') {
  $moduleRoot=drupal_get_path('module', 'islandora_form_elements');

  if ($string == '') {
     $string = $collection;
     $collection = FALSE;
   }

   $string=ucwords($string);
   
   $dom = DOMDocument::load ( $moduleRoot .'/xml/languages.xml' );
   $rootEl = $dom->getElementsByTagName('codelist');
   if ($rootEl->length > 0) {
     $rootEl=$rootEl->item(0);
     
     $xpath = new DOMXPath($dom);
     $xpath->registerNamespace ( 'a' , $rootEl->getAttribute('xmlns') );
     
     $result = $xpath->query('/a:codelist/a:languages/a:language[starts-with(a:name, "'. $string .'")]');

     $languages = array();
     foreach ($result as $lang) {
       $languages[$lang->getElementsByTagName('name')->item(0)->nodeValue]= $lang->getElementsByTagName('name')->item(0)->nodeValue;
     }
     echo drupal_json($languages);
   }
   
   
}

function ife_autocomplete($field, $collection, $string='') {
  
	if ($string == '') {
		$string=$collection; $collection=FALSE;
	}
  
  module_load_include('php', 'islandora_solr_search', 'Solr/Service');
  $host = variable_get('islandora_solr_search_block_host', 'localhost');
  $port = variable_get('islandora_solr_search_block_port', '8080');
  $appName = variable_get('islandora_solr_search_block_app_name', 'solr');
  $solr = new Apache_Solr_Service($host, $port, '/'. $appName .'/');
  
  $additionalParams = array(
    'fl' => $field
  );

  $query = $field .':'. $solr->escape(strtolower($string)) .'*';
  
  if ($collection != FALSE) {
    $query .= ' AND related_item_identifier_t:'. $solr->escape($collection);
  }
  
  try {
    $results = $solr->search($query, 0, 10, $additionalParams);
  }
  catch (Exception $e) {
    drupal_set_message(t('error searching ') . $e->getMessage());
  }  
  
  $docs = $results->response->docs;
  
  $values = array();
  if ($docs != NULL) {
    foreach ($docs as $doc) {
      $resfield=$doc->getField($field);
      $values[$resfield['value']]=$resfield['value'];
    }
  }

  return drupal_json($values);
}

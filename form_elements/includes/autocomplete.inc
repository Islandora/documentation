<?php

/**
 * @file 
 * 
 */

function ife_autocomplete_person($string) {
  $string = trim(strtolower($string));

  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  module_load_include('php', 'islandora_solr_search', 'Solr/Service');
  $host = variable_get('islandora_solr_search_block_host', 'localhost');
  $port = variable_get('islandora_solr_search_block_port', '8080');
  $app_name = variable_get('islandora_solr_search_block_app_name', 'solr');
  $request_handler = variable_get("islandora_solr_search_block_request_handler", t("standard"));

  $solr = new Apache_Solr_Service($host, $port, '/' . $app_name . '/');

  $string = $solr->escape($string);
  $query = 'name_personal_t:' . $string . '* OR name_organization_t:' . $string . '* OR name_conference_t:' . $string . '*';

  $additional_params = array(
    'facet' => 'false',
    'qt' => $request_handler,
  );

  try {
    $results = $solr->search($query, 0, 10, $additional_params);
  } catch (Exception $e) {
    drupal_set_message(t('error searching') ." " . $e->getMessage());
  }
  if (empty($results)) {
    drupal_set_message(t('Error searching solr index. Is the solr search block configured properly?'), 'error');
    return ' ';
  }

  header('content-type: text/plain');
  $matches = array();

  foreach ($results->response->docs as $doc) {

    $item = new fedora_item($doc->id);

    $mods = $item->get_datastream_dissemination('MODS');
    if (trim($mods) != '') {
      $mods_dom = new DomDocument();
      $mods_dom->loadXML($mods);

      $xpath = new DOMXPath($mods_dom);
      $xpath->registerNamespace("php", "http://php.net/xpath");
      $xpath->registerPHPFunctions();
//  	echo $mods;
      $node_list = $xpath->evaluate('/mods:mods/mods:name');
      foreach ($node_list as $node) {

        $type = $node->getAttribute('type');
        $title = NULL;
        $name = NULL;
        $given = NULL;
        $family = NULL;
        $date = NULL;
        $name_parts = $node->getElementsByTagName('namePart');
        foreach ($name_parts as $part) {
          switch ($part->getAttribute('type')) {
            case 'given': $given = $part->nodeValue;
              break;
            case 'family': $family = $part->nodeValue;
              break;
            case 'termsOfAddress': $title = $part->nodeValue;
              break;
            case 'date': $date = $part->nodeValue;
              break;
            default: $name = $part->nodeValue;
              break;
          }
        }

        if ($name == NULL && isset($given) && isset($family)) {
          $name = (isset($title) ? $title . ' ' : '') . $family . ', ' . $given;
        }

        $role = NULL;
        $role_term = $node->getElementsByTagName('roleTerm');
        if ($role_term->length > 0) {
          $role = $role_term->item(0)->nodeValue;
        }

        if (strpos(strtolower($name), $string) !== FALSE) {
          $display = $name;
          $matches[json_encode(array('type' => trim($type), 'name' => trim($name), 'role' => trim($role), 'date' => trim($date)))] = $display . ' - ' . $role . ' ' . $date;
        }
      }
    }
  }
  drupal_json($matches);
  exit();
}

function ife_autocomplete_marcrelator($collection, $string='') {
  $module_root = drupal_get_path('module', 'islandora_form_elements');

  if ($string == '') {
    $string = $collection;
    $collection = FALSE;
  }

  $string = ucfirst(trim($string));

  $dom = DOMDocument::load($module_root . '/xml/relators.rdf');
  $root_el = $dom->getElementsByTagName('RDF');
  if ($root_el->length > 0) {
    $root_el = $root_el->item(0);

    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('skos', $root_el->getAttribute('xmlns:skos'));
    $xpath->registerNamespace('rdf', $root_el->getAttribute('xmlns:rdf'));


    $result = $xpath->query('/rdf:RDF/rdf:Description[starts-with(skos:prefLabel, "' . $string . '")]');

    $terms = array();
    foreach ($result as $term) {
      $term_val = $term->getElementsByTagName('prefLabel')->item(0)->nodeValue;
      $term_val = preg_replace('/\s+/', ' ', $term_val);
      $terms[$term->getElementsByTagName('notation')->item(0)->nodeValue] = $term_val;
    }
    echo drupal_json($terms);
  }
}

function ife_autocomplete_gacs($collection, $string='') {
  $module_root = drupal_get_path('module', 'islandora_form_elements');

  if ($string == '') {
    $string = $collection;
    $collection = FALSE;
  }

  $string = ucwords(trim($string));

  $dom = DOMDocument::load($module_root . '/xml/gacs.xml');
  $root_el = $dom->getElementsByTagName('codelist');
  if ($root_el->length > 0) {
    $root_el = $root_el->item(0);

    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('a', $root_el->getAttribute('xmlns'));

    $result = $xpath->query('/a:codelist/a:gacs/a:gac[starts-with(a:name, "' . $string . '")]');

    $gacs = array();
    foreach ($result as $gac) {
      $gacs[$gac->getElementsByTagName('name')->item(0)->nodeValue] = $gac->getElementsByTagName('name')->item(0)->nodeValue;
    }
    echo drupal_json($gacs);
  }
}

function ife_autocomplete_language($collection, $string='') {
  $module_root = drupal_get_path('module', 'islandora_form_elements');

  if ($string == '') {
    $string = $collection;
    $collection = FALSE;
  }

  $string = ucwords(trim($string));

  $dom = DOMDocument::load($module_root . '/xml/languages.xml');
  $root_el = $dom->getElementsByTagName('codelist');
  if ($root_el->length > 0) {
    $root_el = $root_el->item(0);

    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('a', $root_el->getAttribute('xmlns'));

    $result = $xpath->query('/a:codelist/a:languages/a:language[starts-with(a:name, "' . $string . '")]');

    $languages = array();
    foreach ($result as $lang) {
      $languages[$lang->getElementsByTagName('name')->item(0)->nodeValue] = $lang->getElementsByTagName('name')->item(0)->nodeValue;
    }
    echo drupal_json($languages);
  }
}

function ife_autocomplete($field, $collection, $string='') {

  if ($string == '') {
    $string = $collection;
    $collection = FALSE;
  }

  module_load_include('php', 'islandora_solr_search', 'Solr/Service');
  $host = variable_get('islandora_solr_search_block_host', 'localhost');
  $port = variable_get('islandora_solr_search_block_port', '8080');
  $app_name = variable_get('islandora_solr_search_block_app_name', 'solr');
  $solr = new Apache_Solr_Service($host, $port, '/' . $app_name . '/');

  $additional_params = array(
    'fl' => $field
  );

  $query = $field . ':' . $solr->escape(strtolower(trim($string))) . '*';

  if ($collection != FALSE) {
    $query .= ' AND related_item_identifier_t:' . $solr->escape($collection);
  }

  try {
    $results = $solr->search($query, 0, 10, $additional_params);
  } catch (Exception $e) {
    drupal_set_message(t('error searching') . " " . $e->getMessage());
  }

  $docs = $results->response->docs;
  $values = array();
  if ($docs != NULL) {
    foreach ($docs as $doc) {
      $resfield = $doc->getField($field);

      if (is_array($resfield['value'])) {
        foreach ($resfield['value'] as $val) {
          if (preg_match('/^' . strtolower($string) . '/i', $val)) {
            $values[$val] = $val;
          }
        }
      }
      else {
        $values[$resfield['value']] = $resfield['value'];
      }
    }
  }

  return drupal_json($values);
}

<?php

/**
 * @file
 * Does rule type stuff,
 */

/**
 * Implements hook_rules_event_info().
 */
function islandora_rules_event_info() {
  return array(
    'islandora_object_ingested' => array(
      'group' => t('Islandora'),
      'label' => t('Object ingested'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('The ingested object'),
          'description' => t('A Tuque object for the ingested Fedora object, as an entity.'),
        ),
      ),
    ),
    'islandora_datastream_ingested' => array(
      'group' => t('Islandora'),
      'label' => t('Datastream ingested'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('The ingested object'),
          'description' => t('A Tuque object for the Fedora object on which the datastream exists, as an entity.'),
        ),
        'datastream' => array(
          'type' => 'islandora_datastream',
          'label' => t('Datastream ID'),
          'description' => t('The ID of the ingested datastream.'),
        ),
      ),
    ),
    'islandora_object_modified' => array(
      'group' => t('Islandora'),
      'label' => t('Object modified'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('The modified object'),
          'description' => t('A Tuque object for the modified Fedora object, as an entity.'),
        ),
      ),
    ),
    'islandora_datastream_modified' => array(
      'group' => t('Islandora'),
      'label' => t('Datastream modified'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('The modified object'),
          'description' => t('A Tuque object for the Fedora object on which the datastream exists, as an entity.'),
        ),
        'datastream' => array(
          'type' => 'islandora_datastream',
          'label' => t('Datastream'),
          'description' => t('The modified datastream.'),
        ),
      ),
    ),
    'islandora_object_purged' => array(
      'group' => t('Islandora'),
      'label' => t('Object purged'),
      'variables' => array(
        'object' => array(
          'type' => 'text',
          'label' => t('Object ID'),
          'description' => t('The ID of the purged object.'),
        ),
      ),
    ),
    'islandora_datastream_purged' => array(
      'group' => t('Islandora'),
      'label' => t('Datastream purged'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Object'),
          'description' => t('A Tuque object for the Fedora object from which the datastream exists, as an entity.'),
        ),
        'datastream' => array(
          'type' => 'text',
          'label' => t('Datastream ID'),
          'description' => t('The identifier of the purged datastream.'),
        ),
      ),
    ),
  );
}

/**
 * Helper function to get reused "parameter" array.
 */
function islandora_rules_relationship_parameter_array() {
  return array(
    'subject' => array(
      'type' => 'islandora_object',
      'label' => t('Subject'),
      'description' => t('An object of which we should check the relationships (The "subject" of the relationship).'),
    ),
    'pred_uri' => array(
      'type' => 'text',
      'label' => t('Predicate URI'),
      'description' => t('The URI namespace to which the predicate belongs.'),
    ),
    'pred' => array(
      'type' => 'text',
      'label' => t('Predicate'),
      'description' => t('The predicate of the relationship.'),
    ),
    'object' => array(
      'type' => 'text',
      'label' => t('Object'),
      'description' => t('The object of the relationship.'),
      'allow null' => TRUE,
      'default value' => NULL,
    ),
    'type' => array(
      'type' => 'integer',
      'label' => t('Object type in the relationship'),
      'description' => t('0=URI, 1=plain literal'),
      'default value' => 0,
    ),
  );
}

function islandora_rules_base_xpath_parameters() {
  return array(
    'datastream' => array(
      'type' => 'islandora_datastream',
      'label' => t('Datastream'),
      'description' => t('The XML datastream to check.'),
    ),
    'xpath' => array(
      'type' => 'text',
      'label' => t('XPath'),
      'description' => t('An XPath to evaluate.'),
    ),
    'xpath_namespaces' => array(
      'type' => 'taxonomy_vocabulary',
      'label' => t('XPath Namespace Taxonomy'),
      'description' => t('A flat taxonomy of which the terms are namespace prefixes and the description contains the URI for the namespace.')
    ),
  );
}


/**
 * Implements hook_rules_condition_info().
 */
function islandora_rules_condition_info() {
  $cond = array();

  $cond['islandora_object_has_relationship'] = array(
    'label' => t('Check object for a relationship'),
    'group' => t('Islandora'),
    'parameter' => islandora_rules_relationship_parameter_array(),
  );
  $cond['islandora_rules_datastream_has_xpath'] = array(
    'label' => t('Check for an XPath match in an XML datastream'),
    'group' => t('Islandora'),
    'parameter' => islandora_rules_base_xpath_parameters(),
  );

  return $cond;
}

/**
 * Implements hook_rules_action_info().
 */
function islandora_rules_action_info() {
  $cond = array();

  $cond['islandora_object_remove_relationship'] = array(
    'label' => t('Remove a relationship from an object'),
    'group' => t('Islandora'),
    'parameter' => islandora_rules_relationship_parameter_array(),
  );

  $cond['islandora_object_add_relationship'] = array(
    'label' => t('Add a relationship to an object'),
    'group' => t('Islandora'),
    'parameter' => islandora_rules_relationship_parameter_array(),
  );

  $cond['islandora_rules_datastream_load_domxpath'] = array(
    'label' => t('Load a DOMXPath for a given XML.'),
    'group' => t('Islandora DOMXPath'),
    'parameter' => array(
      'datastrea,' => array(
        'type' => 'text',
        'label' => t('XML'),
        'description' => t('A string containing the XML to load.'),
      ),
    ),
    'provides' => array(
      'islandora_domxpath' => array(
        'type' => 'islandora_domxpath',
        'label' => t('Loaded DOMXPath instance'),
      ),
    ),
  );
  $cond['islandora_rules_datastream_load_xpath'] = array(
    'label' => t('Load a DOMXPath from a datastream.'),
    'group' => t('Islandora DOMXPath'),
    'parameter' => array(
      'datastream' => array(
        'type' => 'islandora_datastream',
        'label' => t('Datastream'),
        'description' => t('A datastream containing the XML to load.'),
      ),
    ),
    'provides' => array(
      'islandora_domxpath' => array(
        'type' => 'islandora_domxpath',
        'label' => t('Loaded DOMXPath instance'),
      ),
    ),
  );
  $cond['islandora_rules_datastream_load_namespace_vocab'] = array(
    'label' => t('Register namespaces on a DOMXPath instance.'),
    'group' => t('Islandora DOMXPath'),
    'parameter' => array(
      'value' => array(
        'type' => 'islandora_domxpath',
        'label' => t('DOMXPath instance'),
        'description' => t('The DOMXPath instance on which to register the namespaces.'),
      ),
      'xpath_namespaces' => array(
        'type' => 'taxonomy_vocabulary',
        'label' => t('XPath Namespace Taxonomy'),
        'description' => t('A flat taxonomy of which the terms are namespace prefixes and the description contains the URI for the namespace.')
      ),
    ),
  );
  $cond['islandora_rules_datastream_query_xpath'] = array(
    'label' => t('Query nodes from DOMXPath instance.'),
    'group' => t('Islandora DOMXPath'),
    'parameter' => array(
      'xpath' => array(
        'type' => 'islandora_domxpath',
        'label' => t('DOMXPath instance'),
        'description' => t('The DOMXPath instance on which to perform the query.'),
      ),
      'query' => array(
        'type' => 'text',
        'label' => t('XPath query'),
        'description' => t('The XPath query to perform.')
      ),
    ),
    'provides' => array(
      'nodes' => array(
        'type' => 'list<islandora_domnode>',
        'label' => t('Queried DOMNode elements'),
      ),
    ),
  );

  $cond['islandora_rules_datastream_set_xpath'] = array(
    'label' => t('Set value in elements matched by an XPath in an XML datastream'),
    'group' => t('Islandora'),
    'parameter' => islandora_rules_base_xpath_parameters() + array(
      'value' => array(
        'type' => 'text',
        'label' => t('Value'),
        'description' => t('The value to set in the XML on elements matched by the XPath.'),
      ),
    ),
  );

  return $cond;
}

/**
 * Checks that there is a relationship match on the given object.
 *
 * Takes a subject (either a AbstractObject or a FedoraDatastream), as well as
 * the parameters for FedoraRelsExt::get() or FedoraRelsInt::get(), to try to
 * find a match.
 *
 * @see FedoraRelsExt::get()
 */
function islandora_object_has_relationship($sub, $pred_uri, $pred, $object, $type) {
  $relationships = $sub->relationships->get($pred_uri, $pred, $object, $type);
  return !empty($relationships);
}

/**
 * Remove a relationship from the given object.
 *
 * Takes a subject (either a AbstractObject or a FedoraDatastream), as well as
 * the parameters for FedoraRelsExt::remove() or FedoraRelsInt::remove(), to
 * try to find a match.
 *
 * @see FedoraRelsExt::get()
 */
function islandora_object_remove_relationship($sub, $pred_uri, $pred, $object, $type) {
  $sub->relationships->remove($pred_uri, $pred, $object, $type);
}

/**
 * Add a relationship to the given object.
 *
 * Takes a subject (either a AbstractObject or a FedoraDatastream), as well as
 * the parameters for FedoraRelsExt::add() or FedoraRelsInt::add(), and adds
 * the represented relationship.
 *
 * @see FedoraRelsExt::get()
 */
function islandora_object_add_relationship($sub, $pred_uri, $pred, $object, $type) {
  $sub->relationships->add($pred_uri, $pred, $object, $type);
}

function islandora_rules_datastream_load_domxpath($string) {
  $doc = new DOMDocument();
  $doc->loadXML($string);
  $xpath = new DOMXPath($doc);
  return array('islandora_domxpath' => $xpath);
}

function islandora_rules_datastream_load_namespace_vocab($xpath, $xpath_vocab) {
  foreach (taxonomy_get_tree($xpath_vocab->vid, 0, 1, FALSE) as $term) {
    $xpath->registerNamespace($term->name, $term->description);
  }
}

/**
 * Rules XPath helper; grab the datastream content and build a DOMXPath.
 */
function islandora_rules_datastream_load_xpath(AbstractDatastream $datastream, $xpath_vocab) {
  $result = islandora_rules_datastream_load_domxpath($datastream->content, $xpath_vocab);
  islandora_rules_datastream_load_namespace_vocab($result['islandora_domxpath'], $xpath_vocab);
  return $result;
}

function islandora_rules_datastream_has_xpath(AbstractDatastream $datastream, $search_xpath, $xpath_vocab) {
  $xpath = islandora_rules_datastream_load_xpath($datastream, $xpath_vocab);
  $result = $xpath['islandora_domxpath']->query($search_xpath);
  return $result->length > 0;
}

function islandora_rules_datastream_set_xpath(AbstractDatastream $datastream, $search_xpath, $xpath_vocab, $value) {
  $xpath = islandora_rules_datastream_load_xpath($datastream, $xpath_vocab);
  $result = $xpath['islandora_domxpath']->query($search_xpath);
  foreach ($result as $node) {
    $node->nodeValue = $value;
  }
  $datastream->content = $xpath['islandora_domxpath']->document->saveXML();
}

/**
 * Implements hook_rules_data_info().
 */
function islandora_rules_data_info() {
  return array(
    'islandora_domxpath' => array(
      'label' => t('DOMXPath instance'),
      'group' => t('Islandora'),
      'property info' => array(
        'content' => array(
           'type' => 'text',
           'label' => t('XML Content'),
           'computed' => TRUE,
           'getter callback' => 'islandora_rules_get_domxpath_document_content',
           'token type' => 'string',
         ),
      ),
      'wrap' => TRUE,
    ),
    'islandora_domnode' => array(
      'label' => t('DOMNode instance'),
      'group' => t('Islandora'),
      'property info' => array(
        'node_value' => array(
          'type' => 'text',
          'label' => t('Node value'),
          'computed' => TRUE,
          'getter callback' => 'islandora_rules_domnode_get',
          'setter callback' => 'islandora_rules_domnode_set',
          'property' => 'nodeValue',
          'token type' => 'string',
        ),
        'text_content' => array(
          'type' => 'text',
          'label' => t('Text content'),
          'computed' => TRUE,
          'getter callback' => 'islandora_rules_domnode_get',
          'property' => 'textContent',
          'token type' => 'string',
        ),
      ),
      'wrap' => TRUE,
    ),
    'islandora_domelement' => array(
      'label' => t('DOMElement instance'),
      'group' => t('Islandora'),
      'parent' => 'islandora_domnode',
      'wrap' => TRUE,
    ),
  );
}

function islandora_rules_domnode_get($data, $options, $name, $type, $info) {
  return $data->$info['property'];
}

function islandora_rules_domnode_set(&$data, $name, $value, $langcode, $type, $info) {
  $data->$info['property'] = $value;
}

function islandora_rules_get_domxpath_document_content(DOMXPath $xpath) {
  return $xpath->document->saveXML();
}

function islandora_rules_datastream_query_xpath(DOMXPath $xpath, $query) {
  return array('nodes' => iterator_to_array($xpath->query($query)));
}

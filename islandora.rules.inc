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
          'type' => 'text',
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
          'type' => 'text',
          'label' => t('Datastream ID'),
          'description' => t('The ID of the ingested datastream.'),
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
    'object' => array(
      'type' => 'islandora_object',
      'label' => t('Object'),
      'description' => t('The object in which to check the XPath.'),
    ),
    'datastream' => array(
      'type' => 'text',
      'label' => t('Datastream ID'),
      'description' => t('The ID of the XML datastream to check.'),
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

function islandora_rules_datastream_has_xpath(AbstractObject $object, $datastream_id, $search_xpath, $xpath_vocab) {
  if (!isset($object[$datastream_id])) {
    // Datastream does no exists... No match.
    return FALSE;
  }

  $datastream = $object[$datastream_id];
  $doc = new DOMDocument();
  $doc->loadXML($datastream->content);
  $xpath = new DOMXPath($doc);
  foreach (taxonomy_get_tree($xpath_vocab->vid, 0, 1, FALSE) as $term) {
    $xpath->registerNamespace($term->name, $term->description);
  }
  $result = $xpath->query($search_xpath);
  return $result->length > 0;
}

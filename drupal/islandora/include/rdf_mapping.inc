<?php

function islandora_get_default_rdf_mapping($bundle) {
  return array(
    array(
      'type' => 'node',
      'bundle' => $bundle,
      'mapping' => array(
        'rdftype' => array('islandora:collection'),
        'field_dcterms_contributor' => array(
          'predicates' => array('dc:contributor'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_coverage' => array(
          'predicates' => array('dc:coverage'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_creator' => array(
          'predicates' => array('dc:creator'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_date' => array(
          'predicates' => array('dc:date'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_description' => array(
          'predicates' => array('dc:description'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_format' => array(
          'predicates' => array('dc:format'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_identifier' => array(
          'predicates' => array('dc:identifier'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_language' => array(
          'predicates' => array('dc:language'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_publisher' => array(
          'predicates' => array('dc:publisher'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_relation' => array(
          'predicates' => array('dc:relation'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_rights' => array(
          'predicates' => array('dc:rights'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_source' => array(
          'predicates' => array('dc:source'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_subject' => array(
          'predicates' => array('dc:subject'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_title' => array(
          'predicates' => array('dc:title'),
          'datatype' => 'xsd:string',
        ),
        'field_dcterms_type' => array(
          'predicates' => array('dc:type'),
          'datatype' => 'xsd:string',
        ),
      ),
    ),
  );
}

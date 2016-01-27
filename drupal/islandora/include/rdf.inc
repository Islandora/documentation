<?php

/**
 * @file
 * Utility functions for working with RDF.
 */

/**
 * Serializes a Drupal node to N-Triples.
 *
 * @param object $node
 *   Node to serialize.
 *
 * @return string
 *   RDF serialized as N-Triples.
 */
function islandora_serialize_node_to_triples($node) {
  $rdf_model = rdfx_get_rdf_model('node', $node);
  $serializer = ARC2::getSer('NTriples', array('ns' => $rdf_model->ns));
  return $serializer->getSerializedIndex($rdf_model->index);
}

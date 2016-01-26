<?php

/**
 * @file
 * Triplestore indexing functionality
 */

function islandora_triplestore_upsert($node) {
  islandora_triplestore_enqueue_action($node, 'upsert');
}

function islandora_triplestore_delete($node) {
  islandora_triplestore_enqueue_action($node, 'delete');
}

function islandora_triplestore_enqueue_action($node, $action) {
  module_load_include('inc', 'islandora', 'include/rdf');
  $broker_url = variable_get("islandora_stomp_url", "tcp://localhost:61613");
  $queue = variable_get("islandora_triplestore_index_queue", "islandora/triplestore/index");
  $msg  = islandora_serialize_node_to_triples($node);

  // Add enough headers to fool the FcrepoCamel code we're re-using to do this.
  global $base_url;
  $headers = array(
    'action' => $action,
    'Content-Type' => 'application/n-triples',
    'CamelFcrepoBaseUrl' => $base_url,
    'CamelFcrepoIdentifier' => "/node/{$node->nid}"
  );

  try {
    $stomp = new Stomp($broker_url);
    $stomp->send($queue, $msg, $headers);
    unset($stomp);
    return TRUE;
  } catch(StompException $e) {
    dsm($e->getMessage());
    return FALSE;
  }
}

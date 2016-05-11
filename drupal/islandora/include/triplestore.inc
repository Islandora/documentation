<?php

/**
 * @file
 * Triplestore indexing functionality.
 */

/**
 * Enqueues a triplestore upsert action.
 *
 * @param object $node
 *   Node to upsert in index.
 *
 * @return bool
 *   True if publishing the message was successful.
 */
function islandora_triplestore_upsert($node) {
  return islandora_triplestore_enqueue_action($node, 'upsert');
}

/**
 * Enqueues a triplestore delete action.
 *
 * @param object $node
 *   Node to delete from index.
 *
 * @return bool
 *   True if publishing the message was successful.
 */
function islandora_triplestore_delete($node) {
  return islandora_triplestore_enqueue_action($node, 'delete');
}

/**
 * Enqueues a triplestore index action.
 *
 * @param object $node
 *   Node to index.
 * @param string $action
 *   Action to take.  Either 'upsert' or 'delete' at the moment.
 *
 * @return bool
 *   True if publishing the message was successful.
 */
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
    'CamelFcrepoIdentifier' => "/node/{$node->nid}",
  );

  try {
    $stomp = new Stomp($broker_url);
    $stomp->send($queue, $msg, $headers);
    unset($stomp);
    return TRUE;
  }
  catch (StompException $e) {
    watchdog('islandora', 'Error publishing triplestore index action to Stomp broker: @err', array('@err' => $e->getMessage()), WATCHDOG_ERROR);
    return FALSE;
  }
}

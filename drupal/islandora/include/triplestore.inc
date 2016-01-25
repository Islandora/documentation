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
  $broker_url = variable_get("islandora_stomp_url", "tcp://localhost:61613");
  $queue = variable_get("islandora_triplestore_index_queue", "islandora/triplestore/index");
  $msg  = json_encode($node);
  $headers = array('action' => $action);

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

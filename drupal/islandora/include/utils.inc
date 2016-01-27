<?php

/**
 * @file
 * Miscellaneous utility functions for the islandora module.
 */

/**
 * Predicate to determine if we should ignore hooks that talk to Fedora.
 *
 * @return bool
 *   True if we shouldn't talk to Fedora (breaks the cycle).
 */
function islandora_ignore_hooks() {
  $header_is_set = isset($_SERVER['HTTP_IGNORE_HOOKS']);

  if (!$header_is_set) {
    return FALSE;
  }

  $header_is_true = strcmp(strtolower($_SERVER['HTTP_IGNORE_HOOKS']), "true") == 0;

  return $header_is_true;
}

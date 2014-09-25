<?php

/**
 * @file
 * Mimetype specific utility functions.
 */

/**
 * Retrieve the correct file extension for a give mimetype.
 *
 * @param string $mimetype
 *   The mimetype whose extension is required.
 *
 * @return string
 *   The extension mapped to the given mimetype. Defaults to 'bin'.
 */
function islandora_get_extension_for_mimetype($mimetype) {
  // file.mimetypes.inc is a part of Drupal core, however is not
  // automatically loaded. Manually require it.
  require_once DRUPAL_ROOT . "/includes/file.mimetypes.inc";
  $extension = 'bin';
  $mimetype_mapping = file_mimetype_mapping();
  $extension_index = array_search($mimetype, $mimetype_mapping['mimetypes']);
  if ($extension_index !== FALSE) {
    $mime_array_flipped = array_reverse($mimetype_mapping['extensions']);
    $extension = array_search($extension_index, $mime_array_flipped);
  }

  // We can only have one mapping in drupal for 'xml'.
  if ($mimetype == "text/xml") {
    return "xml";
  }
  return $extension;
}

/**
 * Retrieve all file extensions for a give mimetype.
 *
 * @param string $mimetype
 *   The mimetype whose extensions are required.
 *
 * @return array
 *   All known legal extensions.
 */
function islandora_get_extensions_for_mimetype($mimetype) {
  // file.mimetypes.inc is a part of Drupal core, however is not
  // automatically loaded. Manually require it.
  require_once DRUPAL_ROOT . "/includes/file.mimetypes.inc";
  $mimetype_mapping = file_mimetype_mapping();
  $index = array_search($mimetype, $mimetype_mapping['mimetypes']);
  $extensions = array();
  if ($index !== FALSE) {
    $extensions = array_keys($mimetype_mapping['extensions'], $index);
  }
  // We can only have one mapping in drupal for 'xml'.
  if ($mimetype == "text/xml") {
    $extensions[] = 'xml';
  }
  return $extensions;
}

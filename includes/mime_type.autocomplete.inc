<?php
/**
 * @file
 * Autocomplete functionality for MIME types in Islandora.
 */

/**
 * Autocomplete the MIME type name.
 *
 * @param string $string
 *   A search string.
 *
 * @return string
 *   The rendered JSON results.
 */
function islandora_mime_type_autocomplete($string) {
  module_load_include('inc', 'islandora', 'includes/mime_detect');
  $mime_detect = new MimeDetect();
  $mime_types = $mime_detect->getMimeTypes();
  $output = array();
  foreach ($mime_types as $mime_type) {
    if (preg_match("/{$string}/i", $mime_type) !== 0) {
      $output[$mime_type] = $mime_type;
    }
  }
  return drupal_json_output($output);
}

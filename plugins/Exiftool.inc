<?php
// $Id$

/*
 *
 *
 * This Class implements the methods defined in the STANDARD_IMAGE content model
 */

class Exiftool {
  function Exiftool() {
  }

  function extractMetadata($parameterArray, $dsid, $file, $file_ext) {
    $system = getenv('System');
    $file_suffix = '_'. $dsid . '.xml';
    $returnValue=TRUE;
    $output=array();
    exec('exiftool -X ' . escapeshellarg($file) . '', $output);
    file_put_contents($file.$file_suffix, implode("\n", $output));
    $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
    return TRUE;
  }
}

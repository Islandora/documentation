<?php

/**
 * @file
 * Fits
 * fits.sh must be in the apache user's path
 */

/**
 * This Class implements the methods defined in the STANDARD_IMAGE content model
 */
class fits {

  private $pid = NULL;
  private $item = NULL;

  /**
   * Constructor
   * @param type $pid
   */
  function __construct($pid) {
    //drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $this->pid = $pid;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->item = new Fedora_Item($this->pid);
  }

  /**
   * extract metadata
   * @param type $parameterArray
   * @param type $dsid
   * @param type $file
   * @param type $file_ext
   * @return type
   */
  function extractMetadata($parameterArray, $dsid, $file, $file_ext) {
    $system = getenv('System');
    $file_suffix = '_' . $dsid . '.xml';
    $returnValue = TRUE;
    $output = array();
    exec('fits.sh -h' . escapeshellarg($file) . '', $output);
    file_put_contents($file . $file_suffix, implode("\n", $output));
    $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
    return TRUE;
  }

  /**
   * display metadata
   * @return type
   */
  function displayMetadata() {
    $output = '';
    $fits = $this->item->get_datastream_dissemination('FITS');
    if (trim($fits) != '') {
      $fitsDom = DOMDocument::loadXML($this->item->get_datastream_dissemination('FITS'));
      if ($fitsDom != NULL) {
        $description = $fitsDom->getElementsByTagNameNS('http://hul.harvard.edu/ois/xml/ns/fits/fits_output', 'fits');
        if ($description->length > 0) {
          $description = $description->item(0);
          $output .= '<div class="fedora_technical_metadata"><ul>';
          for ($i = 0; $i < $description->childNodes->length; $i++) {
            $name = $description->childNodes->item($i)->nodeName;
            $value = $description->childNodes->item($i)->nodeValue;
            if ($name != '#text' && !preg_match('/^System\:.*$/', $name) && trim($value) != '') {
              list($type, $name) = preg_split('/\:/', $name);
              $name = trim(preg_replace('/(?<!^)([A-Z][a-z]|(?<=[a-z])[A-Z])/', " $1", $name));
              $output .= '<li><b>' . $name . '</b>:  ' . $value . ' </li>';
            }
          }
          $output .= '</ul></div>';

          $fieldset = array(
            '#title' => t("!text", array('!text' => 'FITS Technical Metadata')),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#value' => $output
          );
          $output = theme('fieldset', $fieldset);
        }
      }
    }
    return $output;
  }

}

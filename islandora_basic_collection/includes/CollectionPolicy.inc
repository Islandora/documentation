<?php

/**
 * @file
 * This file contains the classes for parsing the collection policy infomration.
 */

/**
 * Collection Policy class
 */
class CollectionPolicy {

  /**
   * Constructor
   * NOTE: Use the static constructor methods whenever possible.
   *
   * @param string $xmlStr
   *   The COLLECTION_POLICY in string form
   *
   * @return CollectionPolicy
   *   The parsed collection policy.
   */
  public function __construct($xml_string) {
    $this->xml = new DOMDocument();
    $this->xml->loadXML($xml_string);
    $this->name = 'Collection Policy';
  }

  /**
   * Gets the name of the relationship to use
   * for members of this collection.
   * Returns FALSE on failure.
   *
   * @return string $relationship
   */
  public function getRelationship() {
    $ret = trim($this->xml->getElementsByTagName('relationship')->item(0)->nodeValue);
    return $ret;
  }

  /**
   * Sets the name of the relationship to use
   * for members of this collection.
   * Returns FALSE on failure.
   *
   * @param string $relationship
   * @return boolean $ret
   */
  public function setRelationship($relationship) {
    $ret = FALSE;
    if ($this->validate()) {
      $relationshipEl = $this->xml->getElementsByTagName('relationship')->item(0);
      $relationshipEl->nodeValue = trim($relationship);
      $ret = TRUE;
    }
    return $ret;
  }

  /**
   * Gets a list of ContentModel objects supported by this collection.
   *
   * @return ContentModel[] $models
   */
  function getContentModels() {
    $ret = array();
    $content_models = $this->xml->getElementsByTagName('content_models')->item(0)->getElementsByTagName('content_model');
    for ($i = 0; $i < $content_models->length; $i++) {
      $cm = array();
      $cm['pid'] = $content_models->item($i)->getAttribute('pid');
      $cm['namespace'] = $content_models->item($i)->getAttribute('namespace');
      $cm['name'] = $content_models->item($i)->getAttribute('name');
      if ($cm !== FALSE) {
        $ret[] = $cm;
      }
    }
    return $ret;
  }

  /**
   * Removes the specified content model from the collection policy.  This will only
   * prevent future ingests of the removed model to the collection.  $cm should be
   * a valid ContentModel object.  Returns FALSE on failure or when the CM was not found in
   * the collection policy.
   *
   * @param ContentModel $cm
   * @return boolean $valid
   */
  function removeModel($cm) {
    $ret = FALSE;
    if ($this->validate() && $cm->validate()) {
      $contentmodelsEl = $this->xml->getElementsByTagName('content_models');
      $models = $contentmodelsEl->item(0)->getElementsByTagName('content_model');
      $found = FALSE;
      for ($i = 0; $found === FALSE && $i < $models->length; $i++) {
        if ($models->item($i)->getAttribute('pid') == $cm->pid) {
          $found = $models->item($i);
        }
      }

      if ($found !== FALSE && $models->length > 1) {
        $contentmodelsEl->item(0)->removeChild($found);
        $ret = TRUE;
      }
    }
    return $ret;
  }

  /**
   * addModel ??
   * @param ContentModel $cm
   * @param type $namespace
   * @return type
   */
  function addModel($cm, $namespace) {
    $ret = FALSE;
//    var_dump(self::valid_pid($namespace));
//    var_dump($this->validate());
//    var_dump($cm->validate());
//    if (self::valid_pid($namespace) && $this->validate() && $cm->validate()) {
      $contentmodelsEl = $this->xml->getElementsByTagName('content_models');
      $models = $contentmodelsEl->item(0)->getElementsByTagName('content_model');
      $found = FALSE;
//      for ($i = 0; !$found && $i < $models->length; $i++) {
//        if ($models->item($i)->getAttribute('pid') == $cm->pid)
//          $found = TRUE;
//      }

//      if (!$found) {
        $cmEl = $this->xml->createElement('content_model');
        $cmEl->setAttribute('name', $cm->name);
        $cmEl->setAttribute('dsid', $cm->dsid);
        $cmEl->setAttribute('namespace', $namespace);
        $cmEl->setAttribute('pid', $cm->pid);
        $contentmodelsEl->item(0)->appendChild($cmEl);
//      }

      $ret = !$found;
//    }
    return $ret;
  }

  /**
   * getName ??
   * @return type
   */
  function getName() {
    $ret = FALSE;
    if ($this->validate()) {
      $ret = $this->xml->getElementsByTagName('collection_policy')->item(0)->getAttribute('name');
    }
    return $ret;
  }

}

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
  public function __construct($xmlStr) {
    $this->xml = new DOMDocument();
    $this->xml->loadXML($xmlStr);
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

 /**
   * valid_pid
   * Validates a fedora PID based on the regexp provided in the fedora 
   * 3.3 documentation.  
   * http://www.fedora-commons.org/confluence/display/FCR30/Fedora+Identifiers
   *
   * @param String $pid
   * @return boolean $valid
   */
  public static function valid_pid($pid) {
    $valid = FALSE;
    if (strlen(trim($pid)) <= 64 && preg_match('/^([A-Za-z0-9]|-|\.)+:(([A-Za-z0-9])|-|\.|~|_|(%[0-9A-F]{2}))+$/', trim($pid))) {
      $valid = TRUE;
    }

    return $valid;
  }
  
    /**
   * Validates the XMLDatastream against the schema location 
   * defined by the xmlns:schemaLocation attribute of the root 
   * element. If the xmlns:schemaLocation attribute does not exist,
   * then it is assumed to be the old schema and it attempts to convert 
   * using the convertFromOldSchema method.
   *
   * TODO:  Maybe change it so that it always validates against a known 
   * schema.  This makes more sense because this class assumes the structure
   * to be known after it has been validated.
   *
   * @return boolean $valid
   */
  public function validate() {
    global $user;
    if ($this->valid === NULL) {
      $ret = TRUE;
      if ($this->xml == NULL) {
        $this->fetchXml();
      }
      // figure out if we're dealing with a new or old schema
      $rootEl = $this->xml->firstChild;
      if (!$rootEl->hasAttributes() || $rootEl->attributes->getNamedItem('schemaLocation') === NULL) {
        //$tmpname = substr($this->pid, strpos($this->pid, ':') + 1);
        $tmpname = user_password(10);
        $this->convertFromOldSchema();
        drupal_add_js("fedora_repository_print_new_schema_$tmpname = function(tagID) {
        var target = document.getElementById(tagID);
        var content = target.innerHTML;
        var text = '<html><head><title>Title'   +
        '</title><body>' + content +'</body></html>';
        printerWindow = window.open('', '', 'toolbar=no,location=no,' + 'status=no,menu=no,scrollbars=yes,width=650,height=400');
        printerWindow.document.open();
        printerWindow.document.write(text);
        }", 'inline');

        if (user_access('administer site configuration')) {
          drupal_set_message('<span id="new_schema_' . $tmpname . '" style="display: none;">' . htmlentities($this->xml->saveXML()) . '</span>Warning: XMLDatastream performed conversion of \'' . $this->getIdentifier() . '\' from old schema.  Please update the datastream. The new datastream contents are <a href="javascript:fedora_repository_print_new_schema_' . $tmpname . '(\'new_schema_' . $tmpname . '\')">here.</a> ');
        }

        $rootEl = $this->xml->firstChild;
      }

      $schemaLocation = NULL;
      if ($this->forceSchema) {
        // hack because you cant easily get the static property value from
        // a subclass.
        $vars = get_class_vars(get_class($this));
        $schemaLocation = $vars['SCHEMA_URI'];
      }
      elseif ($rootEl->attributes->getNamedItem('schemaLocation') !== NULL) {
        //figure out where the schema is located and validate.
        list(, $schemaLocation) = preg_split('/\s+/', $rootEl->attributes->getNamedItem('schemaLocation')->nodeValue);
      }
      $schemaLocation = NULL;
      return TRUE;
      if ($schemaLocation !== NULL) {
        if (!$this->xml->schemaValidate($schemaLocation)) {
          $ret = FALSE;
          $errors = libxml_get_errors();
          foreach ($errors as $err) {
            self::$errors[] = 'XML Error: Line ' . $err->line . ': ' . $err->message;
          }
        }
        else {
          $this->name = $rootEl->attributes->getNamedItem('name')->nodeValue;
        }
      }
      else {
        $ret = FALSE;
        self::$errors[] = 'Unable to load schema.';
      }

      $this->valid = $ret;
    }

    return $this->valid;
  }

}

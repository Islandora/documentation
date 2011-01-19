<?php

// $Id$

/*
 * Created on 18-Feb-08
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
// module_load_include('nc', 'CollectionClass', '');
// This CLASS caches the streams so once you call a getstream once it will always return the same stream as long as you are using the
// instance of this class.  Cached to prevent mutiple hits to fedora.  maybe a bit confusing though if this class is used in a different context.
class CollectionClass {

  public static $COLLECTION_CLASS_COLLECTION_POLICY_STREAM = 'COLLECTION_POLICY';
  public static $COLLECTION_CLASS_COLLECTION_VIEW_STREAM = 'COLLECTION_VIEW';
  private $contentModelStream = NULL;
  private $collectionPolicyStream = NULL;
  private $collectionViewStream = NULL;
  public $collectionObject = NULL;

  /**
   * Creates a collection object. Optionally can associate it with a single collection with parameter $pid.
   *
   * @param string $pid The pid of the collection to represent.
   * @return CollectionClass
   */
  function __construct($pid = NULL) {
    if (!empty($pid)) {
      module_load_include('inc', 'fedora_repository', 'ObjectHelper');
      $this->collectionObject = new ObjectHelper($pid);
      $this->pid = $pid;
    }
    
  }

  /* gets objects related to this object.  must include offset and limit
   * calls getRelatedItems but enforces limit and offset
   */

  function getRelatedObjects($pid, $limit, $offset, $itqlquery=NULL) {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $objectHelper = new ObjectHelper();
    if (!isset($itqlquery)) {
      // $query_string = $objectHelper->getStream($pid, 'QUERY', 0);
      $itqlquery = $objectHelper->getStream($pid, 'QUERY', 0);
    }
    return $this->getRelatedItems($pid, $itqlquery, $limit, $offset);
  }

  /**
   * Gets objects related to this item.  It will query the object for a Query stream and use that as a itql query
   * or if there is no query stream it will use the default.  If you pass a query to this method it will use the passed in query no matter what
   */
  function getRelatedItems($pid, $itqlquery = NULL, $limit = NULL, $offset = NULL) {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
    if (!isset($limit)) {
      $limit = 1000;
    }
    if (!isset($offset)) {
      $offset = 0;
    }
    global $user;
    if (!fedora_repository_access(OBJECTHELPER :: $OBJECT_HELPER_VIEW_FEDORA, $pid, $user)) {
      drupal_set_message(t("You do not have access to Fedora objects within the attempted namespace or access to Fedora denied."), 'error');
      return ' ';
    }
    $objectHelper = new ObjectHelper();
    $query_string = $itqlquery;
    if (!isset($query_string)) {
      $query_string = $objectHelper->getStream($pid, 'QUERY', 0);
      if ($query_string == NULL) {
        $query_string = 'select $object $title $content from <#ri>
                             where ($object <dc:title> $title
                             and $object <fedora-model:hasModel> $content
                             and ($object <fedora-rels-ext:isMemberOfCollection> <info:fedora/' . $pid . '>
                             or $object <fedora-rels-ext:isMemberOf> <info:fedora/' . $pid . '>)
                             and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>)
                             minus $content <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
                             order by $title';
      }
    }

    $query_string = htmlentities(urlencode($query_string));

    $content = '';
    $url = variable_get('fedora_repository_url', 'http://localhost:8080/fedora/risearch');
    $url .= "?type=tuples&flush=TRUE&format=Sparql&limit=$limit&offset=$offset&lang=itql&stream=on&query=" . $query_string;
    $content .= do_curl($url);

    return $content;
  }

  function getCollectionPolicyStream($collection_pid) {
    if ($this->collectionPolicyStream != NULL) {
      return $this->collectionPolicyStream;
    }
    $this->collectionPolicyStream = $this->getStream($collection_pid, CollectionClass :: $COLLECTION_CLASS_COLLECTION_POLICY_STREAM, 0);
    return $this->collectionPolicyStream;
  }

  function getRelationshipElement($collection_pid) {
    $stream = $this->getCollectionPolicyStream($collection_pid);
    try {
      $xml = new SimpleXMLElement($stream);
    } catch (Exception $e) {
      drupal_set_message(t('Error getting relationship element from policy stream !e', array('!e' => $e->getMessage())), 'error');
      return;
    }
    $relationship = $xml->relationship[0];
    return $relationship;
  }

  function getCollectionViewStream($collection_pid) {
    $this->collectionViewStream = $this->getStream($collection_pid, CollectionClass :: $COLLECTION_CLASS_COLLECTION_VIEW_STREAM, 0);
    return $this->collectionViewStream;
  }

  function getStream($pid, $dsid, $showError = 1) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new fedora_item($pid);
    return isset($item->datastreams[$dsid]) ? $item->get_datastream_dissemination($dsid) : NULL;
  }

  function getPidNameSpace($pid, $dsid) {
    $stream = $this->getCollectionPolicyStream($pid);
    try {
      $xml = new SimpleXMLElement($stream);
    } catch (Exception $e) {
      drupal_set_message(t('Error getting PID namespace !e', array('!e' => $e->getMessage())), 'error');
      return;
    }
    foreach ($xml->contentmodels->contentmodel as $contentModel) {
      // $contentModelName=strip_tags($contentModel['name']);
      $contentdsid = strip_tags($contentModel->dsid);
      if (strtolower($contentdsid) == strtolower($dsid)) {
        return strip_tags($contentModel->pid_namespace->asXML());
      }
    }
    drupal_set_message(t('Error getting PID namespace! using collection pid of !pid and content model of !dsid', array('!pid' => $pid, '!dsid' => $dsid)), 'error');
    return NULL;
  }

  /**
   * gets a list of content models from a collection policy
   */
  function getContentModels($collection_pid, $showError = TRUE) {
    module_load_include('inc', 'Fedora_Repository', 'ContentModel');
    $collection_stream = $this->getCollectionPolicyStream($collection_pid);
    try {
      $xml = new SimpleXMLElement($collection_stream);
    } catch (Exception $e) {
      if ($showError) {
        drupal_set_message(t("!e", array('!e' => $e->getMessage())), 'error');
      }
      return NULL;
    }
    foreach ($xml->contentmodels->contentmodel as $content_model) {
      $contentModel = new ContentModel();
      $contentModel->contentModelDsid = $content_model->dsid;
      $contentModel->contentModelPid = $content_model->pid;
      $contentModel->pidNamespace = $content_model->pidNamespace;
      $contentModel->contentModelName = $content_model['name'];
      $models[] = $contentModel;
    }
    return $models;
  }

  /**
   * using the collection policies pid namespace get a new pid by calling fedora' get next pid and appending it to the namespace
   * $pid is the $pid of the content model
   * $dsid is the datastream id of the content model.
   */
  function getNextPid($pid, $dsid) {
    module_load_include('inc', 'Fedora_Repository', 'ConnectionHelper');
    $pidNameSpace = $this->getPidNameSpace($pid, $dsid);
    $pname = substr($pidNameSpace, 0, strpos($pidNameSpace, ":"));
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    return Fedora_Item::get_next_pid_in_namespace($pname);
  }

  /**
   * gets the form handler file, class and method and returns them in an array
   *
   * @param string $pid The content model PID
   * @param string $dsid The content model DSID
   * @return array The file, class and method name to handle the ingest form.
   */
  function getFormHandler($pid, $dsid) {
    $stream = $this->getContentModelStream($pid, $dsid);
    try {
      $xml = new SimpleXMLElement($stream);
    } catch (Exception $e) {
      drupal_set_message(t('Error Getting FormHandler: !e', array('!e' => $e->getMessage())), 'error');
      return NULL;
    }
    $formHandler = $xml->ingest_form;
    if ($formHandler != NULL) {
      $handlerDsid = strip_tags($formHandler['dsid']);
      $handlerMethod = strip_tags($formHandler->form_builder_method->form_handler->asXML());
      $handlerFile = strip_tags($formHandler->form_builder_method->file->asXML());
      $handlerClass = strip_tags($formHandler->form_builder_method->class_name->asXML());
      $handlerModule = strip_tags($formHandler->form_builder_method->module->asXML());
      $returnArray = array();
      $returnArray['module'] = $handlerModule;
      $returnArray['method'] = $handlerMethod;
      $returnArray['class'] = $handlerClass;
      $returnArray['file'] = $handlerFile;
      return $returnArray;
    }

    drupal_set_message(t("Error getting form handler. No handler found for !pid - !dsid", array('!pid' => $pid, '!dsid' => $dsid)), 'error');
    return NULL;
  }

  function getAllowedMimeTypes($contentModelPid, $contentModel_dsid) {
    $stream = $this->getContentModelStream($contentModelPid, $contentModel_dsid);
    try {
      $xml = new SimpleXMLElement($stream);
    } catch (Exception $e) {
      drupal_set_message(t('Error getting content model stream for mime types !e', array('!e' => $e->getMessage())), 'error');
      return;
    }
    foreach ($xml->mimetypes->type as $type) {
      $types[] = $type;
    }
    return $types;
  }

  /**
   * Grabs the rules from the content model stream
   * file the file that has been uploaded
   */
  function getAndDoRules($file, $mimetype, $pid, $dsid) {
    if (!user_access('ingest new fedora objects')) {
      drupal_set_message(t('You do not have permission to ingest objects.'));
      return FALSE;
    }

    $stream = $this->getContentModelStream($pid, $dsid);

    try {
      $xml = new SimpleXMLElement($stream);
    } catch (Exception $e) {
      drupal_set_message(t('Error getting content model stream !e', array('!e' => $e->getMessage())), 'error');
      return FALSE;
    }
    foreach ($xml->ingest_rules->rule as $rule) {
      foreach ($rule->applies_to as $type) {
        if (!strcmp(trim($type), trim($mimetype))) {
          $methods = $rule->methods->method;
          return $this->callMethods($file, $methods);
        }
      }
    }
  }

  /**
   * calls the methods defined in the content model rules .xml file stored in a Fedora object
   */
  function callMethods($file, $methods) {
    foreach ($methods as $method) {
      $phpFile = strip_tags($method->file->asXML());
      $phpClass = strip_tags($method->class_name->asXML());
      $phpMethod = strip_tags($method->method_name->asXML());
      $file_ext = strip_tags($method->modified_files_ext->asXML());
      $parameters = $method->parameters->parameter;
      $dsid = strip_tags($method->datastream_id);
      $parametersArray = array();
      if (isset($parameters)) {
        foreach ($parameters as $parameter) {
          $name = strip_tags($parameter['name']);
          $value = strip_tags($parameter->asXML());
          $parametersArray["$name"] = $value;
        }
      }
      // module_load_include( $phpFile, 'Fedora_Repository', ' ');
      require_once(drupal_get_path('module', 'fedora_repository') . '/' . $phpFile);
      $thisClass = new $phpClass ();
      $returnValue = $thisClass->$phpMethod($parametersArray, $dsid, $file, $file_ext);
      if (!$returnValue) {
        drupal_set_message('Error! Failed running content model method !m !rv', array('!m' => $phpMethod, '!rv' => $returnValue));
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * grabs a xml form definition from a content model and builds
   * the form using drupals forms api
   */
  function build_ingest_form(&$form, &$form_state, $contentModelPid, $contentModelDsid) {
    $stream = $this->getContentModelStream($contentModelPid, $contentModelDsid);
    try {
      $xml = new SimpleXMLElement($stream);
    } catch (Exception $e) {
      drupal_set_message(t('Error getting ingest form stream !e', array('!e' => $e->getMessage())), 'error');
      return FALSE;
    }
    $docRoot = $_SERVER['DOCUMENT_ROOT'];

    $file = (isset($form_state['values']['ingest-file-location']) ? $form_state['values']['ingest-file-location'] : '');
    // $fullPath = $docRoot . '' . $file;
    $fullpath = $file;
    //	$form = array();
    $form['step'] = array(
        '#type' => 'hidden',
        '#value' => (isset($form_state['values']['step']) ? $form_state['values']['step'] : 0) + 1,
    );
    $form['ingest-file-location'] = array(
        '#type' => 'hidden',
        '#value' => $file,
    );

    $form['content_model_name'] = array(
        '#type' => 'hidden',
        '#value' => $contentModelDsid
    );
    $form['models'] = array(//content models available
        '#type' => 'hidden',
        '#value' => $form_state['values']['models'],
    );
    $form['fullpath'] = array(
        '#type' => 'hidden',
        '#value' => $fullpath,
    );

    $form['#attributes']['enctype'] = 'multipart/form-data';
    $form['indicator']['ingest-file-location'] = array(
        '#type' => 'file',
        '#title' => t('Upload Document'),
        '#size' => 48,
        '#description' => t('Full text'),
    );
    if ($xml->ingest_form->hide_file_chooser == 'TRUE') {
      $form['indicator']['ingest-file-location']['#type'] = 'hidden';
    }
    $ingest_form = $xml->ingest_form; //should only be one
    $drupal_module = strip_tags($ingest_form->form_builder_method->module->asXML());
    if (empty($drupal_module)) {
      $drupal_module = 'fedora_repository';
    }
    $phpFile = strip_tags($ingest_form->form_builder_method->file->asXML());
    $phpClass = strip_tags($ingest_form->form_builder_method->class_name->asXML());
    $phpMethod = strip_tags($ingest_form->form_builder_method->method_name->asXML());
    $dsid = strip_tags($ingest_form['dsid']);
    // module_load_include('php', 'Fedora_Repository', $phpFile);
    require_once(drupal_get_path('module', $drupal_module) . '/' . $phpFile);
    $thisClass = new $phpClass ();

    return $thisClass->$phpMethod($form, $ingest_form, $form_state['values'], $form_state);
  }

  //this will also create a personal collection for an existing user if they don't have one
  //not using this function currently
  function createUserCollection(& $user) {
    if (isset($user->fedora_personal_pid)) {
      return;
    }
    $username = array(
        'name' => variable_get('fedora_admin_user', 'fedoraAdmin')
    );
    $admin_user = user_load($username);
    module_load_include('inc', 'fedora_repository', 'ConnectionHelper');
    $connectionHelper = new ConnectionHelper();
    try {
      $soapClient = $connectionHelper->getSoapClient(variable_get('fedora_soap_manage_url', 'http://localhost:8080/fedora/services/management?wsdl'));
      $pidNameSpace = variable_get('fedora_repository_pid', 'vre:');
      $pidNameSpace = substr($pidNameSpace, 0, strpos($pidNameSpace, ":"));
      $params = array(
          'numPIDs' => '',
          'pidNamespace' => $pidNameSpace
      );
      $object = $soapClient->__soapCall('getNextPID', array(
                  $params
              ));
    } catch (exception $e) {
      drupal_set_message(t('Error getting Next PID: !e', array('!e' => $e->getMessage())), 'error');
      return FALSE;
    }
    $pid = implode(get_object_vars($object));
    $pidNumber = strstr($pid, ":");
    $pid = $pidNameSpace . ':' . 'USER-' . $user->name . '-' . substr($pidNumber, 1);
    $personal_collection_pid = array(
        'fedora_personal_pid' => $pid
    );
    module_load_include('inc', 'fedora_repository', 'plugins/PersonalCollectionClass');
    $personalCollectionClass = new PersonalCollectionClass();
    if (!$personalCollectionClass->createCollection($user, $pid, $soapClient)) {
      drupal_set_message("Did not create a personal collection object for !u", array('!u' => $user->name));
      return FALSE; //creation failed don't save the collection pid in drupal db
    }
    user_save($user, $personal_collection_pid);
    return TRUE;
  }
  
  function showFieldSets($page_number) {
    
    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );
    
    global $user;
    $objectHelper = new ObjectHelper();
    $results = $this->getRelatedItems($this->pid, NULL);
    
    
    
    $html = $objectHelper->renderCollection($results, $this->pid, NULL, NULL, $page_number );


    
    $tabset['my_tabset']['second_tab'] = array(
//     $collection_fieldset = array (
      '#type' => 'tabpage',
      '#title' => t('Members'),
      '#content' => $html
    );
    $tabset['my_tabset']['first_tab'] = array(
      // #type and #title are the minimum requirements.
      '#type' => 'tabpage',
      '#title' => t('View'),
      // This will be the content of the tab.
      '#content' => 'what what',
    );
   $tabset['my_tabset']['third_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
    );
    $tabset['my_tabset']['third_tab']['tabset'] = array(
      '#type' => 'tabset',
    );
    
    $tabset['my_tabset']['third_tab']['tabset']['view'] = array(
      '#type' => 'tabpage',
      '#title' => t('Darwin Core'),
      '#content' => "METADATA",
    );
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
/*    $obj = new ObjectHelper();
    if (fedora_repository_access(OBJECTHELPER :: $EDIT_FEDORA_METADATA, $this->pid, $user)) {
      $editform = drupal_get_form('fedora_repository_edit_qdc_form', $this->pid, 'DARWIN_CORE');
      $tabset['my_tabset']['third_tab']['tabset']['edit'] = array(
        '#type' => 'tabpage',
        '#title' => t('Edit'),
        '#content' => $editform,
      );
      $tabset['my_tabset']['third_tab']['tabset']['source'] = array(
        '#type' => 'tabpage',
        '#title' => t('Vew Source'),
      );
      $xmlsrc = $dwc->asXML();
      $tabset['my_tabset']['third_tab']['tabset']['source']['srctext'] = array(
        '#name' => 'source',
        '#type' => 'textarea',
        '#value' => $xmlsrc,
        '#id' => 'source',
        '#height' => 50,
      );
    }
*/
    return tabs_render($tabset);
  }

}


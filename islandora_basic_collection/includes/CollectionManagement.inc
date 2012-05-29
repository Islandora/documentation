<?php

/**
 * @file 
 * CollectionManagement.inc
 */

/**
 * collection creation form 
 * @param array $form_state
 * @param string $parent_collection_pid
 * @param string $content_models
 * @return array
 */
function collection_management_form($this_collection_pid, $content_models) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
  module_load_include('inc', 'fedora_repository', 'CollectionPolicy');
  $restricted = FALSE;
  if (variable_get('fedora_namespace_restriction_enforced', TRUE)) {
    $restricted = TRUE;
    $allowed_string = variable_get('fedora_pids_allowed', 'default: demo: changeme: islandora:');
    $namespaces = explode(':', $allowed_string);
    foreach ($namespaces as $namespace) {
      if ($namespace) {
        $allowed[trim($namespace)] = trim($namespace);
      }
    }
  }
  $collection_policy = CollectionPolicy::loadFromCollection($this_collection_pid);

  if ($collection_policy) {
    $supported_collection_models = $collection_policy->getContentModels();
  }
  $collection_namespace = substr($this_collection_pid, 0, strpos($this_collection_pid, ":"));

  $represented_content_models = get_represented_content_models($this_collection_pid);
  $item = new Fedora_Item($this_collection_pid);
  $collection_name = $item->objectProfile->objLabel;
  $new_content_models = get_content_models_as_option_array();
  $current_models_in_policy = array();

  if ($supported_collection_models) {
    foreach ($supported_collection_models as $supported_model) {
      $current_models_in_policy[$supported_model->pid] = $supported_model->pid;
    }
  }
  $cm_options = array();
  $name_mappings = array();
  foreach ($content_models as $content_model) {
    if ($content_model->pid != "islandora:collectionCModel") {
      $item = new fedora_item($content_model->pid);
      $cm_name = $item->objectProfile->objLabel;
      $cm_options[$content_model->pid] = $cm_name;
    }
  }

  if (!empty($current_models_in_policy)) {
    $show_delete = TRUE;
  }


  $content_models = get_content_models_as_option_array();
  unset($content_models['islandora:collectionCModel']);

  $form['child_creation'] = array(
    '#title' => "Create Child Collection",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['child_creation']['titlebox'] = array(
    '#type' => 'item',
    '#value' => t("Create New Child Collection within $this_collection_pid"),
  );

  $form['child_creation']['collection_name'] = array(
    '#title' => "Collection Name",
    '#type' => 'textfield',
    '#description' => t("Human readable name for this collection"),
  );

  $form['child_creation']['new_collection_pid'] = array(
    '#title' => "Collection PID",
    '#type' => 'textfield',
    '#size' => 15,
    '#default_value' => Fedora_Item::get_next_PID_in_namespace($collection_namespace),
    '#description' => t("Unique PID for this collection.  <br />Pids take the general form of namespace:collection eg islandora:pamphlets"),
  );
  if (!$restricted) {
    $form['child_creation']['collection_namespace'] = array(
      '#title' => "Collection Namespace",
      '#type' => 'textfield',
      '#size' => 15,
      '#default_value' => $collection_namespace,
      '#description' => t("Namespace for objects in this collection."),
    );
  }
  else {
    $form['child_creation']['collection_namespace'] = array(
      '#title' => "Collection Namespace",
      '#type' => 'select',
      '#options' => $allowed,
      '#default_value' => 'default',
      '#description' => t("Namespace for objects in this collection."),
    );
  }
  $form['parent_collection'] = array(
    '#type' => 'hidden',
    '#value' => $this_collection_pid,
  );

  $form['collection_pid'] = array(
    '#type' => 'hidden',
    '#value' => $this_collection_pid,
  );
  $form['child_creation']['content_models'] = array(
    '#title' => "Choose allowable content models for this collection",
    '#type' => 'checkboxes',
    '#options' => $content_models,
    '#description' => t("Content models describe the behaviours of objects with which they are associated."),
  );


  $form['child_creation']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Collection'),
    '#id' => 'create_child'
  );
  $form['manage_collection_policy'] = array(
    '#title' => "Manage Collection Policies",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['manage_collection_policy']['add'] = array(
    '#title' => "Add Content Model to Collection Policy",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => $show_delete,
  );

  $new_options = array();
  if (is_array($content_models) && is_array($cm_options)) {
    $new_options = array_diff_key($content_models, $cm_options);
  }

  $form ['manage_collection_policy']['add']['content_model_to_add'] = array(
    '#title' => "Choose Content Model",
    '#type' => 'select',
    '#options' => $new_options,
    '#description' => t("Choose content model to add to this collection policy."),
  );

  $form ['manage_collection_policy']['add']['new_cp_namespace'] = array(
    '#title' => "Choose Namespace",
    '#type' => 'textfield',
    '#size' => 15,
    '#default_value' => $collection_namespace,
    '#description' => t("Choose namespace for objects in this collection associated with this content model"),
  );
  $form['manage_collection_policy']['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Content Model to Collection Policy'),
    '#id' => 'add_cm'
  );

  if (count($current_models_in_policy) > 0) {
    $form['manage_collection_policy']['remove'] = array(
      '#title' => "Delete Content Model from Collection Policy",
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form ['manage_collection_policy']['remove']['content_models_to_remove'] = array(
      '#title' => "Choose Content Model to Remove",
      '#type' => 'checkboxes',
      '#options' => $current_models_in_policy,
      '#description' => t("Choose content models to remove from this collection policy."),
    );


    $form['manage_collection_policy']['remove']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Remove Content Model From Collection Policy'),
      '#id' => 'remove_cm'
    );
  }


  $form['change_cmodel'] = array(
    '#title' => "Change Content Models",
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['change_cmodel']['titlebox'] = array(
    '#type' => 'item',
    '#value' => t("Change Content Models within $this_collection_pid"),
  );

  $form['change_cmodel']['current_content_model'] = array(
    '#title' => "Choose content model to be changed",
    '#type' => 'select',
    '#options' => $represented_content_models,
    '#description' => t("All objects in this collection with the selected content model will be changed."),
  );
  $form['change_cmodel']['new_content_model'] = array(
    '#title' => "Choose new content model",
    '#type' => 'select',
    '#options' => $new_content_models,
    '#description' => t("The new content model to be assigned to selected objects."),
  );
  $form['change_cmodel']['collection_pid'] = array(
    '#type' => 'hidden',
    '#value' => $this_collection_pid,
  );
  $form['change_cmodel']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change Content Model Associations'),
    '#id' => 'change_model',
  );

  return $form;
}

/**
 * collection creation form validate 
 * @param array $form
 * @param array $form_state
 */
function collection_management_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'create_child') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
    $pid = $form_state['values']['new_collection_pid'];
    $item = new fedora_item($pid);
    $needs_model = FALSE;
    foreach ($form_state['values']['content_models'] as $key => $value) {
      if (is_string($value)) {

        $needs_model = FALSE;
      }
    }
    $checked = array_values($form_state['values']['content_models']);
    if ($item->exists()) {
      form_set_error('new_collection_pid', t("$pid already exists within your repository. the PID must be unique. Click on 'Manage This Collection' tab and enter new value."));
      return;
    }
    if (!valid_pid($pid)) {
      form_set_error('new_collection_pid', t("$pid is not a valid identifier. Click on 'Manage This Collection' tab and enter new value."));
      return;
    }
    if ($needs_model) {
      form_set_error('content_models', t("At least one content model must be selected. Click on 'Manage This Collection' tab and enter content model."));
      return;
    }
  }
  if ($form_state['clicked_button']['#id'] == 'add_cm') {

    $name_parts = explode(":", $form_state['values']['new_cp_namespace']);
    $namespace = $name_parts[0] . ":1";
    if (!valid_pid($namespace)) {
      form_set_error('new_cp_namespace', t("Namespace must be valid. Click on 'Manage This Collection' tab and enter new value."));

      return;
    }
    $form_state['values']['new_cp_namespace'] = $namespace;
  }
}

/**
 * collection creation form submit
 * @global user $user
 * @param arary $form
 * @param array $form_state
 */
function collection_management_form_submit($form, &$form_state) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_collection');
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  module_load_include('inc', 'fedora_repository', 'api/dublin_core');
  global $user;
  $collection_pid = $form_state['values']['parent_collection'];
  $policy = CollectionPolicy::loadFromCollection($collection_pid, TRUE);
  $collection_policy = '<?xml version="1.0" encoding="UTF-8"?>
<collection_policy xmlns="http://www.islandora.ca" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" name="" xsi:schemaLocation="http://www.islandora.ca http://syn.lib.umanitoba.ca/collection_policy.xsd">
    <content_models>
    <content_model dsid="ISLANDORACM" name="Islandora Collection Model ~ islandora:collectionCModel" namespace="islandora:1" pid="islandora:collectionCModel"/>
    </content_models>
    <search_terms>
    </search_terms>
    <staging_area></staging_area>
    <relationship>isMemberOfCollection</relationship>
</collection_policy>';


  // add child collection to policy
  if ($form_state['clicked_button']['#id'] == 'create_child') {
    $module_path = drupal_get_path('module', 'fedora_repository');
    $thumbnail = drupal_get_path('module', 'Fedora_Repository') . '/images/Crystal_Clear_filesystem_folder_grey.png';
    $new_collection_pid = $form_state['values']['new_collection_pid'];
    $new_collection_label = $form_state['values']['collection_name'];
    $pid_namespace = $form_state['values']['collection_namespace'];
    $all_cModels = get_content_models_as_option_array();

    $content_models = $form_state['values']['content_models'];
    $collection_policy_xml = simplexml_load_string($collection_policy);
    foreach ($content_models as $content_model) {
      if ($content_model) {
        $node = $collection_policy_xml->content_models->addChild('content_model');
        $node->addAttribute('dsid', 'ISLANDORACM');
        $node->addAttribute('name', $all_cModels[$content_model]);
        $node->addAttribute('namespace', $pid_namespace . ':1');
        $node->addAttribute('pid', $content_model);
      }
    }
    $item = fedora_item::ingest_new_item($new_collection_pid, 'A', $new_collection_label, $user->name);
    $item->add_relationship('isMemberOfCollection', $collection_pid, RELS_EXT_URI);
    $item->add_relationship('hasModel', 'islandora:collectionCModel', FEDORA_MODEL_URI);
    $item->add_datastream_from_string($collection_policy_xml->saveXML(), 'COLLECTION_POLICY', 'COLLECTION_POLICY', 'text/xml', 'X');
    $item->add_datastream_from_file($thumbnail, 'TN');
    drupal_goto("/fedora/repository/$new_collection_pid");
  }


  // add content model to collection policy
  if ($form_state['clicked_button']['#id'] == 'add_cm') {
    if (!$policy) {
      $item = new Fedora_Item($collection_pid);
      $item->add_datastream_from_string($collection_policy, 'COLLECTION_POLICY', 'COLLECTION_POLICY', 'text/xml', 'X');
      $policy = CollectionPolicy::loadFromCollection($collection_pid, TRUE);
    }

    $cp_namespace = $form_state['values']['new_cp_namespace'];
    $cp_content_model = $form_state['values']['content_model_to_add'];
    $policy->addModel(ContentModel::loadFromModel($cp_content_model), $cp_namespace);
    $policy->saveToFedora();
    drupal_set_message("Collection model successfully added");
  }

  //remove content model from collection policy
  if ($form_state['clicked_button']['#id'] == 'remove_cm') {
    $candidates = $form_state['values']['content_models_to_remove'];
    $count = 0;
    foreach ($candidates as $candidate) {
      if (is_string($candidate)) {
        $policy->removeModel(ContentModel::loadFromModel($candidate));
        $count++;
      }
    }
    if ($count > 0) {
      $policy->saveToFedora();
      if ($count > 1) {
        $s = 's';
      }
      drupal_set_message("$count collection model$s removed");
    }
  }


  //change content model on selected objects
  if ($form_state['clicked_button']['#id'] == 'change_model') {
    $current_content_model = $form_state['values']['current_content_model'];
    $new_content_model = $form_state['values']['new_content_model'];

    $add_to_policy = TRUE;
    $policy_cms = $policy->getContentModels();
    foreach ($policy_cms as $policy_cm) {
      if ($policy_cm->pid == $current_content_model) {
        $namespace = $policy_cm->pid_namespace;
      }
      if ($policy_cm->pid == $new_content_model) {
        $add_to_policy = FALSE;
      }
    }
    if ($add_to_policy) {
      $policy->addModel(ContentModel::loadFromModel($new_content_model), $namespace);
      $policy->saveToFedora();
    }
    $query = "select \$object  from <#ri>
    where (\$object <info:fedora/fedora-system:def/model#hasModel> <info:fedora/$current_content_model>
      and (\$object <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/$collection_pid>
           or \$object <info:fedora/fedora-system:def/relations-external#isMemberOf> <info:fedora/$collection_pid>)
      and \$object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>)";
    $query = htmlentities(urlencode($query));
    $content = '';

    $url = variable_get('fedora_repository_url', 'http://localhost:8080/fedora/risearch');
    $url .= "?type=tuples&flush=TRUE&format=csv&limit=$limit&offset=$offset&lang=itql&stream=on&query=" . $query;
    $content .= do_curl($url);
    $results = explode("\n", $content);
    $object_pids = preg_replace('/^info:fedora\/|"object"/', '', $results);
    $count = 0;
    foreach ($object_pids as $object_pid) {
      if (!$object_pid) {
        continue;
      }
      $item = new fedora_item($object_pid);
      $item->purge_relationship('hasModel', $current_content_model);
      $item->add_relationship('hasModel', $new_content_model, FEDORA_MODEL_URI);
      $count++;
    }
    drupal_set_message("$current_content_model changed to $new_content_model on $count objects");
  }

}

/**
 * deletes PID 
 * @param string $pid
 */
function delete_objects_as_batch($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  $name = $user->name;
  $restConnection->repository->purgeObject($pid);
}

/**
 * removes association of this object to this collection
 * @param string $pid
 */
function remove_collection_association($pid, $collection_pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  $fedora_object = new FedoraObject($pid, $restConnection->repository);
  $fedora_object->relationships->remove(NULL, 'isMemberOfCollection', $collection_pid);
}

/**
 * returns content models associated with all objects in a collection
 * @param string $pid
 * @return array
 */
function get_represented_content_models($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

  $query = "select \$model \$title from <#ri>
where (\$object <info:fedora/fedora-system:def/relations-external#isMemberOf> <info:fedora/$pid>
       or \$object <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/$pid>)
       and \$object <info:fedora/fedora-system:def/model#hasModel> \$model
       and \$object <dc:title> \$title";

  $model_pids = $restConnection->repository->ri->itqlQuery($query, 'unlimited', '0');
  $represented_models = array();
  foreach ($model_pids as $model_pid) {
    if ($model_pid && $model_pid['model']['value'] != 'fedora-system:FedoraObject-3.0') {

      $represented_models[$model_pid['model']['value']] = $model_pid['model']['value'] . ' ~ ' . $model_pid['title']['value'];
    }
  }
  return $represented_models;
}

function get_child_collections($collection_pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

  $query = <<<EOD
  select \$object from <#ri>
    where \$object <info:fedora/fedora-system:def/model#hasModel> <info:fedora/islandora:collectionCModel>
    and \$object  <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/$collection_pid>
EOD;

  $lines = $restConnection->repository->ri->itqlQuery($query, 'unlimited', '0');
  $collection_pids = array_values(array_filter($lines));
  return $collection_pids;
}

function Islandora_collections_get_collection_from_pid($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

  $query = 'select  $parent from <#ri>
		where ($object <fedora-rels-ext:isMemberOf> $parent
    or $object <fedora-rels-ext:isMemberOfCollection> $parent)
		and $object <dc:identifier> \'' . $pid . '\'
		order by $object';

  $object_pids = $restConnection->repository->ri->itqlQuery($query, 'unlimited', '0');
  $object_pids = array_values(array_filter($object_pids));
  return $object_pids;
}


/**
 * Returns an array of pids that match the query contained in the collection
 * object's QUERY datastream or in the suppled $query parameter.
 * @param <type> $collection_pid
 * @param <type> $query
 * @param <type> $query_format R
 */
function get_related_items_as_array($collection_pid, $relationship = array('isMemberOfCollection'), $limit = 10000, $offset = 0, $active_objects_only = TRUE, $cmodel = NULL, $orderby = '$title') {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';
  
  global $user;
//  if (!fedora_repository_access(OBJECTHELPER :: $OBJECT_HELPER_VIEW_FEDORA, $pid, $user)) {
//    drupal_set_message(t("You do not have access to Fedora objects within the attempted namespace or access to Fedora denied."), 'error');
//    return array();
//  }

  $query_string = 'select $object $title $content from <#ri>
                             where ($object <fedora-model:label> $title
                             and $object <fedora-model:hasModel> $content
                             and (';

  if (is_array($relationship)) {
    foreach ($relationship as $rel) {
      $query_string .= '$object <fedora-rels-ext:' . $rel . '> <info:fedora/' . $collection_pid . '>';
      if (next($relationship)) {
        $query_string .= ' OR ';
      }
    }
  }
  elseif (is_string($relationship)) {
    $query_string .= '$object <fedora-rels-ext:' . $relationship . '> <info:fedora/' . $collection_pid . '>';
  }
  else {
    return '';
  }

  $query_string .= ') ';
  $query_string .= $active_objects_only ? 'and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>' : '';

  if ($cmodel) {
    $query_string .= ' and $content <mulgara:is> <info:fedora/' . $cmodel . '>';
  }

  $query_string .= ')
                    minus $content <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
                    order by ' . $orderby;
  
  $results = $restConnection->repository->ri->itqlQuery($query_string, $limit, $offset);

  return $results;
}


//function get_related_items_as_array($collection_pid, $relationship = 'isMemberOfCollection', $limit = 10000, $offset = 0, $active_objects_only = TRUE, $cmodel = NULL, $orderby = '$title') {
//  $content = get_related_items_as_xml($collection_pid, $relationship, $limit, $offset, $active_objects_only, $cmodel, $orderby);
//  if (empty($content)) {
//    return array();
//  }
//
//  $content = new SimpleXMLElement($content);
//
//  $resultsarray = array();
//  foreach ($content->results->result as $result) {
//    $resultsarray[] = substr($result->object->attributes()->uri, 12); // Remove 'info:fedora/'.
//  }
//  return $resultsarray;
//}

  /**
   * gets the name of the content models for the specified object
   * this now returns an array of pids as in Fedora 3 we can have more then one Cmodel for an object
   * @param type $pid
   * @param type $include_fedora_system_content_models
   * @return array 
   */
  function get_content_models_list($pid, $include_fedora_system_content_models = FALSE) {
//    module_load_include('inc', 'fedora_repository', 'CollectionClass');
//    module_load_include('inc', 'fedora_repository', 'ContentModel');
//    $collectionHelper = new CollectionClass();
  module_load_include('inc', 'islandora', 'RestConnection');
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';
  $restConnection = new RestConnection();
    $pids = array();
    $query = 'select $object from <#ri>
            where <info:fedora/' . $pid . '> <fedora-model:hasModel> $object
            and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>';
//    $content_models = $collectionHelper->getRelatedItems($pid, $query);
    $content_models = $restConnection->repository->ri->itqlQuery($query, 'unlimited', '0');
    
    if (empty($content_models)) {
      return $pids;
    }

    $cmodels = array();
    foreach ($content_models as $content_model) {
      if (strpos($content_model['object']['uri'], 'fedora-system:FedoraObject-3.0') != FALSE && $include_fedora_system_content_models == FALSE) {
        continue;
      }
      $cmodels[] = substr(strstr($content_model['object']['uri'], '/'), 1);
//      $cm = ContentModel::loadFromModel($cmodel_pid);
//      if ($cm) {
//        $cmodels[] = $cm;
//      }
    }

    return $cmodels;
  }
  
  /**
 * Function: get_content_models_as_option_array
 *
 * Description: Returns an associative array of all available content models in Fedora instance
 *
 * @return array
 */
function get_content_models_as_option_array() {
  module_load_include('inc', 'islandora', 'RestConnection');
  $restConnection = new RestConnection();
    require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

    $restricted = variable_get('fedora_namespace_restriction_enforced', TRUE);
    $allowed_string = variable_get('fedora_pids_allowed', 'default: demo: changeme: islandora:');
    $namespaces = explode(':', $allowed_string);
    foreach ($namespaces as $namespace) {
        if ($namespace) {
            $allowed[] = trim($namespace);
        }
    }
    $query = 'select $object $title from <#ri>
          where ($object <fedora-model:label> $title
          and ($object <fedora-model:hasModel> <info:fedora/fedora-system:ContentModel-3.0>
          or $object <fedora-rels-ext:isMemberOfCollection> <info:fedora/islandora:ContentModelsCollection>)
          and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>)
          order by $title';
    
    $list = $restConnection->repository->ri->itqlQuery($query, 'unlimited', '0');
    $options = array();
    foreach ($list as $item) {   //removes  blanks
        if ($item) {
          $item_namespace = explode(':', $item['object']['value']);
            if (!$restricted || in_array($item_namespace[0], $allowed)) {

                if (!preg_match('/fedora-system/', $item['object']['value'])) {
                    $options[$item['object']['value']] = $item['title']['value'] . ' ~ ' . $item['object']['value'];
                }
            }
        }
    }

    return $options;
}

  function getContentModels($collection_pid, $showError = TRUE) {
    module_load_include('inc', 'islandora', 'RestConnection');
    $restConnection = new RestConnection();
//    module_load_include('inc', 'Fedora_Repository', 'ContentModel');
    $collection_stream = $this->getCollectionPolicyStream($collection_pid);
    $collection_object = new FedoraObject($collection_pid, $restConnection->repository);
    $collection_stream = $collection_object->getDatastream('COLLECTION_POLICY');
    try {
      $xml = new SimpleXMLElement($collection_stream);
    } catch (Exception $e) {
      if ($showError) {
        drupal_set_message(t('@e', array('@e' => check_plain($e->getMessage()))), 'error');
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
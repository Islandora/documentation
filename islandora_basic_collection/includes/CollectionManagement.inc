<?php

/**
 * @file 
 * CollectionManagement.inc
 */

/**
 * deletes PID 
 * @param string $pid
 */
function delete_objects_as_batch($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $rest_connection = new RestConnection();
  $name = $user->name;
  $rest_connection->repository->purgeObject($pid);
}

/**
 * removes association of this object to this collection
 * @param string $pid
 */
function remove_collection_association($pid, $collection_pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $rest_connection = new RestConnection();
  $fedora_object = new FedoraObject($pid, $rest_connection->repository);
  $fedora_object->relationships->remove(NULL, 'isMemberOfCollection', $collection_pid);
}

/**
 * returns content models associated with all objects in a collection
 * @param string $pid
 * @return array
 */
function get_represented_content_models($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $rest_connection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

  $query = "select \$model \$title from <#ri>
where (\$object <info:fedora/fedora-system:def/relations-external#isMemberOf> <info:fedora/$pid>
       or \$object <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/$pid>)
       and \$object <info:fedora/fedora-system:def/model#hasModel> \$model
       and \$object <dc:title> \$title";

  $model_pids = $rest_connection->repository->ri->itqlQuery($query, 'unlimited', '0');
  $represented_models = array();
  foreach ($model_pids as $model_pid) {
    
    if ($model_pid['model']['value'] && $model_pid['model']['value'] != 'fedora-system:FedoraObject-3.0') {
      $fedora_object = new FedoraObject($model_pid['model']['value'], $rest_connection->repository);
      $content_model_title = $fedora_object->label;
      $represented_models[$model_pid['model']['value']] = $model_pid['model']['value'] . ' ~ ' . $content_model_title;
    }
  }
  return $represented_models;
}

function get_child_collections($collection_pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $rest_connection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

  $query = <<<EOD
  select \$object from <#ri>
    where \$object <info:fedora/fedora-system:def/model#hasModel> <info:fedora/islandora:collectionCModel>
    and \$object  <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/$collection_pid>
EOD;

  $lines = $rest_connection->repository->ri->itqlQuery($query, 'unlimited', '0');
  $collection_pids = array_values(array_filter($lines));
  return $collection_pids;
}

function islandora_collections_get_collection_from_pid($pid) {
  module_load_include('inc', 'islandora', 'RestConnection');
  $rest_connection = new RestConnection();
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';

  $query = 'select  $parent from <#ri>
		where ($object <fedora-rels-ext:isMemberOf> $parent
    or $object <fedora-rels-ext:isMemberOfCollection> $parent)
		and $object <dc:identifier> \'' . $pid . '\'
		order by $object';

  $object_pids = $rest_connection->repository->ri->itqlQuery($query, 'unlimited', '0');
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
  $rest_connection = new RestConnection();
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
  
  $results = $rest_connection->repository->ri->itqlQuery($query_string, $limit, $offset);

  return $results;
}

  /**
   * gets the name of the content models for the specified object
   * this now returns an array of pids as in Fedora 3 we can have more then one Cmodel for an object
   * @param type $pid
   * @param type $include_fedora_system_content_models
   * @return array 
   */
  function get_content_models_list($pid, $include_fedora_system_content_models = FALSE) {

  module_load_include('inc', 'islandora', 'RestConnection');
  require_once 'sites/all/libraries/tuque/RepositoryQuery.php';
  $rest_connection = new RestConnection();
    $pids = array();
    $query = 'select $object from <#ri>
            where <info:fedora/' . $pid . '> <fedora-model:hasModel> $object
            and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>';
    $content_models = $rest_connection->repository->ri->itqlQuery($query, 'unlimited', '0');
    
    if (empty($content_models)) {
      return $pids;
    }

    $cmodels = array();
    foreach ($content_models as $content_model) {
      if (strpos($content_model['object']['uri'], 'fedora-system:FedoraObject-3.0') != FALSE && $include_fedora_system_content_models == FALSE) {
        continue;
      }
      $cmodels[] = substr(strstr($content_model['object']['uri'], '/'), 1);
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
  $rest_connection = new RestConnection();
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
    
    $list = $rest_connection->repository->ri->itqlQuery($query, 'unlimited', '0');
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

  function get_content_models($collection_pid, $show_error = TRUE) {
    module_load_include('inc', 'islandora', 'RestConnection');
    $rest_connection = new RestConnection();
    $collection_stream = $this->getCollectionPolicyStream($collection_pid);
    $collection_object = new FedoraObject($collection_pid, $rest_connection->repository);
    $collection_stream = $collection_object->getDatastream('COLLECTION_POLICY');
    try {
      $xml = new SimpleXMLElement($collection_stream);
    } catch (Exception $e) {
      if ($show_error) {
        drupal_set_message(t('@e', array('@e' => check_plain($e->getMessage()))), 'error');
      }
      return NULL;
    }
    foreach ($xml->contentmodels->contentmodel as $content_model) {
      $content_model_object = new ContentModel();
      $content_model_object->contentModelDsid = $content_model->dsid;
      $content_model_object->contentModelPid = $content_model->pid;
      $content_model_object->pidNamespace = $content_model->pidNamespace;
      $content_model_object->contentModelName = $content_model['name'];
      $models[] = $content_model_object;
    }
    return $models;
  }
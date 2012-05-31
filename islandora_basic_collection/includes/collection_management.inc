<?php

/**
 * @file 
 * CollectionManagement.inc
 * A set of useful functions to help with collection management
 */

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

// Not sure if this is necessary given that we should never be able to delete objects in a namespace that we don't have access to.
//  if (!fedora_repository_access('view fedora repository', $collection_pid['object']['value'])) {
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
 * determines whether we can see the object or not
 * checks PID namespace permissions, and user permissions
 * @global type $user
 * @param type $op
 * @param type $pid
 * @return type 
 */
function fedora_repository_access($permission, $pid) {
  global $user;
  $name_space_access = FALSE;
  $is_restricted = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if (!$is_restricted) {
    $name_space_access = TRUE;
  }
  if ($pid == NULL) {
    $pid = variable_get('islandora_repository_pid', 'islandora:root');
  }
  $name_space_allowed = explode(" ", variable_get('islandora_pids_allowed', 'default: demo: changeme: islandora: ilives: islandora-book: books: newspapers: '));
  $pos = NULL;
  foreach ($name_space_allowed as $name_space) {
    $pos = stripos($pid, $name_space);
    if ($pos === 0) {
      $name_space_access = TRUE;
    }
  }
  if ($name_space_access) {
    $user_access = user_access($permission);
    if ($user_access == NULL) {
      return FALSE;
    }
    return $user_access;
  }
  else {
    return FALSE;
  }
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

  $restricted = variable_get('islandora_namespace_restriction_enforced', FALSE);
  $allowed_string = variable_get('islandora_pids_allowed', 'default: demo: changeme: islandora:');
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
<?php

/**
 * @file
 *
 * This file contains any functions meant to be part of the global space, ie. functions that and other modules will use without including a module file.
 */

/**
 * Just a wrapper around fetchings the IslandoraTuque object, with some very basic error logging.
 *
 * @return IslandoraTuque
 *   A IslandoraTuque instance
 */
function islandora_get_tuque_connection() {
  $tuque = &drupal_static(__FUNCTION__);
  if (!$tuque) {
    try {
      $tuque = new IslandoraTuque();
    } catch (Exception $e) {
      drupal_set_message(t('Unable to connect to the repository %e', array('%e' => $e)), 'error');
    }
  }
  return $tuque;
}

/**
 * Gets the given object if found, NULL if it is inaccessible and FALSE if it was not found.
 *
 * @param string $object_id
 *   The identifier of the object to get.
 *
 * @return FedoraObject
 *   The object if found, NULL if it is inaccessible and FALSE if it was not found.
 */
function islandora_get_object_by_id($object_id) {
  $tuque = islandora_get_tuque_connection();
  if ($tuque) {
    try {
      $object = $tuque->repository->getObject($object_id);
      drupal_alter('islandora_object', $object);
      return $object;
    } catch (Exception $e) {
      if ($e->getCode() == '404') {
        return FALSE;
      }
      else {
        return NULL;
      }
    }
  }
  else {
    IslandoraTuque::getError();
  }
  return NULL; // Assuming access denied in all other cases for now.
}

/**
 * Ingest the given object into Fedora calling its pre/post hooks as well.
 *
 * @param NewFedoraObject $object
 *   An ingestable FedoraObject.
 *
 * @return FedoraObject
 *   The ingested FedoraObject, after running the pre/post ingest hooks.
 */
function islandora_add_object(NewFedoraObject &$object) {
  islandora_pre_add_object($object);
  $object->repository->ingestObject($object);
  islandora_post_add_object($object);
  return $object;
}

/**
 * Calls the ISLANDORA_PRE_INGEST_HOOK hooks.
 *
 * @param NewFedoraObject $object
 *   An ingestable FedoraObject.
 */
function islandora_pre_add_object(NewFedoraObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  foreach (islandora_build_hook_list(ISLANDORA_PRE_INGEST_HOOK, $object->models) as $hook) {
    module_invoke_all($hook, $object);
  }
}

/**
 * Calls the ISLANDORA_POST_INGEST_HOOK hooks.
 *
 * @param FedoraObject $object
 *   A recently ingestable FedoraObject.
 */
function islandora_post_add_object(FedoraObject $object) {
  foreach (islandora_build_hook_list(ISLANDORA_POST_INGEST_HOOK, $object->models) as $hook) {
    module_invoke_all($hook, $object);
  }
}

/**
 * Deletes the given object into Fedora calling its pre/post hooks as well.
 *
 * @param FedoraObject $object
 *   An object to delete.
 *
 * @return FedoraObject
 *   The ingested FedoraObject, after running the pre/post ingest hooks.
 */
function islandora_delete_object(FedoraObject &$object) {
  $object_id = $object->id;
  $models = $object->models;
  $action = islandora_pre_delete_object($object);
  switch ($action) {
    case 'blocked': // Do nothing.
      return FALSE;
    case 'delete': // Change the state to deleted.
      $object->delete();
      islandora_post_delete_object($object_id, $models);
      return TRUE;
    default: // Purge
      $object->repository->purgeObject($object_id);
      islandora_post_delete_object($object_id, $models);
      $object = NULL;
      return TRUE;
  }
}

/**
 * Calls the ISLANDORA_PRE_PURGE_OBJECT_HOOK hooks.
 *
 * @param FedoraObject $object
 *   The object that is about to be deleted.
 */
function islandora_pre_delete_object(FedoraObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $results = array();
  foreach (islandora_build_hook_list(ISLANDORA_PRE_PURGE_OBJECT_HOOK, $object->models) as $hook) {
    $results = array_merge_recursive($results, module_invoke_all($hook, $object));
  }
  $action = (isset($results['block']) && $results['block']) ? 'block' : FALSE;
  $action = (!$action && isset($results['delete']) && $results['delete']) ? 'delete' : $action;
  $action = !$action ? 'purge' : $action;
  return $action;
}

/**
 * Calls the ISLANDORA_POST_PURGE_OBJECT_HOOK hooks.
 *
 * @param string $object_id
 *   The object id of an the recently deleted object.
 * @param array $models
 *   The list of content models the delete object subsribed to.
 */
function islandora_post_delete_object($object_id, array $models) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  foreach (islandora_build_hook_list(ISLANDORA_POST_PURGE_OBJECT_HOOK, $models) as $hook) {
    module_invoke_all($hook, $object_id, $models);
  }
}

/**
 * Delete's/Purges the given datastream.
 *
 * @throws Exception
 *  Which types are undefined, but more than likely because of the hooks there will be several kinds.
 *
 * @param FedoraDatastream $datastream
 *   The datastream to delete.
 *
 * @return boolean
 *   TRUE is returned if the datastream was Deleted/Purged, FALSE if it was blocked.
 */
function islandora_delete_datastream(FedoraDatastream &$datastream) {
  $datastream_id = $datastream->id;
  $object = $datastream->parent;
  $action = islandora_pre_delete_datastream($datastream);
  switch ($action) {
    case 'blocked': // Do nothing.
      return FALSE;
    case 'delete': // Change the state to deleted.
      $object[$datastream_id]->state = 'D';
      islandora_post_delete_datastream($object, $datastream_id); // @todo Differentiate between delete/purge in the hooks.
      return TRUE;
    default: // Purge
      $object->purgeDatastream($datastream_id);
      islandora_post_delete_datastream($object, $datastream_id);
      $datastream = NULL;
      return TRUE;
  }
}

/**
 * The default behaviour is to 'purge' the datastream but this can be overridden by modules that implement
 * the 'islandora_pre_purge_datastream' hook.
 *
 * @todo make this an alter.
 *
 * The returned array can include a 'block' => TRUE
 * pair which will prevent the datastream from being deleted if it particularly needed for
 * a certain function. Returning 'delete' => TRUE will cause the datastream to be put into
 * a deleted state.
 *
 * @param FedoraDatastream $datastream
 *   The datastream to delete.
 *
 * @return string
 *   The action to take when deleting the given datastream, either 'purge', 'delete', or 'block'.
 */
function islandora_pre_delete_datastream(FedoraDatastream $datastream) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $results = array();
  foreach (islandora_build_hook_list(ISLANDORA_PRE_PURGE_DATASTREAM_HOOK, $datastream->parent->models) as $hook) {
    $results = array_merge_recursive($results, module_invoke_all($hook, $datastream)); // @note Not sure this will work the greatest, probably an alter would be better...
  }
  $action = (isset($results['block']) && $results['block']) ? 'block' : FALSE;
  $action = (!$action && isset($results['delete']) && $results['delete']) ? 'delete' : $action;
  $action = !$action ? 'purge' : $action;
  return $action;
}

/**
 * Calls the post purge datastream hooks.
 *
 * @todo Should differentiate between purging/deleting.
 *
 * @param FedoraObject $object
 *   The parent object of the deleted datastream.
 * @param string $datastream_id
 *   The datastream id of the deleted datastream.
 */
function islandora_post_delete_datastream(FedoraObject $object, $datastream_id) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  foreach (islandora_build_hook_list(ISLANDORA_POST_PURGE_DATASTREAM_HOOK, $object->models) as $hook) {
    module_invoke_all($hook, $object, $datastream_id);
  }
}

/**
 * Gets any objects that the given object has a (isMemberOf, isMemberOfCollection) relationship with.
 *
 * This function gets its info from the RELS-EXT directly rather than through an risearch.
 *
 * @param FedoraObject $object
 *   The object whose parents will be returned.
 *
 * @return array
 *   An array of FedoraObject's that the given object has a (isMemberOf, isMemberOfCollection) relationship with.
 */
function islandora_get_parents_from_rels_ext(FedoraObject $object) {
  try {
    $collections = array_merge(
      $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection'),
      $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOf'));
  }
  catch (RepositoryException $e) {
    return array();
  } // @todo some logging would be nice, not sure what this throws.
  $collections = array_map(function($o) { return islandora_get_object_by_id($o['object']['value']); }, $collections);
  return array_filter($collections);
}

/**
 * Checks what datastreams the object already has against its required datastreams as defined by its content models, and returns their intersection.
 *
 * @param FedoraObject $object
 *   The object which models will be used to determine what datastreams it should have.
 *
 * @return array
 *   The DS-COMPOSITE-MODEL defined datastreams that are required for the given object, but not already present.
 */
function islandora_get_missing_datastreams_requirements(FedoraObject $object) {
  $datastreams = islandora_get_datastreams_requirements($object);
  foreach ($datastreams as $dsid => $requirements) {
    if (isset($object[$dsid])) {
      unset($datastreams[$dsid]);
    }
  }
  return $datastreams;
}

/**
 * Checks the object's content model's for which datastream are expected to be used with this object, as defined by the DS-COMPOSITE-MODEL datastreams.
 *
 * For duplicate datastreams in the models, the first model defines the datastreams attributes regardless of what other models define.
 * This should be undefined behavior according to the documentation.
 * @see https://wiki.duraspace.org/display/FEDORA34/Fedora+Digital+Object+Model#FedoraDigitalObjectModel-ContentModelObjectCMODEL
 *
 * @param FedoraObject $object
 *   The object which models will be used to determine what datastreams it should have.
 *
 * @see islandora_get_required_datastreams_from_content_model() from more details on the return value.
 *
 * @return array
 *   The DS-COMPOSITE-MODEL defined datastreams that are required for the given object.
 */
function islandora_get_datastreams_requirements(FedoraObject $object) {
  $dsids = array();
  foreach ($object->models as  $model) {
    $model = islandora_get_object_by_id($model);
    $dsids += islandora_get_datastreams_requirements_from_content_model($model);
  }
  unset($dsids['AUDIT']); // The AUDIT Datastream can not really be added, so it can't really be missing.
  return $dsids;
}

/**
 * Checks the given content model for which datastreams are required for subscribing objects, as defined by it's DS-COMPOSITE-MODEL datastream.
 *
 * @todo Add support for fetching the schema information.
 *
 * @param FedoraObject $object
 *   The content model whose DS-COMPOSITE-MODEL datastream will be used to determine what datastreams are required.
 *
 * @return array
 *   The DS-COMPOSITE-MODEL defined datastreams that are required for the given object.
 *
 * @code
 *   array(
 *     'DC' => array(
 *       'id' => 'DC',
 *       'mime' => 'text/xml',
 *       'optional' => FALSE,
 *     )
 *   )
 * @endcode
 */
function islandora_get_datastreams_requirements_from_content_model(FedoraObject $object) {
  if (empty($object[DS_COMP_STREAM])) {
    return array();
  }
  $xml = new SimpleXMLElement($object[DS_COMP_STREAM]->content);
  foreach ($xml->dsTypeModel as $ds) {
    $dsid = (string) $ds['ID'];
    $optional = strtolower((string) $ds['optional']);
    $mime = array();
    foreach ($ds->form as $form) {
      $mime[] = (string) $form['MIME'];
    }
    $dsids[$dsid] = array(
      'id' => $dsid,
      'mime' => $mime,
      'optional' => ($optional == 'true') ? TRUE : FALSE
    );
  }
  return $dsids;
}


/**
 * Prepare an ingestable object.
 *
 * @param string $namespace
 *   The namespace in which the PID for the new object will be created.
 * @param string $label
 *   An optional label to apply to the object.
 * @param array $datastreams
 *   A array of datastreams to add, where each datastream definition is an
 *   associative array containing:
 *   - dsid: The datastream ID.
 *   - label: An optional label for the datastream.
 *   - mimetype: A MIMEtype for the datastream; defaults to text/xml.
 *   - control_group: One of X, M, R and E; defaults to M.
 *   - datastream_file: A web-accessible path, for which we try to get an
 *     absolute path using url().
 * @param array $content_models
 *   An array of content model PIDs to which the new object should subscribe.
 * @param array $relationships
 *   An array of relationships, where each relationship is an associative array
 *   containing:
 *   - relationship: The predicate for the relationship, from the Fedora
 *     RELS-EXT namespace.
 *   - pid: The object for the relationship, to which we are creating the
 *     relationhsip.
 *
 * @return NewFedoraObject
 *   An ingestable NewFedoraObject.
 */
function islandora_prepare_new_object($namespace = NULL, $label = NULL, $datastreams = array(), $content_models = array(), $relationships = array()) {
  $tuque = islandora_get_tuque_connection();
  $object = isset($namespace) ? $tuque->repository->constructObject($namespace) : new NewFedoraObject(NULL, $tuque->repository);
  $object->owner = isset($user->name) ? $user->name : $object->owner;
  $object->label = isset($label) ? $label : $object->label;
  foreach ($content_models as $content_model) {
    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model);
  }
  foreach ($relationships as $relationship) {
    $object->relationships->add(FEDORA_RELS_EXT_URI, $relationship['relationship'], $relationship['pid']);
  }
  foreach ($datastreams as $ds) {
    $dsid = $ds['dsid'];
    $label = isset($ds['label']) ? $ds['label'] : '';
    $mimetype = isset($ds['mimetype']) ? $ds['mimetype'] : 'text/xml';
    $control_group = (isset($ds['control_group']) && in_array($ds['control_group'], array('X', 'M', 'R', 'E'))) ? $ds['control_group'] : 'M'; // Default 'Managed'
    $datastream_file = url($ds['datastream_file'], array('absolute' => TRUE));
    $datastream = $object->constructDatastream($dsid, $control_group);
    $datastream->label = $label;
    $datastream->mimetype = $mimetype;
    switch ($control_group) {
      case 'M':
        $datastream->setContentFromUrl($datastream_file);
        break;
      case 'X':
        $datastream->setContentFromString(file_get_contents($datastream_file));
        break;
    }
    $object->ingestDatastream($datastream);
  }
  return $object;
}

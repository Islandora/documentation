<?php

/**
 * @file
 * This file lists and documents all available hook functions to manipulate data.
 */

/**
 * Allows modules to add to a repository objects view/edit(/misc) process.
 *
 * If you implement this hook you must also register your module for view
 * with the hook_islandora_get_types().
 *
 * Islandora gets all displays back in an array.  The order they are displayed
 * is based on the order of the key of the array that each module returns.
 *
 * "HOOK_NAME" reflects those provided by the islandora_get_types() function,
 * and these functions should be called via islandora_invoke().  There are a
 * number of these hooks defined at the top of islandora.module, including:
 * - ISLANDORA_VIEW_HOOK: Invoked in islandora_view_object(); returns an array
 *     containing string values, and whose keys will be used for sorting.
 *     Takes four parameters:
 *   - fedora_object: A Tuque FedoraObject being operated on.
 *   - user: The user accessing the object.
 *   - page_number: The page in the content.
 *   - page_size: The size o the page.
 * - ISLANDORA_EDIT_HOOK: Invoked in islandora_edit_object(); returns an array
 *     containing string values, whose keys will be used for sorting.
 *     Takes one parameter:
 *   - fedora_object: A Tuque FedoraObject being operated on.
 * - ISLANDORA_POST_INGEST_HOOK: Invoked in islandora_ingest_add_object(); no
 *     return.  Probably want to use this to add additional datastreams, or
 *     or perhap remove some.  Takes one parameter:
 *   - The object in question.
 * - ISLANDORA_PRE_PURGE_OBJECT_HOOK: Invoked in islandora_object_purge(); can
 *     return an associative array of with the key "delete" mapped to TRUE,
 *     which will cause the object to be marked "deleted" instead of actually
 *     purging the object. (This should be verirfied?)
 *
 * @param type $islandora_object
 *   A Tuque FedoraObject
 * @param ...
 *   A variable list of arguments, as required by the in question hook.
 *
 * @return array
 *   An array to merge in.
 */
function hook_HOOK_NAME($islandora_object) {}

/**
 * Alter an object before further processing in islandora_invoke().
 */
function hook_HOOK_NAME_alter($fedora_object) {}

/**
 * Allow output of hooks passing through islandora_invoke() to be altered.
 * @param type $arr
 *   an arr of rendered views
 */
function hook_HOOK_NAME_output_alter($arr) {}


/**
 * Get an array of object types provided by other modules.
 *
 * @return array
 *   An associative array mapping cmodel PIDs to module names to hooks to
 *   booleans indicating that the given module's implementation of the hook
 *   should be invoked.
 */
function hook_islandora_get_types() {
  $types = array(
    'my:coolCModel' => array(
      'my_cool_module' => array(
        ISLANDORA_VIEW_HOOK => TRUE,
      ),
    ),
  );
  return $types;
}

/**
 * Allows modules to define an object edit page by cmodel.
 *
 * Your module must return TRUE for ISLANDORA_EDIT_HOOK in its
 * implementation of hook_islandora_get_types().
 *
 * Islandora provides a default implementation that should work for most use
 * cases.
 *
 * @param object $islandora_object
 *   A Tuque FedoraObject to be edited.
 * @return array
 *   An array of strings containing markup, which will be imploded together.
 */
function hook_islandora_edit_object($islandora_object) {}

/**
 * Register potential ingest routes.
 *
 * Implementations should return an array containing possible routes.
 */
function hook_islandora_ingest_registry($collection_object) {
  $reg = array(
    array(
      'name' => t('Ingest Route Name'),
      'url' => 'ingest_route/url',
      'weight' => 0,
    ),
  );
  return $reg
}

/**
 * Register a datastream edit route/form.
 *
 * @param $islandora_object
 * @param $ds_id
 */
function hook_islandora_edit_datastream_registry($islandora_object, $ds_id) {}

/**
 * Alter an object before it gets used further down the stack.
 * @param type $object
 *   A Tuque FedoraObject
 */
function hook_islandora_object_alter($fedora_object) {}

/**
 * Allow modification of an object before ingesting.
 *
 * @param type $islandora_object
 *   a tuque FedoraObject
 * @param array $content_models
 * @param string $collection_pid
 */
function hook_islandora_ingest_pre_ingest($islandora_object, $content_models, $collection_pid) {}

/**
 * Allow modules to setup for the purge of a datastream.
 *
 * @param object $datastream
 *   A Tuque FedoraDatastream object.
 */
function hook_islandora_pre_purge_datastream($datastream) {}

/**
 * Allow modules to react after a datastream is purged.
 *
 * @param object $object
 *   A Tuque FedoraObject.
 * @param string $dsid
 *   A id of the former datastream.
 */
function hook_islandora_post_purge_datastream($object, $dsid) {}

/**
 * Allow modules to react post-purge.
 *
 * @param string $object_id
 *   The former object's PID.
 * @param array $content_models
 *   An array containing the models to which the former object.
 */
function hook_islandora_post_purge_object($object_id, $content_models) {}


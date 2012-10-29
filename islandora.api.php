<?php

/**
 * @file
 * This file lists and documents all available hook functions to manipulate data.
 */

/**
 * Generate a repository objects view.
 *
 * If you implement this hook you must also register your module with
 * hook_islandora_hook_info().
 *
 * @param type $islandora_object
 *   A Tuque FedoraObject
 * @param FedoraObject $fedora_object
 *   A Tuque FedoraObject being operated on.
 * @param object $user
 *   The user accessing the object.
 * @param string $page_number
 *   The page in the content.
 * @param string $page_size: The size of the page.
 *
 * @return array
 *   An array whose values are markup.
 */
function hook_islandora_view_object($fedora_object, $user, $page_number, $page_size) {}

/**
 * Alter an object before processing in hook_islandora_view_object().
 *
 * @param FedoraObject $fedora_object
 *   The Tuque FedoraObject being displayed.
 */
function hook_islandora_view_object_alter(&$fedora_object) {}

/**
 * Alter display output after it has been generated.
 *
 * @param array $arr
 *   An arr of rendered views.
 */
function hook_islandora_view_object_output_alter(&$arr) {}

/**
 * Generate an object's management display.
 *
 * If you implement this hook you must also register your module with
 * hook_islandora_hook_info().
 *
 * @param type $fedora_object
 *   A Tuque FedoraObject
 *
 * @return array
 *   An array whose values are markup.
 */
function hook_islandora_edit_object($fedora_object) {}

/**
 * Alter an object before processing in hook_islandora_edit_object().
 * 
 * @param FedoraObject $fedora_object
 *   The Tuque FedoraObject to alter.
 */
function hook_islandora_edit_object_alter(&$fedora_object) {}

/**
 * Allow management display output to be altered.
 *
 * @param type $arr
 *   an arr of rendered views
 */
function hook_islandora_edit_object_output_alter(&$arr) {}

/**
 * Allows modules to add to an objects ingest process.
 *
 * If you implement this hook you must also register your module for with
 * hook_islandora_hook_info().
 *
 * NOTE: This doesn't normally return any output.
 *
 * @param FedoraObject $fedora_object
 *   A Tuque FedoraObject.
 */
function hook_islandora_ingest_post_ingest($fedora_object) {}

/**
 * Alter an object before processing in hook_islandora_ingest_post_ingest().
 *
 * @param FedoraObject $fedora_object
 *   A Tuque FedoraObject.
 */
function hook_islandora_ingest_post_ingest_alter(&$fedora_object) {}

/**
 * Allow output of hook_islandora_ingest_post_ingest() be altered.
 *
 * @param array $arr
 *   The array of hook output.
 */
function hook_islandora_ingest_post_ingest_output_alter(&$arr) {}

/**
 * Allows modules to add to a repository objects view/edit(/misc) process.
 *
 * If you implement this hook you must also register your module with
 * hook_islandora_hook_info().
 *
 * @param FedoraObject $fedora_object
 *   A Tuque FedoraObject.
 *
 * @return array|null
 *   An associative array with 'deleted' mapped to TRUE--indicating that the
 *   object should just be marked as deleted, instead of actually being
 *   purged--or NULL/no return if we just need to do something before the
 *   is purged.
 */
function hook_islandora_pre_purge_object($fedora_object) {}

/**
 * Alter an object before processing in hook_islandora_pre_purge_object_alter().
 *
 * @param FedoraObject $fedora_object
 *   A Tuque FedoraObject.
 */
function hook_islandora_pre_purge_object_alter(&$fedora_object) {}

/**
 * Allow output of hook_islandora_pre_purge_object() to be altered.
 *
 * @param array $arr
 *   The array of hook output.
 */
function hook_islandora_pre_purge_object_output_alter(&$arr) {}


/**
 * Get an array of object types provided by other modules.
 *
 * @return array
 *   An associative array mapping cmodel PIDs to module names to hooks to
 *   booleans indicating that the given module's implementation of the hook
 *   should be invoked.
 */
function hook_islandora_type_info() {
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


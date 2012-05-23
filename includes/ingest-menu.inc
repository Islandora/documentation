<?php

/**
 * @file
 * Menu callback functionality for islandora/ingest
 */

/**
 * Callback function for islandora/ingest/%collection_pid
 * @TODO: validate?: pid, registry return
 * @param string $pid
 */
function islandora_ingest_callback($collection_pid) {
  $ingest_registry = module_invoke_all('islandora_ingest_registry', $collection_pid);

  $registry_count = count($ingest_registry);
  islandora_ingest_object();
//  if ($registry_count == 0) {
//    // No ingest implementations.
//    drupal_set_message(t('There are no ingest methods specified for this collection.'));
//    drupal_goto('islandora/object/' . $collection_pid);
//  }
//  elseif ($registry_count == 1) {
//    // One registry implementation, go there
//    drupal_goto($ingest_registry[0]['url']);
//  }
//  else {
//    // Multiple ingest routes registered
//    return islandora_ingest_registry_render($ingest_registry);
//  }
}

//@TODO: theme
function islandora_ingest_registry_render($ingest_registry) {
  $output = array(
    '#type' => 'markup',
    '#markup' => '',
  );
  foreach ($ingest_registry AS $ingest_route) {
    $output['#markup'] .= l($ingest_route['name'], $ingest_route['url']) . '<br/>';
  }
  return $output;
}

function islandora_ingest_object($object = NULL) {
  module_load_include('inc', 'islandora', 'RestConnection');
  include_once 'sites/all/libraries/tuque/FoxmlDocument.php';
  $user = new stdClass();
  $user->name = 'fedoraAdmin';
  $user->password = 'fedoraAdmin';
  try {
    $restConnection = new RestConnection($user);
    $object = $restConnection->repository->constructObject('test');
    $new_pid = $object->id;
    $object->label = 'foo';
    $object->state = 'a';
    $object->owner = 'woot';

    $test = $object->constructDatastream('TEST', 'M', $object, $restConnection->repository);
    $test->label = 'Test';
    $test->setContentFromFile('http://d7test/sites/default/files/coder_upgrade/theme_cache.txt');
    $test->checksumType = 'MD5';
    $test->format = 'http://www.openarchives.org/OAI/2.0/oai_dc/';
    $object->ingestDatastream($test);

    $test2 = $object->constructDatastream('TEST2', 'R', $object, $restConnection->repository);
    $test2->label = 'Test2';
    $test2->url = 'http://d7test/sites/default/files/coder_upgrade/theme_cache.txt';
    $object->ingestDatastream($test2);

    $test3 = $object->constructDatastream('TEST3', 'E', $object, $restConnection->repository);
    $test3->label = 'Test3';
    $test3->url = 'http://d7test/sites/default/files/coder_upgrade/theme_cache.txt';
    $object->ingestDatastream($test3);

    $mods = $object->constructDatastream('MODS', 'X', $object, $restConnection->repository);

    $mods_string = '<mods xmlns="http://www.loc.gov/mods/v3" ID="TopTier/Breast/">
          <titleInfo>
            <title>Selective chemical probe
                        inhibitor of Stat3, identified through structure-based virtual screening,
                        induces antitumor activity</title>
          </titleInfo>
          <name type="personal">
            <namePart type="given">K</namePart>
            <namePart type="family">Siddiquee</namePart>
            <role>
              <roleTerm authority="marcrelator" type="text">author</roleTerm>
            </role>
          </name>
        </mods>';
    $mods->label = 'MODS record';
    $mods->setContentFromString($mods_string);
    $object->ingestDatastream($mods);

    $mads = $object->constructDatastream('MADS', 'M', $object, $restConnection->repository);

    $mads_string = '<mods xmlns="http://www.loc.gov/mods/v3" ID="TopTier/Breast/">
          <titleInfo>
            <title>Selective chemical probe
                        inhibitor of Stat3, identified through structure-based virtual screening,
                        induces antitumor activity</title>
          </titleInfo>
          <name type="personal">
            <namePart type="given">K</namePart>
            <namePart type="family">Siddiquee</namePart>
            <role>
              <roleTerm authority="marcrelator" type="text">author</roleTerm>
            </role>
          </name>
        </mods>';
    $mads->label = 'MADS record';
    $mads->setContentFromString($mads_string);
    $object->ingestDatastream($mads);
//    $object->relationship = 'isConnectedTo';
    $object->collectionPid = 'islandora:root';
    $object->contentModelPid = 'islandora:collectionCModel';

//    $datastream = $object->constructDatastream('test');

    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasAwesomeness', 'jonathan:green');
//    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:model');
//    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isPage', '22', TRUE);
//    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'theawesomecollection:awesome');
//    $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:woot');
//    $object->ingestDatastream($datastream);

//    var_dump($object);

    $return = $restConnection->repository->ingestObject($object);
  } catch (Exception $e) {
    drupal_set_message(t('Error getting Islandora object %s', array('%s' => $object->id)), 'error');
    return"";
  }
}
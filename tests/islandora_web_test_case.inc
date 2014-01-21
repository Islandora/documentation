<?php

/**
 * @file
 * Defines the class IslandoraWebTestCase, which allows tests to access Fedora.
 */

class IslandoraWebTestCase extends DrupalWebTestCase {

  /**
   * An array of users that may be created over the course of a test.
   *
   * @var array
   */
  protected $users = array();

  /**
   * Sets up the Drupal filter to access this test Drupal instances database.
   *
   * @see DrupalWebTestCase::setUp()
   */
  public function setUp() {
    $args = func_get_args();
    $args = (isset($args[0]) && is_array($args[0])) ? $args[0] : $args;
    // Always enable islandora.
    $args[] = 'islandora';
    parent::setUp($args);

    // Its possible test are running before class autoloading.
    module_load_include('inc', 'islandora', 'includes/tuque');
    module_load_include('inc', 'islandora', 'includes/tuque_wrapper');

    $this->configuration = $this->getTestConfiguration();
    if ($this->configuration['use_drupal_filter']) {
      $this->backUpDrupalFilter();
      $this->setUpDrupalFilter();
    }
    $this->admin = $this->createAdminUser();
  }

  /**
   * Parses and returns the settings from the test configuration file.
   *
   * If no install specific test_config.ini file is found, it will use the
   * assumed default configs found in default.test_config.ini.
   *
   * @return array
   *   The test configuration.
   *
   * @see parse_ini_file()
   */
  protected function getTestConfiguration() {
    $path = drupal_get_path('module', 'islandora');
    if (file_exists("$path/tests/test_config.ini")) {
      $this->pass('Using custom test configuration.');
      return parse_ini_file("$path/tests/test_config.ini");
    }
    elseif (file_exists("$path/tests/default.test_config.ini")) {
      $this->pass('Using default test configuration.');
      return parse_ini_file("$path/tests/default.test_config.ini");
    }
    throw new Exception('Required default.test_config.ini/test_config.ini file not found');
  }

  /**
   * Stores the content of the Drupal Filter for later restoration.
   */
  protected function backUpDrupalFilter() {
    if (file_exists($this->configuration['drupal_filter_file'])) {
      $this->originalDrupalFilterContent = file_get_contents($this->configuration['drupal_filter_file']);
    }
    else {
      throw new Exception('Failed to find the required Drupal Filter configuration file.');
    }
  }

  /**
   * Sets up a drupal filter that can read for the tests users table.
   */
  protected function setUpDrupalFilter() {
    $connection_info = Database::getConnectionInfo('default');
    $drupal_filter_dom = new DomDocument();
    $drupal_filter_dom->loadXML($this->originalDrupalFilterContent);
    $server = $connection_info['default']['host'];
    $dbname = $connection_info['default']['database'];
    $user = $connection_info['default']['username'];
    $password = $connection_info['default']['password'];
    $port = $connection_info['default']['port'] ? $connection_info['default']['port'] : '3306';
    $prefix = $connection_info['default']['prefix']['default'];
    $filter_drupal_connection_node = $drupal_filter_dom->getElementsByTagName('FilterDrupal_Connection')->item(0);
    $first_connection_node = $drupal_filter_dom->getElementsByTagName('connection')->item(0);
    $connection_node = $filter_drupal_connection_node->insertBefore($drupal_filter_dom->createElement('connection'), $first_connection_node);
    $connection_node->setAttributeNode(new DOMAttr('server', $server));
    $connection_node->setAttributeNode(new DOMAttr('dbname', $dbname));
    $connection_node->setAttributeNode(new DOMAttr('user', $user));
    $connection_node->setAttributeNode(new DOMAttr('password', $password));
    $connection_node->setAttributeNode(new DOMAttr('port', $port));
    $sql_node = $connection_node->appendChild(new DOMElement('sql'));
    $sql_node->appendChild($drupal_filter_dom->createTextNode("SELECT DISTINCT u.uid AS userid, u.name AS Name, u.pass AS Pass, r.name AS Role FROM ({$prefix}users u LEFT JOIN {$prefix}users_roles ON u.uid={$prefix}users_roles.uid) LEFT JOIN {$prefix}role r ON r.rid={$prefix}users_roles.rid WHERE u.name=? AND u.pass=?;"));
    file_put_contents($this->configuration['drupal_filter_file'], $drupal_filter_dom->saveXML());
  }

  /**
   * Creates the a full fedora admin user with a repository connection.
   */
  protected function createAdminUser() {
    $roles = user_roles();
    $index = array_search('administrator', $roles);
    $user = $this->drupalCreateUser();
    $user->roles[$index] = 'administrator';
    $user->name = $this->configuration['admin_user'];
    $user->pass = $this->configuration['admin_pass'];
    $user = user_save($user);
    $url = variable_get('islandora_base_url', $this->configuration['fedora_url']);
    $connection = islandora_get_tuque_connection($user, $url);
    $user->repository = $connection->repository;
    return $user;
  }

  /**
   * Logs in the given user, handles the special case where the user is admin.
   *
   * @see DrupalWebTestCase::drupalLogin()
   */
  protected function drupalLogin(stdClass $account) {
    if ($account->uid == $this->admin->uid) {
      // Create password for Drupal.
      $edit = array('pass' => user_password());
      $account = user_save($account, $edit);
      // Raw password is used to login.
      $account->pass_raw = $edit['pass'];
      // We must login before changing the password for fedora.
      parent::drupalLogin($account);
      $account->name = $this->configuration['admin_user'];
      $account->pass = $this->configuration['admin_pass'];
      // Save the fedora admin credentials for later GET/POST requests.
      $account = user_save($account);
    }
    else {
      parent::drupalLogin($account);
      $this->users[] = $account->name;
    }
  }


  /**
   * Stores the content of the Drupal Filter for later restoration.
   */
  protected function restoreDrupalFilter() {
    $file = $this->configuration['drupal_filter_file'];
    if (isset($this->originalDrupalFilterContent)) {
      file_put_contents($file, $this->originalDrupalFilterContent);
    }
    elseif (file_exists($file)) {
      // Remove if there was never an original.
      drupal_unlink($file);
    }
  }

  /**
   * Restores the original Drupal filter, frees any allocated resources.
   *
   * @see DrupalWebTestCase::tearDown()
   */
  public function tearDown() {
    foreach ($this->users as $user) {
      $this->deleteUserCreatedObjects($user);
    }
    if ($this->configuration['use_drupal_filter']) {
      $this->restoreDrupalFilter();
    }
    unset($this->admin);
    unset($this->configuration);
    parent::tearDown();
  }

  /**
   * Asserts that the given datastreams exist correctly on the object.
   *
   * @param AbstractObject $object
   *   The PID of the object
   * @param array $datastreams
   *   An array of strings containing datastream names
   *
   * @return bool
   *   TRUE on success, FALSE on fail.
   */
  public function assertDatastreams($object, array $datastreams) {
    if (!is_object($object)) {
      $this->fail("Failed. Object passed in is invalid.", 'Islandora');
    }
    else {
      $fails = FALSE;
      foreach ($datastreams as $datastream) {
        if (isset($object[$datastream])) {
          $this->pass("Loaded datastream {$datastream} from PID {$object->id}.", 'Islandora');
        }
        else {
          $this->fail("Failed to load datastream {$datastream} from PID {$object->id}.", 'Islandora');
          $fails = TRUE;
        }
      }
      return $fails;
    }
  }

  /**
   * Attempts to validate an array of datastreams, generally via binary checks.
   *
   * These functions exist in, and can be added to, datastream_validators.inc,
   * which is found in this folder.
   *
   * $param AbstractObject $object
   *   The object to load datastreams from.
   * $param array $datastreams
   *   An array of paired DSIDs, validate function names, and optional params.
   */
  public function validateDatastreams($object, array $datastreams) {
    if (!is_object($object)) {
      $this->fail("Failed. Object passed in is invalid.", 'Islandora');
    }
    module_load_include('inc', 'islandora', 'tests/datastream_validators');
    foreach ($datastreams as $datastream) {
      if (isset($object[$datastream[0]])) {
        $function = 'islandora_validate_' . $datastream[1] . '_datastream';
        if (function_exists($function)) {
          if (isset($datastream[2])) {
            $results = $function($object, $datastream[0], $datastream[2]);
          }
          else {
            $results = $function($object, $datastream[0]);
          }
          foreach ($results as $result) {
            $this->assertTrue($result[0], $result[1], 'Islandora');
          }
        }
        else {
          $this->fail("No {$datastream[0]} validation function exists for the {$datastream[1]} datastream.", 'Islandora');
        }
      }
    }
  }

  /**
   * Gets a tuque object from a path.
   *
   * @param string $path
   *   A full or partial path to an islandora object.
   *
   * @return AbstractObject
   *   The pid of the object or FALSE if a PID is not found.
   */
  public function getObjectFromPath($path) {
    $path_parts = explode('/', $path);
    $array_length = count($path_parts);
    for ($i = 0; $i < $array_length; $i++) {
      if ($path_parts[$i] == 'islandora' && isset($path_parts[$i + 1]) && $path_parts[$i + 1] == 'object') {
        if (isset($path_parts[$i + 2])) {
          return islandora_object_load(urldecode($path_parts[$i + 2]));
        }
      }
    }
    $this->fail("Failed to parse path: $path.");
    return FALSE;
  }

  /**
   * Deletes an object using the PID. This does the deletion using the UI.
   *
   * Deprecated: please use IslandoraWebTestCase::deleteTestObject() instead, as
   * it doesn't require a label, is safer, and is also not generally ridiculous.
   *
   * @param string $pid
   *   The PID of the collection to be deleted
   * @param string $button
   *   The label of the first 'Delete' button
   */
  public function deleteObject($pid, $button = NULL) {
    // Hey, good for you coming down to check this out.
    $message = islandora_deprecated('7.x-1.3', 'Use the "IslandoraWebTestCase::deleteTestObject()" function.');
    $this->assert('debug', $message, "Debug", array(
      'file' => "islandora_web_test_case.inc",
      'function' => "IslandoraWebTestCase->deleteObject()",
      'line' => '278',
    ));

    $path = 'islandora/object/' . $pid . '/manage/properties';
    $edit = array();
    if (isset($button)) {
      $this->drupalPost($path, $edit, $button);
    }
    else {
      $object = islandora_object_load($pid);
      $this->drupalPost($path, $edit, "Permanently remove '{$object->label}' from repository");
    }
    $this->drupalPost($this->url, $edit, t('Delete'));

    $this->drupalGet("islandora/object/$pid");
    $this->assertResponse(404, "Object $pid successfully deleted.");
  }

  /**
   * Deletes an object using the PID. This does the deletion using the UI.
   *
   * @param string $pid
   *   The PID of the object.
   * @param bool $safety
   *   If TRUE, this will only delete objects owned by users in $this->users.
   *
   * @return bool
   *   TRUE on success, FALSE on failure.
   */
  public function deleteTestObject($pid, $safety = TRUE) {
    $object = islandora_object_load($pid);
    if (!$safety || in_array($object->owner, $this->users)) {
      $this->drupalPost('islandora/object/' . $pid . '/delete', array(), 'Delete');
      $this->drupalGet('islandora/object/' . $pid);
      $this->assertResponse(404, "Successfully deleted object $pid", 'Islandora');
      return TRUE;
    }
    else {
      $this->fail("Cannot delete object {$pid}; it is owned by non-test user {$object->owner}, and this function was called with the safety on.");
      return FALSE;
    }
  }

  /**
   * Constructs and ingests a Fedora object and datastream(s) via tuque.
   *
   * All keys inside the parameter arrays for this function are optional. it
   * can be run simply by calling $this->ingestConstructedObject();.
   *
   * @param array $properties
   *   An array containing object information using these keys:
   *   'label' - The object label; randomized if not set.
   *   'pid' - 'namespace:pid', or just 'namespace' to generate the suffix.
   *   'models' - An array that can contain multiple content model PIDs.
   *   'owner' - The object's owner. Defaults to the currently logged-in user,
   *   if available. It is recommended to set this to a value that can be found
   *   in $this->users; otherwise, this object will have to be manually deleted.
   *   'parent' - The PID of the parent collection.
   * @param array $datastreams
   *   An array containing zero or more datastream arrays that use the keys:
   *   'dsid' - the datastream ID; randomized if not set.
   *   'path' - The path to the file to use; defaults to fixtures/test.jpg.
   *   'control_group' - The single-letter control group identifier.
   *   'mimetype' - The datastream's mimetype.
   *
   * @return bool|array
   *   FALSE if the object ingest failed, or the object array if successful.
   */
  public function ingestConstructedObject(array $properties = array(), array $datastreams = array()) {
    module_load_include('inc', 'islandora', 'includes/tuque');
    $tuque = new IslandoraTuque();
    $repository = $tuque->repository;
    if (!isset($properties['pid'])) {
      $properties['pid'] = "islandora";
    }
    $object = $repository->constructObject($properties['pid']);

    // Set the object properties before ingesting it.
    if (isset($properties['label'])) {
      $object->label = $properties['label'];
    }
    else {
      $properties['label'] = $this->randomName(16);
      $object->label = $properties['label'];
    }

    if (isset($properties['owner'])) {
      $object->owner = $properties['owner'];
    }
    elseif (is_object($this->loggedInUser)) {
      $object->owner = $this->loggedInUser->name;
    }

    if (isset($properties['models']) && is_array($properties['models'])) {
      foreach ($properties['models'] as $model) {
        $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $model);
      }
    }
    elseif (isset($properties['models']) && !is_array($properties['models'])) {
      $this->fail(t("'models' key of properties variable is not an array. Content model(s) will not be set."), 'Islandora');
    }

    $repository->ingestObject($object);
    if (!$object) {
      $this->fail(t("Failed to ingest object."), 'Islandora');
      return FALSE;
    }
    else {
      $this->pass(t("Ingested object %object", array('%object' => $object->id)), 'Islandora');
    }

    // Chuck in some datastreams.
    if (!empty($datastreams)) {
      foreach ($datastreams as $datastream) {
        if (!isset($datastream['dsid'])) {
          $datastream['dsid'] = $this->randomName(8);
        }
        if (!isset($datastream['path'])) {
          $datastream['path'] = drupal_get_path('module', 'islandora') . '/tests/fixtures/test.jpg';
        }
        if (!isset($datastream['control_group'])) {
          $new_datastream = $object->constructDatastream($datastream['dsid']);
        }
        else {
          $new_datastream = $object->constructDatastream($datastream['dsid'], $datastream['control_group']);
        }
        $new_datastream->label = $datastream['dsid'];
        if (isset($datastream['mimetype'])) {
          $new_datastream->mimetype = $datastream['mimetype'];
        }
        $new_datastream->setContentFromFile($datastream['path']);
        $object->ingestDatastream($new_datastream);
      }
    }

    // Add a parent relationship, if necessary.
    if (isset($properties['parent'])) {
      $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $properties['parent']);
    }

    return $object;
  }

  /**
   * Deletes all objects created by the given user.
   *
   * @param object $username
   *   The user whose objects we'd like to remove.
   *
   * @return bool
   *   TRUE on success, FALSE on failure.
   */
  public function deleteUserCreatedObjects($username) {
    if ($username === $this->configuration['admin_user']) {
      $this->fail("This function will under no circumstance attempt deletion of all objects owned by the configured Fedora admin user ({$this->configuration['admin_user']}), as this would irreparably damage the repository.", 'Islandora');
      return FALSE;
    }

    $query = <<<QUERY
SELECT ?object FROM <#ri> WHERE
{
  ?object <fedora-model:ownerId> "$username"
}
QUERY;

    $objects = $this->admin->repository->ri->sparqlQuery($query);
    $fail = FALSE;
    foreach ($objects as $object) {
      $loaded_object = islandora_object_load($object['object']['value']);
      islandora_delete_object($loaded_object);
      if ($this->assertFalse(islandora_object_load($object['object']['value']), "Object {$object['object']['value']} successfully removed from repository.", 'Islandora')) {
        $fail = TRUE;
      }
    }
    return $fail;
  }

}

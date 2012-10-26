<?php

/**
 * @file
 * Generic test case to be extended to implement Islandora testing.
 *
 * In order to use this properly, you must place a test user in
 * your Fedora installation.  Please add the follwing snippet to your
 * $FEDORA_HOME/server/config/fedora-users.xml:
 *
 *    <user name="simpletestuser" password="41fe63c9636c6649f0a4747400f0f95e">
 *      <attribute name="fedoraRole">
 *        <value>administrator</value>
 *      </attribute>
 *    </user>
 */
abstract class IslandoraTestCase extends DrupalWebTestCase {

  /**
   * User with the rights required to perform the test
   */
  protected $privileged_user;

  /**
   * Override this method to provide the short form name of the module,
   * e.g. the name of the .module file 
   *
   * @return string - The short form name of the module
   */
  abstract protected function getModuleShortFormName();

  /**
   * Returns basic permissions needed for running Islandora tests.
   * Override this method to provide additional permissions, but 
   * be sure to append your new permissions to the return value
   * of parent::getUserPermissions and return that.
   *
   * @return array - An array of strings describing permissions
   */
  protected function getUserPermissions() {
    return array(
      'access content',
      'add fedora datastreams',
      'edit fedora meta data',
      'edit tags datastream',
      'ingest new fedora objects',
      'purge objects and datastreams',
      'view fedora collection',
      'view detailed list of content',
    );
  }

  /**
   * Creates a user with permissions required for the test, and logs in.
   */
  protected function createPrivilegedUser() {
    // Create a role with the permissions from getUserPermissions()
    $role = $this->drupalCreateRole($this->getUserPermissions());
  
    // Create a user model 
    $user = array(
      'name' => 'simpletestuser',
      'mail' => 'simpletestuser@example.com',
      'roles' => array( $role => $role ),
      'pass' => 'simpletestpass',
      'status' => 1,
    );

    // Create a user from the model
    $this->privileged_user = user_save('', $user);

    // Add the raw password so we can log in
    $this->privileged_user->pass_raw = $user['pass'];

    // Log in
    !$this->drupalLogin($this->privileged_user); 
  }

  /**
   * Automatically generates all module dependencies and enables them in order.
   * Also logs in a privileged user with the appropriate permissions for the
   * test.
   *
   * If you need to override this method to provide extra functionality,
   * please be sure to call parent::setUp so dependency resolution and 
   * authentication still happen.
   */
  public function setUp() {    
    // Grab all the available modules
    $modules = module_rebuild_cache();

    // Grab the module to test's dependencies
    $dependencies = $modules[$this->getModuleShortFormName()]->info['dependencies'];

    // Sort them so they're in the correct order to enable
    $dependencies = array_reverse($dependencies);

    // Add our module to the end
    $dependencies[] = $this->getModuleShortFormName();
    
    // Enable the module's dependencies in order
    call_user_func_array('parent::setUp', $dependencies);

    // Authenticate the privileged user
    $this->createPrivilegedUser();
  } 
}

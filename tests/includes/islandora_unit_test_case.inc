<?php
/**
 * @file
 * Islandora extensions for DrupalUnitTestCase.
 */

class IslandoraUnitTestCase extends DrupalUnitTestCase {

  /**
   * By default, deleteUserCreatedObjects() runs on each tearDown() step.
   *
   * @var bool
   */
  protected $deleteObjectsOnTeardown = TRUE;

  /**
   * An instance of IslandoraDrupalFilterManipulator carried between tests.
   *
   * @var IslandoraDrupalFilterManipulator
   */
  protected $filterManipulator;

  /**
   * Defers to IslandoraTestUtilities for missing methods.
   *
   * @param string $method
   *   The method being called.
   * @param array $args
   *   The arguments for that method.
   */
  public function __call($method, $args) {
    module_load_include('inc', 'islandora', 'tests/includes/utilities');
    $params = array('db_access' => FALSE);
    $utilities = new IslandoraTestUtilities($this->configuration, $params);
    if (!method_exists($utilities, $method)) {
      $caller = $this->getAssertionCall();
      throw new BadMethodCallException("Exception: undefined method $method in {$caller['file']}, line {$caller['line']}.");
    }
    $result = call_user_func_array(array(&$utilities, $method), $args);
    $this->parseUtilityResults($utilities);
    return $result;
  }

  /**
   * Parses utility results and passes them to the test results as an assertion.
   *
   * @param IslandoraTestUtilities $utility
   *   An instance of IslandoraTestUtilities with populated results.
   */
  public function parseUtilityResults($utility) {
    foreach ($utility->getResults() as $result) {
      $this->assert($result->getType(), $result->getMessage(), 'Islandora', $result->getCaller());
    }
  }

  /**
   * Sets up the Drupal filter to access this test Drupal instances database.
   *
   * @see DrupalWebTestCase::setUp()
   */
  public function setUp() {
    parent::setUp();

    // It's possible test are running before class autoloading.
    module_load_include('inc', 'islandora', 'includes/tuque');
    module_load_include('inc', 'islandora', 'includes/tuque_wrapper');
    module_load_include('inc', 'islandora', 'tests/includes/utilities');

    $this->configuration = islandora_get_test_configuration();
    if ($this->configuration['use_drupal_filter']) {
      $this->filterManipulator = new IslandoraDrupalFilterManipulator($this->configuration);
      $this->filterManipulator->setUpDrupalFilter();
    }

    $this->connection = new RepositoryConnection($this->configuration['fedora_url'], $this->configuration['admin_user'], $this->configuration['admin_pass']);
    $api = new FedoraApi($this->connection);
    $this->repository = new FedoraRepository($api, new SimpleCache());
  }

  /**
   * Restores the original Drupal filter, frees any allocated resources.
   *
   * To safeguard against leaving test objects in the repository, tearDown()
   * calls deleteUserCreatedObjects() every time by default. This feature can be
   * toggled by setting $this->deleteObjectsOnTeardown to TRUE or FALSE.
   *
   * @see DrupalWebTestCase::tearDown()
   */
  public function tearDown() {
    if ($this->configuration['use_drupal_filter']) {
      $this->filterManipulator->restoreDrupalFilter();
    }
    unset($this->configuration);
    parent::tearDown();
  }

}

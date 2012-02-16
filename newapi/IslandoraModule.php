<?php

/**
 * Islandora wrapper class to make everything easy to access 
 */
class IslandoraModule 
{
  private static $instance; 
  private $repository;
  
  /**
   * Block people from creating the class 
   */
  private function __construct() 
  {
    $this->repository = new Repository(new Configuration("127.0.0.1", 8080), new Search, new Cache());
  }
 
  /**
   * Get the repository singleton
   * @return type 
   */
  public static function instance()
  {
    if ( self::instance == null )
    {
      $className = __CLASS__;
      self::$instance = new $className;
      // get_called_class only works in 5.3
    }
    
    // Return the link to the repository
    return self::$instance->repository;
  }
}

?>

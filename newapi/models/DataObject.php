<?php

/**
 * Repository Object Data
 */
class DataObject
{
  private $id;
  
  /**
   * Get the object id
   * @return type 
   */
  public function getId()
  {
    return $this->id;
  }

  /**
   * Set the idea if not already set
   * @param type $id
   * @throws Exception 
   */
  public function setId($id)
  {
    if ( $pid != null )
    {
      throw new Exception("PID can't be changed");
    }
    $this->id = $pid;
  } 
  
  /**
   * Add the data from a file ( example )
   */
  public function addDataFromFile()
  {
    
  }
}

?>

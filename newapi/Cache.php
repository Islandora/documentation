<?php

/**
 * Hashtable cache ( nothing special ) 
 * Should be replaced with something like 
 */
class Cache 
{
  private $objectList = array();
  
  /**
   * Add an object to the cache
   * @param ObjectModel $object 
   */
  public function addObject(ObjectModel &$object)
  {
    if ( !isset($this->objectList[$object->getId()]))
    {
      $this->objectList[$object->getId()] = array();
    }
    
    $this->objectList[$object->getId()]['checksum'] = sha1(serialize($object));
    $this->objectList[$object->getId()]['object'] = $object;
  }
  
  /**
   * Check to see if the object has been updated
   * @param ObjectModel $object 
   */
  public function hasChanged(ObjectModel &$object)
  {
    // Is the object even cached
    if ( !isset($this->objectList[$object->getId()]))
    {
      return false;
    }
    
    // Does it have the same checksum
    if ($this->objectList[$object->getId()]['checksum'] == sha1(serialize($object)) )
    {
      return true;
    }
    
    // If they were the same then it would have already exited
    return false;
  }     
  
  /**
   * Get object from the cache. Returns null if not found.
   * @param type $id 
   */
  public function getObject($id)
  {
    // Is the object cached
    if ( isset($this->objectList[$id]['object'] ) ) {
      
      // Return the object from the cache
      return $this->objectList[$id]['object'];
    }
    
    // Object wasn't found so return null
    return null;
  }
  
  /**
   * Remove the object from the cache
   * @param type $id 
   */
  public function deleteObject($id)
  {
    // Unset it from the array
    unset( $this->objectList[$id]);
  }
}

?>

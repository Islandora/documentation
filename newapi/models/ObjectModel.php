<?php

/**
 * Repository Object 
 * 
 * @todo Create an iterator for the datas
 */
class ObjectModel 
{
  private $id;
  private $label;
  private $status;
  private $dataObjects = array();
  
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
    if ( $id != null )
    {
      throw new Exception("ID can't be changed");
    }
    $this->id = $id;
  }
  
  /**
   * Get the object label
   * @return type 
   */
  public function getLabel()
  {
    return $this->label;
  }

  /**
   * Set the label
   * @param type $label 
   */
  public function setLabel($label)
  {
    $this->label = $label;
  }

  /**
   * Get the object status
   * @return type 
   */
  public function getStatus()
  {
    return $this->status;
  }
  
  /**
   * Set the status
   * @param type $status 
   */
  public function setStatus($status)
  {
    $this->label = $status;
  }
  
  /**
   * Get a data using the id
   * @param type $id
   * @return null 
   */
  public function getData($id)
  {
    foreach($this->getAllDataObjects() as $data)
    {
      if ($data->getId() == $id)
      {
        return $data;
      }
    }
    return null;
  }
  
  /**
   * Get all the data object
   * @return type 
   */
  public function getAllDataObjects()
  {
    return $this->dataObjects;
  }
  
  /**
   * Add a data object
   * @param type $data 
   */
  public function addData(DataObject &$data)
  {
    $this->dataObjects[$data->getId()] = $data;
  }
  
  /**
   * Delete a data object
   * @param type $data 
   */
  public function deleteData(DataObject &$data)
  {
    unset( $this->datas[$data->getId()] );
  }
}

?>

<?php

/**
 * Subject Predicate Object 
 * I.E. This, has, that 
 */
class SPO 
{
  private $subject;
  private $predicate;
  private $object; 
  
  public function __construct($subject, $predicate, $object) 
  {
    $this->subject = $subject;
    $this->predicate = $predicate;
    $this->object = $object;
  }
  
  /**
   * Get the subject
   * @return type 
   */
  public function getSubject()
  {
    return $this->subject;
  }
  
  /**
   * Get the predicate
   * @return type 
   */
  public function getPredicate()
  {
    return $this->predicate;
  }
  
  /**
   * Get the object
   * @return type 
   */
  public function getObject()
  {
    return $this->object;
  }
  
  /**
   * Set the subject
   * @param type $value 
   */
  public function setSubject($value)
  {
    $this->subject = $value;
  }
  
  /**
   * Set the predicate
   * @param type $value 
   */
  public function setPredicate($value)
  {
    $this->predicate = $value;
  }
  
  /**
   * Set the object
   * @param type $value 
   */
  public function setObject($value)
  {
    $this->object = $value;
  }
}

?>

<?php

/**
 * Basic config should be replaced with a specific implementation 
 */
class Configuration 
{
  private $baseUrl;
  private $port;
  
  /**
   * Default constructor
   * @param type $base_url
   * @param type $port 
   */
  public function __construct($base_url, $port) {      
    $this->$baseUrl = $base_url;
    $this->$port = $port;
  }
  
  /**
   * Get the base url
   * @return type 
   */
  public function getBaseURL()
  {
    return $this->$baseUrl;
  }
  
  /**
   * Set the base url
   * @param type $url 
   */
  public function setBaseURL($url)
  {
    $this->$baseUrl = $url;
  }
  
  /**
   * Get the port
   * @return type 
   */
  public function getPort()
  {
    return $this->$port;
  }
  
  /**
   * Set the port
   * @param type $port 
   */
  public function setPort($port)
  {
    $this->$port = $port;
  }
}

?>

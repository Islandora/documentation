<?php

/**
 * Fedora repository
 */
class Repository 
{
  private $config;
  private $search;
  private $cache;
  
  public function __construct(Configuration &$config, Search &$search, Cache &$cache) 
  {
    // Store all the dependencies
    $this->setConfig($config);
    $this->setSearch($search);
    $this->setCache($cache);
  }
  
  /**
   * Get the configuration implementation
   * @return type 
   */
  public function getConfig()
  {
    return $this->config;
  }
  
  /**
   * Get the search implementation
   * @return type 
   */
  public function getSearch()
  {
    return $this->search;
  }
  
  /**
   * Get the cache implementation
   * @return type 
   */
  public function getCache()
  {
    return $this->cache;
  }
  
  /**
   * Set the configuration implementation
   * @param Configuration $config 
   */
  public function setConfig(Configuration &$config)
  {
    if ( $config == null )
    {
      throw new Exception("Config implementation can't be null");
    }
    $this->config = $config;
  }
  
  /**
   * Set the search implementation
   * @param Search $search 
   */
  public function setSearch(Search &$search)
  {
    if ( $search == null )
    {
      throw new Exception("Search implementation can't be null");
    }
    $this->search = $search;
  }

  /**
   * Set the cache implementation
   * @param Cache $cache 
   */
  public function setCache(Cache &$cache)
  {
    if ( $cache == null )
    {
      throw new Exception("Cache implementation can't be null");
    }
    $this->cache = $cache;
  }
  
  /**
   * Get the object from the repo
   * @param type $id 
   */
  public function loadObject($id, $cache=true)
  {
    // Check to see if its already cached
    if ( $this->getCache()->getObject( $id ) && $cache = true)
    {
      // Return the cached object
      return $this->getCache()->getObject( $id );
    }
    
    // Create the request
    $results = $this->makeRequest( '/objects/' .$id );
    
    // Return the object model
    return unserialize($results);
  }
  
  /**
   * Save an object to the repository
   * @param ObjectModel $model
   * @param type $force 
   */
  public function saveObject(ObjectModel $model, $force=false)
  {
    // Has the object been created at all
    if ( $model->getId() == null ) {
     
      // Get the next free persistent id
      $id = $this->makeRequest( '/objects/nextPID' );
    
      // Set the id on the model
      $model->setId($id);
      
    }
    
    // If it hasn't changed then done't save unless it's forced
    if ( ! $force && ! $this->getCache()->hasChanged($model) )
    {
      return;
    }
    
    // Add the object to the cache so everybody has the new copy
    $this->getCache()->addObject($model);
    
    // Post the serialized model to the object endpoint
    $this->makeRequest('/objects', $this->serialize($model));
  }
    
  /**
   * Find an object using a search term
   * @param type $term
   * @return type 
   */
  public function findObjectByTerm($term)
  {
    // Create results
    $results = $this->makeRequest('/objects?terms=' . $term);
    
    // Do something with results
    return $results;
  }
  
  /**
   * Find an object with a query
   * @param type $query
   * @return type 
   */
  public function findObjectWithQuery($query)
  {
    // Create results
    $results = $this->makeRequest('/objects?query=' . $term);
  
    // Do something with results
    return $results;
  }

  /**
   * Search the repository using either a SPO or an array of SPOs
   * @param type $query 
   */
  public function SearchSPO($SPO)
  {
    // Search 
    $results = $this->getSearch()->SearchSPO($SPO);
    
    // Do something with the results
    return $results;
  }
  
  /**
   * Unserialize the object from foxml
   * @param type $xml
   * @return \ObjectModel 
   */
  protected function unserialize($xml)
  {
    // Create the object model
    $model = new ObjectModel();
    
    // Do something with the xml
    $xml = $xml; 
    
    // Return the model;
    return $model; 
  }
  
  /**
   * Serialize the object to foxml
   * @param ObjectModel $model
   * @return string 
   */
  protected function serialize(ObjectModel $model)
  {
    // Do something with the model
    serialize($model);
    
    // return a string
    return "";
  }

  /**
   * Make a request
   * @param type $request
   * @return type 
   */
  private function makeRequest($request, array $postData = null, $responseCode=200, $format="xml")
  {
    // Check to see if we already have parameters
    $pos = strpos($request, "?");
    if ($pos === false) {
      $request.="?format=".$format;
    } else {
      $request.="&format=".$format;
    }

    // Initialize Curl
    $curl = curl_init();
    
    // Set all the options
    curl_setopt($curl, CURLOPT_URL, $this->getConfig()->getBaseURL() . $request); 
    curl_setopt($curl, CURLOPT_PORT , $this->getConfig()->getPort() ); 
    curl_setopt($curl, CURLOPT_VERBOSE, 1);
    // If we have post data then append that
    if ( $postData ) {
      curl_setopt($curl, CURLOPT_POSTFIELDS, $postData);
    }

    // Execute the curl call
    $results = curl_exec($curl);
    
    // Check for an error
    if( ! curl_errno($curl) ) {
      
      // Get information regarding the curl connection
      $info = curl_getinfo($curl);
      
      // Check the response code
      if ( $info['http_code'] != $responseCode )
      {
        var_dump($results);
        var_dump($info);
        throw new Exception("Curl request failed");
      } else {
        return $results;
      }
    }
    
    // Close connection
    curl_close($curl); 
    
  } // Close makeRequest

} 


?>

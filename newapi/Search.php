<?php


/**
 * Basic search that uses RI but could be replaced with solr 
 */
class Search 
{
  /**
   * Search the repository using either a SPO or an array of SPOs
   * @param type $query 
   */
  public function SearchSPO($SPO)
  {
    $queryString = "";
    if (is_array($SPO))
    {
      foreach($SPO as $spo)
      {
        $queryString .= $spo->getSubject() . " " . $spo->getPredicate() . " " . $spo->getObject() . ", ";
      }
      // Strip off the extra comma
      $queryString = substr($queryString, '', -2);
    } 
    else {
      $queryString .= $SPO->getSubject() . " " . $SPO->getPredicate() . " " . $SPO->getObject();
    }
   
    // Do a search
    $results = "";
    
    // Return results;
    return $results;
  }
}

?>

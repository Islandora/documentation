<?
// $Id$

/* 
 * @file fesl_block.inc
 */

function _fedora_fesl_block_content() {
  module_load_include('inc', 'fedora_repository', 'CollectionClass');
  module_load_include('inc', 'fedora_repository', 'ObjectHelper');
  global $user;
  if ($user != null && $user->uid != 0) { 
    $roles = $user->roles;

    $write = array(); $read = array();
    $content ='';
    foreach ($roles as $role) {
      if (preg_match('/^(([A-Za-z0-9]|-|\.)+:(([A-Za-z0-9])|-|\.|~|_|(%[0-9A-F]{2}))+)\s+(write|read)/',$role,$matches)) {
      $pid = $matches[1]; 
      $perm = $matches[6];
      
      switch (strtolower($perm))
      {
	case 'write':
	  $write[]='$object <dc:identifier> \''.$pid.'\'';
	  break;
	  
	case 'read':
	  $read[]=$pid;
	  break;
      }
      
      }
    }

    $content .= t('This page lists the collections which your user has special access to.  You may add, edit, or delete from any collections listed under "Write Access".  You have full read access to any collections listed under "Read Access".   The permissions listed here also extend to any child collections within the collections listed.'); 
    $content .= '<h3>Write Access</h3>';
    if (count($write) > 0) {
      $itqlquery = 'select $object $title from <#ri>
    where ('.join(' or ', $write).')
    and $object <dc:title> $title';

      $content .= _fedora_fesl_block_getList($itqlquery);
    } else if (in_array('administrator',$user->roles)) {
      $content .= t('You are an administrator with write access to everything.');
    } else {
      $content .= t('You do not have write access to any collections.');
    }
    $content .= '<h3>Read Access</h3>';
    if (count($read) > 0) { 
      $itqlquery = 'select $object $title from <#ri>
    where ('.join(' or ', $read).')
    and $object <dc:title> $title';

      $content .= _fedora_fesl_block_getList($itqlquery); 
    } else {
      $content .= t('You do not have read access to any non-public collections.');
    }

    return $content;
  }

}

function _fedora_fesl_block_getList($itqlquery)
{
  global $base_url;
  $collection = new CollectionClass(null);
  $results = $collection->getRelatedItems(null, $itqlquery);
  
  $resultsdoc = new DomDocument();
  $resultsdoc->loadXML($results);
  
  $resultslist = $resultsdoc->getElementsByTagName('result');
  if ( $resultslist->length == 0 ) {
    return '';
  }

  $content = '';
  
  for ($i=0;$i<$resultslist->length;$i++) {
    $randresult=$resultslist->item($i);

    $objs = $randresult->getElementsByTagName('object');
    $obj = $objs->item(0);
    
    $pid = substr( $obj->getAttribute('uri'), 12);
    $titles = $randresult->getElementsByTagName('title');
    $title = $titles->item(0);

    $content .= '<a href="'.$base_url.'/fedora/repository/'.$pid.'"/>'.$title->nodeValue.'</a><br/>';
  }
  
  return $content;
}

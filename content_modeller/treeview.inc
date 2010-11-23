<?php

// $Id$

class treeview {
  private $type;
  private $content = '';
  private $icons = '';
  private $id = NULL;
  private $children = array();
  
  function __construct($content, $type = NULL, $icons = NULL, $id = NULL) {
    $this->type = $type;
    $this->content = $content;
    $this->id = $id;
    $this->icons = $icons;
  }
  
  function buildTree($class = NULL, $includeul = TRUE) {
    $ret = '';
    if ($includeul)
      $ret .= '<ul ' . ($this->id != NULL ? 'id="'. $this->id .'"' : '') . ($class != NULL ? 'class="'. $class .'"' : '') .'>';
    $ret .= '<li>';
    if ($this->type != NULL) {
      $ret .='<span class="'. $this->type .'">';
    }
    $ret .= $this->content;
    if ($this->type != NULL) {
      $ret .= '</span>';
    }
   
    $ret .= $this->icons;
   
    if (count($this->children) > 0) {
      if ($includeul)
        $ret .= '<ul>';
      else
        $ret .= '<ul '. ($this->id != NULL ? 'id="' . $this->id .'"' : '') .'>';
        foreach ($this->children as $tree) {
          $ret .= $tree->buildTree(NULL, FALSE);
        }
        $ret .= '</ul>';
    }
    $ret .= '</li>';
    if ($includeul)
      $ret .= '</ul>';
      
    return $ret;
  }
  
  function addChild($content, $type = 'file', $icons = NULL, $id = NULL) {
    $tree = (is_object($content) && get_class($content) == 'treeview') ? $content : new treeview($content, $type, $icons, $id);
    $this->children[] = $tree;
    return $tree;
  }
}

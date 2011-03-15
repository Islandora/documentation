<?php

// $Id$

class Newspaper {

  function  __construct($pid = '') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {

      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }


  public function showFieldSets() {

    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );

    global $user;
    $qs = '';
    if ($user->uid != 0) {
      $qs = '?uid='. base64_encode($user->name . ':'. $user->pass);
    }

    $viewer_url = variable_get('fedora_base_url', '') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer'. $qs;
    $html = '<iframe src="'. $viewer_url . '"  scrolling="no" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';

    $tabset['my_tabset']['first_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Read'),
      '#content' => $html
    );

    $item = new Fedora_Item($this->pid);
    
    $tabset['my_tabset']['second_tab'] = array(
      '#type' => 'tabpage',
      '#title' => 'Description',
      '#content' => $item->get_dissemination('islandora:mods2htmlSdef', 'mods2html'),
    );

    return tabs_render($tabset);
  }

  public function showPageFieldSets() {

    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );

    global $user;
    $qs = '';
    if ($user->uid != 0) {
      $qs = '?uid='. base64_encode($user->name . ':'. $user->pass);
    }

    $viewer_url = variable_get('fedora_base_url', '') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer'. $qs;
    $html = '<iframe src="'. $viewer_url . '" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';

    $tabset['my_tabset']['first_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Read'),
      '#content' => $html
    );

    $item = new Fedora_Item($this->pid);

    // Get this page's parent item to show the issue's metadata.
    $rels = $item->get_relationships();
    $parent_pid = '';
    foreach ($rels as $rel) {
      if (strstr($rel['predicate'], 'isPartOf')) {
        $parent_pid = $rel['object'];
        break;
      }      
    }    
    $parent_item = new Fedora_Item($parent_pid);
    $tabset['my_tabset']['second_tab'] = array(
      '#type' => 'tabpage',
      '#title' => 'Description',
      '#content' => $parent_item->get_dissemination('islandora:mods2htmlSdef', 'mods2html'),
    );

    return tabs_render($tabset);
  }
}

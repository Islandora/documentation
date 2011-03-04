<?php
// $Id$

class ShowSlideStreamsInFieldSets {

  private $pid = NULL;

  function ShowSlideStreamsInFieldSets($pid) {
    //drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $this->pid = $pid;
  }
  
  function showJPG() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/ShowStreamsInFieldSets');
    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );
    
    global $user;
    $qs = '';
    if ($user->uid != 0) {
//      $qs = '?uid=' . base64_encode($user->name . ':' . $user->sid);
      $qs = '?uid=' . base64_encode($user->name . ':' . $user->pass);
    }

    $viewer_url = variable_get('fedora_base_url', 'http://localhost:8080/fedora') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer'. $qs;
    $html = '<iframe src="'. $viewer_url . '" scrolling="no" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';

    drupal_add_css(path_to_theme() . '/header-viewer.css', 'theme');

    $tabset['my_tabset']['second_tab'] = array(
//     $collection_fieldset = array (
      '#type' => 'tabpage',
      '#title' => t('Full-size'),
      '#content' => $html);

    $tabset['my_tabset']['first_tab'] = array(
      // #type and #title are the minimum requirements.
      '#type' => 'tabpage',
      '#title' => t('View'),
      // This will be the content of the tab.
      '#content' => '<img src="'. $base_url .
        '/fedora/imageapi/'. $this->pid . '/JPG/JPG.jpg'. '" />'. '<p>'. drupal_get_form('fedora_repository_image_tagging_form', $this->pid) . '</p>',
    );
    
    $ssifs = new ShowStreamsInFieldSets($this->pid);
    $tabset['my_tabset']['third_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
      '#content' => $ssifs->showQdc(),
    );
    // Render the tabset.
    return tabs_render($tabset);

    return theme('fieldset', $collection_fieldset);
//      . (user_access('add fedora datastreams') ? drupal_get_form('fedora_ilives_image_tagging_form', $this->pid) : '');
  }
}

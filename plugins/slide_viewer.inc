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
      '#content' => '<a href="'. $base_url . '/fedora/repository/'. $this->pid . '/FULL_JPG/"><img src="'. $base_url .
        '/fedora/imageapi/'. $this->pid . '/JPG/JPG.jpg'. '" /></a>'. '<p>'. drupal_get_form('fedora_repository_image_tagging_form', $this->pid) . '</p>',
    );
    
    $ssifs = new ShowStreamsInFieldSets($this->pid);
    $tabset['my_tabset']['third_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
    );
    
    $tabset['my_tabset']['third_tab']['tabset'] = array(
      '#type' => 'tabset',
    );
    $objectHelper = new ObjectHelper($this->pid);
    $item = new Fedora_Item($this->pid);
    $dc_html = $objectHelper->getFormattedDC($item);
    
    $ds_list = $objectHelper->getFormattedDatastreamList($this->pid, NULL, $item);
    
    $tabset['my_tabset']['third_tab']['tabset']['view'] = array(
      '#type' => 'tabpage',
      '#title' => t('View'),
      '#content' => $dc_html . $ds_list,
    );
    
    if (fedora_repository_access(OBJECTHELPER :: $EDIT_FEDORA_METADATA, $this->pid, $user)) {
      $editform = drupal_get_form('fedora_repository_edit_qdc_form', $this->pid, 'DC');
      $tabset['my_tabset']['third_tab']['tabset']['edit'] = array(
        '#type' => 'tabpage',
        '#title' => t('Edit'),
        '#content' => $editform,
      );
    }
    
    
    // Render the tabset.
    return tabs_render($tabset);

    return theme('fieldset', $collection_fieldset);
//      . (user_access('add fedora datastreams') ? drupal_get_form('fedora_ilives_image_tagging_form', $this->pid) : '');
  }
}

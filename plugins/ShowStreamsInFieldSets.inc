<?php
// $Id$

/*
 * Created on 17-Apr-08
 */
class ShowStreamsInFieldSets {
  private $pid = NULL;
  function ShowStreamsInFieldSets($pid) {
    $this->pid = $pid;
  }

  function showFlv() {
    //FLV is the datastream id
    $path = drupal_get_path('module', 'Fedora_Repository');
    $fullPath = base_path() . $path;
    $content = "";
    $pathTojs = drupal_get_path('module', 'Fedora_Repository') . '/js/swfobject.js';
    drupal_add_js("$pathTojs");
    $content .= '<div id="player'. $this->pid . 'FLV"><a href="http://www.macromedia.com/go/getflashplayer">Get the Flash Player</a> to see this player.</div>';
    drupal_add_js('var s1 = new SWFObject("'. $fullPath . '/flash/flvplayer.swf","single","320","240","7");
    s1.addParam("allowfullscreen","TRUE");
    s1.addVariable("file","'. base_path() . 'fedora/repository/'. $this->pid . '/FLV/FLV.flv");
    s1.write("player'. $this->pid . 'FLV");', 'inline', 'footer');
    $collection_fieldset = array(
      '#title' => t('Flash Video'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#value' => $content);
    return theme('fieldset', $collection_fieldset);
  }

  function showTN() {
    global $base_url;
    $collection_fieldset = array(
      '#title' => '',
      '#attributes' => array(),
      '#collapsible' => FALSE,
      '#value' => '<a href="'. $base_url . '/fedora/repository/'. $this->pid . '/OBJ/"><img src="'. $base_url . '/fedora/repository/'. $this->pid . '/TN/TN'.'" /></a>',
    );
    return theme('fieldset', $collection_fieldset);
  }


  // Same as showTN but artinventory stores the image in a dsid of IMAGE instead of OBJ
  function showArtInventoryTN() {
    global $base_url;
    $collection_fieldset = array(
      '#collapsible' => FALSE,
      '#value' => '<a href="'. $base_url . '/fedora/repository/'. $this->pid . '/IMAGE/image.jpg"><img src="'. $base_url . '/fedora/repository/'. $this->pid . '/TN/TN'. '" /></a>',
    );
    return theme('fieldset', $collection_fieldset);
  }

  /**
   * Embed Google Docs' PDF viewer into the page.
   */
  function showPDFPreview() {
    global $base_url;
    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );
    $tabset['my_tabset']['first_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
      '#content' => $this->showQdc(),
    );

    $tabset['my_tabset']['second_tab'] = array(
//     $collection_fieldset = array (
      '#type' => 'tabpage',
      '#title' => t('Read Online'),
      '#content' => "<iframe src=\"http://docs.google.com/viewer?url=". $base_url . '/fedora/repository/'. 
        $this->pid . '/OBJ/preview.pdf'. "&embedded=TRUE\" style=\"width:600px; height:500px;\" frameborder=\"0\"></iframe>"
    );

    // Render the tabset.
    return tabs_render($tabset);

    return theme('fieldset', $collection_fieldset);
//      . (user_access('add fedora datastreams') ? drupal_get_form('fedora_ilives_image_tagging_form', $this->pid) : '');
  }


  function showQdc() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $objectHelper = new ObjectHelper();
    $content=$objectHelper->getQDC($this->pid);
    $collection_fieldset = array(
      '#title' => t('Description'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#value' => $content,
    );
    return theme('fieldset', $collection_fieldset);
  }

  function showOBJLink() {
    global $base_url;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new Fedora_Item($this->pid);
    $streams = $item->get_datastreams_list_as_array();
    return "<a href='". $base_url ."/fedora/repository/". $this->pid ."/OBJ/". $streams['OBJ']['label'] ."'>". $streams['OBJ']['label'] ."</a>";
  }



  function showRefworks() {
    $path=drupal_get_path('module', 'fedora_repository');
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $collectionHelper = new CollectionClass();
    $xmlstr=$collectionHelper->getStream($this->pid, "refworks");
    html_entity_decode($xmlstr);
    if ($xmlstr == NULL || strlen($xmlstr) < 5) {
      return " ";
    }
    try {
      $proc = new XsltProcessor();
    }
    catch (Exception $e) {
      drupal_set_message(t("!e", array('!e' => $e->getMessage())), 'error');
      return " ";
    }
    $xsl = new DomDocument();
    $xsl->load($path . '/xsl/refworks.xsl');
    $input = new DomDocument();
    $input->loadXML(trim($xmlstr));
    $xsl = $proc->importStylesheet($xsl);
    $newdom = $proc->transformToDoc($input);
    $content = $newdom->saveXML();

    $collection_fieldset = array(
      '#title' => t('Refworks'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#value' => $content);
    return theme('fieldset', $collection_fieldset);
  }

  function showJP2($collapsed = FALSE) {
    $viewer_url = variable_get('fedora_base_url', '') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer';
    $html = '<iframe src="'. $viewer_url . '" frameborder="0" style="width: 100%; height: 400px;">Errors: unable to load viewer</iframe>';
    $fieldset = array(
      '#title' => t('Viewer'),
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
      '#value' => $html
    );
    return theme('fieldset', $fieldset);
  }

  function showRomeo($collapsed = FALSE) {
    $path = drupal_get_path('module', 'Fedora_Repository');
    module_load_include('inc', 'fedora_repository', 'CollectionClass');
    $collectionHelper = new CollectionClass();
    $xmlstr = $collectionHelper->getStream($this->pid, "ROMEO", 0);

    if ($xmlstr == NULL || strlen($xmlstr) < 5) {
      return " ";
    }

    try {
      $proc = new XsltProcessor();
    }
    catch (Exception $e) {
      drupal_set_message(t("!e", array('!e' => $e->getMessage())), 'error');
      return;
    }
    $xsl = new DomDocument();
    $xsl->load($path . '/xsl/romeo.xsl');
    $input = new DomDocument();
    $input->loadXML(trim($xmlstr));
    $xsl = $proc->importStylesheet($xsl);
    $newdom = $proc->transformToDoc($input);
    $content = $newdom->saveXML();

    $collection_fieldset = array(
      '#title' => t('Romeo'),
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
      '#value' => $content
    );
    return theme('fieldset', $collection_fieldset);
  }
}

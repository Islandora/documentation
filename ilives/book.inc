<?php

// $Id$

class IslandoraBook {

  function  __construct($pid = '') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {

      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }

  public function buildDrupalForm($form = array(), $ingest_form = array(), &$form_state = array()) {
    $mods_save = '';
    if (isset($form_state['mods_save'])) {
      $mods_save = $form_state['mods_save'];
    }

    // Set #cache to true to create the $form_state cache
    $form['#cache'] = TRUE;
    
    // Give the user an option to enter a custom PID
    $form['custom_pid'] = array(
      '#type' => 'textfield',
      '#title' => 'Custom PID',
      '#description' => 'If you want to manually specify the PID for the new object, enter it here. '.
                        'Leave it blank for an automatically-generated PID.',
    );
    
    // Prompt the user to enter a record ID to be looked up in Evergreen.
    $form['unapi_url'] = array(
      '#type' => 'textfield',
      '#title' => 'Catalogue item URL',
    );
    $form['unapi_url_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Retrieve MODS record'),
      '#submit' => array('fedora_ilives_retrieve_unapi_submit'),
      '#ahah' => array(
        'path' => 'fedora/ilives/retrieve_unapi/js', // path we defined in hook_menu
        'wrapper' => 'mods-wrapper', // the HTML that wraps the element that needs to be replaced
        'method' => 'replace', // the method we're going to use: a replace operation
        //'effect' => 'fade', // the effect used when replacing the element (try fade!)
      ),
      '#suffix' => '<br/>',
    );
    drupal_add_js('', 'inline');
    // We define a <div> wrapper. Everything in it will be replaced.
    $form['mods'] = array(
      '#tree' => TRUE,
      '#prefix' => '<div id="mods-wrapper">',
      '#suffix' => '</div>',
    );
    $form['mods']['mods_record'] = array(
      '#type' => 'textarea',
      '#title' => 'MODS Record to Import',
      '#rows' => 20,
      '#value' => (!empty($mods_save) ? $mods_save['mods']['mods_record'] : ''),
    );
    return $form;
  }

  public function buildEditMetadataForm($form = array()) {

    $form['submit'] = array(
      '#type' => 'submit',
      '#weight' => 10,
      '#value' => 'Update'
    );
    $form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $this->pid,
    );
    $form['dsid'] = array(
      '#type' => 'hidden',
      '#value' => "DARWIN_CORE",
    );

    return $this->buildDrupalForm($form);
  }

  public function handleEditMetadataForm($form_id, $form_values) {
    /*
     * Process the metadata form
     * Update the datastreams
     */

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    module_load_include('inc', 'fedora_repository', 'MimeClass');
    global $user;
    $mimetype = new MimeClass();
    $dwc = new DarwinCore($this->item);
    $dwc->handleForm($form_values);
    $this->item->purge_datastream('DARWIN_CORE');
    $this->item->add_datastream_from_string($dwc->darwinCoreXML, 'DARWIN_CORE',
            'Darwin Core Metadata', 'text/xml', 'X');
    return TRUE;
  }

  public function handleIngestForm($form_values) {
    /*
     * process the metadata form
     * Create fedora object
     * Add the datastreams
     */
    module_load_include('inc', 'fedora_repository', 'MimeClass');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if ($form_state['clicked_button']['#value'] == 'Retrieve MODS record') {
      return;
    }


    $mods_simple = simplexml_load_string($form_values['mods']['mods_record']);
    $title = '';
    foreach ($mods_simple->children('http://www.loc.gov/mods/v3')->mods[0]->titleInfo[0]->children() as $child) {
      if ($child->getName() == 'subTitle') {
        // We don't care about subtitles for creating the item label.
        continue;
      }
      $title .= $child;
    }

    global $user;
    $mimetype = new MimeClass();
    
    $new_item = Fedora_Item::ingest_new_item(!empty($form_values['custom_pid']) ? $form_values['custom_pid'] : $form_values['pid'], 'A', $title,
            $user->name);

    $new_item->add_datastream_from_string($form_values['mods']['mods_record'], 'MODS',
            'MODS Metadata', 'text/xml', 'X');

    $dc = transform_mods_to_dc($form_values['mods']['mods_record']);
    if ($dc) {
      // Add the PID to a dc:identifier field.
      $dc_doc = simplexml_load_string($dc);  
      $dc_item = $dc_doc->xpath('/srw_dc:dcCollection/srw_dc:dc[1]');
      foreach($dc_item as $node) {
        $node->addChild('identifier', $new_item->pid, 'http://purl.org/dc/elements/1.1/');
      }
      $new_item->modify_datastream_by_value($dc_doc->saveXML(), 'DC', 'Dublin Core XML Metadata', 'text/xml');
    }
    $new_item->add_relationship('hasModel', $form_values['content_model_pid'], FEDORA_MODEL_URI);
    $new_item->add_relationship(!empty($form_values['relationship']) ? $form_values['relationship'] : 'isMemberOfCollection', $form_values['collection_pid']);
  }
  
  public function buildAddPagesForm($form = array()) {

  }

  public function showFieldSets() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    module_load_include('module', 'fedora_ilives');
    global $base_url;

    $tabset = array();
    
    global $user;
    $qs = '';
    if ($user->uid != 0) {
      $qs = '?uid='. base64_encode($user->name . ':'. $user->pass);
    }

    $viewer_url = variable_get('fedora_base_url', '') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer'. $qs;
    $html = '<iframe src="'. $viewer_url . '" scrolling="0" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';
    $tabset['second_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Read'),
      '#content' => $html
    );
    $tabset['first_tab'] = array(
      // #type and #title are the minimum requirements.
      '#type' => 'tabpage',
      '#title' => t('Description'),
      // This will be the content of the tab.
      '#content' => fedora_ilives_create_book_view($this->pid),
    );
    
    if (user_access('ingest new fedora objects')) { 
      $tabset['add_pages'] = array(
        '#type' => 'tabpage',
        '#title' => t('Add pages'),
        '#content' => drupal_get_form('book_add_pages_form', $this->pid),
      );
    }
    return $tabset;
  }
}

function book_add_pages_form(&$form_state, $pid) {
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );

  $form['tiff_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('TIFF folder'),
    '#required' => TRUE,
    '#description' => t('Path to a web-accessible folder contining the book\'s page images in TIFF format on this server.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 10,
    '#value' => 'Ingest Pages'
  );
  return $form;
}

function book_add_pages_form_validate($form, &$form_state) {
  if ($form_state['values']['tiff_dir'] == '') {
    form_set_error('', t('You must select a name for this group of settings.'));
  }
}

function book_add_pages_form_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  if (!empty($form_state['values']['tiff_dir'])) {
    $file_list = array();
    if ($tiff_dir = opendir($form_state['values']['tiff_dir'])) {
      while (FALSE !== ($file_name = readdir($tiff_dir))) {
        $ext = strrchr($file_name, '.');
        if ($ext == '.tif' || $ext == '.tiff') {
          array_push($file_list, $form_state['values']['tiff_dir'] .'/'. $file_name);
        }
      }
      closedir($tiff_dir);
      sort($file_list);
      ilives_create_page_objects($pid, $file_list);
    }
  }
}

/**
 * Create a page object for each element in $image_list as a child object of $pid
 */
function ilives_create_page_objects($pid, $image_list = array()) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $book = new Fedora_Item($pid);
  if (!$book->exists()) {
    return FALSE;
  }
  $page_pids = array();
  $batch = array(
    'title' => 'Creating page objects',
    'operations' => array(),
    'file' => drupal_get_path('module', 'fedora_ilives') .'/book.inc',
  );
  $page_num = 1;
  $book_title = $book->objectProfile->objLabel;
  foreach ($image_list as $image_path) {
    $batch['operations'][] = array('ilives_add_single_page_object', array($pid, $image_path, $page_num, "$book_title - Page ". sprintf("%03d", $page_num)));
    $page_num++;
  }
  batch_set($batch);
}

function ilives_add_single_page_object($book_pid, $page_file, $page_num = 1, $page_title = NULL) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $page_item = Fedora_Item::ingest_new_item("$book_pid-". sprintf("%03d", $page_num), 'A', $page_title);
  $page_item->add_relationship('hasModel', 'ilives:pageCModel', FEDORA_MODEL_URI);
  $page_item->add_relationship('isMemberOf', $book_pid);
  $page_item->add_datastream_from_file($page_file, 'TIFF', 'Archival TIFF', 'image/tiff', 'M');
}

function transform_mods_to_dc($mods) {
  $xp = new XsltProcessor();
  // create a DOM document and load the XSL stylesheet
  $xsl = new DomDocument;
  $xsl->load(drupal_get_path('module', 'fedora_ilives').'/xsl/MODS3-22simpleDC.xsl');
  
  // import the XSL styelsheet into the XSLT process
  $xp->importStylesheet($xsl);
  
   // create a DOM document and load the XML datat
  $xml_doc = new DomDocument;
  $xml_doc->loadXML($mods);
  
   // transform the XML into HTML using the XSL file
  if ($dc = $xp->transformToXML($xml_doc)) {
    return $dc;
  }
  else {
    return FALSE;
  }
}
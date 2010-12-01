<?php

// $Id$

class SCORMObject {

  function  __construct($pid = '') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {

      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }

  public function buildIngestForm($form = array(), $form_state = array()) {
    $form['bulk_ingest_location'] = array(
      '#title' => 'Bulk ingest location',
      '#type' => 'textfield',
      '#size' => 60,
      '#description' => "Server location from which to upload SCORM objects.  Leave this blank if you are uploading a single file.",
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

    /**
     * process the metadata form
     * Create fedora object
     * Add the datastreams
     */
  public function handleIngestForm($form_values) {

    module_load_include('inc', 'fedora_repository', 'MimeClass');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');

    global $user;
    $mimetype = new MimeClass();
    $file_list = array();
    if (!empty($form_values['bulk_ingest_location'])) {
      if ($scorm_dir = opendir($form_values['bulk_ingest_location'])) {
        while (FALSE !== ($file_name = readdir($scorm_dir))) {
          $ext = strrchr($file_name, '.');
          if ($ext == '.zip') {

            array_push($file_list, $form_values['bulk_ingest_location'] .'/'. $file_name);
          }
        }
        closedir($scorm_dir);
        sort($file_list);
      }
    }
    else {
      array_push($file_list, $form_values['ingest-file-location']);
    }
    scorm_create_scorm_objects($form_values['collection_pid'], $file_list, $form_values['content_model_pid'], $form_values['relationship'] );
  }
  
  public function showFieldSets() {
    global $base_url;
// Try and get a node that references this object.
    //$result = db_query("SELECT * FROM {content_node_field} nf  INNER JOIN {content_node_field_instance} ni ON nf.field_name = ni.field_name WHERE nf.type='field_fedora_pid_reference'");

    fedora_pidfield_redirect_to_node($this);
    $tabset['my_tabset']['first_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
    );
    if (module_load_include('module', 'SCORM', 'SCORM')) {
      $dest_array = explode('/', urldecode(drupal_get_destination()));
      $nid = $dest_array[count($dest_array) - 1];
      $node = node_load($nid);

      $tabset = array();

      $tabset['my_tabset'] = array(
        '#type' => 'tabset',
      );
      $tabset['my_tabset']['second_tab'] = array(
          '#type' => 'tabpage',
          '#title' => t('Results'),
          '#content' => scorm_show_results($node),
      );
    }
   
    
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $obj = new ObjectHelper();
    $tabset['my_tabset']['first_tab']['tabset'] = array(
      '#type' => 'tabset',
    );

    $tabset['my_tabset']['first_tab']['tabset']['view'] = array(
      '#type' => 'tabpage',
      '#title' => t('View'),
      '#content' => $obj->getQDC($this->pid),
    );
    if (fedora_repository_access(OBJECTHELPER :: $EDIT_FEDORA_METADATA, $this->pid, $user)) {
      $editform = drupal_get_form('fedora_repository_edit_qdc_form', $this->pid, 'DC');
      $tabset['my_tabset']['first_tab']['tabset']['edit'] = array(
        '#type' => 'tabpage',
        '#title' => t('Edit'),
        '#content' => $editform,
      );

    }
    
    return tabs_render($tabset);
  }

  public function extractFile($filename) {
    // Get a file from the zip and put it in a temporary location.
    $tmpdir = sys_get_temp_dir();

    exec("unzip $this->file $filename -d $tmpdir", $output);
    if (file_exists($tmpdir.'/'.$filename)) {
      return $tmpdir.'/'.$filename;
    }
    else {
      return FALSE;
    }
  }

  private function _cleanUpHTML($html) {

    $tmp = substr($html, strpos($html, '<body>'));

    return _filter_html($tmp, FILTER_HTML_STRIP);

  }

  public function _createSCORMObjectNode($scorm_file, $item) {
    module_load_include('inc', 'node', 'node.pages'); // required\
    module_load_include('inc', 'fedora_repository', 'api/dublin_core');

    global $user;
    $node = array('type' => 'repository_item');
    $form_state = array();
    
    $dc = new Dublin_Core($item);
    $title = $dc->dc['dc:title'][0];
    if (empty($title)) {
      $title = $pid;
    }
    $form_state['values']['title'] = $title; // node's title

    // Prepare the file field.
    $mime =  'application/zip';
    $file = new stdClass();
    $file->filename = basename($scorm_file);
    $file->filepath = $scorm_file;
    $file->filemime = $mime;
    $file->filesize = filesize($scorm_file);

    $file->uid = $user->uid;
    $file->status = 0;
    $file->timestamp = time();
    drupal_write_record('files', $file);
    $file->fid = db_result(db_query("SELECT fid FROM {files} WHERE filepath = '%s'", $file->filepath));



    // Create a drupal node that includes a SCORM object and reference to this Fedora PID.
    // Drupal Content Type is SCORM Fedora Object. Fields are
    // field_scorm_obj-0-value
    // field_fedora_pid_reference-0-value
    $form_state['values']['field_fedora_pid_reference'][0]['value'] = $item->pid;
    /*$form_state['values']['field_scorm_obj'] = array (
      array(
        'fid' => $file->fid,
        'title' => basename($file->filename),
        'filename' => $file->filename,
        'filepath' => $file->filepath,
        'filesize' => $file->filesize,
        'mimetype' => $mime,
        'description' => basename($file->filename),
        'list' => 1,
      ),
    );*/
    $form_state['values']['op'] = t('Save'); // required value
    drupal_execute('repository_item_node_form', $form_state, (object)$node);

    // you can probably configure the node-author in $form_state or $node,
    // but i'm doing it this way to demonstrate the use of $form_state['nid'].
    // the following lines aren't required, but otherwise the node-author will be "Anonymous" in this case.
    $node = node_load($form_state['nid']); // nid from the node that gets created is set in $form_state['nid'] after drupal_execute()
    $node->uid = $user->uid; // set author to the current user
    $field = content_fields('field_scorm_obj', 'repository_item');
    // Load up the appropriate validators
    //$validators = array_merge(filefield_widget_upload_validators($field), SCORM_widget_upload_validators($field));
    $validators = SCORM_widget_upload_validators($field);
    // Where do we store the files?
    $files_path = filefield_widget_file_path($field);
    // Create the file object
    $file = field_file_save_file($scorm_file, $validators, $files_path);
    // Apply the file to the field
    $file['data']['width'] = 640;
    $file['data']['height'] = 480;
    $node->field_scorm_obj = array(0 => $file);
    $this->_processSCORMObject($file);
    node_save($node);
    return $node;
  }

  private function _processSCORMObject($field) {

       $packagedata=scorm_validate_file($field['filepath']);

        $scorm->pkgtype = $packagedata->pkgtype;
        $scorm->datadir = $packagedata->datadir;
        $scorm->launch = $packagedata->launch;
        $scorm->parse = 1;

        $scorm->timemodified = time();
       /* if (!scorm_external_link($scorm->reference)) {
            $scorm->md5hash = md5_file($CFG->dataroot.'/'.$scorm->course.'/'.$scorm->reference);
        } else {
            $scorm->dir = $CFG->dataroot.'/'.$scorm->course.'/moddata/scorm';
            $scorm->md5hash = md5_file($scorm->dir.$scorm->datadir.'/'.basename($scorm->reference));
        }*/

        $scorm = scorm_option2text($scorm);
        //TODO: Implement form for user to set height and width for SCORM package
        $scorm->width = '640';
        $scorm->height = '480';

        if (!isset($scorm->whatgrade)) {
            $scorm->whatgrade = 0;
        }
        $scorm->grademethod = ($scorm->whatgrade * 10) + $scorm->grademethod;

        //TODO: Do we need this fields:
        $scorm->name="SCORM";
        $scorm->summary="SCORM 2004.";
        $scorm->grademethod='';
        $scorm->maxgrade=100;
        $scorm->maxattempt=0;
        $scorm->updatefreq=0;
        $scorm->course=1;


         //At this point it is still empty
        $scorm->version='';
        $scorm->skipview=0;
        $scorm->hidebrowse=0;
        $scorm->hidetoc=0;
        $scorm->hidenav=0;
        $scorm->auto=0;
        $scorm->popup=0;

        //TODO: Do we still need it?
        //$scorm->reference=$field->filepath;


        //TODO: Remove MD5 field, we dont use it.
        //$id = insert_record('scorm', $scorm);

        $result = db_query("INSERT INTO {scorm}
        (course,name,nodereference,reference,summary,version,maxgrade,grademethod,whatgrade,maxattempt,updatefreq,md5hash,launch,
        skipview,hidebrowse,hidetoc,hidenav,auto,popup,options,width,height,timemodified)
        VALUES (%d,'%s',%d,'%s','%s','%s',%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d)", $scorm->course, $scorm->name, NULL, NULL, $scorm->summary, $scorm->version,
        $scorm->maxgrade, $scorm->grademethod, $scorm->whatgrade, $scorm->maxattempt, $scorm->updatefreq, NULL, $scorm->launch, $scorm->skipview, $scorm->hidebrowse,
        $scorm->hidetoc, $scorm->hidenav, $scorm->auto, $scorm->popup, $scorm->options, $scorm->width, $scorm->height, $scorm->timemodified );
        
        $id = db_last_insert_id('scorm', 'id'); //$id=mysql_insert_id();

          //TODO: Test it on Linux
          // Move SCORM from temp dir to scorm dir

            $storedir=file_directory_path() .'/SCORM';
            $path=$storedir .'/'. $id;

            if (!file_exists($storedir)) {
                    mkdir($storedir);
            }
            $res=mkdir($path);
            if ($res==TRUE) {
             full_copy($packagedata->tempdir, $path);
             //rmdirr($packagedata->tempdir);
             scorm_delete_files($packagedata->tempdir);
             //Replace reference field with node field.
             db_query("UPDATE {scorm} SET reference = '%s' WHERE id = %d", $field['fid'], $id);
            }
            else
             return FALSE;


            $scorm->id = $id;
            //Parse SCORM manifest
            $scorm->launch=scorm_parse_scorm($path, $scorm->id);

            //Save SCORM launch instance
             db_query("UPDATE {scorm} SET launch = '%s' WHERE id = %d", $scorm->launch, $scorm->id);

  }
}

function scorm_create_scorm_objects($collection_pid, $file_list = array(), $content_model_pid = 'islandora:SCORMCModel', $relationship = 'isMemberOfCollection') {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');

  $batch = array(
    'title' => 'Ingesting SCORM objects',
    'operations' => array(),
    'file' => drupal_get_path('module', 'fedora_nmlt') .'/scorm.inc',
  );
  
  foreach ($file_list as $file_path) {
    $batch['operations'][] = array('_create_single_SCORM_object', array($file_path, $collection_pid, $content_model_pid, $relationship));
  }
  batch_set($batch);
}

function _create_single_SCORM_object($file, $collection_pid, $content_model_pid, $relationship = 'isMemberOfCollection') {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $url = parse_url(variable_get('fedora_base_url', 'http://localhost:8080/fedora'));
  $fedora_host = ("{$url['scheme']}://{$url['host']}" . (!empty($url['port']) ? ":{$url['port']}/" : '/'));
  $scorm2fedora_url = $fedora_host . 'scorm2fedora';
  $ch = curl_init($scorm2fedora_url);
  curl_setopt($ch, CURLOPT_POSTFIELDS, array('scormfile' => "@{$file}"));
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  $post_result = curl_exec($ch);
  curl_close($ch);
  $scorm2fedora_result = json_decode($post_result);
  $scorm_object_pid = $scorm2fedora_result->pid;
  if (!empty($scorm_object_pid)) {
    $scorm_object = new Fedora_Item($scorm_object_pid);
    $scorm_object->add_relationship('hasModel', $content_model_pid, FEDORA_MODEL_URI);
    $scorm_object->add_relationship($relationship, $collection_pid);
    $scorm = new SCORMObject($scorm_object_pid);
    $scorm->_createSCORMObjectNode($file, $scorm_object);
  }
}
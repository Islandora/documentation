<?php

// $Id$

function theme_filechooser($element) {
  global $base_url;
  $path=drupal_get_path('module', 'islandora_form_elements');
  
  drupal_add_js($path .'/js/jcarousellite_1.0.1.js');
  drupal_add_js($path .'/js/jquery.easing.1.1.js');
  drupal_add_js($path .'/js/jquery.mousewheel.min.js');
  drupal_add_js($path .'/js/jquery.loadImages.1.0.1.js');
  drupal_add_js($path .'/js/jquery.breakly-1.0.js');
  drupal_add_js($path .'/js/filechooser.js');
  drupal_add_css($path .'/css/filechooser.css');
  
  module_load_include('inc', 'fedora_repository', 'ContentModel');
  $contentModelPid = ContentModel::getPidFromIdentifier($element['#model_pid']);
  
  
  $output='<div class="carousel">
      <div class="jCarouselLite">
    <ul id="fileList" class="clear-block">
    <li class="loading"><div>Generating File Previews...<br/>(this could take a minute or two.)<br/><img src="'.$base_url.'/'. $path .'/images/ajax-loader.gif" id="ajaxBusy" alt="Loading"/></div></li>
    </ul>
      </div>
  <br clear="all"/>
  <a href="#" class="prev">Prev</a> <a href="#" class="next">Next</a>
  <input type="hidden" id="model_pid" value="'. $contentModelPid .'"/>
  <input type="hidden" name="'. $element['#name'] .'" value="'. check_plain($element['#value']) .'" id="fileField"/></div>';
  return theme('form_element', $element, $output);
}



function ife_filechooser_get_thumbnail($cp_pid, $cm_pid, $file) {
  module_load_include('inc', 'fedora_repository', 'CollectionPolicy');
  module_load_include('inc', 'fedora_repository', 'ContentModel');
  module_load_include('inc', 'fedora_repository', 'MimeClass');
  
  $mimeClass = new MimeClass();
  
  $cm = ContentModel::loadFromModel($cm_pid);
  $cp = CollectionPolicy::loadFromCollection($cp_pid);
  $path=$cp->getStagingArea() .'/';
  
  if ($cm !== FALSE && $cp !== FALSE) { 
    if (is_file($path . $file)) {
      
      $mime=$mimeClass->getType($path . $file);  
      
      $allowed_types = $cm->getMimetypes();

      if (in_array($mime, $allowed_types)) {
 
      $thumbnail = '';
      //executes ingest rules with the "preview" parameter set.
      if ($cm->execIngestRules($path . $file, $mime, TRUE) && isset($_SESSION['fedora_ingest_files']['TN']) && file_exists($_SESSION['fedora_ingest_files']['TN'])) {
        $thumbnail=$_SESSION['fedora_ingest_files']['TN'];
      }
    
      $thumbmime=$mimeClass->getType($thumbnail); 
    
      header('Pragma: no-cache');
      header('Content-Type: '. $thumbmime);
      echo file_get_contents($thumbnail);
      exit();
      } 
    }
       
  }
  
}
function ife_filechooser_cmp($a,$b)
{
    return strcmp($a['name'],$b['name']);
}

function ife_filechooser_generate_thumbnails($cp_pid, $cm_pid) {
  module_load_include('inc', 'fedora_repository', 'CollectionPolicy');
  module_load_include('inc', 'fedora_repository', 'ContentModel');
  module_load_include('inc', 'fedora_repository', 'MimeClass');
  
  $cm = ContentModel::loadFromModel($cm_pid);
  $cp = CollectionPolicy::loadFromCollection($cp_pid);

	if ($cm === FALSE || $cp === FALSE) {
		echo '[]'; exit();
	}

  $mimeClass = new MimeClass();
  $path=$cp->getStagingArea() .'/';
  $start=isset($_GET['start'])?intval($_GET['start']):0;
  $end=isset($_GET['end'])?intval($_GET['end']):50;
  $thumbnails = array();
  $files = array();
  if ($cm !== FALSE && $cp !== FALSE) {
     $allowed_types = $cm->getMimetypes();
		 if (file_exists($path) && is_dir($path)) {
       $dir = opendir($path);
       for ($i=$start; $i < $end && ($file = readdir($dir)) !== FALSE;) {
         if (is_file($path . $file)) {
           $mime=$mimeClass->getType($path . $file);  
           if (in_array($mime, $allowed_types)) {
             
             $thumbnail = FALSE;
             //executes ingest rules with the "preview" parameter set.
             if ($cm->execIngestRules($path . $file, $mime, TRUE) && isset($_SESSION['fedora_ingest_files']['TN']) && file_exists($_SESSION['fedora_ingest_files']['TN'])) {
               $thumbnail=$_SESSION['fedora_ingest_files']['TN'];
               unset($_SESSION['fedora_ingest_files']['TN']);
             }
             
             $res_array = getimagesize($path .'/'. $file);
             $res = '';
             if (is_array($res_array)) {
               $res = $res_array[0] .'x'. $res_array[1];
             }
             
             $size = filesize($path .'/'. $file);
             $labels = array('kB', 'MB', 'GB', 'TB');
             $label = 'B';
             while ($size > 1024) {
               $size=$size/1024;
               $label=array_shift($labels); 
             }
             $size = round($size, 2) .' '. $label;
	     
             $files[] = array('name' => $file, 'mime' => $mime, 'size' => $size, 'resolution' => $res, 'thumb' => ($thumbnail != FALSE));
	      $i++;
           } 
         }
       }
     }    
  }
  usort($files,'ife_filechooser_cmp');
  echo json_encode($files);
  
}

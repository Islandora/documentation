<?php

// $Id$

// this is a bit of a hack.  It iterates through the 
// form and assigns a dummy value to any field in the $post 
// data that is marked as #required.  This is so that the 
// AHAH method can still function with required fields that 
// are first passed to the core form validation function.
function _dummy_post_info($form, $post, $key = '') {
  $children = element_children($form);
  if (count($children) > 0) {
     foreach ($children as $key) {
       $post = _dummy_post_info($form[$key], $post, $key);
    }
  }
  if ($key!='' && isset($form['#required']) && $form['#required'] == TRUE && trim($post[$key]) == '') {
   $post[$key] = 'not empty';
  }
   
  return $post;
}

function _ife_find_people_element($form, &$keys = array()) {
  if (isset($form['#type']) && $form['#type'] == 'people') {
    return $form['people'];
  } 
  
  $children = element_children($form);
  foreach ($children as $key) {
    $found = _ife_find_people_element($form[$key], $keys);
    if ($found !== FALSE) {
      $keys[] = $key;
      return $found;
    }
  }
  
  return FALSE;

}

function ife_people_ahah() {
  if (!isset($_POST['form_build_id']))
      return;
  
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  
  $_POST = _dummy_post_info($form, $_POST);
  $form_state['post'] = $_POST;
  $form['#post'] = $_POST;
  
  drupal_process_form($form_id, $form, $form_state);

  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $qt_form = _ife_find_people_element($form);
//    ob_start();  echo '<pre>'; echo $form_state['clicked_button']['#parents'][2]; var_dump($form_state['storage']['people']); echo'</pre>';  $dump = ob_get_contents(); ob_end_clean();
  
  unset($qt_form['#prefix'], $qt_form['#suffix']); // Prevent duplicate wrappers.
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array(
    'status'   => TRUE,
    'data'     => $dump . theme('status_messages') . drupal_render($qt_form),
    'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
  ));
}

/**
 * Submit handler for the "Remove Person" button.
 */
function ife_remove_person_submit($form, &$form_state) {
  
  // Get the tab delta for the clicked button.
  $delta = $form_state['clicked_button']['#parents'][2];
  
  $keys=array();
  $qt_form = _ife_find_people_element($form, $keys);
  
  $peopleVals = &$form_state['values'];
  while (count($keys) > 0) {
    $key = array_pop($keys);
    if (isset($peopleVals[$key])) {
      $peopleVals = &$peopleVals[$key];
    }
  }  
  
  $people = array();  
  $i = 0;
  foreach ($peopleVals['people'] as $key => $val) {
    if ($key != $delta) {
      $people[] = $val;
    }
    $i++;
  }
  
//    ob_start();  var_dump($form_state['clicked_button']['#parents'][1]); var_dump($people);  $dump = ob_get_contents(); ob_end_clean();
//   watchdog('people_field','<pre>'.$dump.'</pre>',array(),WATCHDOG_NOTICE);  
  
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['storage']['people'] = $people;
  $form_state['rebuild'] = TRUE;
}


/**
 * Submit handler for the "Add Person" button.
 */
function ife_add_person_submit($form, &$form_state) {
  $people = array();
  $keys = array();
  $qt_form = _ife_find_people_element($form, $keys);
  $peopleVals = &$form_state['values'];
  while (count($keys) > 0) {
    $key = array_pop($keys);
    if (isset($peopleVals[$key])) {
      $peopleVals = &$peopleVals[$key];
    }
  }  
  if (!is_array($peopleVals['people']))
      $peopleVals['people'] = array();
  
  foreach ($peopleVals['people'] as $val) {
    $people[] = $val;
  }
  $people[] = array('fname' => '', 'lname' => '' , 'title' => '', 'role' => '');
  
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['storage']['people'] = $people;
  $form_state['rebuild'] = TRUE;
}



/**
 * Submit handler for the "Add Person" button.
 */
function ife_add_org_submit($form, &$form_state) {
  $people = array();
  $keys = array();
  $qt_form = _ife_find_people_element($form, $keys);
  $peopleVals = &$form_state['values'];
  while (count($keys) > 0) {
    $key = array_pop($keys);
    if (isset($peopleVals[$key])) {
      $peopleVals = &$peopleVals[$key];
    }
  }
  if (!is_array($peopleVals['people']))
      $peopleVals['people'] = array();
    
  foreach ($peopleVals['people'] as $val) {
    $people[]=$val;
  }
  $people[] = array('role' => '', 'organization' => '');
  
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['storage']['people'] = $people;
  $form_state['rebuild'] = TRUE;
}



/**
 * Submit handler for the "Add Person" button.
 */
function ife_add_conf_submit($form, &$form_state) {
  $people = array();
  $keys=array();
  $qt_form = _ife_find_people_element($form, $keys);
  $peopleVals = &$form_state['values'];
  while (count($keys) > 0) {
    $key = array_pop($keys);
    if (isset($peopleVals[$key])) {
      $peopleVals=&$peopleVals[$key];
    }
  }  
  if (!is_array($peopleVals['people']))
      $peopleVals['people'] = array();
  
  foreach ($peopleVals['people'] as $val) {
    $people[] = $val;
  }
  $people[] = array('role' => '', 'conference' => '');
  
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['storage']['people'] = $people;
  $form_state['rebuild'] = TRUE;
}



function theme_people($element) {
    drupal_add_js(drupal_get_path('module', 'islandora_form_elements') .'/js/people_ahah.js');
    if (!empty($element['#collapsible'])) {
    drupal_add_js('misc/collapse.js');

    if (!isset($element['#attributes']['class'])) {
      $element['#attributes']['class'] = '';
    }

    $element['#attributes']['class'] .= ' collapsible';
    if (!empty($element['#collapsed'])) {
      $element['#attributes']['class'] .= ' collapsed';
    }
  }
   return '<fieldset'. drupal_attributes($element['#attributes']) .'>'. ($element['#title'] ? '<legend>'. $element['#title'] .'</legend>' : '') . (isset($element['#description']) && $element['#description'] ? '<div class="description">'. $element['#description'] .'</div>' : '') . (!empty($element['#children']) ? $element['#children'] : '') . "</fieldset>\n";
}


function theme_people_items($form) {
  $rows = array();
  $headers = array(
    t('Entity Name'),
    t('Role'),
    t('Operations'),
  );

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['organization'])) {
      $form[$key]['weight']['#attributes']['class'] = 'ife-people-weight';
      // Build the table row.
      $row = array(
        'data' => array(
           array('data' => drupal_render($form[$key]['organization']), 'class' => 'ife-person-name container-inline'),
           array('data' => drupal_render($form[$key]['role']), 'class' => 'ife-person-role'),
           array('data' => drupal_render($form[$key]['remove']), 'class' => 'ife-person-remove'),
        ),
      );
    } 
    elseif (isset($form[$key]['conference'])) {
      $form[$key]['weight']['#attributes']['class'] = 'ife-people-weight';
      // Build the table row.
      $row = array(
        'data' => array(
          array('data' => drupal_render($form[$key]['conference']).'<br clear="all"/>'. drupal_render($form[$key]['date']), 'class' => 'ife-person-name container-inline' ),
          array('data' => drupal_render($form[$key]['role']), 'class' => 'ife-person-role'),
          array('data' => drupal_render($form[$key]['remove']), 'class' => 'ife-person-remove'),
         ),
      );
    } 
    else {
      $form[$key]['weight']['#attributes']['class'] = 'ife-people-weight';
      // Build the table row.
      $row = array(
        'data' => array(
          array('data' => drupal_render($form[$key]['fname']) .'<br clear="all"/>'. drupal_render($form[$key]['lname']) .'<br clear="all"/>'. drupal_render($form[$key]['title']).'<br clear="all"/>'. drupal_render($form[$key]['date']), 'class' => 'ife-person-name container-inline'),
          array('data' => drupal_render($form[$key]['role']), 'class' => 'ife-person-role'),
          array('data' => drupal_render($form[$key]['remove']), 'class' => 'ife-person-remove'),
         ),
      );
    }

    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  $output = theme('table', $headers, $rows, array('id' => 'ife-peoplelist-table'));
  $output .= drupal_render($form);

  // Add our JS file, which has some Drupal core JS overrides, and ensures ahah behaviours get re-attached
  drupal_add_css(drupal_get_path('module', 'islandora_form_elements') .'/css/people.css');
  return $output;  
}



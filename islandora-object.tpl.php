<?php

/*
 * islandora-object-default-view.tpl.php
 * 
 *
 * 
 * This file is part of Islandora.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with the program.  If not, see <http ://www.gnu.org/licenses/>.
 */
?>
<?php

/* 
 * this is a template for objects that do not have a module to registered to build their display.
 * 
 * islandora_object is a fedora tuque Object
 *    $object->label
 *    $object->id
 * to get the contents of a datastream
 *    $object['dsid']->content 
 * 
 * $dublin_core is a Dublin_Core object
 * which is an array of elements, such as dc.title
 * and each element has an array of values.  dc.title can have none, one or many titles
 * this is the case for all dc elements.
 * 
 * we can get a list of datastreams by doing
 * foreach ($object as $ds){
 * do something here
 * }
 * 
 */

//dsm($object);
drupal_set_title($islandora_object->label);
$dublin_core = $variables['islandora_dublin_core'];
print($islandora_object->label . ' ' . $islandora_object->id);
print ('<h3>datastreams</h3>');
foreach ($islandora_object as $ds) {
  print $ds->label . '<br>';
  //do something
}
print('<h3>Dublin Core</h3>');

foreach ($dublin_core as $element) {
  if (!empty($element)) {
   // print($element);
   foreach ($element as  $key => $value) {      
      foreach($value as $v){
        if(!empty($v)){
        print '<strong>'.($key).'</strong>: ';print($v).'<br />';
        }
      }
    }
  }
}
if(isset($variables['islandora_thumbnail_url'])){
  print('<img src = "'.$variables['islandora_thumbnail_url'].'"/>');
}
?>


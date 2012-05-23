<?php

/*
 * fedora-repository-view-object.tpl.php
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
/**
 * This template is an example of overiding a template by pid
 * 
 * This template must be placed in your themes template directory and 
 * allows you to override a template file for a specific object
 * an example use case would be for theming a collection object differently
 * based on the actual collection object.  
 * 
 * Best practice would be to create a new cmodel if you have many objects that
 * need to be themed differently but if you only have a few objects that need different
 * templates this method would work. 
 */
  $object = $variables['islandora_object'];
  $image_url = $variables['islandora_image_url'];
  drupal_set_title($object->label);
  print ('This template has been overridden by a theme suggestion');
  foreach ($variables['islandora_dublin_core'] as $element) {
    if (!empty($element)) {
      foreach ($element as $key => $value) {
        foreach ($value as $v) {
          if (!empty($v)) {
            print '<strong>' . ($key) . '</strong>: ';
            print($v) . '<br />';
          }
        }
      }
    }
  }
  print('<img src = "' . $image_url . '"/>');
?>


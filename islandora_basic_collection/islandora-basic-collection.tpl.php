<?php

/*
 * islandora-basic-collection.tpl.php
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

  $object = $variables['islandora_object'];
  $image_url = $variables['islandora_thumbnail_url'];
  drupal_set_title($object->label);
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
  print('<h3>Associated Objects</h3>');
  foreach($variables['islandora_associated_objects'] as $associated_objects){
    global $base_url;
    $link = l($associated_objects['title']['value'],$base_url.'/islandora/object/'.$associated_objects['object']['value']);
    print ($link).'<br />';
    
  }
?>


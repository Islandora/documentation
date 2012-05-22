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
/*object is a fedora tuque Object
 * $object->label
 * $object->id
 * to get the contents of a datastream
 * $object['dsid']->content 
 * etc.
 * 
 */
 
  
  //dsm($object);
  
  print('this is the default view for '. $object->label .' ' .$object->id); 
  
?>


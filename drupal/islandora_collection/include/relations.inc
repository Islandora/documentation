<?php

/**
 * @file
 * Utility functions for defining collection relationships.
 */

/**
 * Utility function to define relation types using CTools export code.
 */
function islandora_collection_define_relations() {
  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'pcdm_hasmember';
  $relation_type->label = 'pcdm:hasMember';
  $relation_type->reverse_label = 'fedora:hasParent';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'node:basic_image',
    1 => 'node:collection',
  );
  $relation_type->target_bundles = array(
    0 => 'node:basic_image',
    1 => 'node:collection',
  );
  // Relation type create adds default keys.  It also handles casting to array.
  $relation_type = relation_type_create($relation_type);
  // Yes, in relation.module 'save' is a distinct step from 'create'.
  relation_type_save($relation_type);
}

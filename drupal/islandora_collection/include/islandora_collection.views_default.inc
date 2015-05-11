<?php

/**
 * @file
 * Exported views code for this module.
 */

/**
 * Implements hook_views_default_views().
 */
function islandora_collection_views_default_views() {
  $view = new view();
  $view->name = 'collection_view';
  $view->description = 'Shows all members of a collection';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Collection View';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Members';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Relationship: Content: Relation: pcdm:hasMember (node → node) */
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['id'] = 'relation_pcdm_hasmember_node';
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['table'] = 'node';
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['field'] = 'relation_pcdm_hasmember_node';
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['required'] = TRUE;
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['r_index'] = '1';
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['entity_deduplication_left'] = 0;
  $handler->display->display_options['relationships']['relation_pcdm_hasmember_node']['entity_deduplication_right'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Node UUID */
  $handler->display->display_options['arguments']['uuid']['id'] = 'uuid';
  $handler->display->display_options['arguments']['uuid']['table'] = 'node';
  $handler->display->display_options['arguments']['uuid']['field'] = 'uuid';
  $handler->display->display_options['arguments']['uuid']['relationship'] = 'relation_pcdm_hasmember_node';
  $handler->display->display_options['arguments']['uuid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uuid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uuid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uuid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uuid']['limit'] = '0';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'collection-view/%';

  $views[$view->name] = $view;
  return $views;
}

<?php

/**
 * @file
 * This file contains functions to create breadcrumbs on Islandora object pages.
 */

/**
 * Get an array of links to be passed to drupal_set_breadcrumb().
 *
 * This is used for generating the bread-crumbs for the view object page.
 *
 * Each link in the bread-crumbs represents a member of the given objects
 * ancestry which is identified by any of the following RELS-EXT terms
 * (isMemberOf,isMemberOfCollection,isPartOf).
 *
 * @param FedoraObject $object
 *   An object whose ancestry will be mapped to bread-crumbs.
 *
 * @see drupal_set_breadcrumb()
 *
 * @return array
 *   Array of links, starting with most distant ancestor proceeding up to but
 *   not including the given object. For use in the function
 *   drupal_set_breadcrumb().
 */
function islandora_get_breadcrumbs($object) {
  $breadcrumbs = islandora_get_breadcrumbs_recursive($object->id, $object->repository);
  array_pop($breadcrumbs);
  return $breadcrumbs;
}

/**
 * Builds an array of drupal links for use in breadcrumbs.
 *
 * @todo Make fully recursive...
 *
 * @param string $pid
 *   THe object id whose parent will be fetched for the next link.
 * @param array $breadcrumbs
 *   The list of existing bread-crumb links in reverse order.
 * @param FedoraRepository $repository
 *   The fedora repository.
 */
function islandora_get_breadcrumbs_recursive($pid, FedoraRepository $repository, array &$context = NULL) {
  // Before executing the query, we have a base case of accessing the top-level
  // collection.
  if ($context === NULL) {
    $context['level'] = 10;
  }

  $root = variable_get('islandora_repository_pid', 'islandora:root');
  if ($pid == $root) {
    return array(
      l(t('Home'), '<front>'),
      l(menu_get_active_title(), 'islandora'),
    );
  }
  else {
    $query_string = <<<EOQ
SELECT ?parentObject ?title
FROM <#ri>
WHERE {
  ?object <fedora-model:label> ?title ;
  {
    ?object <fedora-rels-ext:isMemberOfCollection> ?parentObject .
  }
  UNION {
    ?object <fedora-rels-ext:isMemberOf> ?parentObject .
  }
  UNION {
    ?object <fedora-rels-ext:isPartOf> ?parentObject .
  }
  ?parentObject <fedora-model:state> <fedora-model:Active> .
  FILTER (sameTerm(?object, <info:fedora/$pid>))
  FILTER (!sameTerm(?object, ?parentObject))
}
ORDER BY ?title
EOQ;
    $results = $repository->ri->sparqlQuery($query_string);

    if (count($results) > 0  && $context['level'] > 0) {
      $parent = $results[0]['parentObject']['value'];
      $this_title = $results[0]['title']['value'];

      if (empty($this_title)) {
        $this_title = t('-');
      }

      $context['level']--;
      return array_merge(
        islandora_get_breadcrumbs_recursive($parent, $repository, $context),
        array(
          l($this_title, "islandora/object/$pid"),
        )
      );
    }
    else {
      // Add an non-link, as we don't know how to get back to the root, and
      // render the last two links and break (on the next pass).
      return array_merge(
        islandora_get_breadcrumbs_recursive($root, $repository),
        array(
          '...'
        )
      );
    }
  }
}

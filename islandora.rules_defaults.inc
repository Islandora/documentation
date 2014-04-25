<?php

/**
 * @file
 * Implementation of Rules hooks, giving default configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function islandora_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'E-mail admin';
  $rule->active = FALSE;
  $rule
    ->event('islandora_object_ingested')
    ->action(
      'mail',
      array(
        'to' => '[site:mail]',
        'subject' => '[[site:name]] "[object:label]" has been ingested',
        'message' => '[object:label] has been ingested as [object:id].',
      )
    );
  return array('islandora_object_ingested_notify_admin' => $rule);
}

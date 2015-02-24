<?php

// Leave 'er wide open so the proof of content works.
user_role_change_permissions(
  DRUPAL_ANONYMOUS_RID, 
  array('bypass node access' => TRUE)
);

user_role_change_permissions(
  DRUPAL_AUTHENTICATED_RID, 
  array('bypass node access' => TRUE)
);

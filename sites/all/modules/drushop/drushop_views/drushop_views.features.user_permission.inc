<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function drushop_views_user_default_permissions() {
  $permissions = array();

  // Exported permission: create info content
  $permissions['create info content'] = array(
    'name' => 'create info content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: create news content
  $permissions['create news content'] = array(
    'name' => 'create news content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: create page content
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: delete any info content
  $permissions['delete any info content'] = array(
    'name' => 'delete any info content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: delete any news content
  $permissions['delete any news content'] = array(
    'name' => 'delete any news content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: delete any page content
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: delete own info content
  $permissions['delete own info content'] = array(
    'name' => 'delete own info content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: delete own news content
  $permissions['delete own news content'] = array(
    'name' => 'delete own news content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: delete own page content
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: edit any info content
  $permissions['edit any info content'] = array(
    'name' => 'edit any info content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: edit any news content
  $permissions['edit any news content'] = array(
    'name' => 'edit any news content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: edit any page content
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: edit own info content
  $permissions['edit own info content'] = array(
    'name' => 'edit own info content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: edit own news content
  $permissions['edit own news content'] = array(
    'name' => 'edit own news content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  // Exported permission: edit own page content
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      '0' => 'модератор',
    ),
  );

  return $permissions;
}

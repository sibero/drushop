<?php
// $Id: views_hierarchy.views.inc,v 1.1 2010/06/04 11:13:26 joachim Exp $
/**
 * @file views_hierarchy.views.inc
 * Contains implementations of views hooks for the module.
 */

/**
 * Implementation of hook_views_plugins().
 */
function views_hierarchy_views_plugins() {
  return array(
    'style' => array(
      'hierarchy' => array(
        'title' => t('Hierarchy'),
        'help' => t('Show items in a hierarchical list.'),
        'handler' => 'views_hierarchy_plugin_style_hierarchy',
        'parent' => 'list', // This is the key name as used by other implementations of this hook, NOT a plugin class!
        //'path' => drupal_get_path('module', 'views_hierarchy') . '/plugins', // not necessary for most modules
        'theme' => 'views_hierarchy_view_hierarchy',
        'theme file' => 'views_hierarchy.theme.inc',
        'theme path' => drupal_get_path('module', 'views_hierarchy'),
        'uses fields' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE, // for now at least. @todo post a patch!
        'uses options' => TRUE,
        'type' => 'normal',
        //'help topic' => 'style-unformatted', // @todo
      ),
    ),
  );
}

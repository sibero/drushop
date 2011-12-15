<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function drushop_flags_flag_default_flags() {
  $flags = array();
  // Exported flag: "Закладки".
  $flags['bookmarks'] = array(
    'content_type' => 'node',
    'title' => 'Закладки',
    'global' => '0',
    'types' => array(
      '0' => 'product',
    ),
    'flag_short' => 'В закладки',
    'flag_long' => 'Добавить товар в закладки',
    'flag_message' => 'Этот товар добавлен в закладки',
    'unflag_short' => 'Удалить из закладок',
    'unflag_long' => 'Удаление товара из закладок',
    'unflag_message' => 'Товар удален из закладок',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 2,
        '1' => 3,
      ),
      'unflag' => array(
        '0' => 2,
        '1' => 3,
      ),
    ),
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'drushop_flags',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implementation of hook_views_api().
 */
function drushop_flags_views_api() {
  return array(
    'api' => '2',
  );
}

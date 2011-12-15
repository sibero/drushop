<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drushop_views_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function drushop_views_node_info() {
  $items = array(
    'info' => array(
      'name' => t('Информационная страница'),
      'module' => 'features',
      'description' => t('Этот тип материала предназначен для создания информационных страниц, таких как: "о сайте", "доставка", "гарантии" и т.д. Комментарии по умолчанию отключены.'),
      'has_title' => '1',
      'title_label' => t('Заголовок'),
      'has_body' => '1',
      'body_label' => t('Содержимое'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'news' => array(
      'name' => t('Новость'),
      'module' => 'features',
      'description' => t('Тип материала для добавления новостей на сайт.'),
      'has_title' => '1',
      'title_label' => t('Заголовок'),
      'has_body' => '1',
      'body_label' => t('Содержимое'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'page' => array(
      'name' => t('Статья'),
      'module' => 'features',
      'description' => t('Этот тип материала предназначен для создания статей. По умолчанию пользователи могут комментировать материал. '),
      'has_title' => '1',
      'title_label' => t('Заголовок'),
      'has_body' => '1',
      'body_label' => t('Содержимое'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function drushop_views_views_api() {
  return array(
    'api' => '2',
  );
}

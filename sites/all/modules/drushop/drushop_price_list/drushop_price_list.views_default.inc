<?php

/**
 * Implementation of hook_views_default_views().
 */
function drushop_price_list_views_default_views() {
  $views = array();

  // Exported view: price
  $view = new view;
  $view->name = 'price';
  $view->description = '';
  $view->tag = 'Прайс лист';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'По умолчанию', 'default');
  $handler->override_option('fields', array(
    'model' => array(
      'label' => 'SKU',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 1,
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'model',
      'table' => 'uc_products',
      'field' => 'model',
      'override' => array(
        'button' => 'Переопределить',
      ),
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Product name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Переопределить',
      ),
    ),
    'sell_price' => array(
      'label' => 'Sell price',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'set_precision' => 0,
      'precision' => '0',
      'decimal' => '.',
      'separator' => ',',
      'format_plural' => 0,
      'format_plural_singular' => '1',
      'format_plural_plural' => '@count',
      'prefix' => '',
      'suffix' => '',
      'format' => 'uc_price',
      'revision' => 'themed',
      'exclude' => 0,
      'id' => 'sell_price',
      'table' => 'uc_products',
      'field' => 'sell_price',
      'override' => array(
        'button' => 'Переопределить',
      ),
      'relationship' => 'none',
    ),
    'weight' => array(
      'label' => 'Weight',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 1,
      'set_precision' => 0,
      'precision' => '0',
      'decimal' => '.',
      'separator' => ',',
      'format_plural' => 0,
      'format_plural_singular' => '1',
      'format_plural_plural' => '@count',
      'prefix' => '',
      'suffix' => '',
      'format' => 'uc_weight',
      'exclude' => 0,
      'id' => 'weight',
      'table' => 'uc_products',
      'field' => 'weight',
      'override' => array(
        'button' => 'Переопределить',
      ),
      'relationship' => 'none',
    ),
    'tid_1' => array(
      'label' => 'Manufacturer',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'не указан',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(
        3 => 3,
        1 => 0,
        8 => 0,
        5 => 0,
        4 => 0,
        7 => 0,
        2 => 0,
      ),
      'exclude' => 0,
      'id' => 'tid_1',
      'table' => 'term_node',
      'field' => 'tid',
      'override' => array(
        'button' => 'Переопределить',
      ),
      'relationship' => 'none',
    ),
    'tid' => array(
      'label' => 'Category',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'не указан',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(
        1 => 1,
        3 => 0,
        8 => 0,
        5 => 0,
        4 => 0,
        7 => 0,
        2 => 0,
      ),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'override' => array(
        'button' => 'Переопределить',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'sticky' => array(
      'order' => 'DESC',
      'id' => 'sticky',
      'table' => 'node',
      'field' => 'sticky',
      'relationship' => 'none',
    ),
    'created' => array(
      'order' => 'DESC',
      'granularity' => 'minute',
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'is_product' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'is_product',
      'table' => 'uc_products',
      'field' => 'is_product',
      'relationship' => 'none',
    ),
    'language' => array(
      'operator' => 'in',
      'value' => array(
        '***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
        '***NO_LANGUAGE***' => '***NO_LANGUAGE***',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'language',
      'table' => 'node',
      'field' => 'language',
      'relationship' => 'none',
    ),
    'title' => array(
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'title_op',
        'identifier' => 'title',
        'label' => 'By name product:',
        'bef_filter_description' => '',
        'remember' => 0,
      ),
      'case' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'sell_price_1' => array(
      'operator' => '>=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'sell_price_1_op',
        'identifier' => 'sell_price_up',
        'label' => 'Price up:',
        'bef_filter_description' => '',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'sell_price_1',
      'table' => 'uc_products',
      'field' => 'sell_price',
      'override' => array(
        'button' => 'Переопределить',
      ),
      'relationship' => 'none',
    ),
    'sell_price' => array(
      'operator' => '<=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'sell_price_op',
        'identifier' => 'sell_price_to',
        'label' => 'Price to:',
        'bef_filter_description' => '',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'sell_price',
      'table' => 'uc_products',
      'field' => 'sell_price',
      'relationship' => 'none',
    ),
    'term_node_tid_depth' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'term_node_tid_depth_op',
        'identifier' => 'term_cat',
        'label' => 'By category:',
        'bef_filter_description' => '',
        'bef_format' => 'default',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'type' => 'select',
      'limit' => TRUE,
      'vid' => '1',
      'depth' => '10',
      'id' => 'term_node_tid_depth',
      'table' => 'node',
      'field' => 'term_node_tid_depth',
      'hierarchy' => 1,
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'term_node_tid_depth_1' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'term_node_tid_depth_1_op',
        'identifier' => 'term_man',
        'label' => 'By manufacturer:',
        'bef_filter_description' => '',
        'bef_format' => 'default',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'type' => 'select',
      'limit' => TRUE,
      'vid' => '3',
      'depth' => '10',
      'id' => 'term_node_tid_depth_1',
      'table' => 'node',
      'field' => 'term_node_tid_depth',
      'hierarchy' => 1,
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'rules',
  ));
  $handler->override_option('title', 'Прайс лист');
  $handler->override_option('css_class', 'grid-center');
  $handler->override_option('empty', 'По запросу ничего не найдено.');
  $handler->override_option('empty_format', '2');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'asc',
    'columns' => array(
      'model' => 'model',
      'title' => 'title',
      'weight' => 'weight',
      'tid_1' => 'tid_1',
      'tid' => 'tid',
      'sell_price' => 'sell_price',
      'buyitnowbutton' => 'buyitnowbutton',
    ),
    'info' => array(
      'model' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'weight' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'tid_1' => array(
        'separator' => '',
      ),
      'tid' => array(
        'separator' => '',
      ),
      'sell_price' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'buyitnowbutton' => array(
        'separator' => '',
      ),
    ),
    'default' => 'title',
  ));
  $handler = $view->new_display('page', 'Страница', 'page_1');
  $handler->override_option('path', 'price');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('views_data_export', 'Экспорт в CSV', 'views_data_export_1');
  $handler->override_option('items_per_page', '0');
  $handler->override_option('style_plugin', 'views_data_export_csv');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
    'attach_text' => 'Скачать прайс лист в формате .csv',
    'provide_file' => 0,
    'filename' => '%view.csv',
    'help' => '',
    'parent_sort' => 0,
    'separator' => ';',
    'quote' => 1,
    'trim' => 1,
    'replace_newlines' => 0,
    'newline_replacement' => ',',
    'header' => 1,
    'encoding' => '',
  ));
  $handler->override_option('row_plugin', '');
  $handler->override_option('path', 'price/price.csv');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array(
    'page_1' => 'page_1',
    'default' => 0,
  ));
  $handler->override_option('sitename_title', FALSE);
  $handler->override_option('use_batch', '0');
  $handler = $view->new_display('views_data_export', 'Экспорт в Doc', 'views_data_export_2');
  $handler->override_option('items_per_page', '0');
  $handler->override_option('style_plugin', 'views_data_export_doc');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
    'attach_text' => 'Download price list in .doc',
    'provide_file' => 0,
    'filename' => '%view.doc',
    'help' => '',
    'parent_sort' => 0,
  ));
  $handler->override_option('row_plugin', '');
  $handler->override_option('path', 'price/price.doc');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array(
    'page_1' => 'page_1',
    'default' => 0,
  ));
  $handler->override_option('sitename_title', FALSE);
  $handler->override_option('use_batch', '0');
  $handler = $view->new_display('views_data_export', 'Экспорт в Xml', 'views_data_export_4');
  $handler->override_option('items_per_page', '0');
  $handler->override_option('style_plugin', 'views_data_export_xml');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
    'attach_text' => 'Download price list in .xml',
    'provide_file' => 0,
    'filename' => '%view.xml',
    'help' => '',
    'parent_sort' => 0,
    'transform' => 1,
    'transform_type' => 'dash',
  ));
  $handler->override_option('row_plugin', '');
  $handler->override_option('path', 'price/price.xml');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array(
    'page_1' => 'page_1',
    'default' => 0,
  ));
  $handler->override_option('sitename_title', FALSE);
  $handler->override_option('use_batch', '0');

  $views[$view->name] = $view;

  return $views;
}

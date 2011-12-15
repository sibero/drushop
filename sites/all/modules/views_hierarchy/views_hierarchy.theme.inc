<?php
// $Id: views_hierarchy.theme.inc,v 1.1 2010/06/04 11:13:26 joachim Exp $
/**
 * @file theme.inc
 * Contains theme functions and preprocessors.
 */

/**
 * Display the view as a hierarchy of HTML list elements.
 */
function template_preprocess_views_hierarchy_view_hierarchy(&$vars) {
  // This does the classes for us, POSSIBLY! @todo look into this.
  template_preprocess_views_view_unformatted($vars);
  
  //dsm($vars);
  $view = $vars['view'];
  // The actual result fetched from the database.
  $data_rows = $view->result;
  // The rendered output rows.
  $original_output_rows = $vars['rows'];
  //dsm($data_rows);
  //dsm($data_rows[0]);
  
  // We need the names of two fields to build the hierarchy: the field that holds
  // the id of the parent item, and the field that actually *IS* the id.
  // For example, with terms we need to know 'term_data_parent_tid' and 'tid':
  // values in 'term_data_parent_tid' point to 'tid' values.
  // Get the base field of the current view type.
  $table_data = views_fetch_data($view->base_table);
  //dsm($table_data);
  $base_field = $table_data['table']['base']['field'];
  
  // Get the parent field. Our incoming value is the index of the field handler 
  // in the $view->field array.
  // Get its field alias so we know what to look for in the row data.
  $parent_field_handler = $view->field[$view->style_plugin->options['parent_data']];
  $parent_field = $parent_field_handler->field_alias;

  // Create a lookup array between index (the key that is merely the incoming 
  // order of results) and item ID (such as nid, tid, etc).
  foreach ($data_rows as $index => $row) {
    $lookup[$row->$base_field] = $index;
  }

  // Create a tree of the $row objects.
  foreach ($data_rows as $index => $row) {
    if (!is_null($row->$parent_field)) {
      $parent_index = $lookup[$row->$parent_field];
      //dsm($parent_index);
      $data_rows[$parent_index]->_views_hierarchy_children[$index] = $row;
    }
    else {
      $hierarchy_rows[$index] = $data_rows[$index];
    }
  }
  //dsm($hierarchy_rows);

  $output = _views_hierarchy_build_row_recursive($hierarchy_rows, $original_output_rows, $vars['view']);
  //dsm($output);
  
  $vars['hierarchy_rows'] = $hierarchy_rows;
  $vars['hierarchy'] = $output;
}

/**
 * Recursive function to output the rendered items as a hierarchical list.
 *
 * @param $hierarchy_rows
 *  An array of the top-level row objects from $view->rows, keyed by the index 
 *  in that array. Children are listed with an extra property '_views_hierarchy_children'
 *  which should be an array of the same form.
 * @param $original_output_rows
 *  The rendered rows. This is just $vars['rows'] from the preprocessor.
 * @param $view
 *  The view object.
 *
 * @return
 *  A list as rendered by theme_item_list().
 */
function _views_hierarchy_build_row_recursive($hierarchy_rows, $original_output_rows, $view) {
  // Get options.
  $type = $view->style_plugin->options['type'];
  
  foreach ($hierarchy_rows as $index => $row) {
     $items[$index] = $original_output_rows[$index]; // @todo classes!
     if (isset($row->_views_hierarchy_children)) {
       $items[$index] .= _views_hierarchy_build_row_recursive($row->_views_hierarchy_children, $original_output_rows, $view);
    }
  }
  return theme('item_list', $items, NULL, $type);
}


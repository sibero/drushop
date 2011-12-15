<?php
// $Id: views_hierarchy_plugin_style_hierarchy.inc,v 1.1.2.1 2010/06/06 09:28:17 joachim Exp $
/**
 * @file
 * Contains the hierarchy style plugin.
 */

/**
 * Style plugin to render each item in an ordered or unordered hierarchical list.
 *
 * @ingroup views_style_plugins
 */
class views_hierarchy_plugin_style_hierarchy extends views_plugin_style_list {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['parent_data'] = array('default' => NULL);

    return $options;
  }

  /**
   * The options form.
   */
  function options_form(&$form, &$form_state) {
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your hierarchy settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
      return;
    }

    parent::options_form($form, $form_state);

    //dsm($this);
    //dsm($this->view->display_handler);
    //dsm($this->display);
    
    // Build list of this view's fields to select from.
   	$display_id = ($this->view->display_handler->is_defaulted('fields')) ? 'default' : $this->view->current_display;
   	$fields = $this->view->display[$display_id]->display_options['fields'];    
    //dsm($fields);

    // Get data about the fields so we can make a human-readable list. 
    $base_tables = $this->view->get_base_tables();
    $field_data = views_fetch_fields(array_keys($base_tables), 'field');
    //dsm($field_data);

    // Add a special default option when nothing is set or the saved value is
    // invalid; otherwise the first item in the dropdown is selected and the
    // user could think they don't need to update the form.
    if (is_null($this->options['parent_data']) || !isset($fields[$this->options['parent_data']])) {
      $options[] = '<Not set>';
      $default_value = 0;
    }
    else {
      $default_value = $this->options['parent_data'];
    }

    // Just to keep our head straight: 
    // - $field_id is the id of the field in the current view, and may be an alias
    //   such as 'name_1'.
    // - $field_key is the absolute id of the field's data, and is of the form
    //   table.field, such as 'term_data.name'.
    foreach ($fields as $field_id => $field) {
      $field_key = $field['table'] . '.' . $field['field'];
      $field_relationship = $field['relationship']; // @todo now we need the human name! if you like this module please file a patch.
      $field_group = $field_data[$field_key]['group'];
      $field_title = $field_data[$field_key]['title'];
      
      // Build up a label that looks the same as those in the UI field list.
      $label = '';
      if ($field_relationship != 'none') {
        $label = "($field_relationship) ";
      }
      $label .= "$field_group: $field_title";
      
      $options[$field_id] = $label;
    }
       
    $form['parent_data'] = array(
      '#type' => 'select',
      '#title' => t('Parent field'),
      '#description' => t("The field that holds the ID of an item's parent."),
      '#options' => $options,
      '#default_value' => $default_value,
      '#required' => TRUE,
    );
    
  }

  /**
   * The options form validation handler.
   */
  function options_validate($form, &$form_state) {
    // Don't let the user choose the special 'Not set' value.
    if ($form_state['values']['style_options']['parent_data'] == '0') {
      form_error($form['parent_data'], t('You must choose a parent field.'));
    }
  }

  /**
   * Validates the handler against the complete View.
   *
   * @return
   *   Empty array if the handler is valid; an array of error strings if it is not.
   */
  function validate() {
    $errors = parent::validate();

    // The row style plugin must use fields.
    if (!$this->row_plugin->definition['uses fields']) {
      $errors[] = t('The hierachy style must be used with a row style which uses fields.');
    }

    // Parent field must be set.
    if (is_null($this->options['parent_data'])) {
      $errors[] = t('The hierarchy style requires a parent field to be set.');
    }
    else {
      // Parent field setting must be a field 
      $display_id = ($this->view->display_handler->is_defaulted('fields')) ? 'default' : $this->view->current_display;

     	$fields = $this->view->display[$display_id]->display_options['fields'];
     	//dsm($fields);
     	if (!isset($fields[$this->options['parent_data']])) {
        $errors[] = t('The hierarchy style requires a parent field to be set from the current display.');
     	}
    }

    return $errors;
  }
}


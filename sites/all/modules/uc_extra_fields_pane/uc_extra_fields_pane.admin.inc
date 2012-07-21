<?php
/**
 * @file
 * Admin functions for adding, editing and deleting fields
 *
 * Form extra address fields at
 * /admin/store/settings/checkout/edit/fields
 * /admin/store/settings/addresfields/add
 * /admin/store/settings/addresfields/#/edit
 *
 * Form custom order fields at
 * /admin/store/settings/checkout/edit/extrafields
 * /admin/store/settings/extrafields/add
 * /admin/store/settings/extrafields/#/edit
 */

// -------------------------------------------------------------------
// EXTRA ADDRESS FIELDS FORM
// - uc_extra_fields_pane_addressfield_form
// -------------------------------------------------------------------

/**
 * Form to add/edit extra address fields
 * Form at /admin/store/settings/addressfields/add
 * @param array $form_state
 * @param object $field
 * @return array
 * @see uc_extra_fields_pane_addressfield_form_submit()
 */
function uc_extra_fields_pane_addressfield_form(&$form_state, $field=NULL) {
  if (empty($field) || empty($field->field_id)) {
    $field = new UCXF_AddressField();
  }
  return uc_extra_fields_pane_field_edit_form($form_state, $field);
}

// -------------------------------------------------------------------
// CUSTOM ORDER FIELDS FORM
// - uc_extra_fields_pane_customfields
// - uc_extra_fields_pane_customfield_form
// -------------------------------------------------------------------

/**
 * List of custom fields
 * @return array
 */
function uc_extra_fields_pane_customfields() {
  $form['fields'] = array(
    '#tree' => TRUE,
  );
  $fields = UCXF_FieldList::getAllCustomFields();

  $columns = array(
    'label',
    'db_name',
    'pane_type',
    'required',
    'weight',
    'description',
  );

  if (count($fields)) {
    foreach ($fields as $field) {
      $form['fields'][$field->db_name] = array();
      $form['fields'][$field->db_name]['field_id'] = array(
        '#type' => 'value',
        '#value' => $field->field_id,
      );
      foreach ($columns as $column) {
        switch ($column) {
          case 'weight':
            $form['fields'][$field->db_name][$column] = array(
              '#type' => 'weight',
              '#delta' => 30,
              '#default_value' => $field->$column,
              '#attributes' => array('class' => 'ucxf-fields-table-ordering'),
            );
            break;
          case 'required':
            $form['fields'][$field->db_name][$column] = array(
              '#value' => ($field->$column)? t('Yes'):t('No'),
            );
            break;
          default:
            $form['fields'][$field->db_name][$column] = array(
              '#value' => $field->output($column),
            );
            break;
        }
      }
      $links = array();
      if (uc_extra_fields_pane_access($field, 'edit')) {
        $links['edit'] = l(t('edit'), 'admin/store/settings/extrafields/' . $field->field_id . '/edit');
      }
      if (uc_extra_fields_pane_access($field, 'delete')) {
        $links['delete'] = l(t('delete'), 'admin/store/settings/extrafields/' . $field->field_id . '/delete');
      }
      if (count($links) > 0) {
        $form['fields'][$field->db_name]['action'] = array(
          '#value' => implode(' | ', $links),
        );
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Saves weights of custom fields
 * @param array $form
 * @param array $form_state
 * @return void
 */
function uc_extra_fields_pane_customfields_submit($form, &$form_state) {
  foreach ($form_state['values']['fields'] as $field) {
    drupal_write_record('uc_extra_fields', $field, array('field_id'));
  }
}

/**
 * Form to add/edit custom order fields
 * Form at /admin/store/settings/extrafields/add
 * @param array $form_state
 * @param UCXF_Field $field
 * @return array
 */
function uc_extra_fields_pane_customfield_form(&$form_state, $field=NULL) {
  if (empty($field) || empty($field->field_id)) {
    $field = new UCXF_CustomField();
  }
  return uc_extra_fields_pane_field_edit_form($form_state, $field);
}

// -----------------------------------------------------------------------------
// EDIT FORMS
// -----------------------------------------------------------------------------

/**
 * A form callback to edit a field.
 * @param array $form_state
 * @param UCXF_Field $field
 * @return array
 */
function uc_extra_fields_pane_field_edit_form($form_state, $field) {
  $form = $field->edit_form();
  $form['#validate'][] = 'uc_extra_fields_pane_field_edit_form_validate';
  $form['#submit'][] = 'uc_extra_fields_pane_field_edit_form_submit';
  return $form;
}

/**
 * Validate the field edit form.
 * @param array $form
 * @param array $form_state
 * @return void
 */
function uc_extra_fields_pane_field_edit_form_validate($form, &$form_state) {
  $field = $form_state['values']['field'];
  $field->edit_form_validate($form, $form_state);
}

/**
 * Submit the field edit form.
 * @param array $form
 * @param array $form_state
 * @return void
 */
function uc_extra_fields_pane_field_edit_form_submit($form, &$form_state) {
  $field = $form_state['values']['field'];
  $field->edit_form_submit($form, $form_state);
}

/**
 * Fixes the option list for the radios field 'value_type' after the form
 * is build.
 * @param array $radio_field
 * @return array
 */
function uc_extra_fields_pane_field_value_type_after_build(&$radios_field) {
  // Rearrange the options
  foreach (element_children($radios_field) as $option_id) {
    $option = $radios_field[$option_id];
    $radios_field[$option_id]['#description'] = $option['#title']['#description'];
    $radios_field[$option_id]['#title'] = $option['#title']['#title'];
  }
  return $radios_field;
}

// -----------------------------------------------------------------------------
// DELETE FORMS
// -----------------------------------------------------------------------------

/**
 * Ask confirmation for deletion of a field.
 * @param array $form_state
 * @param UCXF_Field $field
 * @return array
 */
function uc_extra_fields_pane_field_delete_confirm_form(&$form_state, $field) {
  $form['field'] = array(
    '#type' => 'value',
    '#value' => $field,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to remove the field %name?', array('%name' => $field->db_name)),
      $field->returnpath,
    t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Delete a field after confirmation.
 * @param array $form
 * @param array $form_state
 * @return void
 */
function uc_extra_fields_pane_field_delete_confirm_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $field = $form_state['values']['field'];
    $field->delete();
    drupal_set_message(t('Field %name deleted.', array('%name' => $field->db_name)));
  }
  $form_state['redirect'] = $field->returnpath;
}

// -----------------------------------------------------------------------------
// THEMING
// -----------------------------------------------------------------------------

/**
 * This function overrides the theme function theme_uc_store_address_fields_form() in uc_store.module
 * Adds tabledrag, the column 'weight' and the column 'action'.
 * @param array $form
 * @return string
 */
function theme_uc_extra_fields_pane_uc_store_address_fields($form) {
  $header = array(
    t('Enabled'),
    t('Field'),
    t('Title'),
    t('Required'),
    array('data' => t('List position'), 'sort' => 'asc'),
    t('Action'),
  );

  foreach (element_children($form['fields']) as $field) {
    $row = array(
      array('data' => drupal_render($form['fields'][$field]['enabled']), 'align' => 'center'),
      drupal_render($form['fields'][$field]['default']),
      drupal_render($form['fields'][$field]['title']),
      drupal_render($form['fields'][$field]['required']),
      drupal_render($form['fields'][$field]['weight']),
    );

    // Add Edit/Delete link to fields that are editable.
    if (isset($form['fields'][$field]['field_id'])) {
      $row[] = drupal_render($form['fields'][$field]['action']);
    }
    else {
      $row[] = '';
    }

    $rows[$field] = array(
      'data' => $row,
      'class' => 'draggable',
      '#weight' => $form['fields'][$field]['#weight'],
    );
  }

  // Sort rows by weight.
  uasort($rows, 'element_sort');

  drupal_add_tabledrag('uc-address-fields-table', 'order', 'sibling', 'uc-address-fields-table-ordering');

  $output = theme('table', $header, $rows, array('id' => 'uc-address-fields-table')) . drupal_render($form);

  return $output;
}

/**
 * Lists all custom order fields
 * @param array $form
 * @return string
 */
function theme_uc_extra_fields_pane_customfields($form) {
  $output = "";

  $header = array();
  $header[] = array('data' => t('Label'));
  $header[] = array('data' => t('Field name'));
  $header[] = array('data' => t('Pane type'));
  $header[] = array('data' => t('Required'));
  $header[] = array('data' => t('List position'));
  $header[] = array('data' => t('Description'));
  $header[] = array('data' => t('Action'));
  $rows = array();
  foreach (element_children($form['fields']) as $field) {
    $row = array(
      drupal_render($form['fields'][$field]['label']),
      drupal_render($form['fields'][$field]['db_name']),
      drupal_render($form['fields'][$field]['pane_type']),
      drupal_render($form['fields'][$field]['required']),
      drupal_render($form['fields'][$field]['weight']),
      drupal_render($form['fields'][$field]['description']),
      drupal_render($form['fields'][$field]['action']),
    );
    $rows[] = array(
      'data' => $row,
      'class' => 'draggable',
    );
  }

  if (count($rows) == 0) {
    $rows[] = array(
      array('data' => t('No custom order fields have been added yet.'), 'colspan' => '7')
    );
  }

  drupal_add_tabledrag('ucxf-fields-table', 'order', 'sibling', 'ucxf-fields-table-ordering');

  $output = theme('table', $header, $rows, array('id' => 'ucxf-fields-table')) . theme('pager', NULL, 30)
          . l(t('Add a custom order field'), 'admin/store/settings/extrafields/add') . '<br />'
          . drupal_render($form);

  return $output;
}

<?php
/**
 * @file
 * These functions adds extra address fields to forms
 */

// -------------------------------------------------------------------
// FORM ALTERS (extra address fields)
// -------------------------------------------------------------------

// --------------------------
// uc_store_address_fields
// --------------------------

/**
 * _uc_extra_fields_pane_address_fields_uc_store_address_fields_alter()
 * Adds extra address fields to form
 * which can be defined at /admin/store/settings/addressfields/add
 * @param array $form
 * @param array $form_state
 * @access private
 * @see _uc_extra_fields_pane_address_fields_uc_store_address_fields_submit()
 */
function _uc_extra_fields_pane_address_fields_uc_store_address_fields_alter(&$form, $form_state) {
  try {
    $fields = UCXF_FieldList::getFieldsFromPane(array('extra_delivery', 'extra_billing'));

    // Similar to uc_store_address_fields_form() from uc_store.module
    foreach ($fields as $fieldname => $field) {
      if (1) {
        $form['fields'][$fieldname]['#summary callback'] = 'summarize_form';
        $form['fields'][$fieldname]['enabled'] = array(
          '#type' => 'checkbox',
          '#summary callback' => 'summarize_checkbox',
          '#summary arguments' => array(
            t('@field is enabled.', array('@field' => $field->label)),
            t('@field is disabled.', array('@field' => $field->label)),
          ),
          '#default_value' => ($field->enabled) ? TRUE : FALSE,
        );
        $form['fields'][$fieldname]['required'] = array(
          '#type' => 'checkbox',
          '#default_value' => ($field->required) ? TRUE : FALSE,
        );
      }
      $form['fields'][$fieldname]['default'] = array(
        '#value' => $field->db_name,
      );
      $form['fields'][$fieldname]['title'] = array(
        '#value' => $field->output('label'),
      );
      // Add id of field
      $form['fields'][$fieldname]['field_id'] = array(
        '#type' => 'value',
        '#value' => $field->field_id,
      );
      // Add action links
      $links = array();
      if (uc_extra_fields_pane_access($field, 'edit')) {
        $links['edit'] = l(t('edit'), 'admin/store/settings/addressfields/' . $field->field_id . '/edit');
      }
      if (uc_extra_fields_pane_access($field, 'delete')) {
        $links['delete'] = l(t('delete'), 'admin/store/settings/addressfields/' . $field->field_id . '/delete');
      }
      if (count($links) > 0) {
        $form['fields'][$fieldname]['action'] = array(
          '#value' => implode(' | ', $links),
        );
      }
    }

    // Add link to add address field
    $form['add_field'] = array(
      '#type' => 'markup',
      '#value' => l(t('Add an address field'), 'admin/store/settings/addressfields/add') . '<br /><br />',
      '#weight' => 2,
    );
    $form['save']['#weight'] = 3;
    $form['reset']['#weight'] = 4;

    // Add submit function so 'enabled' and 'required' can be saved
    $form['#submit'][] = 'uc_extra_fields_pane_address_fields_uc_store_address_fields_submit';
  }
  catch (UCXF_Exception $e) {
    $e->printMessage();
    $e->logError();
  }
}

/**
 * uc_extra_fields_pane_address_fields_uc_store_address_fields_submit()
 * Saves 'enabled' and 'required' for address fields
 * @param array $form
 * @param array $form_state
 * @see _uc_extra_fields_pane_address_fields_uc_store_address_fields_alter()
 */
function uc_extra_fields_pane_address_fields_uc_store_address_fields_submit($form, $form_state) {
  try {
    $address_fields = UCXF_FieldList::getFieldsFromPane(array('extra_delivery', 'extra_billing'));
    $fields = $form_state['values']['fields'];
    foreach ($address_fields as $fieldname => $field) {
      db_query("UPDATE {uc_extra_fields} SET enabled=%d, required=%d, weight=%d WHERE field_id=%d", $fields[$fieldname]['enabled'], $fields[$fieldname]['required'], $fields[$fieldname]['weight'], $fields[$fieldname]['field_id']);
    }
  }
  catch (UCXF_Exception $e) {
    $e->printMessage();
    $e->logError();
  }
}

// --------------------------
// uc_cart_checkout_form
// --------------------------

/**
 * Fixes drop down 'saved addresses' and prefills extra address fields when UC Addresses is enabled.
 * @param array $form
 * @param array $form_state
 * @access private
 * @see uc_extra_fields_pane_form_uc_cart_checkout_form_alter()
 */
function uc_extra_fields_pane_address_fields_uc_cart_checkout_form_alter(&$form, $form_state) {
  if (uc_addresses_version() === 2) {
    return;
  }
  global $user;

  try {
    // Check if there are extra address fields
    $fields = UCXF_FieldList::getFieldsFromPane(array('extra_delivery', 'extra_billing'));
    if (count($fields) < 1) {
      // No address fields. No need to alter form
      return;
    }

    // Include file with code to get addresses
    module_load_include('inc', 'uc_extra_fields_pane', 'includes/address_select');

    // --------------------------
    // Alter address selection of delivery and billing pane
    // --------------------------
    $types = array('delivery', 'billing');
    foreach ($types as $type) {
      if (isset($form['panes'][$type][$type . '_address_select'])) {
        $default_address = NULL;
        $addresses = uc_extra_fields_pane_get_addresses($user->uid, $type);
        $options = array('0' => t('Select one...'));

        foreach ($addresses as $key => $address) {
          if (isset($address->address_name) && $address->address_name) {
            // Use address name as label for the option if available
            $option = $address->address_name;
          }
          elseif (uc_addresses_version() === 1) {
            // Format option label following uc_addresses 6.x-1.x logics
            $option = preg_replace('/<.*?>/', ', ',
              uc_address_format(
                $address->first_name,
                $address->last_name,
                $address->company,
                $address->street1,
                $address->street2,
                $address->city,
                $address->zone,
                $address->postal_code,
                $address->country
              )
            );
            // MegaChriz: the next lines are useful when using the (unfinished) address display feature,
            // see also http://drupal.org/node/904050
            if (module_exists('ucxfdev_addressdisplay') && function_exists('ucxfdev_addressdisplay_format_address')) {
              $option = ucxfdev_addressdisplay_format_address($address, '', $option);
            }
          }
          else {
            // Format option label following uc_store logics
            $option = $address->street1;
            // Check if the address is a duplicate (i.e. same address, but sent to different person)
            if ((isset($addresses[$key - 1]) && $option == $addresses[$key - 1]->street1) ||
                (isset($addresses[$key + 1]) && $option == $addresses[$key + 1]->street1)) {
              $option .= ' - ' . $address->first_name . ' ' . $address->last_name;
            }
          }
          $addresskey = drupal_to_js($address);
          $options[$addresskey] = check_plain($option);
          if (uc_addresses_version() === 1 && $address->is_default) {
            // If this address is the default, hold the key so the default value
            // for the address selection field can be set later.
            $default_address = $addresskey;
          }
        }

        // Apply options
        $form['panes'][$type][$type . '_address_select']['#options'] = $options;
        $form['panes'][$type][$type . '_address_select']['#attributes']['onchange'] .= "ucxf_apply_address('" . $type . "', this.value);";
        // Apply default value for select field if there is a default address
        // and if the default address may be filled in automatically.
        if ($default_address && variable_get('uc_addresses_default_' . $type . '_address', TRUE)) {
          $form['panes'][$type][$type . '_address_select']['#default_value'] = $default_address;
        }

        // Additions to copy address behaviour (because else values of checkboxes won't be copied)
        if (isset($form['panes'][$type]['copy_address'])) {
          $source_pane = ($type == 'delivery')? 'billing':'delivery';
          $target_pane = $type;
          $form['panes'][$type]['copy_address']['#attributes']['onclick'] .= "ucxf_copy_address(this.checked, '$source_pane' , '$target_pane');";
        }
      }
    }

    // Add JS function so that data from extra address fields can be applied.
    $fields = UCXF_FieldList::getAllAddressFields();
    $script = "
      function ucxf_apply_address(type, address_str) {
        if (address_str == '0') {
          return;
        }
        eval('var address = ' + address_str + ';');
        var temp = type + '-' + type;
    ";
    foreach ($fields as $fieldname => $field) {
      if ($field->value_type == UCXF_Field::UCXF_WIDGET_TYPE_CHECKBOX) {
        $set_function = ".attr('checked', address." . $fieldname . " == 1 ? true : false)";
      }
      else {
        $set_function = ".val(address." . $fieldname . ")";
      }
      $script .= "$('#edit-panes-' + temp + '-" . form_clean_id($fieldname) . "')" . $set_function . ".trigger('change');\n";
    }
    $script .= '}';
    drupal_add_js($script, 'inline');

    // Add JS function for copying over checkbox fields
    drupal_add_js(drupal_get_path('module', 'uc_extra_fields_pane') . '/uc_extra_fields_pane.js');

    // --------------------------
    // Fill in defaults for extra address fields when UC Addresses 6.x-1.x is enabled
    // and when at least one of the following settings are true:
    // - 'uc_addresses_default_delivery_address'
    // - 'uc_addresses_default_billing_address'
    // --------------------------
    if (
      uc_addresses_version() === 1 && (
        variable_get('uc_addresses_default_delivery_address', TRUE) ||
        variable_get('uc_addresses_default_billing_address', TRUE)
      )
    ) {
      // Load default UC Addresses address if available
      $default_address = NULL;
      $addresses = _uc_addresses_db_get_address($user->uid);
      if (is_array($addresses) && count($addresses) > 0) {
        foreach ($addresses as $address) {
          if ($address->is_default) {
            $default_address = $address;
          }
        }
        if ($default_address) {
          // Load values of extra address fields for this address
          $values = uc_extra_fields_pane_value_list_load($default_address->aid, UCXF_VALUE_ADDRESS);
          foreach ($values as $oValue) {
            $fieldname = $oValue->db_name;
            $value = $oValue->value;
            // Apply default value for both delivery and billing address,
            // but only if the default value is not already set.
            if (!isset($form['panes']['delivery']['delivery_' . $fieldname]['#default_value']) && isset($form['panes']['delivery']['delivery_' . $fieldname]) && variable_get('uc_addresses_default_delivery_address', TRUE)) {
              $form['panes']['delivery']['delivery_' . $fieldname]['#default_value'] = $value;
            }
            if (!isset($form['panes']['billing']['billing_' . $fieldname]['#default_value']) && isset($form['panes']['billing']['billing_' . $fieldname]) && variable_get('uc_addresses_default_billing_address', TRUE)) {
              $form['panes']['billing']['billing_' . $fieldname]['#default_value'] = $value;
            }
          }
        }
      }
    }
  }
  catch (UCXF_Exception $e) {
    $e->printMessage();
    $e->logError();
  }
}

// --------------------------
// UC Addresses integration: uc_addresses_get_address_form
// --------------------------

/**
 * uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_alter()
 * @param array $form
 * @param array $form_state
 * @return void
 * @see uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_submit()
 */
function uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_alter(&$form, $form_state) {
  $fields = UCXF_FieldList::getFieldsFromPane(array('extra_delivery', 'extra_billing'));

  foreach ($fields as $fieldname => $field) {
    // Generate field
    $generated_field = $field->generate();

    // Set default value
    $oValue = uc_extra_fields_pane_value_load($form['panes']['address']['aid']['#value'], UCXF_VALUE_ADDRESS, $field->field_id);
    $generated_field['#default_value'] = $oValue->value;

    // Add field to pane
    $form['panes']['address'][$fieldname] = $generated_field;
  }

  // Add submit function
  $form['#submit'][] = 'uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_submit';
}

/**
 * uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_submit()
 * @param array $form
 * @param array $form_state
 * @see uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_alter()
 */
function uc_extra_fields_pane_addressfields_uc_addresses_get_address_form_submit($form, $form_state) {
  $address = $form_state['values']['panes']['address'];

  // Load field definitions
  $fields = UCXF_FieldList::getFieldsFromPane(array('extra_delivery', 'extra_billing'));

  // MegaChriz:
  //  address id is not available when address is added, only when it is edited,
  //  we need to wait until uc_addresses is improved on this part.
  //  see: drupal.org/node/851658
  //  with the patch applied in #4, $form_state['storage']['aid'] will contain the address id.

  // Get address id
  // WARNING: $form_state['storage']['aid'] will never contain anything with current version of uc_addresses (v1.0)
  $address_id = ($address['aid'])? $address['aid']:$form_state['storage']['aid'];

  // Save values
  if ($address_id) {
    foreach ($fields as $fieldname => $field) {
      uc_extra_fields_pane_value_save(
        array(
          'element_id' => $address_id,
          'element_type' => UCXF_VALUE_ADDRESS,
          'field_id' => $field->field_id,
          'value' => $address[$fieldname],
        )
      );
    }
  }
}

// --------------------------
// UC Addresses integration: user_register
// --------------------------

/**
 * uc_extra_fields_pane_addressfields_user_register_form_alter
 * @param array $form
 * @param array $form_state
 * @return void
 * @see uc_extra_fields_pane_user()
 */
function uc_extra_fields_pane_addressfields_user_register_form_alter(&$form, $form_state) {
  $fields = UCXF_FieldList::getFieldsFromPane(array('extra_delivery', 'extra_billing'));

  foreach ($fields as $fieldname => $field) {
    $form[0][$fieldname] = $field->generate();
  }
}

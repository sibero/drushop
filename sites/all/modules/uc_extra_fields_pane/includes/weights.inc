<?php
/**
 * @file
 * These functions adds weight to the address fields,
 * so it becomes possible to change the order of the address fields.
 */

// -------------------------------------------------------------------
// FORM ALTERS (address fields weight)
// -------------------------------------------------------------------

// --------------------------
// uc_store_address_fields
// --------------------------

/**
 * _uc_extra_fields_pane_weight_uc_store_address_fields_alter()
 * Adds option to order address fields by adding a weight field
 * @param array $form
 * @param array $form_state
 * @access private
 * @return void
 * @see uc_extra_fields_pane_weight_uc_store_address_fields_submit()
 * @see theme_uc_extra_fields_pane_weight_uc_store_address_fields()
 */
function _uc_extra_fields_pane_weight_uc_store_address_fields_alter(&$form, $form_state) {
  if (isset($form['uc_address_fields'])) {
    // We're dealing with an Ubercart version dated July 18, 2012 or later.
    // Some fields are structured differently.
    $uc_version_20120718 = TRUE;
    $form['uc_address_fields_weight']['#tree'] = TRUE;
  }
  else {
    $uc_version_20120718 = FALSE;
  }

  // Get weight settings.
  $weights = variable_get('uc_address_fields_weight', _uc_extra_fields_pane_getDefaultAddressFieldsWeights($form['fields']));

  // Merge weight settings with default weight settings in case extra fields were added.
  $weights = array_merge(_uc_extra_fields_pane_getDefaultAddressFieldsWeights($form['fields']), $weights);

  foreach ($weights as $fieldname => $weight) {
    if ($uc_version_20120718) {
      $form['uc_address_fields_weight'][$fieldname] = array(
        '#type' => 'weight',
        '#delta' => 30,
        '#default_value' => $weight,
        '#attributes' => array('class' => 'uc-address-fields-table-ordering'),
      );

      // Apply weight settings on fields.
      $form['fields'][$fieldname]['#weight'] = $weight;
    }
    elseif (isset($form['fields'][$fieldname])) {
      // Add weight fields.
      $form['fields'][$fieldname]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 30,
        '#default_value' => $weight,
        '#attributes' => array('class' => 'uc-address-fields-table-ordering'),
      );

      // Apply weight settings on fields.
      $form['fields'][$fieldname]['#weight'] = $weight;

      // Add submit function in order to save the weight settings.
      $form['#submit'][] = 'uc_extra_fields_pane_weight_uc_store_address_fields_submit';
    }
  }
}

/**
 * uc_extra_fields_pane_weight_uc_store_address_fields_submit()
 * Saves the weight settings for the address fields
 * @param array $form
 * @param array $form_state
 * @return void
 * @see _uc_extra_fields_pane_weight_uc_store_address_fields_alter()
 */
function uc_extra_fields_pane_weight_uc_store_address_fields_submit($form, $form_state) {
  $weights = array();
  foreach ($form_state['values']['fields'] as $fieldname => $fieldsettings) {
    $weights[$fieldname] = $fieldsettings['weight'];
  }
  variable_set('uc_address_fields_weight', $weights);
}

// --------------------------
// uc_cart_checkout_form_alter
// --------------------------

/**
 * uc_extra_fields_pane_weight_uc_cart_checkout_form_alter().
 * Applies ordering to address fields following the 'uc_address_fields_weight'-settings.
 * @param array $form
 * @param array $form_state
 * @access private
 * @return void
 */
function uc_extra_fields_pane_weight_uc_cart_checkout_form_alter(&$form, $form_state) {
  // Apply weight for delivery fields (fieldnames are prefixed with 'delivery_')
  _uc_extra_fields_pane_applyWeights($form['panes']['delivery'], 'delivery_');

  // Apply weight for billing fields (fieldnames are prefixed with 'billing_')
  _uc_extra_fields_pane_applyWeights($form['panes']['billing'], 'billing_');
}

// -------------------------------------------------------------------
// HELPER FUNCTIONS
// Helper functions to apply weights to address fields
// -------------------------------------------------------------------

/**
 * _getDefaultAddressFieldsWeights()
 * Get the default weight settings for the address fields.
 *
 * These settings will be used as a default value for variable_get('uc_address_fields_weight').
 * Because the contents of this setting will be called on several places, a general function
 * is used to get the default settings.
 *
 * If the fields don't have the #weight-attribute then the default $iWeight
 * will be considered the weight of the field, this value will be
 * increased for each field.
 *
 * @param array $fields
 * @param string $prefix
 * 	On some forms the field names are prefixed, for example on the checkout forms
 *	where the fields names are prefixed with 'delivery_' and 'billing_'
 *	The settings are saved for the fields without the prefix, so we need to load
 *  the settings without the prefix.
 * @access private
 * @return array $aWeightFields
 */
function _uc_extra_fields_pane_getDefaultAddressFieldsWeights($fields, $prefix='') {
  // Get all available fields for the weight
  $aWeightFields = array();
  $iWeight = -31;
  foreach (element_children($fields) as $fieldname) {
    $iWeight++;

    // Substract prefix from fieldname, we don't want the prefix
    // saved into the default weight settings
    $fixedfieldname = $fieldname;
    if (strlen($prefix)) {
      if (strpos($fieldname, $prefix) === 0) {
        $fixedfieldname = substr($fieldname, strlen($fieldname));
      }
    }

    if (isset($fields[$fieldname]['#weight'])) {
      $aWeightFields[$fixedfieldname] = $fields[$fieldname]['#weight'];
      $iWeight = $fields[$fieldname]['#weight'];
    }
    else {
      $aWeightFields[$fixedfieldname] = $iWeight;
    }
  }
  return $aWeightFields;
}

/**
 * _applyWeights()
 * Applies ordering to address fields following the 'uc_address_fields_weight'-settings.
 * This function is used by some form-alter functions of this module that alters address forms.
 * @param array $form_part
 * @param string $prefix
 *	On the checkout-form the address fields got prefixes: 'delivery_' & 'billing_'.
 * @access private
 * @return void
 */
function _uc_extra_fields_pane_applyWeights(&$form_part, $prefix='') {
  // Get weight settings
  $weights = variable_get('uc_address_fields_weight', _uc_extra_fields_pane_getDefaultAddressFieldsWeights($form_part, $prefix));

  // Ubercart Addresses integration:
  // If a field 'address_select' exists, give this field the lowest weight, so that this field appears at first
  if (isset($form_part[$prefix . 'address_select'])) {
    $iLowestWeight = min($weights);
    $weights['address_select'] = $iLowestWeight -1;
  }

  // Apply weight to fields
  foreach ($weights as $fieldname => $weight) {
    if (isset($form_part[$prefix . $fieldname])) {
      $form_part[$prefix . $fieldname]['#weight'] = $weight;
    }
    // Fieldtype can be a constant. In that case an extra field typed '#item' can be shown.
    if (isset($form_part[$prefix . $fieldname . '_i'])) {
      $form_part[$prefix . $fieldname . '_i']['#weight'] = $weight;
    }
  }
}
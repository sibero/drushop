<?php
/**
 * @file
 * Integration code for Ubercart Addresses 6.x-2.x
 *
 * This file implements an Ubercart Addresses field handler that is used
 * by all Extra Fields Pane fields.
 */

/**
 * Field handler for all Extra Fields Pane fields.
 */
class UcAddressesUCXFHandler extends UcAddressesFieldHandler {
  // -----------------------------------------------------------------------------
  // PROPERTIES
  // -----------------------------------------------------------------------------

  /**
   * Extra Fields Pane field object
   *
   * @var UCXF_Field
   * @access private
   */
  private $ucxf_field;

  // -----------------------------------------------------------------------------
  // CONSTRUCT
  // -----------------------------------------------------------------------------

  /**
   * Can be used by subclasses to do some initialization upon
   * construction of the object
   *
   * @access public
   * @return void
   */
  public function init() {
    parent::init();
    $fieldName = $this->getFieldName();
    $this->ucxf_field = UCXF_FieldList::getFieldByName($fieldName);
  }

  // -----------------------------------------------------------------------------
  // ACTION
  // -----------------------------------------------------------------------------

  /**
   * Returns the editable field
   *
   * @param array $form
   * @param array $form_values
   * @access public
   * @return array
   */
  public function getFormField($form, $form_values) {
    $fieldName = $this->getFieldName();
    $fieldValue = $this->getAddress()->getField($fieldName);
    $default = (isset($form_values[$fieldName]))? $form_values[$fieldName]:$fieldValue;

    $field[$fieldName] = $this->ucxf_field->generate();
    $field[$fieldName] += array(
      '#default_value' => $default,
    );
    if ($field[$fieldName]['#type'] == 'hidden' && $this->ucxf_field->may_display('checkout')) {
      $field[$fieldName . '_i'] = array(
        '#type' => 'item',
        '#title' => $this->ucxf_field->output('label'),
        '#value' => $field[$fieldName]['#value'],
      );
    }
    if ($form['#uc_addresses_required'] === FALSE || $this->isFieldRequired() === FALSE) {
      $field[$fieldName]['#required'] = FALSE;
    }

    // In case of select fields, add default value as an option if it is not an available option yet.
    if ($this->getContext() == 'order_form' && $field[$fieldName]['#type'] == 'select') {
      $default_value = $field[$fieldName]['#default_value'];
      if ($default_value != '' && !isset($field[$fieldName]['#options'][$default_value])) {
        $field[$fieldName]['#options'][$default_value] = t('Deleted option: @option', array('@option' => $default_value));
      }
    }

    return $field;
  }

  /**
   * Check to see if the field is enabled.
   *
   * @access public
   * @return boolean
   *	 TRUE if the field is enabled.
   */
  public function isFieldEnabled() {
    return $this->ucxf_field->enabled;
  }

  /**
   * Check to see if the field is required.
   *
   * @access public
   * @return boolean
   *	 TRUE if the field is required.
   */
  public function isFieldRequired() {
    if ($this->getContext() == 'order_form') {
      return FALSE;
    }
    return $this->ucxf_field->required;
  }

  // -----------------------------------------------------------------------------
  // OUTPUT
  // -----------------------------------------------------------------------------

  /**
   * Output a fields value
   *
   * @param mixed $value
   * @access public
   * @return string
   */
  public function outputValue($value = '', $format = '') {
    if ($value === '') {
      $value = $this->getAddress()->getField($this->getFieldName());
    }
    return $this->ucxf_field->output_value($value);
  }
}

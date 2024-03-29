<?php
/**
 * @file
 * Field handlers for Ubercart core address fields:
 * first_name, last_name, company, etc.
 */

/**
 * Base class for Ubercart core address fields
 */
abstract class UcAddressesUcFieldHandler extends UcAddressesFieldHandler {
  /**
   * Override of UcAddressesFieldHandler::getFieldTitle().
   *
   * Returns the title to use when displaying the field.
   *
   * @access public
   * @return string
   *	 The field title.
   */
  public function getFieldTitle() {
    return uc_get_field_name($this->getFieldName());
  }

  /**
   * Check to see if the field is enabled.
   *
   * @access public
   * @return boolean
   *	 TRUE if the field is enabled.
   */
  public function isFieldEnabled() {
    return uc_address_field_enabled($this->getFieldName());
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
    return uc_address_field_required($this->getFieldName());
  }
}

/**
 * Class for an Ubercart core address text field
 */
class UcAddressesUcTextFieldHandler extends UcAddressesUcFieldHandler {
  /**
   * Returns the editable field
   *
   * @param array $form
   * @param array $form_values
   * @abstract
   * @access public
   * @return array
   */
  public function getFormField($form, $form_values) {
    $fieldName = $this->getFieldName();
    $fieldValue = $this->getAddress()->getField($fieldName);
    $default = (isset($form_values[$fieldName])) ? $form_values[$fieldName] : $fieldValue;

    return array(
      $fieldName => array(
        '#type' => 'textfield',
        '#title' => $this->getFieldTitle(),
        '#required' => $this->isFieldRequired(),
        '#default_value' => $default,
        '#size' => 32,
      ),
    );
  }
}

/**
 * Class for the Ubercart zone field
 */
class UcAddressesUcZoneFieldHandler extends UcAddressesUcFieldHandler {
  /**
   * Returns the editable field
   *
   * @param array $form
   * @param array $form_values
   * @abstract
   * @access public
   * @return array
   */
  public function getFormField($form, $form_values) {
    $fieldName = $this->getFieldName();
    $fieldValue = $this->getAddress()->getField($fieldName);
    $default = (isset($form_values[$fieldName])) ? $form_values[$fieldName] : $fieldValue;
    $country_id = (isset($form_values['country'])) ? $form_values['country'] : $this->getAddress()->getField('country');

    if (empty($country_id)) {
      $country_id = uc_store_default_country();
    }

    $result = db_query("SELECT * FROM {uc_zones} WHERE zone_country_id = %d ORDER BY zone_name", $country_id);

    $options[''] = t('Please select');
    while ($zone = db_fetch_object($result)) {
      $options[$zone->zone_id] = $zone->zone_name;
    }

    if (count($options) == 1) {
      $options = array(-1 => t('Not applicable'));
    }

    return array(
      $fieldName => array(
        '#type' => 'select',
        '#title' => $this->getFieldTitle(),
        '#required' => $this->isFieldRequired(),
        '#options' => $options,
        '#default_value' => $default,
        '#prefix' => '<div id="uc-address-' . $this->getAddress()->getId() . '-zone">',
        '#suffix' => '<span class="zone-throbber"></span></div>',
      ),
    );
  }

  /**
   * Returns a default value for this field.
   *
   * Subclasses can override this method to provide a default
   * value for their field.
   *
   * @access public
   * @return int
   */
  public function getDefaultValue() {
    return 0;
  }

  /**
   * Returns an array of output formats for the zone field.
   *
   * @access public
   * @return array
   */
  public function getOutputFormats() {
    return array(
      'zone_code' => t('Abbreviation of the zone'),
      'zone_name' => t('Full name of the zone'),
    );
  }

  /**
   * Output zone name.
   *
   * @param mixed $value
   *   The value to output
   * @param string $format
   *   The format in which the value should be outputted.
   *   Possible formats are declared by field handlers: getOutputFormats().
   * @access public
   * @return string
   */
  public function outputValue($value = '', $format = '') {
    if ($value === '') {
      $value = $this->getAddress()->getField($this->getFieldName());
    }

    // Get zone data
    $result = db_query("SELECT * FROM {uc_zones} WHERE zone_id = %d", $value);
    if (!($zone_data = db_fetch_array($result))) {
      $zone_data = array('zone_code' => t('N/A'), 'zone_name' => t('Unknown'));
    }

    if (isset($zone_data[$format])) {
      return $zone_data[$format];
    }

    // If no format is specified, return zone name
    return $zone_data['zone_name'];
  }
}

/**
 * Class for the Ubercart country field
 */
class UcAddressesUcCountryFieldHandler extends UcAddressesUcFieldHandler {
  /**
   * @var array $countries
   *   An array of all loaded countries
   * @access private
   * @static
   */
  private static $countries;

  /**
   * Returns the editable field
   *
   * @param array $form
   * @param array $form_values
   * @abstract
   * @access public
   * @return array
   */
  public function getFormField($form, $form_values) {
    $fieldName = $this->getFieldName();
    $fieldValue = $this->getAddress()->getField($fieldName);
    $default = (isset($form_values[$fieldName])) ? $form_values[$fieldName] : $fieldValue;

    $result = db_query("SELECT * FROM {uc_countries} WHERE version > 0 ORDER BY country_name");

    $options = array();
    while ($country = db_fetch_object($result)) {
      // Create option
      $options[$country->country_id] = t($country->country_name);
      // Save for later use
      self::$countries[$country->country_id] = $country;
    }
    if (count($options) == 0) {
      $options[] = t('No countries found.');
    }

    drupal_add_js(drupal_get_path('module', 'uc_store') .'/uc_country_select.js');

    // Ensure the path setting only gets added once.
    static $added = FALSE;
    if (!$added) {
      drupal_add_js(array('ucURL' => array('zoneSelect' => url('uc_js_util/zone_select'))), 'setting');
      $added = TRUE;
    }

    return array(
      $fieldName => array(
        '#type' => 'select',
        '#title' => $this->getFieldTitle(),
        '#options' => $options,
        '#required' => $this->isFieldRequired(),
        '#default_value' => empty($default) ? uc_store_default_country() : $default,
      ),
    );
  }

  /**
   * Returns a default value for this field.
   *
   * Subclasses can override this method to provide a default
   * value for their field.
   *
   * @access public
   * @return int
   */
  public function getDefaultValue() {
    return variable_get('uc_store_country', 840);
  }

  /**
   * Returns an array of output formats country field.
   *
   * @access public
   * @return array
   */
  public function getOutputFormats() {
    return array(
      'country_name' => t('Name of the country'),
      'country_code2' => t('2 digit country abbreviation'),
      'country_code3' => t('3 digit country abbreviation'),
      'country_name_if' => $this->getCountryDescriptionMessage(t('Name of the country')),
      'country_code2_if' => $this->getCountryDescriptionMessage(t('2 digit country abbreviation')),
      'country_code3_if' => $this->getCountryDescriptionMessage(t('3 digit country abbreviation')),
    );
  }

  /**
   * Used in getOutputFormats() to output a description for the country format
   *
   * @access private
   * @return string
   */
  private function getCountryDescriptionMessage($country_description) {
    return t('!country_description, display only for addresses whose country is different than the default store country.',
      array('!country_description' => $country_description)
    );
  }

  /**
   * Output country name
   *
   * @param mixed $value
   *   The value to output
   * @param string $format
   *   The format in which the value should be outputted.
   *   Possible formats are declared by field handlers: getOutputFormats().
   * @access public
   * @return string
   */
  public function outputValue($value = '', $format = '') {
    if ($value === '') {
      $value = $this->getAddress()->getField($this->getFieldName());
    }

    $country = $this->getCountry($value);
    if (!$country) {
      return t('Unknown');
    }

    // Return country only if the country is not equal to the store's default country
    if (preg_match('/\_if$/', $format)) {
      if (uc_store_default_country() == $country->country_id) {
        return '';
      }
    }

    switch ($format) {
      case 'country_name':
      case 'country_name_if':
        return t($country->country_name);
      case 'country_code2':
      case 'country_code2_if':
        return $country->country_iso_code_2;
      case 'country_code3':
      case 'country_code3_if':
        return $country->country_iso_code_3;
    }

    // If no format is specified, return country name
    return t($country->country_name);
  }

  /**
   * Returns country data
   *
   * @param int $country_id
   * @return object
   *   Data of country
   */
  private function getCountry($country_id) {
    if (isset(self::$countries[$country_id])) {
      return self::$countries[$country_id];
    }

    $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = %d", $country_id);
    if ($country = db_fetch_object($result)) {
      self::$countries[$country->country_id] = $country;
      return $country;
    }
    return NULL;
  }
}

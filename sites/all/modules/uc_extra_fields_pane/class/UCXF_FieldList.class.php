<?php
/**
 * @file
 * Contains the UCXF_FieldList class.
 */

/**
 * This class is used to keep track of all loaded fields in one request.
 * It's also used as a central place to request fields.
 */
class UCXF_FieldList {
  // -----------------------------------------------------------------------------
  // CONSTANTS
  // -----------------------------------------------------------------------------

  // Load by constants
  const BY_ID = 1;
  const BY_NAME = 2;

  // -----------------------------------------------------------------------------
  // STATIC PROPERTIES
  // -----------------------------------------------------------------------------

  /**
   * An array of all loaded fields
   * @var array
   * @access private
   * @static
   */
  private static $fields = array();

  /**
   * An array of boolean static which panes are loaded
   * @var array
   * @access private
   * @static
   */
  private static $loadedPanes = array();

  /**
   * Whether or not all fields are loaded
   * @var boolean
   * @access private
   * @static
   */
  private static $allLoaded = FALSE;

  // -----------------------------------------------------------------------------
  // CONSTRUCT
  // -----------------------------------------------------------------------------

  /**
   * Create a field based on pane type
   * @param string $pane_type
   * @access public
   * @static
   * @return UCXF_Field
   */
  public static function createField($pane_type) {
    $pane_types = explode('|', $pane_type);

    if (
      in_array('extra_delivery', $pane_types) ||
      in_array('extra_billing', $pane_types)
    ) {
      $field = new UCXF_AddressField();
    }
    else {
      $field = new UCXF_CustomField();
    }
    return $field;
  }

  /**
   * Resets the field list.
   *
   * This will remove all fields currently tracked by the field
   * list. All the properties will be set back to the default
   * values.
   *
   * Calling this method is bad for performance as it will force to
   * reload fields from the database, so use it with caution.
   *
   * This method is generally only of use within automated tests.
   *
   * @access public
   * @static
   * @return void
   */
  public static function reset() {
    self::$fields = array();
    self::$loadedPanes = array();
    self::$allLoaded = FALSE;
  }

  // -----------------------------------------------------------------------------
  // GETTERS
  // -----------------------------------------------------------------------------

  /**
   * Get a single field by ID
   * @param int $field_id
   * @access public
   * @static
   * @return UCXF_Field
   */
  public static function getFieldByID($field_id) {
    self::loadOne(self::BY_ID, $field_id);
    if (isset(self::$fields[$field_id])) {
      return self::$fields[$field_id];
    }
  }

  /**
   * Get a single field by name
   * @param string $db_name
   * @access public
   * @static
   * @return UCXF_Field
   */
  public static function getFieldByName($db_name) {
    self::loadOne(self::BY_NAME, $db_name);
    return self::findByName($db_name);
  }

  /**
   * Get all fields from a particular pane type
   * @param mixed $pane_types
   *   string: fields from single pane are loaded
   *   array: fields from multiple panes are loaded
   * @access public
   * @static
   * @return array
   */
  public static function getFieldsFromPane($pane_types) {
    if (is_string($pane_types)) {
      $pane_types = array($pane_types);
    }
    $fields = array();
    foreach ($pane_types as $pane_type) {
      self::loadAllFromPane($pane_type);
      $fields = array_merge($fields, self::findByPane($pane_type));
    }
    return $fields;
  }

  /**
   * Get all available fields
   * @access public
   * @static
   * @return array
   */
  public static function getAllFields() {
    self::loadAll();
    return self::$fields;
  }

  /**
   * Get all available address fields
   * @access public
   * @static
   * @return array
   */
  public static function getAllAddressFields() {
    return self::getFieldsFromPane(array('extra_delivery', 'extra_billing'));
  }

  /**
   * Get all available custom order fields
   * @access public
   * @static
   * @return array
   */
  public static function getAllCustomFields() {
    self::loadAll();
    $fields = array();
    foreach (self::$fields as $field) {
      if (!$field->in_pane('extra_delivery') && !$field->in_pane('extra_billing')) {
        $fields[$field->db_name] = $field;
      }
    }
    return $fields;
  }

  // -----------------------------------------------------------------------------
  // DELETE
  // -----------------------------------------------------------------------------

  /**
   * Deletes an field from the database by giving the field object
   *
   * @param UCXF_Field $field
   * @return boolean
   * @static
   * @throws UCXF_DbException
   */
  public static function deleteField(UCXF_Field $field) {
    return self::deleteOne(self::BY_ID, $field->field_id);
  }

  /**
   * Deletes a field by ID from the database
   *
   * @param int $field_id
   *   The id of the field
   * @access public
   * @static
   * @return boolean
   * @throws UCXF_DbException
   */
  public static function deleteFieldById($field_id) {
    return self::deleteOne(self::BY_ID, $field_id);
  }

  /**
   * Deletes a field by db_name from the database
   *
   * @param string $db_name
   *   The nickname of the address
   * @access public
   * @static
   * @return boolean
   * @throws UCXF_DbException
   */
  public static function deleteFieldByName($db_name) {
    return self::deleteOne(self::BY_NAME, $db_name);
  }

  // -----------------------------------------------------------------------------
  // DATABASE REQUESTS
  // -----------------------------------------------------------------------------

  /**
   * Load a single field if not already loaded
   *
   * No database call is done in these cases:
   * - Field is already loaded
   *
   * @param int $type
   *   Type of the argument given, can be the field id (BY_ID) or the field db_name (BY_NAME)
   * @param mixed $arg
   *   Either the field id or the field db_name
   * @access private
   * @static
   * @return void
   * @throws UCXF_DbException
   */
  private static function loadOne($type, $arg) {
    // Reasons to skip out early
    if (self::$allLoaded) {
      return;
    }
    if ($type == self::BY_ID && isset(self::$fields[$arg])) {
      return;
    }
    if ($type == self::BY_NAME && self::findByName($arg)) {
      return;
    }

    if ($type == self::BY_ID) {
      $result = db_query("SELECT * FROM {uc_extra_fields} WHERE field_id = %d", $arg);
    }
    elseif ($type == self::BY_NAME) {
      $result = db_query("SELECT * FROM {uc_extra_fields} WHERE db_name = '%s'", $arg);
    }
    if ($result === FALSE) {
      throw new UCXF_DbException(t('Failed to read from database table uc_extra_fields'));
    }

    self::dbResultToField($result);
  }

  /**
   * Loads all fields from a specific pane type
   * @param string $pane_type
   * @access private
   * @static
   * @return void
   * @throws UCXF_DbException
   */
  private static function loadAllFromPane($pane_type) {
    // Reasons to skip out early
    if (self::$allLoaded) {
      return;
    }
    if (isset(self::$loadedPanes[$pane_type])) {
      return;
    }

    //$query = "SELECT * FROM {uc_extra_fields} WHERE pane_type LIKE '%|%s|%'";
    $query = "SELECT * FROM {uc_extra_fields} WHERE pane_type LIKE '%%%s%%'";
    $result = db_query($query, $pane_type);
    if ($result === FALSE) {
      throw new UCXF_DbException(t('Failed to read from database table uc_extra_fields'));
    }

    // Set flag that fields for this pane are loaded
    self::$loadedPanes[$pane_type] = TRUE;

    self::dbResultToField($result);
  }

  /**
   * Loads all fields
   * @access private
   * @static
   * @return void
   * @throws UCXF_DbException
   */
  private static function loadAll() {
    // Reasons to skip out early
    if (self::$allLoaded) {
      return;
    }

    $query = "SELECT * FROM {uc_extra_fields}";
    $result = db_query($query);
    if ($result === FALSE) {
      throw new UCXF_DbException(t('Failed to read from database table uc_extra_fields'));
    }

    // Set flag that all fields are loaded
    self::$allLoaded = TRUE;

    self::dbResultToField($result);
  }

  /**
   * Creates UCXF_Field objects from a database resource.
   *
   * @param resource $result
   *   Database result
   * @access private
   * @static
   * @return void
   */
  private static function dbResultToField($result) {
    // Create each UCXF_Field object from the database record
    while ($field_data = db_fetch_array($result)) {
      // Skip fields that have already been loaded (and perhaps modified)
      if (!isset(self::$fields[$field_data['field_id']])) {
        $field = self::createField($field_data['pane_type']);

        // Populate field information
        $field->from_array($field_data);

        // Add field to array
        self::$fields[$field->field_id] = $field;

        // Give other modules to react on this
        module_invoke_all('ucxf_field', $field, 'load');
      }
    }
  }

  /**
   * Deletes one field
   *
   * @param int $type
   *   Type of the argument given, can be the field id (BY_ID) or the field db_name (BY_NAME)
   * @param mixed $arg
   *   Either the field id or the field db_name
   * @access private
   * @static
   * @return boolean
   * @throws UCXF_DbException
   */
  private static function deleteOne($type, $arg) {
    // Make sure the field is loaded
    self::loadOne($type, $arg);
    if ($type == self::BY_ID) {
      $field = self::getFieldById($arg);
    }
    if ($type == self::BY_NAME) {
      $field = self::getFieldByName($arg);
    }
    if (!$field) {
      // Field does not exists.
      return FALSE;
    }

    // Delete the field
    $result = db_query("DELETE FROM {uc_extra_fields} WHERE field_id = %d", $field->id);
    if ($result === FALSE || db_affected_rows() == 0) {
      throw new UCXF_DbException(t('Failed to delete a field from database table uc_extra_fields'));
    }

    // Remove field from list
    unset(self::$fields[$field->id]);

    // Give other modules a chance to react on this
    module_invoke_all('ucxf_field', $field, 'delete');
    return TRUE;
  }

  // -----------------------------------------------------------------------------
  // SEARCH
  // -----------------------------------------------------------------------------

  /**
   * Search for a field by giving the db_name
   *
   * @param string $name
   *   The db_name of the field
   * @access private
   * @static
   * @return
   *   UCXF_Field if field is found
   *   FALSE otherwise
   */
  private static function findByName($name) {
    if ($name) {
      foreach (self::$fields as $field) {
        if (isset($field->db_name) && $field->db_name == $name) {
          return $field;
        }
      }
    }
    return FALSE;
  }

  /**
   * Search for fields by giving the pane_type
   *
   * @param string $pane_type
   *   The pane_type of the field
   * @access private
   * @static
   * @return array
   */
  private static function findByPane($pane_type) {
    $fields = array();
    if ($pane_type) {
      foreach (self::$fields as $field) {
        if ($field->in_pane($pane_type)) {
          $fields[$field->db_name] = $field;
        }
      }
    }
    return $fields;
  }
}

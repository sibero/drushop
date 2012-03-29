<?php
/**
 * @file
 * Contains the UCXF_Pane class.
 */

/**
 * Base class for checkout panes and order panes implemented by Extra Fields Pane.
 *
 * The extra information pane uses this class directly, while the delivery and
 * billing panes make use of UCXF_AddressPane instead.
 */
class UCXF_Pane {
  // -----------------------------------------------------------------------------
  // CONSTANTS
  // -----------------------------------------------------------------------------

  const PANE_CHECKOUT = 'checkout';
  const PANE_ORDER = 'order';

  // -----------------------------------------------------------------------------
  // PROPERTIES
  // -----------------------------------------------------------------------------

  /**
   * Pane ID
   * @var string
   * @access protected
   */
  protected $pane_id;

  /**
   * Ubercart pane type
   * 'order' or 'checkout'
   * @var string
   * @access protected
   */
  protected $uc_pane_type;

  /**
   * Order object
   * @var object
   * @access protected
   */
  protected $order;

  /**
   * Filled in values in pane form
   * @var array
   * @access protected
   */
  protected $values;

  // -----------------------------------------------------------------------------
  // STATIC PROPERTIES
  // -----------------------------------------------------------------------------

  /**
   * An array of all processed order panes
   * Used by uc_extra_fields_pane_order().
   * @var array
   * @access private
   */
  private static $processed_order_panes = array();

  // -----------------------------------------------------------------------------
  // CONSTRUCT
  // -----------------------------------------------------------------------------

  /**
   * UCXF_Pane object constructor
   * @param string $pane_type
   * @param string $uc_pane_type
   * @param object $order
   * @param mixed $arg2
   * @access public
   * @return void
   */
  public function __construct($pane_id, $uc_pane_type, $order, &$values) {
    $this->pane_id = $pane_id;
    $this->uc_pane_type = $uc_pane_type;
    $this->order = $order;
    $this->values = $values;
  }

  // -----------------------------------------------------------------------------
  // ACTION
  // -----------------------------------------------------------------------------

  /**
   * Executes an operation
   * @param string $op
   * @return mixed
   */
  public function execute($op) {
    try {
      // Replace hyphens with underscores
      $op = str_replace('-', '_', $op);

      switch ($this->uc_pane_type) {
        case self::PANE_CHECKOUT:
          switch ($op) {
            // Supported operations for checkout
            case 'view':
            case 'process':
            case 'review':
              return $this->{$this->uc_pane_type . '_' . $op}();
          }
          break;
        case self::PANE_ORDER:
          switch ($op) {
            // Supported operations for order
            case 'customer':
            case 'view':
            case 'edit_form':
            case 'edit_theme':
            case 'edit_process':
              return $this->{$this->uc_pane_type . '_' . $op}();
          }
          break;
      }
      return NULL;
    }
    catch (UCXF_Exception $e) {
      $e->printMessage();
      $e->logError();
    }
  }

  // -----------------------------------------------------------------------------
  // CHECKOUT
  // -----------------------------------------------------------------------------

  /**
   * View the pane to the customer
   * @access protected
   * @return string
   */
  protected function checkout_view() {
    $fields = $this->loadFields();
    $contents = array();
    // Dynamically generate form elements
    $description = '';
    if (count($fields) > 0) {
      foreach ($fields as $field) {
        if ($field->enabled) {
          $generated_field = $field->generate();
          $order_field_name = $this->getFieldName($field);

          // Adding default value for every field except for hidden fields (php and constant)
          if (isset($this->order->$order_field_name) && $generated_field['#type'] !== 'hidden') {
            $generated_field['#default_value'] = $this->order->$order_field_name;
          }

          // Add field
          $contents[$order_field_name] = $generated_field;

          // If the field happens to be a hidden field, display value if the user asks
          // This currently applies to value_type of php and constant ONLY
          if ($generated_field['#type'] == 'hidden' && $field->may_display('checkout')) {
            $contents[$order_field_name . '_i'] = array(
              '#type' => 'item',
              '#title' => $field->output('label'),
              '#value' => $generated_field['#value'],
            );
          }
        }
      }
    }
    return array(
      'description' => $description,
      'contents' => $contents,
      'theme' => 'uc_extra_fields_pane_checkout_pane'
    );
  }

  /**
   * Processes filled in values
   * @access public
   * @return protected
   */
  protected function checkout_process() {
    // If there were hidden fields, add them to the values array
    if (isset($this->values['hidden_fields'])) {
      foreach ($this->values['hidden_fields'] as $fieldname => $value) {
        $this->values[$fieldname] = $value;
      }
    }

    foreach ($this->values as $fieldname => $value) {
      $this->order->$fieldname = $value;
    }
    return TRUE;
  }

  /**
   * Outputs data for the review page
   * @access protected
   * @return string
   */
  protected function checkout_review() {
    $fields = $this->loadFields();
    if (count($fields)) {
      foreach ($fields as $field) {
        // Display it as data
        $order_field_name = $this->getFieldName($field);
        if ($field->may_display('review')) {
          $data = $field->output_value($this->order->$order_field_name);
          $review[] = array(
            'title' => $field->output('label'),
            'data' => $data,
          );
        }
      }
      return $review;
    }
  }

  // -----------------------------------------------------------------------------
  // ORDER
  // -----------------------------------------------------------------------------

  /**
   * View the order data for the customer
   * @access protected
   * @return string
   */
  protected function order_customer() {
    return $this->order_view();
  }

  /**
   * View the order data
   * @access protected
   * @return string
   */
  protected function order_view() {
    $fields = $this->loadFields();
    $output = '';

    // Load values (depends on pane type)
    switch ($this->pane_id) {
      case 'delivery':
        $values = uc_extra_fields_pane_value_list_load($this->order->order_id, UCXF_VALUE_ORDER_DELIVERY);
        break;
      case 'billing':
        $values = uc_extra_fields_pane_value_list_load($this->order->order_id, UCXF_VALUE_ORDER_BILLING);
        break;
      default:
        $values = uc_extra_fields_pane_value_list_load($this->order->order_id, UCXF_VALUE_ORDER_INFO);
        break;
    }
    $custom_order_fields = array();

    if (count($fields)) {
      foreach ($fields as $field) {
        if (isset($values[$field->field_id])) {
          // Only display if it may be displayed
          if ($field->may_display('order')) {
            $field_contents = $field->output_value($values[$field->field_id]->getValue());
            $custom_order_fields[] = '<strong>' . $field->output('label') . '</strong>: ' . $field_contents;
          }
        }
      }
    }

    if (count($custom_order_fields)) {
      $output .= '<br />' . implode('<br />', $custom_order_fields);
    }
    return $output;
  }

  /**
   * View the editable form
   * @access protected
   * @return array
   */
  protected function order_edit_form() {
    $fields = $this->loadFields();

    $form['ucxf_' . $this->pane_id] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional order information'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    // Dynamically generate form elements
    if (count($fields)) {
      foreach ($fields as $field) {
        if ($field->enabled) {
          $generated_field = $field->generate();
          $order_field_name = $this->getFieldName($field);
          $generated_field['#default_value'] = isset($this->order->$order_field_name) ? $this->order->$order_field_name : NULL;

          // On the order edit form, a generated field shouldn't be a hidden field.
          // In this case the field will be set to a normal textfield, so it's editable.
          if ($generated_field['#type'] == 'hidden') {
            $generated_field['#type'] = 'textfield';
            // Unset value, field already has an default value
            unset($generated_field['#value']);
            // Set title and size of field.
            $generated_field['#title'] = $field->output('label');
            $generated_field['#size'] = 32;
          }

          // In case of select fields, add default value as an option if it is not an available option yet.
          if ($generated_field['#type'] == 'select') {
            $default_value = $generated_field['#default_value'];
            if ($default_value != '' && !isset($generated_field['#options'][$default_value])) {
              $generated_field['#options'][$default_value] = t('Deleted option: @option', array('@option' => $default_value));
            }
          }

          // Do not make any field required on the edit form
          $generated_field['#required'] = FALSE;
          // Add generated field to form
          $form['ucxf_' . $this->pane_id][$order_field_name] = $generated_field;
        }
      }
    }
    return $form;
  }

  /**
   * Theme the editable form
   * @access protected
   * @return string
   */
  protected function order_edit_theme() {
    $fields = $this->loadFields();

    // Remove pane_fields that are not enabled
    foreach ($fields as $fieldname => $field) {
      if (!$field->enabled && isset($this->order['ucxf_' . $this->pane_id][$fieldname])) {
        unset($this->order['ucxf_' . $this->pane_id][$fieldname]);
      }
    }

    $output = theme('uc_extra_fields_pane_order_pane', $this->order['ucxf_' . $this->pane_id]);
    return $output;
  }

  /**
   * Process order data
   * @access protected
   * @return array
   */
  protected function order_edit_process() {
    $fields = $this->loadFields();
    $pane_name = $this->getPaneName();

    if (count($fields)) {
      foreach ($fields as $field) {
        $order_field_name = $this->getFieldName($field);
        if (isset($this->order['ucxf_' . $pane_name][$order_field_name])) {
          $changes[$order_field_name] = $this->order['ucxf_' . $pane_name][$order_field_name];
        }
        elseif (isset($this->order[$order_field_name])) {
          $changes[$order_field_name] = $this->order[$order_field_name];
        }
      }
    }
    self::$processed_order_panes[] = 'ucxf_' . $pane_name;
    return $changes;
  }

  // -----------------------------------------------------------------------------
  // STATIC METHODS
  // -----------------------------------------------------------------------------

  /**
   * Returns all order panes processed by order_edit_process
   * This method is used in uc_extra_fields_pane_order().
   * @access public
   * @return array
   */
  public static function getProcessedOrderPanes() {
    return self::$processed_order_panes;
  }

  // -----------------------------------------------------------------------------
  // UTIL
  // -----------------------------------------------------------------------------

  /**
   * Load fields from db
   * @access protected
   * @return array
   */
  protected function loadFields() {
    return UCXF_FieldList::getFieldsFromPane('extra_' . $this->pane_id);
  }

  /**
   * Create key for field
   * @param UCXF_Field $field
   * @access protected
   * @return string
   */
  protected function getFieldName($field) {
    return $this->pane_id . '_' . $field->db_name;
  }

  /**
   * Returns name of original pane
   * @access protected
   * @return string
   */
  protected function getPaneName() {
    return $this->pane_id;
  }
}

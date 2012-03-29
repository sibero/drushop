<?php
/**
 * @file
 * Contains the UCXF_AddressPane class.
 */

/**
 * Class that deals with checkout panes and order panes especially for the address
 * fields.
 *
 * Basically, this class is used to inherit code from the original delivery and
 * billing checkout panes and the original ship_to and bill_to order panes defined
 * by Ubercart.
 */
class UCXF_AddressPane extends UCXF_Pane {
  // -----------------------------------------------------------------------------
  // ACTION
  // -----------------------------------------------------------------------------

  /**
   * Executes an operation
   * @param string $op
   * @return mixed
   */
  public function execute($op) {
    // Execute operation following the logics of UCXF_Pane first
    $result = parent::execute($op);
    // Check if operation was succesfull
    if (is_null($result)) {
      // No result, follow logics of original address panes
      return $this->getOriginalPane($op);
    }
    return $result;
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
    $extra_pane = parent::checkout_view();

    // Get contents from original pane
    $original_pane = $this->getOriginalPane('view');
    // Loop through all extra address fields
    foreach ($extra_pane['contents'] as $fieldname => $field) {
      // Adding default value for every field except for hidden fields (php and constant)
      if ($field['#type'] == 'hidden') {
        // If the field is a hidden field, the field will be added to the array 'hidden_fields',
        // because else it will be displayed as if it was a 'normal' field.
        $extra_pane['contents']['hidden_fields'][$fieldname] = $field;
        unset($extra_pane['contents'][$fieldname]);
      }
    }

    // Merge contents
    $pane = $original_pane;
    $pane['contents'] = array_merge($original_pane['contents'], $extra_pane['contents']);
    return $pane;
  }

  /**
   * Process values
   * @access protected
   * @return boolean
   */
  protected function checkout_process() {
    // Process original pane first
    $original_pane = $this->getOriginalPane('process');
    return parent::checkout_process();
  }

  /**
   * Outputs data for the review page
   * @access protected
   * @return string
   */
  protected function checkout_review() {
    // Get contents from original pane
    $original_pane = $this->getOriginalPane('review');

    // Extra address fields
    $review = array();
    $fields = $this->loadFields();
    if (count($fields)) {
      foreach ($fields as $field) {
        // Only display if the field is enabled and if it may be displayed.
        if ($field->enabled == 1 && $field->may_display('review')) {
          // Get field name
          $order_field_name = $this->getFieldName($field);
          // Initialize field contents
          $field_contents = $field->output_value($this->order->$order_field_name);
          $review[] = array(
            'title' => $field->output('label'),
            'data' => $field_contents,
          );
        }
      }
    }

    // Merge data original pane with data from extra address fields
    $review = array_merge($original_pane, $review);
    return $review;
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
    $output_original_pane = $this->getOriginalPane('customer');
    // If original address pane has no output, don't return output here either.
    if (!$output_original_pane) {
      return;
    }
    $output = parent::order_view();
    if ($output) {
      $output = $output_original_pane . '<br />' . $output;
    }
    else {
      $output = $output_original_pane;
    }
    return $output;
  }

  /**
   * View the order data
   * @access protected
   * @return string
   */
  protected function order_view() {
    $output = parent::order_view();
    $output_original_pane = $this->getOriginalPane('view');
    if ($output) {
      $output = $output_original_pane . '<br />' . $output;
    }
    else {
      $output = $output_original_pane;
    }
    return $output;
  }

  /**
   * View the editable form
   * @access protected
   * @return array
   */
  protected function order_edit_form() {
    $form = parent::order_edit_form();
    $form_original_pane = $this->getOriginalPane('edit-form');
    $pane_name = $this->getPaneName();
    $form = array(
      $pane_name => array_merge($form['ucxf_' . $this->pane_id], $form_original_pane[$pane_name])
    );
    unset($form[$pane_name]['#tree']);
    return $form;
  }

  /**
   * Theme the editable form
   * @access protected
   * @return string
   */
  protected function order_edit_theme() {
    $pane_name = $this->getPaneName();
    // Sort fields here, because parent function renders the fields individual.
    // Use Drupal function "element_sort" to sort fields
    uasort($this->order[$pane_name], "element_sort");
    return $this->getOriginalPane('edit-theme');
  }

  /**
   * Process order data
   * @access protected
   * @return array
   */
  protected function order_edit_process() {
    $changes = parent::order_edit_process();
    $changes_original_pane = $this->getOriginalPane('edit-process');
    $changes = array_merge($changes, $changes_original_pane);
    return $changes;
  }

  // -----------------------------------------------------------------------------
  // UTIL
  // -----------------------------------------------------------------------------

  /**
   * Loads data from original address panes
   * @param string $op
   * @access private
   * @return mixed
   */
  private function getOriginalPane($op) {
    $pane_name = $this->getPaneName();
    switch ($this->uc_pane_type) {
      case self::PANE_CHECKOUT:
        $function = 'uc_checkout_pane_' . $pane_name;
        return $function($op, $this->order, $this->values);
      case self::PANE_ORDER:
        $function = 'uc_order_pane_' . $pane_name;
        return $function($op, $this->order);
    }
  }

  /**
   * Returns name of original pane
   * @access protected
   * @return string
   */
  protected function getPaneName() {
    switch ($this->uc_pane_type) {
      case self::PANE_CHECKOUT:
        return $this->pane_id;
      case self::PANE_ORDER:
        switch ($this->pane_id) {
          case 'delivery':
            return 'ship_to';
          case 'billing':
            return 'bill_to';
        }
        break;
    }
  }
}

<?php
/**
 * @file
 * Contains the UCXF_CustomField class.
 */

/**
 * Class for a Extra Fields Pane Custom field.
 *
 * Custom fields are shown on checkout and on the order administration pages
 * in the extra information pane.
 */
class UCXF_CustomField extends UCXF_Field {
  // -----------------------------------------------------------------------------
  // CONSTRUCT
  // -----------------------------------------------------------------------------

  /**
   * UCXF_CustomField object constructor
   * @access public
   * @return void
   */
  public function __construct() {
    parent::__construct();
    $this->returnpath = 'admin/store/settings/checkout/edit/extrafields';
  }
}
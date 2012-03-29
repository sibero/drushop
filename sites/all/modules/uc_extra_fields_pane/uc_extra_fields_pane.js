/**
 * @file
 * Javascript functions for Extra Fields Pane
 */

/**
 * Behavior for the copy address checkbox for copying checkbox values.
 *
 * This function adds support for copying values of checkboxes over from one
 * address pane to another. The original uc_cart_copy_address() wasn't able
 * to do this.
 *
 * @param boolean checked
 *   If the checkbox for copy over the address is checked
 * @param string source
 *   The address pane to copy values from: 'delivery' or 'billing'
 * @param string target
 *   The address pane to copy values to: 'delivery' or 'billing'
 * @return void
 * @see uc_cart_copy_address() in uc_cart/uc_cart.js
 */
function ucxf_copy_address(checked, source, target) {
  if (checked) {
    if (target == 'billing') {
      var x = 28;
    }
    else {
      var x = 26;
    }

    // Copy over checkbox information from delivery to billing or from billing to delivery.
    $('#' + source + '-pane input').each(
      function() {
        if (this.id.substring(0, x) == 'edit-panes-' + source + '-' + source && this.type == 'checkbox') {
          var source_field = 'edit-panes-' + source + '-' + source + this.id.substring(x);
          var target_field = 'edit-panes-' + target + '-' + target + this.id.substring(x);
          $('#' + target_field).attr('checked', $(this).attr('checked'));
          if (target == 'billing') {
            $(this).change(function () { ucxf_update_field(source_field, target_field); });
          }
          else {
            $(this).change(function () { ucxf_update_field(source_field, target_field); });
          }
        }
      }
    );
  }
}

/**
 * Copy a value from one pane to another when the source field has been changed.
 *
 * It's meant to copy checkboxes data over, but it can also do text and select fields.
 *
 * @param string source_field
 *   ID of the field that has been changed
 * @param string target_field
 *   ID of the field to update
 * @return void 
 */
function ucxf_update_field(source_field, target_field) {
  if (copy_box_checked) {
    if ($('#' + source_field).attr('type') == 'checkbox') {
      $('#' + target_field).attr('checked', $('#' + source_field).attr('checked')).change();
    }
    else {
      source_value = $('#' + source_field).val();
      $('#' + target_field).val(source_value).change();
    }
  }
}

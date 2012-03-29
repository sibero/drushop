<?php
/**
 * @file
 * Theme functions for the panes
 */

/**
 * Theme the extra fields forms on the checkout page.
 * @param array $form
 * @return string
 * @ingroup themeable
 */
function theme_uc_extra_fields_pane_checkout_pane($form) {
  $req = '<span class="form-required">*</span>';
  $output = '<div class="uc-extra-fields-pane-table address-pane-table"><table>';
  foreach (element_children($form) as $field) {
    if ($form[$field]['#type'] != 'hidden') {
      $title = $form[$field]['#title'];
      unset($form[$field]['#title']);
      $output .= '<tr><td class="field-label">';
      if ($form[$field]['#required']) {
        $output .= $req;
      }
      if (!empty($title)) {
        // Add title with colon only if title is not empty
        $output .= '<strong>' . $title .':</strong>';
      }
      $output .= '</td><td>'. drupal_render($form[$field]) .'</td></tr>';
    }
  }
  $output .= '</table></div>';

  foreach (element_children($form) as $element) {
    $output .= drupal_render($form[$element]);
  }

  return $output;
}

/**
 * Theme the extra fields forms on the order edit page.
 * @param array $form
 * @return string
 * @ingroup themeable
 */
function theme_uc_extra_fields_pane_order_pane($form) {
  $output = '';
  if (count(element_children($form)) > 0) {
    $output .= '<table class="order-edit-table">';
    foreach (element_children($form) as $key => $field) {
      $title = $form[$field]['#title'];
      unset($form[$field]['#title']);
      unset($form[$field]['#description']);
      if (!empty($title)) {
        // Add colon only if title is not empty
        $title .= ':';
      }
      $output .= '<tr><td class="oet-label">' . $title . '</td><td>' . drupal_render($form[$field]) . '</td></tr>';
    }
    $output .= '</table>';
  }
  return $output;
}
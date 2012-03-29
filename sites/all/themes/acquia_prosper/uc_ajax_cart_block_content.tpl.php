<?php
/**
 * @file
 *
 * Theme file for non empty cart.
 */
?>

<table>
  <tbody>
    <tr>
      <td class="cart-block-summary-items">
        <?php print $items_text; ?>
      </td>
      <td class="cart-block-summary-total">
        <label><?php print t('Total'); ?>: </label><?php print $total ;?>
      </td>
    </tr>
    <tr class="cart-block-summary-links">
      <td colspan="2">
        <?php print $cart_links; ?>
      </td>
    </tr>
  </tbody>
</table>

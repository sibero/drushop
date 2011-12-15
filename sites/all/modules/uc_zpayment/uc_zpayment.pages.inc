<?php
// $Id: uc_zpayment.pages.inc,v 1.1.2.1 2009/01/18 22:07:04 ibabushka Exp $

function uc_zpayment_complete (){

  watchdog('zpayment', 'Receiving new order notification for order !order_id.', array('!order_id' => check_plain($_POST['LMI_PAYMENT_NO'])));
	
	$cart_id = intval($_POST['LMI_PAYMENT_NO']);
	
	$order = uc_order_load($cart_id);
	
	if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
    print t('An error has occurred during payment.  Please contact us to ensure your order has submitted.');
    exit();
  }
  
  
  if (strtolower($_POST['CLIENT_MAIL']) !== strtolower($order->primary_email)) {
    uc_order_comment_save($order->order_id, 0, t('Customer used a different e-mail address during payment: !email', array('!email' => check_plain($_POST['CLIENT_MAIL']))), 'admin');
  }
  
	$url = 'cart/zpayment/finalize/'. $order->order_id;
	uc_zpayment_redirect_url($url);

}


function uc_zpayment_result () {
	drupal_goto('cart');
}
 
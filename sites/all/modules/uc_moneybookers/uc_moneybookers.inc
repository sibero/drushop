<?php
// $Id$

function uc_moneybookers_payment_made() {
  $order_id = intval($_SESSION['cart_order']);
  return drupal_get_form('uc_moneybookers_payment_made_form', $order_id);
}
function uc_moneybookers_payment_made_form($form_state, $order_id) {
  $order = uc_order_load($order_id);
  uc_cart_complete_sale($order);

  sleep(3);
  $result = db_query("SELECT COUNT(*) FROM {uc_moneybookers_notification} WHERE order_id = %d and status = 2",
										$order_id);
  $count = db_result($result);
										
  if( $count == 1) {
    $form['uc_moneybookers_payment_made'] = array(
      '#value' => '<strong>' . t('Your payment was handled successfully.') . '</strong><br/>' . 
                 t('We will process your order immediately.') . '<br/>' . 
                 t('You can check the status of your order in your profile.') . '<br/>' . 
                 t('Feel free to continue shopping on our site.'),
      '#prefix' => '<div class="moneybookers_infotext">', 
      '#suffix' => '</div>', 
    );
  } else {
    $form['uc_moneybookers_payment_made'] = array(
      '#value' => '<strong>' . t('Your payment was handled successfully.') . '</strong><br/>' . 
                  t('We will process your order as soon as the we\'ll receive the payment.') . '<br/>' . 
                  t('You can check the status of your order in your profile.') . '<br/>' .
                  t('Feel free to continue shopping on our site.'),
      '#prefix' => '<div class="moneybookers_infotext">', 
      '#suffix' => '</div>', 
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('continue'),
  );
  $form['#action'] = url('cart');
	
  return $form;
}

function uc_moneybookers_payment_cancelled() {
  return drupal_get_form('uc_moneybookers_payment_cancelled_form', $order_id);
}

function uc_moneybookers_payment_cancelled_form() {
  $form['uc_moneybookers_payment_cancelled_form'] = array(
    '#value' => '<strong>' . t('Your payment was canceld.') . '</strong><br/>' . 
                t('Feel free to continue shopping on our site.'),
    '#prefix' => '<div class="moneybookers_infotext">', 
    '#suffix' => '</div>', 
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('continue'),
  );
  $form['#action'] = url('cart');

  return $form;
}

/**
 *	Function to receive the notification from Moneybookers about
 *	handled payment
 */
function uc_moneybookers_notification() {

//	does the call have a transaction id? If not, forget it!
  if (!isset($_POST['transaction_id'])) {
    watchdog('uc_moneybookers', 'Notification attempt without order ID.', array(), WATCHDOG_ERROR);
    return;
  }
//	Is the md5 hash valid?	
  if ( !uc_moneybookers_md5check($_POST['merchant_id'], $_POST['transaction_id'], 
        $_POST['mb_amount'], $_POST['currency'], $_POST['status'], $_POST['md5sig'])) {
        watchdog('uc_moneybookers', 'Wrong MD5 signature.', array(), WATCHDOG_ERROR);
     return;			
  }
//	Do we have the corresponding order?
  $order_id = intval($_POST['transaction_id']);
  $order = uc_order_load($order_id);
  if ($order == FALSE) {
    watchdog('uc_moneybookers', 'Notification attempt for non-existent order.', array(), WATCHDOG_ERROR);
    return;
  }

  $pay_to_email = check_plain($_POST['pay_to_email']);
  $pay_from_email = check_plain($_POST['pay_from_email']);
  $merchant_id = intval($_POST['merchant_id']);
  $customer_id = intval($_POST['customer_id']);
  $mb_transaction_id = check_plain($_POST['mb_transaction_id']);
  $mb_amount = check_plain($_POST['mb_amount']);
  $mb_currency = check_plain($_POST['mb_currency']);
  $status = intval($_POST['status']);
  $md5sig = check_plain($_POST['md5sig']);
  $amount = check_plain($_POST['amount']);
  $currency = check_plain($_POST['currency']);
  $payment_type = check_plain($_POST['payment_type']);

  db_query("INSERT INTO {uc_moneybookers_notification} (order_id, receiver_email, payer_email, merchant_id,
           customer_id, mb_transaction_id, mb_amount, mb_currency, status, amount, currency, payment_type,
           received) VALUES (%d, '%s', '%s', %d, %d, '%s', '%s', '%s', %d,  '%s', '%s', '%s', %d)",
           $order_id, $pay_to_email, $pay_from_email, $merchant_id, $customer_id, $mb_transaction_id, 
           $mb_amount, $mb_currency, $status, $amount, $currency, $payment_type, time());
	
  $context = array(
    'revision' => 'formatted-original',
    'location' => 'Moneybookers',
  );
  $options = array(
    'sign' => FALSE,
  );

  switch($status) {
    case 2:		// processed
      $comment = t('Moneybookers transaction ID: @mb_transaction_id and Payment Type: @payment_type', 
        array('@mb_transaction_id' => $mb_transaction_id, '@payment_type' => _uc_moneybookers_payment_type($payment_type)));
      uc_payment_enter($order_id, 'Moneybookers', $amount, $order->uid, NULL, $comment);
      uc_cart_complete_sale($order);
      uc_order_comment_save($order_id, 0, t('Payment of @amount @currency submitted through Moneybookers.', 
      	array('@amount' => uc_price($amount, $context, $options), '@currency' => $currency)),
       	'order', 'payment_received');
      uc_order_comment_save($order_id, 0, t('Moneybookers reported a payment of @amount @currency.', 
      	array('@amount' => uc_price($amount, $context, $options), '@currency' => $currency)));
      break;
    case 0:		// pending
      uc_order_update_status($order_id, 'pending');
      uc_order_comment_save($order_id, 0, t('Payment is pending at Moneybookers.')); 
      break;
    case -1:	// canceled
      uc_order_update_status($order_id, 'canceled');
      uc_order_comment_save($order_id, 0, t('Payment canceled at Moneybookers.')); 
      break;
    case -2:	// failed
      uc_order_update_status($order_id, 'canceled');
      uc_order_comment_save($order_id, 0, t('Payment  failed at Moneybookers.'));
      break;
    case -3:	// chargeback
      watchdog('uc_moneybookers', 'Chargeback Notification received.', array(), WATCHDOG_NOTICE);
      break;
	}

}

function uc_moneybookers_md5check($merchant_id, $transaction_id, $mb_amount, $mb_currency, $status, $mb_md5sig) {
  $md5value = strtoupper(md5(variable_get('uc_moneybookers_secret_word', '')));
  $txt = $merchant_id . $transaction_id . $md5value . $mb_amount . $mb_currency . $status;
  if(strtoupper(md5($txt)) == $mb_md5sig) return TRUE;
  return FALSE;
}

function _uc_moneybookers_payment_type($type) {
  switch ($type) {
    case 'WLT':
      return 'Moneybookers Wallet';
    case 'VSA':
      return 'Visa';
    case 'MSC':
      return 'MasterCard';
    case 'VSD':
      return 'Visa Delta/Debit';
    case 'VSE':
      return 'Visa Electron';
    case 'MAE':
      return 'Maestro';
    case 'SLO':
      return 'Solo';
    case 'AMX':
      return 'American Express';
    case 'DIN':
      return 'Diners';
    case 'JCB':
      return 'JCB';
    case 'LSR':
      return 'Laser';
    case 'GCB':
      return 'Carte Bleue';
    case 'DNK':
      return 'Dankort';
    case 'PSP':
      return 'PostePay';
    case 'CSI':
      return 'CartaSi';
    case 'GIR':
      return 'Giropay';
    case 'DID':
      return 'Direct Debit';
    case 'SFT':
      return 'Sofortueberweisung';
    case 'NPY':
      return 'EPS';
    case 'ENT':
      return 'eNETS';
    case 'EPY':
      return 'ePay';
    case 'IDL':
      return 'iDeal';
    case 'SO2':
      return 'Nordea Solo';
    case 'EBT':
      return 'Nordea Solo';
    case 'PLI':
      return 'Poli';
    case 'PWY5':
      return 'ING Bank Slaski';
    case 'PWY6':
      return 'PKO BP';
    case 'PWY7':
      return 'Multibank';
    case 'PWY14':
      return 'Lukas Bank';
    case 'PWY15':
      return 'Bank BPH';
    case 'PWY17':
      return 'InvestBank';
    case 'PWY18':
      return 'PeKaO S.A.';
    case 'PWY19':
      return 'Citibank handlowy';
    case 'PWY20':
      return 'Bank Zachodni WBK';
    case 'PWY21':
      return 'BGZ';
    case 'PWY22':
      return 'Millenium';
    case 'PWY25':
      return 'mBank';
    case 'PWY26':
      return 'Place z Inteligo';
    case 'PWY28':
      return 'Bank Ochrony Drodowiska';
    case 'PWY32':
      return 'Nordea';
    case 'PWY33':
      return 'Fortis Bank';
    case 'PWY37':
      return 'Kredyt Bank';
    default:
      return $type;
  }
}

<?php

/**
 * @file
 * Rules hook functions
 */

/**
 * Implementation of hook_rules_event_info().
 */
function uc_rules_rules_event_info() {
  
  $items['uc_rules_order_state_update'] = array(
    'label' => t('Order state is updated'),
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'old_order_status' => array(
        'type' => 'string',
        'label' => t('Old Order Status'),
      ),
      'new_order_status' => array(
        'type' => 'string',
        'label' => t('New Order Status'),
      ),
      
    ),
    'module' => 'Ubercart Rules',
  );
  
  $items['uc_rules_order_state_update_line_item'] = array(
    'label' => t('Order state is updated - per order line'),
    'arguments' => array(
     'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Updated Order'),
      ),
      'uc_line_item' => array(
        'type' => 'uc_line_item',
        'label' => t('Updated Order Line item'),
      ),
      'old_order_status' => array(
        'type' => 'string',
        'label' => t('Old Order Status'),
      ),
      'new_order_status' => array(
        'type' => 'string',
        'label' => t('New Order Status'),
      ),
    ),
    'module' => 'Ubercart Rules',
  );
   
  $items['uc_rules_order_checkout_complete'] = array(
    'label' => t('Order checkout complete'),
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Updated Order'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
    ),
    'module' => 'Ubercart Rules',
  ); 
   
  $items['uc_rules_payment_entered'] = array(
    'label' => t('Payment entered'),
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
    ),
    'module' => 'Ubercart Rules',
  ); 

  return $items;
}

/**
 * Implementaion of hook_rules_action_info()
 */
function uc_rules_rules_action_info() {
  $items = array();
  $items['uc_rules_load_customer_user'] = array(
    'label' => t('Load customer by order (User)'),
    'module' => 'Ubercart Rules',
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
    'new variables' => array(
      'user_loaded' => array('type' => 'user', 'label' => t('Loaded user')),
    ),
  );
  
  $items['uc_rules_load_product_node'] = array(
    'label' => t('Load product node by order line item.'),
    'module' => 'Ubercart Rules',
    'arguments' => array(
      'uc_line_item' => array(
        'type' => 'uc_line_item',
        'label' => t('Ubercart Order Line Item'),
      ),
    ),
    'new variables' => array(
      'node_loaded' => array('type' => 'node', 'label' => t('Loaded node')),
    ),
  );
  
  $items['uc_rules_load_order_by_id'] = array(
    'label' => t('Load Ubercart Order by id'),
    'module' => 'Ubercart Rules',
    'arguments' => array(),
    'eval input' => array('uc_order_id'),
    'new variables' => array(
      'uc_order_loaded' => array('type' => 'uc_order', 'label' => t('Loaded Ubercart Order')),
    ),
  );
  
  
  $items['uc_rules_send_order_email_invoice'] = array(
    'label' => t('Sends the invoice email'),
    'module' => 'Ubercart Rules',
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
    'eval input' => array('from', 'addresses', 'subject'),
  );
  
  $items['uc_rules_order_add_comment'] = array(
    'label' => t('Adds a comment to the order'),
    'module' => 'Ubercart Rules',
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
    'eval input' => array('comment'),
  );
  
  $items['uc_rules_order_update_status'] = array(
    'label' => t('Updates the order status'),
    'module' => 'Ubercart Rules',
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
    'eval input' => array('comment'),
  );
  
  return $items;
}



/**
 * Implementation of hook_rules_condition_info().
 * @ingroup rules
 */
function uc_rules_rules_condition_info() {
  $items['uc_rules_order_has_status'] = array(
    'label' => t('Filter order by status'),
    'arguments' => array(
      'uc_order' => array(
        'type' => 'uc_order',
        'label' => t('Ubercart Order'),
      ),
    ),
    'help' => t('Evaluates to TRUE, if the order has the given status.'),
    'module' => 'Ubercart Rules',
    'eval input' => array('status'),
  );
  
  return $items;
}


/**
 * This function updates the order status of the given method.
 *
 * @param $order Order to change
 * @param $settings Settings array incl. the new status
 * @return void
 *
 */
function uc_rules_order_update_status($order, $settings) {
  if (uc_order_update_status($order->order_id, $settings['order_status'])) {
    $order->order_status = $settings['order_status'];
  }
}

/**
 * This function implements the form for the order status update actions form.
 * 
 * @param $settings Settings array
 * @param $form Form to extend
 * @return void
 *
 */
function uc_rules_order_update_status_form($settings, &$form) {
  foreach (uc_order_status_list('general') as $status) {
    $options[$status['id']] = $status['title'];
  }
  foreach (uc_order_status_list('specific') as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['settings']['order_status'] = array(
    '#type' => 'select',
    '#title' => t('Order status'),
    '#options' => $options,
    '#default_value' => $settings['order_status'],
  );
}

/**
 * This function adds a new comment to the given order.
 *
 * @param $order Order on which the comment should be added
 * @param $settings Settings array incl. the new comment
 * @return void
 *
 */
function uc_rules_order_add_comment($order, $settings) {
  uc_order_action_add_comment($order, $settings);
}

/**
 * This function is the implementation of the settings form for
 * the comment action.
 *
 * @param $settings Settings array
 * @param $form Form to extend
 * @return void
 *
 */
function uc_rules_order_add_comment_form($settings, &$form) {
  $form['settings']['comment_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select an order comment type'),
    '#options' => array(
      'admin' => t('Enter this as an admin comment.'),
      'order' => t('Enter this as a customer order comment.'),
      'notified' => t('Enter this as a customer order comment with a notified icon.'),
    ),
    '#default_value' => $settings['comment_type'],
  );
  $form['settings']['comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#description' => t('Enter the comment message. Uses <a href="!url">order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'))),
    '#default_value' => $settings['comment'],
  );
}

/**
 * This function sends the invoice mail again. This is more or less the same 
 * e-mail as the customer receive when he completes the order.
 * 
 * @param $order Order to send again
 * @param $settings Settings array
 * @return void
 *
 */
function uc_rules_send_order_email_invoice($order, $settings) {

  $recipients = array();
  $addresses = $settings['addresses'];

  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  $settings['message'] = theme('uc_order', $order, $settings['view'], $settings['template']);

  if (empty($recipients)) {
    watchdog('ca', 'Attempted to e-mail an invoice with no recipient.', array(), WATCHDOG_ERROR);
    return;
  }

  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, empty($settings['from']) ? uc_store_email_from() : $settings['from']);

    if (!$sent['result']) {
      watchdog('ca', 'Attempt to e-mail invoice for order @order_id to @email failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * This functions is the implementation of the form for the e-mail invoice
 * send action.
 * 
 * @param $settings Settings array
 * @param $form Form to extend
 * @return void
 *
 */
function uc_rules_send_order_email_invoice_form($settings, &$form) {
  $form['settings']['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => isset($settings['from']) ? $settings['from'] : uc_store_email_from(),
    '#description' => t('The "From" address.'),
    '#required' => TRUE,
  );
  $form['settings']['addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#default_value' => isset($settings['addresses']) ? $settings['addresses'] : '[uc_order:uc_order_primary_email]',
    '#description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
    '#required' => TRUE,
  );
  $form['settings']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['subject'],
    '#required' => TRUE,
  );

  $form['settings']['template'] = array(
    '#type' => 'select',
    '#title' => t('Invoice template'),
    '#description' => t('Select the invoice template to use for this email.'),
    '#options' => uc_order_template_options(),
    '#default_value' => $settings['template'],
  );
  
  $form['settings']['view'] = array(
    '#type' => 'radios',
    '#title' => t('Included information'),
    '#options' => array(
      'print' => t('Show the business header and shipping method.'),
      'admin-mail' => t('Show all of the above plus the help text, email text, and store footer.'),
      'checkout-mail' => t('Show all of the above plus the "thank you" message.'),
    ),
    '#default_value' => $settings['view'],
  );

}

/**
 * This function loads the product node associated with the order item line.
 * 
 * @param $uc_line_item Line Item
 * @param $settings Settings array
 * @return Array with the loaded node
 * 
 */
function uc_rules_load_product_node($uc_line_item, $settings) {
  return array('node_loaded' => node_load($uc_line_item->nid));
}

/**
 * This functions loads the order from an id.
 *
 * @$settings Settings array incl. order id
 * @return void
 *
 */
function uc_rules_load_order_by_id($settings) {
  $order_id = $settings['uc_order_id'];
  $order = uc_order_load($order_id);
  
  return array('uc_order_loaded' => $order);
}

/**
 * This function implements the settings form for the 
 * load order by id.
 *
 * @param $settings Settings array
 * @param $form Form to extend
 * @return void
 *
 */
function uc_rules_load_order_by_id_form($settings, &$form) {
  $form['settings']['uc_order_id'] = array(
    '#type' => 'textarea',
    '#title' => t('Order Id'),
    '#default_value' => $settings['uc_order_id'],
    '#description' => t('Enter here the order id which should be loaded.'),
    '#required' => TRUE,
  );
    
}

/**
 * This function load the user (customer) from the order.
 *
 * @param $uc_order Order
 * @param $settings Settings array
 * @return Array with the loaded user
 *
 */
function uc_rules_load_customer_user($uc_order, $settings) {
  $user = user_load(array('uid' => $uc_order->uid));
  return array('user_loaded' => $user);
}

/**
 * This function implements the condtions 'has status'. It checks if
 * an order has a certain order status.
 *
 * @param $uc_order Order
 * @param $settings Settings incl. order status
 * @return boolean Matchs or not
 */
function uc_rules_order_has_status($uc_order, $settings) {
  if($uc_order->order_status == $settings['status']) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * This functions implements the has status form. 
 *
 * @param $settings Settings
 * @param $form Form to extend
 * @return void
 *
 */
function uc_rules_order_has_status_form($settings, &$form) {
  foreach( uc_order_status_list() as $status) {
    $status_list[$status['id']] = $status['title'];
  }

  $form['settings']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => $status_list,
    '#default_value' => $settings['status'],
    '#description' => t('Select the status on which the order must be.'),
  );
}



/**
 * Implementation of hook_token_list()
 */
function uc_rules_token_list($type = 'all') {
  if ($type == 'uc_order') {
    $tokens['uc_rules'] = array(
      'uc_order_id' => t('Ubercart Order Id'),
      'uc_order_uid' => t('User ID of the customer'),
      'uc_order_order_status' => t('Order Status'),
      'uc_order_order_total' => t('Order Total'),
      'uc_order_product_count' => t('Product Count'),
      'uc_order_primary_email' => t('Primary E-mail'),
      'uc_order_delivery_first_name' => t('Delivery Firstname'),
      'uc_order_delivery_last_name' => t('Delivery Lastname'),
      'uc_order_delivery_phone' => t('Delivery Phone'),
      'uc_order_delivery_company' => t('Delivery Company'),
      'uc_order_delivery_street1' => t('Delivery Street 1'),
      'uc_order_delivery_street2' => t('Delivery Street 2'),
      'uc_order_delivery_city' => t('Delivery City'),
      'uc_order_delivery_zone' => t('Delivery Zone'),
      'uc_order_delivery_postal_code' => t('Delivery Postal Code'),
      'uc_order_delivery_country' => t('Delivery Country'),
      'uc_order_billing_first_name' => t('Billing Firstname'),
      'uc_order_billing_last_name' => t('Billing Lastname'),
      'uc_order_billing_phone' => t('Billing Phone'),
      'uc_order_billing_company' => t('Billing Company'),
      'uc_order_billing_street1' => t('Billing Street 1'),
      'uc_order_billing_street2' => t('Billing Street 2'),
      'uc_order_billing_city' => t('Billing City'),
      'uc_order_billing_zone' => t('Billing Zone'),
      'uc_order_billing_postal_code' => t('Billing Postal Code'),
      'uc_order_billing_country' => t('Billing Country'),
      'uc_order_payment_method' => t('Payment Method'),
      'uc_order_data' => t('Additional Data'),
      'uc_order_created' => t('Order Created'),
      'uc_order_modified' => t('Order Modified'),
      'uc_order_host' => t('Host'),
    );
  }
  elseif($type == 'uc_line_item') {
    $tokens['uc_rules'] = array(
      'uc_line_item_id' => t('Ubercart Order Item Id'),
      'uc_line_item_order_id' => t('Order Id'),
      'uc_line_item_nid' => t('Node Id of the Product'),
      'uc_line_item_title' => t('Line Title'),
      'uc_line_item_manufacturer' => t('Manufacturer'),
      'uc_line_item_model' => t('Product Model'),
      'uc_line_item_qty' => t('Product Quantity'),
      'uc_line_item_cost' => t('Product Cost'),
      'uc_line_item_price' => t('Product Price'),
      'uc_line_item_weight' => t('Weight'),
      'uc_line_item_data' => t('Additional Data'),
    );
  }

  return $tokens;
}

/**
 * Implementation of hook token api
 */
function uc_rules_token_values($type, $object = NULL, $options = array()) {
  if ($type == 'uc_order') {
    $tokens = array(
      'uc_order_id' => $object->order_id,
      'uc_order_uid' => $object->uid,
      'uc_order_order_status' => $object->order_status,
      'uc_order_order_total' => $object->order_total,
      'uc_order_product_count' => $object->product_count,
      'uc_order_primary_email' => $object->primary_email,
      'uc_order_delivery_first_name' => $object->delivery_first_name,
      'uc_order_delivery_last_name' => $object->delivery_last_name,
      'uc_order_delivery_phone' => $object->delivery_phone,
      'uc_order_delivery_company' => $object->delivery_company,
      'uc_order_delivery_street1' => $object->delivery_street1,
      'uc_order_delivery_street2' => $object->delivery_street2,
      'uc_order_delivery_city' => $object->delivery_city,
      'uc_order_delivery_zone' => $object->delivery_zone,
      'uc_order_delivery_postal_code' => $object->delivery_postal_code,
      'uc_order_delivery_country' => $object->delivery_country,
      'uc_order_billing_first_name' => $object->billing_first_name,
      'uc_order_billing_last_name' => $object->billing_last_name,
      'uc_order_billing_phone' => $object->billing_phone,
      'uc_order_billing_company' => $object->billing_company,
      'uc_order_billing_street1' => $object->billing_street1,
      'uc_order_billing_street2' => $object->billing_street2,
      'uc_order_billing_city' => $object->billing_city,
      'uc_order_billing_zone' => $object->billing_zone,
      'uc_order_billing_postal_code' => $object->billing_postal_code,
      'uc_order_billing_country' => $object->billing_country,
      'uc_order_payment_method' => $object->payment_method,
      'uc_order_data' => $object->data,
      'uc_order_created' => $object->created,
      'uc_order_modified' => $object->modified,
      'uc_order_host' => $object->host,      
    );
  }
  elseif($type == 'uc_line_item') {
    $tokens = array(
      'uc_line_item_id' => $object->order_product_id,
      'uc_line_item_order_id' => $object->order_id,
      'uc_line_item_nid' => $object->nid,
      'uc_line_item_title' => $object->title,
      'uc_line_item_manufacturer' => $object->manufacturer,
      'uc_line_item_model' => $object->model,
      'uc_line_item_qty' => $object->qty,
      'uc_line_item_cost' => $object->cost,
      'uc_line_item_price' => $object->price,
      'uc_line_item_weight' => $object->weight,
      'uc_line_item_data' => $object->data,
    );  
  }

  return $tokens;
}


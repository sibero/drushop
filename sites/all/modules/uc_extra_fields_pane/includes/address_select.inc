<?php
/**
 * @file
 * This file contains code for getting all addresses for one user
 */

/**
 * get_addresses()
 * Similar to uc_get_addresses() in uc_store.module
 * @param int $uid
 * @param string $type
 * @return array
 */
function uc_extra_fields_pane_get_addresses($uid, $type = 'billing') {
  if ($uid == 0) {
    return NULL;
  }

  // Load fields (Ubercart or Ubercart Addresses 6.x-1.x)
  if (uc_addresses_version() === 1) {
    return _uc_extra_field_pane_get_uc_addresses($uid, $type);
  }
  else {
    return _uc_extra_field_pane_get_order_addresses($uid, $type);
  }
}

/**
 * _get_uc_addresses()
 * Returns addresses using the uc_addresses table
 * @param int $uid
 * @param string $type
 * @return array
 */
function _uc_extra_field_pane_get_uc_addresses($uid, $type) {
  $fields = UCXF_FieldList::getAllAddressFields();
  $element_type = UCXF_VALUE_ADDRESS;

  // Make queryparts
  $queryparts = _uc_extra_field_pane_get_addresses_queryparts($fields, 'uc_addresses', 'aid', $element_type);
  $uc_addresses_schema = drupal_get_schema('uc_addresses');
  $select = array();
  foreach ($uc_addresses_schema['fields'] as $fieldname => $field) {
    $select[] = $fieldname;
  }
  $select = array_merge($select, $queryparts['select']);
  $joins = $queryparts['joins'];

  // Load addresses from UC Addresses
  $query = "SELECT " . implode(", ", $select) . "
  FROM {uc_addresses}
  " . implode(" ", $joins) . "
  WHERE uid = %d
  ORDER BY created
  ";
  $result = db_query($query, $uid);

  // --------------------------------
  // following is copied from uc_addresses.module
  // --------------------------------
  $default_aid = _uc_addresses_get_default_address_id($uid);

  // Gather up everything
  $num_rows = 0;
  while ($address = db_fetch_object($result)) {
    $num_rows++;
    $address->is_default = ($default_aid == $address->aid);
    $addresses[] = $address;
  }
  if ($num_rows == 0) return FALSE;

  return $addresses;
}

/**
 * _get_order_addresses()
 * Returns addresses using the order table
 * @param int $uid
 * @param string $type
 * @return array
 */
function _uc_extra_field_pane_get_order_addresses($uid, $type) {
  //$fields = uc_extra_fields_pane_load_fields_from_db('extra_' . $type);
  $fields = UCXF_FieldList::getFieldsFromPane('extra_' . $type);
  $element_type = ($type == 'delivery')? UCXF_VALUE_ORDER_DELIVERY:UCXF_VALUE_ORDER_BILLING;

  // Make queryparts
  $queryparts = _uc_extra_field_pane_get_addresses_queryparts($fields, 'uc_orders', 'order_id', $element_type);
  $select = array(
    $type ."_first_name AS first_name",
    $type ."_last_name AS last_name",
    $type ."_phone AS phone",
    $type ."_company AS company",
    $type ."_street1 AS street1",
    $type ."_street2 AS street2",
    $type ."_city AS city",
    $type ."_zone AS zone",
    $type ."_postal_code AS postal_code",
    $type ."_country AS country",
  );
  $select = array_merge($select, $queryparts['select']);
  $joins = $queryparts['joins'];

  // Set ORDER BY
  // See uc_store.module, function uc_get_addresses() for more information about this behaviour
  switch ($GLOBALS['db_type']) {
    case 'mysqli':
    case 'mysql':
      $order_by = "ORDER BY created DESC";
    case 'pgsql':
      $order_by = "ORDER BY " . $type . "_street1 DESC";
      break;
  }

  // Load addresses from Ubercart
  $query = "SELECT DISTINCT " . implode(", ", $select) . "
   FROM {uc_orders}
  " . implode(" ", $joins) . "
   WHERE uid = %d AND order_status IN ". uc_order_status_list('general', TRUE)
   . $order_by
  ;
  $result = db_query($query, $uid);

  $addresses = array();
  while ($address = db_fetch_object($result)) {
    if (!empty($address->street1) || !empty($address->postal_code)) {
      $addresses[] = $address;
    }
  }
  return $addresses;
}

/**
 * _get_addresses_queryparts()
 * Generate joins and selects for get addresses query
 * @param int $uid
 * @param string $type
 * @return array
 */
function _uc_extra_field_pane_get_addresses_queryparts($fields, $table, $element_id, $element_type) {
  $select = array();
  $joins = array();
  foreach ($fields as $field) {
    $table_name = 'uc_extra_fields_pane_values_' . $field->db_name;
    $alias = $field->db_name;
    $select[] = "$table_name.value AS $alias";
    $joins[] = "LEFT JOIN uc_extra_fields_values $table_name ON $table.$element_id = $table_name.element_id AND ($table_name.field_id = '" . $field->field_id . "' AND $table_name.element_type = '" . $element_type . "')";
  }
  return array(
    'select' => $select,
    'joins' => $joins,
  );
}
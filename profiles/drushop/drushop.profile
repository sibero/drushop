<?php
// $Id$

/**
 * Name of profile; visible in profile selection form.
 */
define('PROFILE_NAME', 'drushop');
define('PROFILE_DESCRIPTION', 'Сборка интернет магазина на базе ubercart ориентированная на пользователей рунета');

/**
 * Implementation of hook_profile_details().
 */
function drushop_profile_details() {
  return array(
    'name' => PROFILE_NAME,
    'description' => PROFILE_DESCRIPTION,
  );
}


/**
 * Implementation of hook_profile_modules().
 */
function drushop_profile_modules() {
  // The database dump will take care of enabling the required modules for us.
  // Return an empty array to just enable the required modules.
  return array(
      // Core Drupal modules:
    'system',
    // Ubercart dependencies:
    'ca',
    'token',
	'xmlsitemap',
    // Ubercart modules:
    'uc_store',
    'uc_product',
    'uc_order',
    'uc_cart',
  );
}

function drushop_profile_task_list() {
  return array(
    'config' => st('Конфигурация магазина'),
  );
}

/**
 * Perform installation tasks for this installation profile.
 */
function drushop_profile_tasks(&$task, $url) {
  switch ($task) {
      case 'profile':
    // Set $task to next task so the UI will be correct.
      $task = 'config';
      drupal_set_title(t('Конфигурация магазина'));
      return drupal_get_form('drushop_store_settings_form', $url); 
	  
    // Perform tasks when the store configuration form has been submitted.
    case 'config':
      // Save the values from the store configuration form.
      drushop_store_settings_form_submit();
	  
	// Move to the completion task.
      $task = 'profile-finished';
      break;
  }
}


/**
 * Build the Ubercart store configuration form.
 *
 * @param $form_state
 * @param $url
 *   URL of current installer page, provided by installer.
 */
function drushop_store_settings_form(&$form_state, $url) {
  $form = array(
    '#action' => $url,
    '#redirect' => FALSE,
  );

  // Add the store contact information to the form.
  $form['uc_store_contact_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Контактная информация'),
  );

  $form['uc_store_contact_info']['uc_store_name'] = uc_textfield(t('Store name'), variable_get('site_name', NULL), FALSE, NULL, 64);
  $form['uc_store_contact_info']['uc_store_owner'] = uc_textfield(t('Store owner'), NULL, FALSE, NULL, 64);

  $form['uc_store_contact_info']['uc_store_phone'] = uc_textfield(t('Phone number'), NULL, FALSE);
  $form['uc_store_contact_info']['uc_store_fax'] = uc_textfield(t('Fax number'), NULL, FALSE);

  $form['uc_store_contact_info']['uc_store_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#description' => NULL,
    '#size' => 32,
    '#maxlength' => 128,
    '#required' => FALSE,
  );

  $form['uc_store_contact_info']['uc_store_email_include_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the store name in the from line of store e-mails.'),
    '#description' => t('May not be available on all server configurations. Turn off if this causes problems.'),
    '#default_value' => TRUE,
  );

  $form['uc_store_address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Адрес магазина'),
  );

  $form['uc_store_address']['uc_store_street1'] = uc_textfield(uc_get_field_name('street1'), NULL, FALSE, NULL, 128);
  $form['uc_store_address']['uc_store_city'] = uc_textfield(uc_get_field_name('city'), NULL, FALSE);
  $form['uc_store_address']['uc_store_country'] = uc_country_select(uc_get_field_name('country'), uc_store_default_country());

  $country_id = uc_store_default_country();
  $form['uc_store_address']['uc_store_zone'] = uc_zone_select(uc_get_field_name('zone'), NULL, NULL, $country_id);
  $form['uc_store_address']['uc_store_postal_code'] = uc_textfield(uc_get_field_name('postal_code'), NULL, FALSE, NULL, 10);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
  );
  
  /*// добавляем адрес по умолчанию.
   $uc_quote_store_default_address = array(
    'first_name' => '',
    'last_name' => '',
    'company' => $form['uc_store_contact_info']['uc_store_owner'],
	'phone' => $form['uc_store_contact_info']['uc_store_phone'],
	'street1' => $form['uc_store_address']['uc_store_street1'],
	'street2' => '',
	'city' => $form['uc_store_address']['uc_store_city'],
	'zone' => $form['uc_store_address']['uc_store_zone'],
	'postal_code' => $form['uc_store_address']['uc_store_postal_code'],
	'country' => $form['uc_store_address']['uc_store_country'],	
	);
	variable_set('uc_quote_store_default_address', $uc_quote_store_default_address);
	*/
  return $form;
  
}
// Сохраняем настройки. Последний шаг.
function drushop_store_settings_form_submit() {
  $form_state = array('values' => $_POST);
  system_settings_form_submit(array(), $form_state);
  // статистика
	module_load_include('inc', 'drushop_general', 'drushop_general.client');
	/*
  // генерируем xml карту сайта
	module_load_include('inc', 'xmlsitemap', 'xmlsitemap.admin');  
	$form = array();
	$form_state = array();
	xmlsitemap_sitemap_edit_form_submit($form, &$form_state); 
	*/
  drupal_cron_run();
  drupal_set_message('Drushop успешно установлен');
  drupal_goto('<front>');

  
}


/**
 * Implementation of hook_profile_form_alter().
 */
function drushop_form_alter(&$form, $form_state, $form_id) {
  // Add an additional submit handler. 
  if ($form_id == 'install_configure') {
    $form['#submit'][] = 'drushop_form_submit';
  }
}

/**
 * Custom form submit handler for configuration form.
 *
 * Drops all data from existing database, imports database dump, and restores
 * values entered into configuration form.
 */
function drushop_form_submit($form, &$form_state) {
  // Import database dump file.
  $dump_file = dirname(__FILE__) . '/drushop.inc';
  $success = import_dump($dump_file);

  if (!$success) {
    return;
  }
  GLOBAL $base_url;
  // Now re-set the values they filled in during the previous step.
  variable_set('site_name', $form_state['values']['site_name']);
  variable_set('site_mail', $form_state['values']['site_mail']);
  variable_set('date_default_timezone', $form_state['values']['date_default_timezone']);


  // Perform additional clean-up tasks.
  variable_del('file_directory_temp');

  // Replace their username and password and log them in.
  $name = $form_state['values']['account']['name'];
  $pass = $form_state['values']['account']['pass'];
  $mail = $form_state['values']['account']['mail'];
  $date = time();
  db_query("UPDATE {users} SET name = '%s', pass = MD5('%s'), mail = '%s', init = '%s', created = '%s', access = '%s', login = '%s'  WHERE uid = 1", $name, $pass, $mail, $mail, $date, $date, $date);
  db_query("UPDATE {users} SET uid = 0 WHERE name = ''");
  db_query("UPDATE {contact} SET recipients = '%s' WHERE cid  = 2", $mail);
  user_authenticate(array('name' => $name, 'pass' => $pass));
  // обновляем переменные
  variable_set('purl_base_domain', $base_url);
  $success = $base_url.'/uc_roboxchange/success';
  variable_set('success', $success);
  $result = $base_url.'/uc_roboxchange/done';
  variable_set('result', $result);
  variable_set('install_profile', 'drushop');
  variable_set('install_task', 'done');
  $fail = $base_url.'/uc_roboxchange/fail';
  variable_set('fail', $fail);
  variable_set('xmlsitemap_base_url', $base_url);
  $uc_url_api = $base_url.'/uc_onpay/done';
  variable_set('uc_url_api', $uc_url_api);

   
  /*
 * Установка своих переводов из po файла.
  // Подключаем нужную библиотеку
include_once 'includes/locale.inc';
// Указываем название po-файла
$file_name = 'ru.po';
// Создаем класс для po-файла
$file = new stdClass();
$file->filename = $file_name;
// Указываем путь к po-файлу
$file->filepath = $_SERVER['DOCUMENT_ROOT'] . base_path() .'profiles/drushop/'. $file_name;
// Загружаем перевод из файла
_locale_import_po($file, 'ru', LOCALE_IMPORT_OVERWRITE, 'default');
*/
  
}


/**
 * Imports a database dump file.
 */
function import_dump($filename) {
  // Open dump file.
  if (!file_exists($filename) || !($fp = fopen($filename, 'r'))) {
    drupal_set_message(t('Unable to open dump file %filename.', array('%filename' => $filename)), 'error');
    return FALSE;
  }

  // Drop all existing tables.
  foreach (list_tables() as $table) {
    db_query("DROP TABLE %s", $table);
  }

  // Load data from dump file.
  $success = TRUE;
  $query = '';
  $new_line = TRUE;

  while (!feof($fp)) {
    // Better performance on PHP 5.2.x when leaving out buffer size to
    // fgets().
    $data = fgets($fp);
    if ($data === FALSE) {
      break;
    }
    // Skip empty lines (including lines that start with a comment).
    if ($new_line && ($data == "\n" || !strncmp($data, '--', 2) || !strncmp($data, '#', 1))) {
      continue;
    }

    $query .= $data;
    $len = strlen($data);
    if ($data[$len - 1] == "\n") {
      if ($data[$len - 2] == ';') {
        // Reached the end of a query, now execute it.
        if (!_db_query($query, FALSE)) {
          $success = FALSE;
        }
        $query = '';
      }
      $new_line = TRUE;
    }
    else {
      // Continue adding data from the same line.
      $new_line = FALSE;
    }
  }
  fclose($fp);

  if (!$success) {
    drupal_set_message(t('Возникла ошибка при импорте файла %filename.', array('%filename' => $filename)), 'error');
  }

  return $success;
}

/**
 * Returns a list of tables in the active database.
 *
 * Only returns tables whose prefix matches the configured one (or ones, if
 * there are multiple).
 *
 * @see demo_enum_tables()
 */
function list_tables() {
  global $db_prefix;

  $tables = array();

  if (is_array($db_prefix)) {
    // Create a regular expression for table prefix matching.
    $rx = '/^' . implode('|', array_filter($db_prefix)) . '/';
  }
  else if ($db_prefix != '') {
    $rx = '/^' . $db_prefix . '/';
  }

  switch ($GLOBALS['db_type']) {
    case 'mysql':
    case 'mysqli':
      $result = db_query("SHOW TABLES");
      break;

    case 'pgsql':
      $result = db_query("SELECT table_name FROM information_schema.tables WHERE table_schema = '%s'", 'public');
      break;
  }

  while ($table = db_fetch_array($result)) {
    $table = reset($table);
    if (is_array($db_prefix)) {
      // Check if table name matches a configured prefix.
      if (preg_match($rx, $table, $matches)) {
        $table_prefix = $matches[0];
        $plain_table = substr($table, strlen($table_prefix));
        if ($db_prefix[$plain_table] == $table_prefix || $db_prefix['default'] == $table_prefix) {
          $tables[] = $table;
        }
      }
    }
    else if ($db_prefix != '') {
      if (preg_match($rx, $table)) {
        $tables[] = $table;
      }
    }
    else {
      $tables[] = $table;
    }
  }

  return $tables;
}

// Устанавливаем русский по умолчанию
function system_form_install_select_locale_form_alter(&$form, $form_state) {
  $form['locale']['ru']['#value'] = 'ru';
  unset($form['locale']['en']);
}

// Убираем настройку параметров чистых ссылок и статуса обновлений при установке.
function system_form_install_configure_form_alter(&$form, $form_state) { 
$form['server_settings']['update_status_module']['#default_value'][0] = FALSE;
unset($form['server_settings']['update_status_module']);
unset($form['server_settings']['clean_url']);
}




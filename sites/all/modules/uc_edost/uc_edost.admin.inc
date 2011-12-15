<?php
// $Id: uc_edost.admin.inc,v 1.0.0.0 2010/11/29 10:00:00 $

/**
 * @file
 * edost administration menu items.
 */

/**
 * Настройка параметров расчета edost (Идентификатор магазина, Пароль, Сервер)
 *
 * @ingroup forms
 */
function uc_edost_admin_settings() {
  $form = array();

  $form['uc_edost_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Идентификатор магазина'),
    '#description' => t('Для работы калькулятора необходима регистрация на сервере <a href="!url">eDost.ru</a>.', array('!url' => 'http://www.edost.ru/shop.php')),
    '#default_value' => variable_get('uc_edost_id', ''),
  );

  $form['uc_edost_ps'] = array(
    '#type' => 'textfield',
    '#title' => t('Пароль для доступа к серверу расчетов edost.ru'),
    '#description' => t(''),
    '#default_value' => variable_get('uc_edost_ps', ''),
  );

  $form['uc_edost_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Сервер расчета доставки (можно не указывать)'),
    '#description' => t(''),
    '#default_value' => variable_get('uc_edost_host', ''),
  );

  return system_settings_form($form);
}

/**
 *
 * Проверка корректного ввода (отключил)
 */
function uc_edost_admin_settings_validate($form, &$form_state) {

/*  if (!is_numeric($form_state['values']['uc_edost_markup'])) {
    form_set_error('uc_edost_markup', t('Rate markup must be a numeric value.'));
  }
*/
}

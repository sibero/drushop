<?php
// $Id;$

/**
 * Admin page
 */
function _preserve_language_admin() {
  $form = array();
  $form['preserve_language_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preserve interface language on language links'),
    '#default_value' => variable_get('preserve_language_on', 1),
    '#description' => t("This will alter all links to translations such that they do not change the interface language when changing the content language"),
    '#required' => TRUE,
  );

  $form['preserve_language_content_neutral'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fix Language-Neutral'),
    '#description' => t('Sets Language-Neutral for *new* nodes with no multilingual support. Drupal default behaviour is to set them as Default site language.'),
    '#default_value' => variable_get('preserve_language_content_neutral', 0),
  );

  $form['preserve_language_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Interface language switcher block'),
    '#default_value' => variable_get('preserve_language_block', 1),
    '#description' => t("Provides an interface language switcher block. This only switches the language of the interface (when i18n is installed, the local language switcher block changes both interface and content language)"),
    '#required' => TRUE,
  );

  if (module_exists('i18n')) {
    $form['preserve_language_selection'] = array(
      '#type'  => 'checkbox',
      '#title' => t('Force content selection mode to off'),
      '#default_value' => variable_get('preserve_language_selection', 1),
      '#description' => t('i18n overrides the content mode selection when viewing node pages. This option reverts this behaviour, and forces the content mode selection to off. In the context of Preserve language, this is added so that all menu items show regardless of the content language (see this issue http://drupal.org/node/283839 ). However, it will have other side effects ; use with caution.'),
    );
  }
  return system_settings_form($form);
}

<?php
/**
 * @file
 * Text module integration.
 */

/**
 * Implements hook_mobile_codes_theme_alter() on behalf of text.module.
 */
function text_mobile_codes_theme_alter($items) {
  ctools_include('export');
  foreach (ctools_export_crud_load_all('mobile_codes_presets') as $preset) {
    if (!$preset->disabled) {
      $items["mobile_codes_formatter_mobile_codes_text_{$preset->name}"] = array(
        'arguments' => array('element' => NULL),
        'function' => 'theme_mobile_codes_text_formatter',
        'file' => 'includes/text.inc',
      );
    }
  }
}

/**
 * Implements hook_mobile_codes_field_formatter_info_alter() on behalf of text.module.
 */
function text_mobile_codes_field_formatter_info_alter($formatters) {
  ctools_include('export');
  foreach (ctools_export_crud_load_all('mobile_codes_presets') as $preset) {
    if (!$preset->disabled) {
      $formatters["mobile_codes_text_{$preset->name}"] = array(
        'label' => t('Mobile code: @preset', array('@preset' => $preset->name)),
        'field types' => array('text')
      );
    }
  }
}

/**
 *
 */
function theme_mobile_codes_text_formatter($element) {
  if (!mobile_codes_content_field_is_empty($element)) {
    return theme('mobilecode', $element['#item']['safe'], array('#preset' => drupal_substr($element['#formatter'], 18)));
  }
}

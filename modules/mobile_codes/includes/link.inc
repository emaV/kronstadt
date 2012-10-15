<?php
/**
 * @file
 * Link module integration.
 */

/**
 * Implements hook_mobile_codes_theme_alter() on behalf of link.module.
 */
function link_mobile_codes_theme_alter($items) {
  ctools_include('export');
  foreach (ctools_export_crud_load_all('mobile_codes_presets') as $preset) {
    if (!$preset->disabled) {
      $items["mobile_codes_formatter_mobile_codes_link_{$preset->name}"] = array(
        'arguments' => array('element' => NULL),
        'function' => 'theme_mobile_codes_link_formatter',
        'file' => 'includes/link.inc',
      );
    }
  }
}

/**
 * Implements hook_mobile_codes_field_formatter_info_alter() on behalf of link.module.
 */
function link_mobile_codes_field_formatter_info_alter($formatters) {
  ctools_include('export');
  foreach (ctools_export_crud_load_all('mobile_codes_presets') as $preset) {
    if (!$preset->disabled) {
      $formatters["mobile_codes_link_{$preset->name}"] = array(
        'label' => t('Mobile code: @preset', array('@preset' => $preset->name)),
        'field types' => array('link')
      );
    }
  }
}

function theme_mobile_codes_link_formatter($element) {
  if (!mobile_codes_content_field_is_empty($element)) {
    return theme('mobilecode', $element['#item']['display_url'], array('#preset' => drupal_substr($element['#formatter'], 18)));
  }
}

<?php
/**
 * @file
 * Content module integration.
 */

/**
 * Implements hook_field_formatter_info().
 */
function mobile_codes_field_formatter_info() {
  $formatters = array();
  drupal_alter('mobile_codes_field_formatter_info', $formatters);
  return $formatters;
}

function mobile_codes_content_field_is_empty($element) {
  $field = content_fields($element['#field_name']);
  if (function_exists($function = "{$field['widget']['module']}_content_is_empty")) {
    return $function($element['#item'], $field);
  }
  return FALSE;
}

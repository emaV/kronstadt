<?php
/**
 * @file
 * Mobile Tools module integration.
 */

/**
 * Implements hook_mobile_codes_settings_alter() on behalf of mobile_tools.module.
 */
function mobile_tools_mobile_codes_settings_alter($settings) {
  $settings['url']['mobile_tools'] = array(
    'label' => t('Use Mobile URL'),
    'description' => t('Use Mobile URL defined via the Mobile Tools module - !configure', array('!configure' => l(t('configure'), 'admin/settings/mobile-tools'))),
  );
}

/**
 * Implements mobile_codes_data_TYPE_SETTING().
 */
function mobile_codes_data_url_mobile_tools(&$data) {
  global $base_url;
  if (strpos($data, $base_url) === 0) {
    $data = mobile_tools_create_mobile_url($data);
  }
}

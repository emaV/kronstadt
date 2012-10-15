<?php
/**
 * @file
 * Bouncer module integration.
 */

/**
 * Implements hook_mobile_codes_settings_alter() on behalf of bouncer.module.
 */
function bouncer_mobile_codes_settings_alter($settings) {
  $settings['url']['bouncer'] = array(
    'label' => t('Track URL'),
    'description' => t('Track URL with the Bouncer module - !configure', array('!configure' => l(t('configure'), 'admin/settings/bouncer'))),
    'weight' => 5,
  );
}

/**
 * Implements mobile_codes_data_TYPE_SETTING().
 */
function mobile_codes_data_url_bouncer(&$data) {
  $data = bouncer_url($data);
}

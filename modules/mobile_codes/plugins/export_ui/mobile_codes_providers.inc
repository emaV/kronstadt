<?php
/**
 * @file
 */

$plugin = array(
  'schema' => 'mobile_codes_providers',
  'access' => 'administer mobile codes',
  'menu' => array(
    'menu prefix' => 'admin/settings/mobile_codes',
    'menu item' => 'providers',
    'menu title' => 'Providers',
    'menu description' => 'Administer Mobile Codes providers.',
  ),
  'title singular' => t('provider'),
  'title singular proper' => t('Provider'),
  'title plural' => t('providers'),
  'title plural proper' => t('Providers'),
  'form' => array(
    'settings' => 'mobile_codes_providers_export_ui_form',
    'validate' => 'mobile_codes_providers_export_ui_form_validate',
    'submit' => 'mobile_codes_providers_export_ui_form_submit',
  ),
);

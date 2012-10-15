<?php
/**
 * @file
 * vCard module integration
 */

/**
 * Implements hook_mobile_codes_default_mobile_codes_preset_alter() on behalf of vcard.module.
 */
function vcard_mobile_codes_default_mobile_codes_preset_alter($export) {
  $preset = new stdClass;
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 2;
  $preset->name = 'block_user_vcard';
  $preset->provider = 'google';
  $preset->defaults = array(
    'width' => '180',
    'height' => '180',
    'output_encoding' => 'UTF-8',
  );

  $export['block_user_vcard'] = $preset;
}

/**
 * Implements hook_mobile_codes_block_alter() on behalf of vcard.module.
 */
function vcard_mobile_codes_block_alter($blocks) {
  $blocks['user_vcard'] = array(
    'info' => t('User vCard'),
  );
}

/**
 *
 */
function mobile_codes_block_user_vcard_configure($edit) {
  $form = array();

  $form['preset'] = array(
    '#type' => 'select',
    '#title' => t('Mobile Codes preset'),
    '#options' => array(),
    '#default_value' => variable_get('mobile_codes_block_user_vcard_preset', 'block_user_vcard'),
  );
  ctools_include('export');
  foreach (ctools_export_crud_load_all('mobile_codes_presets') as $preset) {
    if (!$preset->disabled) {
      $form['preset']['#options'][$preset->name] = $preset->name;
    }
  }

  return $form;
}

/**
 *
 */
function mobile_codes_block_user_vcard_save($edit) {
  variable_set('mobile_codes_block_user_vcard_preset', $edit['preset']);
}

/**
 *
 */
function mobile_codes_block_user_vcard_view($edit) {
  switch (arg(0)) {
    case 'node':
      if (arg(2) == '') {
        $node = node_load(arg(1));
        $account = user_load($node->uid);
      }
      break;

    case 'user':
      $account = user_load(arg(1));
      break;
  }

  if ($account) {
    $vcard = vcard_get($account);
    $vcard_text = $vcard->fetch();

    return array(
      'subject' => t('Mobile Codes: User vCard'),
      'content' => theme('mobilecode', $vcard_text, array('#preset' => variable_get('mobile_codes_block_user_vcard_preset', 'block_user_vcard')))
    );
  }
}

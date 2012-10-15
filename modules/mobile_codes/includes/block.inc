<?php
/**
 * @file
 * Block module integration.
 */

/**
 * Implements hook_mobile_codes_default_mobile_codes_preset_alter() on behalf of block.module.
 */
function block_mobile_codes_default_mobile_codes_preset_alter($export) {
  $preset = new stdClass;
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 2;
  $preset->name = 'block_node_url';
  $preset->provider = 'google';
  $preset->defaults = array(
    'width' => '180',
    'height' => '180',
    'output_encoding' => 'UTF-8',
  );

  $export['block_node_url'] = $preset;
}

/**
 * Implements hook_block().
 */
function mobile_codes_block($op = 'list', $delta = 0, $edit = array()) {
  switch ($op) {
    case 'list':
      $blocks = array();
      drupal_alter('mobile_codes_block', $blocks);
      return $blocks;

    case 'save':
      if (function_exists($function = "mobile_codes_block_{$delta}_{$op}")) {
        $function($edit);
      }
      break;

    default:
      if (function_exists($function = "mobile_codes_block_{$delta}_{$op}")) {
        return $function($edit);
      }
      return array();
  }
}

/**
 * Implements hook_mobile_codes_block_alter() on behalf of block.module.
 */
function block_mobile_codes_block_alter($blocks) {
  $blocks['node_url'] = array(
    'info' => t('Node URL'),
  );

  $blocks['generate_code'] = array(
    'info' => t('Mobile Code generator'),
  );
}

/**
 *
 */
function mobile_codes_block_node_url_configure($edit) {
  $form = array();

  $form['preset'] = array(
    '#type' => 'select',
    '#title' => t('Mobile Codes preset'),
    '#options' => array(),
    '#default_value' => variable_get('mobile_codes_block_node_url_preset', 'block_node_url'),
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
function mobile_codes_block_node_url_save($edit) {
  variable_set('mobile_codes_block_node_url_preset', $edit['preset']);
}

/**
 *
 */
function mobile_codes_block_node_url_view($edit) {
  if (arg(0) == 'node' && arg(1) != '' && arg(2) == '') {
    return array(
      'subject' => t('Node URL'),
      'content' => theme('mobilecode', url('node/' . arg(1), array('absolute' => TRUE, 'alias' => TRUE)), array('#preset' => variable_get('mobile_codes_block_node_url_preset', 'block_node_url')))
    );
  }
}

/**
 *
 */
function mobile_codes_block_generate_code_view($edit) {
  return array(
    'subject' => t('Mobile Code generator'),
    'content' => drupal_get_form('mobile_codes_block_generate_code_form'),
  );
}

/**
 *
 */
function mobile_codes_block_generate_code_form(&$form_state) {
  $form = array();

  if (is_array($form_state['values'])) {
    $form['mobile_code'] = array(
      '#title' => t('Output'),
      '#type' => 'fieldset',
    );

    $form['mobile_code']['sample'] = array(
      '#value' => theme('mobilecode', $form_state['values']['text'], array('#preset' => $form_state['values']['preset'])),
    );

    $form['mobile_code']['filter'] = array(
      '#type' => 'textfield',
      '#title' => t('Filter'),
      '#value' => "[mobilecode #preset='{$form_state['values']['preset']}']{$form_state['values']['text']}[/mobilecode]",
    );

    $form['mobile_code']['theme'] = array(
      '#type' => 'textfield',
      '#title' => t('Theme'),
      '#value' => "theme('mobile_code', '{$form_state['values']['text']}', array('#preset' => '{$form_state['values']['preset']}'));",
    );
  }

  $form['preset'] = array(
    '#type' => 'select',
    '#title' => t('Preset'),
    '#options' => array(),
    '#default_value' => is_array($form_state['values']) ? $form_state['values']['preset'] : array(),
  );
  ctools_include('export');
  foreach (ctools_export_crud_load_all('mobile_codes_presets') as $preset) {
    if (!$preset->disabled) {
      $form['preset']['#options'][$preset->name] = $preset->name;
    }
  }

  $form['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Data'),
    '#default_value' => is_array($form_state['values']) ? $form_state['values']['text'] : '',
  );

  $form['generate'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
    '#ahah' => array(
      'path' => 'ahah/mobile_codes/generator',
      'wrapper' => 'mobile-codes-block-generate-code-form',
    ),
  );

  return $form;
}

/**
 *
 */
function mobile_codes_ahah_generator($form_id, $form, $form_state, $args, $form_build_id) {
  // Process the form with drupal_process_form(), which calls the submit
  // handlers that puts whatever was worthy of keeping in the $form_state.
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  return drupal_render($form);
}

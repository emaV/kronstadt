<?php
/**
 * @file
 * CTools module integration.
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mobile_codes_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "mobile_codes" && in_array($api, array("default_mobile_codes_presets", "default_mobile_codes_providers"))) {
    return array("version" => 2);
  }
}

/**
 * Implements hook_ctools_plugin_directory().
 */
function mobile_codes_ctools_plugin_directory($module, $plugin) {
  if ($module == 'ctools' && $plugin == 'export_ui') {
    return "plugins/{$plugin}";
  }
}

/**
 * Implements hook_mobile_codes_theme_alter() on behalf of ctools.module.
 */
function ctools_mobile_codes_theme_alter($items) {
  $items['mobile_codes_providers_export_ui_form_parameters'] = array(
    'arguments' => array(
      'element' => array(),
    ),
    'file' => 'includes/ctools.inc',
  );
}

/**
 *
 */
function mobile_codes_presets_export_ui_form(&$form, &$form_state) {
  $providers = ctools_export_crud_load_all('mobile_codes_providers');

  $form['info']['provider'] = array(
    '#type' => 'select',
    '#title' => t('Provider'),
    '#options' => array(),
    '#default_value' => $form_state['item']->provider,
    '#ahah' => array(
      'path' => 'ahah/mobile_codes/presets/provider',
      'wrapper' => 'defaults-wrapper',
    ),
  );
  foreach ($providers as $provider) {
    if (!$provider->disabled) {
      $form['info']['provider']['#options'][$provider->name] = $provider->name;
    }
  }
  asort($form['info']['provider']['#options']);

  $form['preset']['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Defaults'),
    '#prefix' => '<div id="defaults-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  mobile_codes_presets_export_ui_form_provider($form, $form_state['item']->provider, $form_state['item']->defaults);
}

/**
 *
 */
function mobile_codes_presets_export_ui_form_provider(&$form, $provider, $defaults = array()) {
  foreach (element_children($form['preset']['defaults']) as $child) {
    unset($form['preset']['defaults'][$child]);
  }

  ctools_include('export');
  $provider = isset($provider) ? $provider : current($form['info']['provider']['#options']);
  $provider = ctools_export_crud_load('mobile_codes_providers', $provider);
  foreach (element_children($provider->parameters) as $parameter) {
    switch ($provider->parameters[$parameter]['type']) {
      case 'select':
        $provider->parameters[$parameter]['value'] = explode("\n", $provider->parameters[$parameter]['value']);
        $options = array();
        foreach ($provider->parameters[$parameter]['value'] as $value) {
          $value = explode('|', $value);
          $options[$value[0]] = isset($value[1]) ? $value[1] : $value[0];
        }
        $form['preset']['defaults'][$parameter] = array(
          '#type' => 'select',
          '#title' => isset($provider->parameters[$parameter]['label']) ? $provider->parameters[$parameter]['label'] : $parameter,
          '#options' => $options,
          '#default_value' => isset($defaults[$parameter]) ? $defaults[$parameter] : current($options),
        );
        break;

      case 'text':
        $form['preset']['defaults'][$parameter] = array(
          '#type' => 'textfield',
          '#title' => isset($provider->parameters[$parameter]['label']) ? $provider->parameters[$parameter]['label'] : $parameter,
          '#size' => 5,
          '#default_value' => isset($defaults[$parameter]) ? $defaults[$parameter] : '',
          '#required' => TRUE,
        );
        break;
    }
  }
}

/**
 *
 */
function mobile_codes_presets_export_ui_form_validate(&$form, &$form_state) {}

/**
 *
 */
function mobile_codes_presets_export_ui_form_submit(&$form, &$form_state) {
  $form_state['values']['defaults'] = $_POST['defaults'];
}

/**
 *
 */
function mobile_codes_ahah_presets_provider($form_id, $form, $form_state) {
  mobile_codes_presets_export_ui_form_provider($form, $form_state['values']['provider'], $form_state['values']['defaults']);

  // Process the form with drupal_process_form(), which calls the submit
  // handlers that puts whatever was worthy of keeping in the $form_state.
  drupal_process_form($form_id, $form, $form_state);

  unset($form['preset']['defaults']['#prefix'], $form['preset']['defaults']['#suffix']);
  return drupal_render($form['preset']['defaults']);
}

/**
 *
 */
function mobile_codes_providers_export_ui_form(&$form, &$form_state) {
  $form['provider']['url']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => isset($form_state['item']->url) ? $form_state['item']->url : '',
    '#ahah' => array(
      'path' => 'ahah/mobile_codes/providers/url',
      'wrapper' => 'parameters-wrapper',
    ),
    '#required' => TRUE,
  );

  $form['provider']['parameters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Parameters'),
    '#theme' => 'mobile_codes_providers_export_ui_form_parameters',
    '#prefix' => '<div id="parameters-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  mobile_codes_providers_export_ui_form_parameters($form, $form_state['item']->url, $form_state['item']->parameters);
}

/**
 *
 */
function mobile_codes_providers_export_ui_form_parameters(&$form, $url, $parameters = array()) {
  foreach (element_children($form['provider']['parameters']) as $child) {
    unset($form['provider']['parameters'][$child]);
  }

  preg_match_all('/\[(.*?)\]/', $url, $tokens, PREG_SET_ORDER);
  foreach ($tokens as $token) {
    $form['provider']['parameters'][$token[1]]['token'] = array(
      '#type' => 'value',
      '#value' => $token[1],
    );

    if ($parameters[$token[1]]['type'] != 'data') {
      $form['provider']['parameters'][$token[1]]['label'] = array(
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#size' => 30,
        '#default_value' => isset($parameters[$token[1]]) ? $parameters[$token[1]]['label'] : '',
      );
    }

    $form['provider']['parameters'][$token[1]]['type'] = array(
      '#type' => 'select',
      '#options' => array(
        'data' => t('Data'),
        'select' => t('Selectbox'),
        'text' => t('Textfield'),
      ),
      '#default_value' => isset($parameters[$token[1]]) ? $parameters[$token[1]]['type'] : 'text',
      '#ahah' => array(
        'path' => 'ahah/mobile_codes/providers/type',
        'wrapper' => 'parameters-wrapper',
      ),
    );

    if (isset($parameters[$token[1]]) && $parameters[$token[1]]['type'] == 'select') {
      $form['provider']['parameters'][$token[1]]['value'] = array(
        '#type' => 'textarea',
        '#title' => t('Values'),
        '#default_value' => isset($parameters[$token[1]]) ? $parameters[$token[1]]['value'] : '',
      );
    }
  }
}

/**
 *
 */
function theme_mobile_codes_providers_export_ui_form_parameters($element) {
  $headers = array(t('Token'), t('Label'), t('Type'), t('Values'));
  $rows = array();
  foreach (element_children($element) as $child) {
    foreach (element_children($element[$child]) as $field) {
      unset($element[$child][$field]['#title']);
    }
    $row = array();
    $row[] = array('data' => drupal_render($element[$child]['token']) . "[{$element[$child]['token']['#value']}]");
    $row[] = array('data' => drupal_render($element[$child]['label']));
    $row[] = array('data' => drupal_render($element[$child]['type']));
    $row[] = array('data' => drupal_render($element[$child]['value']));
    $rows[] = $row;
  }
  return theme('table', $headers, $rows);
}

function mobile_codes_providers_export_ui_form_validate(&$form, &$form_state) {}

/**
 *
 */
function mobile_codes_providers_export_ui_form_submit(&$form, &$form_state) {
  $form_state['values'] = $_POST;
  $form_state['item']->parameters = $form_state['values']['parameters'];
}

/**
 *
 */
function mobile_codes_ahah_providers_url($form_id, $form, $form_state) {
  mobile_codes_providers_export_ui_form_parameters($form, $form_state['values']['url'], $form_state['values']['parameters']);

  // Process the form with drupal_process_form(), which calls the submit
  // handlers that puts whatever was worthy of keeping in the $form_state.
  drupal_process_form($form_id, $form, $form_state);

  unset($form['provider']['parameters']['#prefix'], $form['provider']['parameters']['#suffix']);
  $output = drupal_render($form['provider']['parameters']);

  $javascript = drupal_add_js(NULL, NULL);
  if (isset($javascript['setting'])) {
    $output .= '<script type="text/javascript">jQuery.extend(Drupal.settings, ' . drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) . ');</script>';
  }

  return $output;
}

/**
 *
 */
function mobile_codes_ahah_providers_type($form_id, $form, $form_state) {
  $form_state['values']['parameters'] = isset($form_state['values']['parameters']) ? $form_state['values']['parameters'] : $_POST['parameters'];
  mobile_codes_providers_export_ui_form_parameters($form, $form_state['values']['url'], $form_state['values']['parameters']);

  // Process the form with drupal_process_form(), which calls the submit
  // handlers that puts whatever was worthy of keeping in the $form_state.
  drupal_process_form($form_id, $form, $form_state);

  unset($form['provider']['parameters']['#prefix'], $form['provider']['parameters']['#suffix']);
  return drupal_render($form['provider']['parameters']);
}

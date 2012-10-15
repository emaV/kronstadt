<?php
/**
 * @file
 * Custom Formatters module integration.
 */

/**
 * Implements hook_custom_formatters_defaults().
 */
function mobile_codes_custom_formatters_defaults() {
  $formatters = array();

  // Example: Mobile Codes.
  $formatters['example_mobile_codes'] = array(
    'name' => 'example_mobile_codes',
    'label' => 'Example: Mobile Codes',
    'field_types' => 'a:1:{i:0;s:4:"text";}',
    'multiple' => '0',
    'description' => 'An example Mobile Codes formatter',
    'mode' => 'advanced',
    'code' => '/**
 * This is an example Advanced Custom Formatter to demonstrate how to output various items as a Mobile Code.
 */
$preset = \'medium\';

/**
 * If Devel module is present (recommended), display the contents of the $element variable to help with construction of the formatter.
 */
if (module_exists(\'devel\')) {
  dpm($element);
}

/**
 * Render Node URL as a Mobile Code.
 */
/* -- Delete this line if you want to use this code -- /
$node = $element[\'#node\'];
$url = url("node/{$node->nid}", array(\'absolute\' => TRUE, \'alias\' => TRUE));
return theme(\'mobilecode\', $url, array(\'#preset\' => $preset));
// */

/**
 * Render Node URL as the path to a Mobile Code.
 */
/* -- Delete this line if you want to use this code -- /
$node = $element[\'#node\'];
$url = url("node/{$node->nid}", array(\'absolute\' => TRUE, \'alias\' => TRUE));
$request = mobile_codes_process_url($url, array(\'#preset\' => $preset));
return mobile_codes_generate($request);
// */

/**
 * Render Node author as a vCard Mobile Code (requires vCard module).
 */
/* -- Delete this line if you want to use this code -- /
if (module_exists(\'vcard\')) {
  $node = $element[\'#node\'];
  $account = user_load($node->uid);
  $vcard = vcard_get($account);
  $vcard_text = $vcard->fetch();
  return theme(\'mobilecode\', $vcard_text, array(\'#preset\' => $preset));
}
// */',
    'status' => '0',
  );

  return $formatters;
}

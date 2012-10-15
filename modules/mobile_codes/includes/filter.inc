<?php
/**
 * @file
 * Filter module integration.
 */

/**
 * Implements hook_filter_tips().
 */
function mobile_codes_filter_tips($delta, $format, $long = FALSE) {
  return t('
    Create a Mobile Code using the following format:<br />
    [mobilecode #preset="<em>preset</em>"]<em>content</em>[/mobilecode]
  ');
}

/**
 * Implements hook_filter().
 */
function mobile_codes_filter($op, $delta = 0, $format = -1, $text = '') {
  switch ($op) {
    case 'list':
      return array(0 => t('Mobile Codes filter'));

    case 'description':
      return t('
        Create a Mobile Code using the following format:<br />
        [mobilecode #preset="<em>preset</em>"]<em>content</em>[/mobilecode]
      ');

    case 'prepare':
      return $text;

    case 'process':
      return mobile_codes_filter_process($text, $format);

    case 'settings':
      return $text;

    default:
      return $text;
  }
}

/**
 *
 */
function mobile_codes_filter_process($text, $format) {
  $attr = '';

  if (preg_match_all('/\[mobilecode([^]]*)\]([^[]*)\[\/mobilecode\]/', $text, $codes, PREG_SET_ORDER)) {
    foreach ($codes as $match) {
      $attr = array();
      if (preg_match_all('/(\S*?)=["\'](.*?)["\']/', $match[1], $attributes, PREG_SET_ORDER)) {
        foreach ($attributes as $attribute) {
          $attr[$attribute[1]] = $attribute[2];
        }
      }
      $text = str_replace($match[0], theme('mobilecode', $match[2], $attr), $text);
    }
  }

  return $text;
}

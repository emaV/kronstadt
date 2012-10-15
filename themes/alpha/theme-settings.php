<?php
// $Id: theme-settings.php,v 1.1 2009/11/01 01:47:10 himerus Exp $

// Include the definition of zen_settings() and zen_theme_get_default_settings().
include_once './' . drupal_get_path('theme', 'omega') . '/theme-settings.php';

/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   An array of saved settings for this theme.
 * @return
 *   A form array.
 */
function alpha_settings($saved_settings) {
  // Get the default values from the .info file.
  //krumo($saved_settings);
  $subtheme_defaults = omega_theme_get_default_settings('alpha');
  //krumo($defaults);
  // Merge the saved variables and their default values.
  //$settings = array_merge($defaults, $saved_settings);
  $form = array();
  
  
  
  // Add the base theme's settings.
  $form += omega_settings($saved_settings, $subtheme_defaults);
  for ($i = 1; $i <= 24; $i++){
    $grids[$i] = $i;
  }
  $containers = array(
    '12' => '12 column grid',
    '16' => '16 column grid',
    '24' => '24 column grid'
  );
  
  // Footer Blocks
  $form['omega_container']['omega_regions']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node Configuration'),
    '#description' => t('Node region zones.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['omega_container']['omega_regions']['node']['omega_node_info_width'] = array(
    '#type' => 'select',
    '#title' => t('Default width for the node info bar'),
    '#default_value' => $saved_settings['omega_node_info_width'],
    '#options' => $grids,
    '#description' => t(''),
  );
  $form['omega_container']['omega_regions']['node']['omega_node_body_width'] = array(
    '#type' => 'select',
    '#title' => t('Default width for the main node body area'),
    '#default_value' => $saved_settings['omega_node_body_width'],
    '#options' => $grids,
    '#description' => t(''),
  );
  // Return the form
  return $form;
}

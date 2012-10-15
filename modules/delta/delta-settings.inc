<?php
// $Id: delta-settings.inc,v 1.5 2009/11/07 19:48:42 himerus Exp $
/**
 * @file
 */
/**
 * Default module settings form for Delta Theme API
 */
function delta_theme_settings_config() {
  $form['delta'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delta Theme API Settings & Configuration'),
    '#description' => t('You can manipulate how your Delta API interacts with your default theme settings here.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // get active themes for the settings form
  $filter_themes = FALSE;
  $themes = delta_get_themes_form_array($filter_themes);
  // create checkboxes to select themes that may be manipulated
  $form['delta']['delta_themes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Themes to manipulate'),
    '#options' => $themes,
    '#default_value' => variable_get('delta_themes', array()),
    '#description' => t('Here you may select the themes that should be manipulated using this Delta Theme API. Only themes that are enabled will be listed. You can enable/disable themes via the <a href="'. base_path() .'admin/build/themes">Theme Administration</a> page.'),
  );
  return system_settings_form($form);
}
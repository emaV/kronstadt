<?php
/**
 * Body Region
 * Configuration of $sidebar_first, $sidebar_last, and the main content zone
 * 
 * This section needs MUCH cleanup as I know I can condense the switch
 * statement to be much shorter than completely redeclaring each zone
 * for each case that can be present.
 * 
 * @todo cleanup switch statement
 */
$vars['content_container_width'] = $settings['omega_content_container_width'];
switch($settings['omega_content_layout']){
  default:
  case 'first_content_last':
  // FIRST - CONTENT - LAST
  $sl_max_width = $settings['omega_sidebar_first_width'] + $settings['omega_sidebar_last_width'];
  
  $sl_pull = $settings['omega_content_main_width'] + $settings['omega_sidebar_last_width'];
  
  if ($vars['sidebar_first']) {
    $vars['sidebar_first_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_last'] || !$vars['sidebar_last'], $settings['omega_sidebar_last_width']
      
    ). ' '. ns('pull-'. $sl_pull,
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    );
  }
  if ($vars['sidebar_last']) {
    $vars['sidebar_last_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_first'] || !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  if ($vars['content']) {
    $vars['main_content_classes'] = 
      ns('grid-'. $vars['content_container_width'],  
      $vars['sidebar_first'], $settings['omega_sidebar_first_width'],
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    ). ' '. ns('push-'. $settings['omega_sidebar_first_width'],
      !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  break;

  // CONTENT  - FIRST - LAST
  case 'content_first_last':
  $sl_max_width = $settings['omega_sidebar_first_width'] + $settings['omega_sidebar_last_width'];
  $sl_pull = $settings['omega_content_main_width'] + $settings['omega_sidebar_last_width'];
  $suffix_value = 1;
  $prefix_value = 1;
  if ($vars['sidebar_first']) {
    $vars['sidebar_first_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_last'] || !$vars['sidebar_last'], $settings['omega_sidebar_last_width']
    );
  }
  if ($vars['sidebar_last']) {
    $vars['sidebar_last_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_first'] || !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  if ($vars['content']) {
    $vars['main_content_classes'] = 
      ns('grid-'. $vars['content_container_width'],  
      $vars['sidebar_first'], $settings['omega_sidebar_first_width'],
      $vars['sidebar_last'], $settings['omega_sidebar_last_width'],  
      $vars['sidebar_first'], $suffix_value,
      $vars['sidebar_last'], $suffix_value
    ). ' '.
      ns('suffix-1', !$vars['sidebar_first'], $suffix_value). ' '.
      ns('suffix-1', !$vars['sidebar_last'], $suffix_value);
  }
  break;

  // FIRST - LAST - CONTENT
  case 'first_last_content':
  $sl_max_width = $settings['omega_sidebar_first_width'] + $settings['omega_sidebar_last_width'];
  
  $sl_pull = $settings['omega_content_main_width'] + $settings['omega_sidebar_last_width'];
  $sr_pull = $settings['omega_content_main_width'];
  
  if ($vars['sidebar_first']) {
    $vars['sidebar_first_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_last'] || !$vars['sidebar_last'], $settings['omega_sidebar_last_width']
    ). ' '. ns('pull-'. $sr_pull
    );
  }
  
  if ($vars['sidebar_last']) {
    $vars['sidebar_last_classes'] = 
      ns('grid-'. $settings['omega_sidebar_last_width']
    ). ' '. ns('pull-'. $sr_pull
    );
  }
  
  if ($vars['content']) {
    $vars['main_content_classes'] = 
      ns('grid-'. $vars['content_container_width'],  
      $vars['sidebar_first'], $settings['omega_sidebar_first_width'],
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    ). ' '. ns('push-'. $sl_max_width,
      !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  break;
}
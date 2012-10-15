<?php
// $Id: template.php,v 1.1 2009/11/01 01:47:10 himerus Exp $

/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */
/* -- Delete this line if you want to use and modify this code
// Example: optionally add a fixed width CSS file.
if (theme_get_setting('alpha_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}
// */


/**
 * Implementation of HOOK_theme().
 */
function alpha_theme(&$existing, $type, $theme, $path) {
  $hooks = omega_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function alpha_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function alpha_preprocess_page(&$vars, $hook) {
  if($vars['node']->nid) {
  	$vars['comment_bubble'] = l($vars['node']->comment_count, 'node/'.$vars['node']->nid, array(
	    'attributes' => array(
	      'title' => format_plural($vars['node']->comment_count, '1 comment', '@count comments'),
	    ),
	    'fragment' => 'comments',
	  ));
	  $vars['comment_bubble_add'] = l('+', 'comment/reply/'.$vars['node']->nid, array(
	    'attributes' => array(
	      'title' => t('Add Comment'),
	    ),
	  ));
  }
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */

function alpha_preprocess_node(&$vars, $hook) {
	global $theme_key;
  $settings = theme_get_settings($theme_key);
  $vars['node_info_width'] = $settings['omega_node_info_width'];
  $vars['node_body_width'] = $settings['omega_node_body_width'];
  $vars['service_links'] = $vars['service_links_rendered'];
  $vars['comment_bubble'] = l($vars['comment_count'], 'node/'.$vars['nid'], array(
    'attributes' => array(
      'title' => format_plural($vars['comment_count'], '1 comment', '@count comments'),
    ),
    'fragment' => 'comments',
  ));
  $vars['comment_bubble_add'] = l('+', 'comment/reply/'.$vars['nid'], array(
    'attributes' => array(
      'title' => t('Add Comment'),
    ),
  ));
  //krumo($vars);
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function alpha_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function alpha_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */


/**
 * Create a string of attributes form a provided array.
 * 
 * @param $attributes
 * @return string
 */
function alpha_render_attributes($attributes) {
  if($attributes) {
    $items = array();
    foreach($attributes as $attribute => $data) {
      if(is_array($data)) {
        $data = implode(' ', $data);
      }
      $items[] = $attribute . '="' . $data . '"';
    }
    $output = ' ' . implode(' ', $items);
  }
  return $output;
}
/**
 * Override for service_links formatting
 * @param $links
 */
function alpha_service_links_node_format($links) {
  return '<div class="service-links">'. theme('links', $links) .'</div>';
}
/**
 * Override theme_node_submitted
 * @param unknown_type $node
 */
function alpha_node_submitted($node) {
  return t('<span class="submitted-by">Posted by !username</span><br /><span class="submitted-date">@datetime</span>',
    array(
      '!username' => theme('username', $node),
      '@datetime' => format_date($node->created, 'small'),
    ));
}

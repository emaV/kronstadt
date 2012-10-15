<?php
// $Id: template.php,v 1.1.2.3 2009/11/05 03:32:15 sociotech Exp $

/**
 * Changed breadcrumb separator
 */
function acquia_prosper_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' &rarr; ', $breadcrumb) .'</div>';
  }
}

function acquia_prosper_preprocess_page(&$vars) {
  $vars['site_name_title'] = '<span class="first-letter">' . substr($vars['site_name'], 0, 1) . '</span>' . substr($vars['site_name'], 1, 99);


}

function acquia_prosper_node_submitted($node) {
  if($node->uid) {
    $u = user_load($node->uid);
    if(!empty($u->profile_signature)) {
      $node->name = $u->profile_signature;
    }
  }

  return t('Submitted by !username on @datetime',
    array(
      '!username' => theme('username', $node),
      '@datetime' => format_date($node->created),
    ));
}

function acquia_prosper_preprocess_user_picture(&$variables) {
//dpm($variables);
  $account = $variables['account'];
  $link = url("user/" . $account->uid);
  $file =  mobile_codes_generate($link);
  $picture = $account->picture ? $account->picture : file_create_url($file);
 
  $alt = t("@user's picture", array('@user' => $account->name ? $account->name : variable_get('anonymous', t('Anonymous'))));
  $variables['picture'] = theme('image', $picture, $alt, $alt, '', FALSE);
  if (!empty($account->uid) && user_access('access user profiles')) {
    $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
    $variables['picture'] = l($variables['picture'], "user/$account->uid", $attributes);
  }
}

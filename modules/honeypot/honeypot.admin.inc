<?php

/**
 * @file
 *
 * Honeypot administration form.
 */

/**
 * Honeypot administration page.
 */
function honeypot_admin_form($form) {
  $form = array();

  // Honeypot Configuration.
  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Honeypot Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
    $form['configuration']['honeypot_protect_all_forms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Protect all forms with Honeypot'),
      '#description' => t('This will enable Honeypot protection for ALL forms on this site, regardless of the settings in the Honeypot enabled forms section below.'),
      '#default_value' => variable_get('honeypot_protect_all_forms', 0),
    );
    $form['configuration']['honeypot_log'] = array(
      '#type' => 'checkbox',
      '#title' => t('Log blocked form submissions'),
      '#description' => t('Log submissions that are blocked due to Honeypot protection.'),
      '#default_value' => variable_get('honeypot_log', 0),
    );
    $form['configuration']['honeypot_element_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Honeypot element name'),
      '#description' => t("The name of the Honeypot form field. It's usually most effective to use a generic name like email, url, or name, but this should be changed if it interferes with fields that are already in your forms. Must not contain spaces or special characters."),
      '#default_value' => variable_get('honeypot_element_name', 'url'),
      '#required' => TRUE,
      '#size' => 30,
    );
    $form['configuration']['honeypot_time_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Honeypot time limit'),
      '#description' => t('Minimum time required before form should be considered entered by a human instead of a bot. Set to 0 to disable. <strong>Page caching will be disabled if there is a form protected by time limit on the page.</strong>'),
      '#default_value' => variable_get('honeypot_time_limit', 5),
      '#required' => TRUE,
      '#size' => 5,
      '#field_suffix' => t('seconds'),
    );

  // Honeypot Enabled forms.
  $form['enabled_forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Honeypot Enabled Forms'),
    '#description' => t("Check the boxes next to individual forms on which you'd like Honeypot protection enabled."),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

    // Generic forms.
    $form['enabled_forms']['general_forms'] = array('#value' => '<h5>' . t('General Forms') . '</h5>');
    // User register form.
    $form['enabled_forms']['honeypot_form_user_register'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Registration form'),
      '#default_value' => variable_get('honeypot_form_user_register', 0),
    );
    // User password form.
    $form['enabled_forms']['honeypot_form_user_pass'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Password Reset form'),
      '#default_value' => variable_get('honeypot_form_user_pass', 0),
    );

    // If webform.module enabled, add webforms.
    if (module_exists('webform')) {
      $form['enabled_forms']['honeypot_form_webforms'] = array(
        '#type' => 'checkbox',
        '#title' => t('Webforms (all)'),
        '#default_value' => variable_get('honeypot_form_webforms', 0),
      );
    }

    // If contact.module enabled, add contact forms.
    if (module_exists('contact')) {
      $form['enabled_forms']['contact_forms'] = array('#value' => '<h5>' . t('Contact Forms') . '</h5>');
      $form['enabled_forms']['honeypot_form_contact_mail_page'] = array(
        '#type' => 'checkbox',
        '#title' => t('Sitewide Contact form'),
        '#default_value' => variable_get('honeypot_form_contact_mail_page', 0),
      );
      $form['enabled_forms']['honeypot_form_contact_mail_user'] = array(
        '#type' => 'checkbox',
        '#title' => t('Personal Contact forms'),
        '#default_value' => variable_get('honeypot_form_contact_mail_user', 0),
      );
    }

    // If profile.module enabled, add profile forms.
    if (module_exists('profile')) {
      $form['enabled_forms']['profile_forms'] = array('#value' => '<h5>' . t('Profile Forms') . '</h5>');
      $form['enabled_forms']['honeypot_form_user_profile_form'] = array(
        '#type' => 'checkbox',
        '#title' => t('Profile forms (all)'),
        '#default_value' => variable_get('honeypot_form_user_profile_form', 0),
      );
    }

    // Node forms.
    $types = node_get_types();
    $form['enabled_forms']['node_forms'] = array('#value' => '<h5>' . t('Node Forms') . '</h5>');
    foreach ($types as $type) {
      $id = 'honeypot_form_' . $type->type . '_node_form';
      $form['enabled_forms'][$id] = array(
        '#type' => 'checkbox',
        '#title' => t('@name node form', array('@name' => $type->name)),
        '#default_value' => variable_get($id, 0),
      );
    }

    // Comment forms.
    if (module_exists('comment')) {
      $form['enabled_forms']['comment_forms'] = array('#value' => '<h5>' . t('Comment Forms') . '</h5>');
      $form['enabled_forms']['honeypot_form_comment_form'] = array(
        '#type' => 'checkbox',
        '#title' => t('Comment forms (all)'),
        '#default_value' => variable_get('honeypot_form_comment_form', 0),
      );
    }

  // Add our own submit handler to clear honeypot's form cache on save.
  $form['#submit'][] = 'honeypot_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Validate the admin form.
 */
function honeypot_admin_form_validate($form, &$form_state) {
  // Make sure the time limit is a positive integer or 0.
  $time_limit = $form_state['values']['honeypot_time_limit'];
  if ((is_numeric($time_limit) && $time_limit > 0) || $time_limit === '0') {
    if (ctype_digit($time_limit)) {
      // Good to go.
    }
    else {
      form_set_error('honeypot_time_limit', t("The time limit must be a positive integer or 0."));
    }
  }
  else {
    form_set_error('honeypot_time_limit', t("The time limit must be a positive integer or 0."));
  }

  // Make sure Honeypot element name only contains A-Z, 0-9.
  if (!preg_match("/^[-_a-zA-Z0-9]+$/", $form_state['values']['honeypot_element_name'])) {
    form_set_error('honeypot_element_name', t("The element name cannot contain spaces or other special characters."));
  }
}

/**
 * Clear the honeypot form cache on submit.
 */
function honeypot_admin_form_submit($form, &$form_state) {
  cache_clear_all('honeypot_protected_forms', 'cache');
}

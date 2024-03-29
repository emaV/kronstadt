<?php

/**
 * @file
 *
 * API Functionality for Honeypot module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the honeypot protections added to a particular form.
 *
 * @param (array) $options
 *   Protections that will be applied to the form. May be empty, or may include
 *   'honeypot' and/or 'time_restriction'.
 * @param (array) $form
 *   The Form API form to which protections will be added.
 */
function hook_honeypot_form_protections_alter(&$options, $form) {
  // Add 'time_restriction' protection to 'mymodule-form' if it's not set.
  if ($form['form_id']['#value'] == 'mymodule_form' && !in_array('time_restriction', $options)) {
    $options[] = 'time_restriction';
  }
}

/**
 * React to an addition of honeypot form protections for a given form_id.
 *
 * After honeypot has added its protections to a form, this hook will be called.
 * You can use this hook to track when and how many times certain protected
 * forms are displayed to certain users, or for other tracking purposes.
 *
 * @param (array) $options
 *   Protections that were applied to the form. Includes 'honeypot' and/or
 *   'time_restriction'.
 * @param (array) $form
 *   The Form API form to which protections were added.
 */
function hook_honeypot_add_form_protection($options, $form) {
  if ($form['form_id']['#value'] == 'mymodule_form') {
    // Do something...
  }
}

/**
 * Add time to the Honeypot time limit.
 *
 * In certain circumstances (for example, on forms routinely targeted by
 * spammers), you may want to add an additional time delay. You can use this
 * hook to return additional time (in seconds) to honeypot when it is calculates
 * the time limit for a particular form.
 *
 * @param (int) $honeypot_time_limit
 *   The current honeypot time limit (in seconds), to which any additions you
 *   return will be added.
 * @param (array) $form_values
 *   Array of form values (may be empty).
 * @param (int) $number
 *   Number of times the current user has already fallen into the honeypot trap.
 *
 * @return (int) $additions
 *   Additional time to add to the honeypot_time_limit, in seconds (integer).
 */
function hook_honeypot_time_limit($honeypot_time_limit, $form_values, $number) {
  $additions = 0;
  // If 'some_interesting_value' is set in a form, add 10 seconds to limit.
  if (!empty($form_values['some_interesting_value']) && $form_values['some_interesting_value']) {
    $additions = 10;
  }
  return $additions;
}

/**
 * @} End of "addtogroup hooks".
 */

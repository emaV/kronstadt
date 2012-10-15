<?php
/**
 * @file
 * Admin module integration.
 */

/**
 * Implements hook_mobile_codes_block_alter() on behalf of block.module.
 */
function admin_mobile_codes_block_alter($blocks) {
  $blocks['generate_code']['admin'] = TRUE;
}

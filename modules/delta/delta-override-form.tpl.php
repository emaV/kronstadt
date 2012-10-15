<?php
// $Id: delta-override-form.tpl.php,v 1.6 2009/11/08 03:15:09 himerus Exp $

/**
 * Themed form for managing Delta API overrides.
 * It kinda is a big deal
 */
?>
<div>
  <table id="delta-overrides" class="sticky-enabled">
    <thead>
      <tr>
        <th><?php print t('Name'); ?></th>
        <th><?php print t('Type'); ?></th>
        <th><?php print t('Operations'); ?></th>
        <th><?php print t('Current Theme Template'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php $row = 0; ?>
        <?php if(is_array($delta_listing)): ?>
        <?php foreach ($delta_listing as $delta => $data): ?>
        <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?><?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
          <td class="block"><?php print $data->name; ?></td>
          <td><?php print $data->types; ?></td>
          <td><?php print $data->edit_link; ?> | <?php print $data->delete_link; ?> | <?php print $data->export_link; ?></td>
          <td class="non-clear"><?php print $data->theme_settings_template; ?> <?php print $data->add_template_link; ?></td>
          <td><?php print $data->weight_select; ?></td>
        </tr>
        <?php $row++; ?>
        <?php endforeach; ?>
        <?php else: ?>
        <td colspan="5"><?php print t('There are currently no overrides for this theme. You may ') . $default_add_override_link; ?></td>
        <?php endif; ?>
    </tbody>
  </table>
</div>
<?php print $form_submit; ?>

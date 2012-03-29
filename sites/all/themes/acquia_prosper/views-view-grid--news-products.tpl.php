<?php
/**
 * @file views-view-grid.tpl.php
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 * - $class contains the class of the table.
 * - $attributes contains other attributes for the table.
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
    <?php foreach ($rows as $row_number => $columns): ?>
      <?php
        $row_class = 'row-' . ($row_number + 1);
        if ($row_number == 0) {
          $row_class .= ' row-first';
        }
        if (count($rows) == ($row_number + 1)) {
          $row_class .= ' row-last';
        }
      ?>
        <?php foreach ($columns as $column_number => $item): ?>
            <div class="corner"><?php print $item; ?></div>
        <?php endforeach; ?>
    <?php endforeach; ?>
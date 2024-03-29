<?php
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($fields as $id => $field): ?>
  <?php if (!empty($field->separator)): ?>
    <?php print $field->separator; ?>
  <?php endif; ?>

  <<?php print $field->inline_html;?> class="views-field-<?php print $field->class; ?>">
    <?php if ($field->label): ?>
      <label class="views-label-<?php print $field->class; ?>">
        <?php print $field->label; ?>:
      </label>
    <?php endif; ?>
      <?php
      // $field->element_type is either SPAN or DIV depending upon whether or not
      // the field is a 'block' element type or 'inline' element type.
      ?>
      <<?php print $field->element_type; ?> class="field-content"><?php print $field->content; ?></<?php print $field->element_type; ?>>
  </<?php print $field->inline_html;?>>
<?php endforeach; ?>

 <?php
 /* 
 // шаг 1. получаем id переменных в шаблоне
<?php foreach ($fields as $id => $field): ?>
<?php echo  $id.'<br />';?>
<?php print $field->content; ?>
<?php endforeach; ?>

// шаг 2. присваиваем полученные значения переменным
<?php foreach ($fields as $id => $field): ?>
<?php 
if($id=='title'){$title = $field->content;}
if($id=='field_image'){$img = $field->content;}
if($id=='body'){$text = $field->content;}
if($id=='created'){$time = $field->content;}
if($id=='view_node'){$link = $field->content;}
?>
<?php endforeach; ?>

// шаг 3. вставляем полученные переменные в шаблон

*/
?>
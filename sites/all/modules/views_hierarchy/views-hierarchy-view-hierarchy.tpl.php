<?php
// $Id: views-hierarchy-view-hierarchy.tpl.php,v 1.1 2010/06/04 11:13:26 joachim Exp $
/**
 * @file views-hierarchy-view-hierarchy.tpl.php
 * Default simple view template to display a hierarchy of rows.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * - $hierarchy: the rendered hierarchy. This comes already rendered by
 *   theme_item_list() because we have to call the rendering recursively and you
 *   really don't want to be recursively loading a template file.
 *   @see template_preprocess_views_hierarchy_view_hierarchy().
 */
?>
<div class="item-list">
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print $hierarchy; ?>
</div>
<?php
/**
 * @file
 * Template file for displaying the node content, associated with an image, in
 * the lightbox.  It displays it without any sidebars, etc.
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
 <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $setting_styles; ?>
  <?php print $local_styles; ?>
  <?php print $scripts; ?>
  </head>
  <body>
    <?php if ($title): ?>
    <h1 class="title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print $content ?>
	<?php print $closure; ?>

</body>
</html>

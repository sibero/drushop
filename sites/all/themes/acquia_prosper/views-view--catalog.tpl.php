<div class="<?php print $classes; ?>">
  <?php if ($admin_links): ?>
    <div class="views-admin-links views-hide">
      <?php print $admin_links; ?>
    </div>
  <?php endif; ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>
  


<?php $path1 = drupal_get_path_alias($_GET['q']); ?> 
<?php $path = $GLOBALS['base_path'].$path1; ?>

<div class="sortby"><?php print t("Sort by:"); ?> <form name="menu2">
<select name="sel" onChange="linklist(document.menu2.sel)">
<option value="#">выбрать</option>
<option title="<?php print t("Цене (от минимальной до максимальной)"); ?>" value="<?php print $path; ?>?orderby=sell_price_asc"><?php print t("Цене (убывание)"); ?></option>
<option title="<?php print t("Цене (от максимальной до минимальной)"); ?>" value="<?php print $path; ?>?orderby=sell_price_desc"><?php print t("Цене (возрастание)"); ?></option>

<option title="<?php print t("Названию"); ?>" value="<?php print $path; ?>?orderby=node_title_asc"><?php print t("Названию (от а до я)"); ?></option>
<option title="<?php print t("Названию"); ?>" value="<?php print $path; ?>?orderby=node_title_desc"><?php print t("Названию (от я до а)"); ?></option>
<option title="<?php print t("По рейтингу"); ?>" value="<?php print $path; ?>?orderby=voting_desc"><?php print t("Рейтингу"); ?></option>
<option title="<?php print t("Новые поступления"); ?>" value="<?php print $path; ?>?orderby=node_created_desc"><?php print t("Новые поступления"); ?></option>
<option title="<?php print t("Самые комментируемые"); ?>" value="<?php print $path; ?>?orderby=node_comment_desc"><?php print t("Самые коменнтируемые"); ?></option>
</select>
</form>
</div>

  <?php if ($exposed): ?>
<input id='filterby' type='button' value='<?php print t("Filter by"); ?>' 
        onclick="toggle_visibility('my_div')">
<div id='my_div' class='div_container'> 
   <div class="view-filters">
   
      <?php print $exposed; ?>
    </div>
   <input type='hidden'
         onclick="toggle_visibility('my_div')">
</div>
	<?php endif; ?>
	
	<?php print $pager; ?>
	<?php if ($rows): ?>
    <div class="view-content">
	
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

 
    <?php print $pager; ?>


  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  
    <?php print $more; ?>


  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div>
<div id="node-<?php print $node->nid; ?>" class="node clear-block <?php print $node_classes; ?>">

  <div class="inner">
    <?php if ($node_top && !$teaser): ?>
    <div id="node-top" class="node-top row nested">
      <div id="node-top-inner" class="node-top-inner inner">
        <?php print $node_top; ?>
      </div><!-- /node-top-inner -->
    </div><!-- /node-top -->
    <?php endif; ?>

    <div id="product-group" class="product-group">
      <div class="images">
        <?php print $product_image; ?>
      </div><!-- /images -->

      <div class="content">
        <div id="content-body">
          <?php print $product_body; ?>
        </div>

        <?php $product_details = $product_weight || $product_dimensions || $product_list_price || $product_sell_price || $product_sku || $product_cost; ?>
        <div id="product-details" class="clear<?php if (!$product_details): ?> field-group-empty<?php endif; ?>">
          <div id="field-group">
			<?php print $product_sku; ?>
            <?php print $product_weight; ?>
			<?php print $product_length ?>
            <?php print $product_width; ?> 
			<?php print $product_height; ?> 
			<?php print $product_cost ?>
			 
			
			
        <div id="product-additional" class="product-additional">
			
			<?php print $field_stock; ?>
			<?php print $product_additional; ?> 	
			</br>
			</br>
			<?php print $product_bookmarks; ?>
		  
		 
        </div>
        
          </div>

          <div id="price-group">
		    <?php print $product_list_price."<br>"; ?>
			
            <?php print t("Sell price:").$product_display_price; ?>
			<br>
			
			<?php if ($product_discounted_price) { 
			print $product_discounted_price; } ?>
			<?php print $product_total_price_discount; ?>
			<br>
			
			<?php print $product_add_to_cart; ?>
          </div>
		  
        </div><!-- /product-details -->
	
<?php print $product_fivestar_widget ?>  
        <?php if ($terms): ?>
        <div class="terms">
      <IMG src="<?php echo $imgPath;?>tags.png"> <?php print t('Tags:'). "  ".$terms ?> 
		  
        </div>
        <?php endif;?>

       
        <div class="links clear">
         <?php print $links; ?>
        </div>
       <?php print $service_links ?> 
      </div><!-- /content -->
    </div><!-- /product-group -->
  </div><!-- /inner -->

  <?php if ($node_bottom && !$teaser): ?>
  <div id="node-bottom" class="node-bottom row nested">
    <div id="node-bottom-inner" class="node-bottom-inner inner">
      <?php print $node_bottom; ?>
    </div><!-- /node-bottom-inner -->
  </div><!-- /node-bottom -->
  <?php endif; ?>
</div>

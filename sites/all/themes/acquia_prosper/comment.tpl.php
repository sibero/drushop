<?php
// $Id: comment.tpl.php,v 1.2 2010/09/17 21:36:06 eternalistic Exp $
?>

<div class="comment-wrapper">
  <div class="comment <?php print $comment_classes;?> clear-block <?php if ($picture): ?>withpicture<?php endif; ?>">

    
    <div class="withpicture-column-left">
      <?php print $picture ?>
    </div>
   
    
   
    <div class="withpicture-column-right">
   
      <?php if ($comment->new): ?>
      <a id="new"></a>
      <span class="new"><?php print $new ?></span>
      <?php endif; ?>
      <div class="submitted">
        <?php print $submitted ?>
      </div>
      
      <div class="content">
        <div class="comment-top-left"></div><!-- /comment-top-left -->
        <div class="content-inner">
          <?php print $content ?>
		  
          <?php if ($signature): ?>
          <div class="signature">
            <?php print $signature ?>
          </div>
          <?php endif; ?>

          <div class="links">
            <?php print $links ?>
          </div>
        </div><!-- /content-inner -->
      </div><!-- /content -->

    <?php if ($picture): ?>
    </div><!-- /withpicture-column-right -->
    <?php endif; ?>
  </div><!-- /comment -->
</div><!-- /comment-wrapper -->
<?php
// $Id: template.php,v 1.5 2010/09/17 21:36:06 eternalistic Exp $

function acquia_prosper_comment_thread_expanded($comment, $node) {
  $links = module_invoke_all('link', 'comment', $comment, 0);
  drupal_alter('link', $links, $node); 
  
  return theme('comment_view', $comment, $node, $links);
}

function acquia_prosper_comment_flat_expanded($comment, $node) {
  $links = module_invoke_all('link', 'comment', $comment, 0);
  drupal_alter('link', $links, $node);
  
  return theme('comment_view', $comment, $node, $links);
}



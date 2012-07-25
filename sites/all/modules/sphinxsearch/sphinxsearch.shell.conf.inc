<?php
function sphinxsearch_source_config($indexes) {
  $source = '';
  $domain = variable_get('sphinx_home_domain', $_SERVER['SERVER_NAME']);
  if (!empty($indexes)) {
    foreach($indexes as $key => $value) {
      if ($key === 0) {
        $source .= 'source source_main'.$key.'
{
  type                                    = xmlpipe
  xmlpipe_command                 = /usr/bin/lynx -source http://' . $domain. '/sphinxsearch_scripts/sphinxsearch_xmlpipe.php?mode=main\&id='.$key.'\&first_nid='.$value['first'].'\&last_nid='.$value['last'].'
}'."\n";
      }
      else {
        $source .= 'source source_main'.$key.' : source_main0
{
  xmlpipe_command                 = /usr/bin/lynx -source http://' . $domain. '/sphinxsearch_scripts/sphinxsearch_xmlpipe.php?mode=main\&id='.$key.'\&first_nid='.$value['first'].'\&last_nid='.$value['last'].'
}'."\n";
      }
    }
    $source .= 'source source_delta : source_main0
{
  xmlpipe_command                 = /usr/bin/lynx -source http://' . $domain . '/sphinxsearch_scripts/sphinxsearch_xmlpipe.php?mode=delta
}';
  }
  return $source;
}


function sphinxsearch_index_config($indexes) {
  $index = '';
  $domain = variable_get('sphinx_home_domain', $_SERVER['SERVER_NAME']);
  if (!empty($indexes)) {
    foreach($indexes as $key => $value) {
      if ($key === 0) {
        $index .= 'index index_main'.$key.'
{
        source                                  = source_main'.$key.'
        path                                    = ' . SPHINX_HOME_SPHINX . '/index/main'.$key.'
        
        docinfo                                 = extern
        mlock                                   = 1
        morphology                              = stem_ru
        charset_type                    = utf-8
        charset_table                   = 0..9, A..Z->a..z, _, a..z, U+410..U+42F->U+430..U+44F, U+430..U+44F
        min_word_len                    = 1
        #       min_infix_len                   = 3
        #       enable_star                             = 1
        #       prefix_fields                   = content
        #       infix_fields                    = subject
        
        html_strip                              = 0
        #       html_index_attrs                = img=alt,title; a=title;
        #       html_remove_elements    = style, script
        
        #       preopen                                 = 1
        }'."\n";
      }
      else {
        $index .= 'index index_main'. $key . ': index_main0
{
        source                                  = source_main' . $key . '
        path                                    = ' . SPHINX_HOME_SPHINX .'/index/index'.$key.'
}'."\n";
      }
    }
    $index .= 'index index_delta: index_main0
{
        source                                  = source_delta
        path                                    = '.SPHINX_HOME_SPHINX . '/index/index_delta
}';
  }
  return $index;
}


function sphinxsearch_join_index($indexes) {
  if (!empty($indexes)) {
    $join = '';
    $join .= 'index index_join
    {
            type                                    = distributed'."\n";
    
    foreach($indexes as $key => $value) {
      $join .= 'local                                   = index_main' . $key . "\n";        
    }
    
    $join .= 'local                                   = index_delta
    }';
  }
  return $join;
}
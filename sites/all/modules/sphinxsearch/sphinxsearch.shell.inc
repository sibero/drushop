<?php
/**
 * @file
 *
 * It-patrol sphinx functionality
 */
define('SPHINX_HOME', sphinxsearch_get_user_dir());
define('SPHINX_HOME_DOMAIN', $_SERVER['DOCUMENT_ROOT']);
define('SPHINX_HOME_SPHINX', SPHINX_HOME.'/sphinx/'.variable_get('sphinxsearch_home_domain', $_SERVER['SERVER_NAME']));
define('SPHINX_DAEMON_PATH', '/usr/sbin/sphinx-searchd');
define('SPHINX_INDEXER_PATH', '/usr/bin/sphinx-indexer');
define('SPHINX_CONF_FILE', SPHINX_HOME_SPHINX.'/config/sphinx.conf');

//create nessesary files and direcroties
function sphinxsearch_install_config() {
  //create
  $create[] = SPHINX_HOME.'/sphinx';
  $create[] = SPHINX_HOME_SPHINX;
  $create[] = SPHINX_HOME_SPHINX.'/config';
  $create[] = SPHINX_HOME_SPHINX.'/index';
  $create[] = SPHINX_HOME_SPHINX.'/log';
  $create[] = SPHINX_HOME_SPHINX.'/socket';
  
  foreach ($create as $value) {
    if (!file_check_directory($value)) {
      mkdir($value, 0744);
    }
  }
  //copy sphinxsearch_scripts directory
  recurse_copy(SPHINX_HOME_DOMAIN.'/'.drupal_get_path('module', 'sphinxsearch').'/sphinxsearch_scripts', SPHINX_HOME_DOMAIN.'/sphinxsearch_scripts');
  //copy sphinxsearch config file
  copy(SPHINX_HOME_DOMAIN.'/'.drupal_get_path('module', 'sphinxsearch').'/docs/contrib/sphinx.conf', SPHINX_HOME_SPHINX.'/config/sphinx.conf');
}


//write nessesary functions for config file
function sphinxsearch_write_config() {
  $file_config = SPHINX_HOME_SPHINX.'/config/sphinx.conf';
  if ($fp = fopen($file_config, 'r')) {
    $buffer = '';
    while (!feof($fp)) {
      $line = fgets($fp);
      $line = str_replace('http://example.com', 'http://'.variable_get('sphinx_home_domain', $_SERVER['SERVER_NAME']), $line);
      $line = str_replace('/home/user/sphinx/example.com', SPHINX_HOME_SPHINX, $line);
      $buffer .= $line;
    }
  }
  fclose($fp);
  
  if ($fp = fopen($file_config, 'w')) {
    fwrite($fp, $buffer);
  }
  fclose($fp);
}


function sphinxsearch_set_variables() {
  $tmp_ip_mask = explode('.', $_SERVER['SERVER_ADDR']);
  array_pop($tmp_ip_mask);
  $ip_mask = implode('.', $tmp_ip_mask).'.0/24';
  
  variable_set('sphinxsearch_connect_method', 'socket');
  variable_set('sphinxsearch_searchd_host', SPHINX_HOME_SPHINX.'/socket/sphinx.s');
  variable_set('sphinxsearch_home_domain', $_SERVER['SERVER_NAME']);
  variable_set('sphinxsearch_searchd_port', 0);
  variable_set('sphinxsearch_indexer_ips', $ip_mask);
  variable_set('sphinxsearch_query_index', 'index_join');
  variable_set('sphinxsearch_excerpts_index', 'index_main0');
  variable_set('sphinxsearch_index_last_time', 0);
  variable_set('sphinxsearch_merge_last_time', 0);
  variable_set('sphinxsearch_delta_index_time', 5);
  variable_set('sphinxsearch_merge_index_time', 7);
}

function sphinxsearch_run_commands() {
  $commands[] = SPHINX_INDEXER_PATH . " --config ". SPHINX_CONF_FILE . " index_main0";
  $commands[] = SPHINX_DAEMON_PATH . " --config ". SPHINX_CONF_FILE;
  $commands[] = SPHINX_INDEXER_PATH . " --config ". SPHINX_CONF_FILE . " index_delta";
  $commands[] = SPHINX_DAEMON_PATH . " --config ". SPHINX_CONF_FILE . " --stop";
  $commands[] = SPHINX_DAEMON_PATH . " --config ". SPHINX_CONF_FILE;
  foreach ($commands as $command) {
    exec($command, $message);
  }
}

//uninstall function
function sphinxsearch_uninstall_config() {
  $command = SPHINX_DAEMON_PATH . " --config ". SPHINX_CONF_FILE . " --stop";
  exec($command);
  
  delete_directory(SPHINX_HOME_SPHINX);
  delete_directory(SPHINX_HOME_DOMAIN .'/sphinxsearch_scripts');
  
  variable_del('sphinxsearch_connect_method');
  variable_del('sphinxsearch_searchd_host');
  variable_del('sphinxsearch_home_domain');
  variable_del('sphinxsearch_searchd_port');
  variable_del('sphinxsearch_indexer_ips');
  variable_del('sphinxsearch_query_index');
  variable_del('sphinxsearch_excerpts_index');
  variable_del('sphinxsearch_index_last_time');
  variable_del('sphinxsearch_merge_last_time');
  variable_del('sphinxsearch_delta_index_time');
  variable_del('sphinxsearch_merge_index_time');
  
  
}


//copy directory
function recurse_copy($src,$dst) { 
    $dir = opendir($src); 
    @mkdir($dst); 
    while(false !== ( $file = readdir($dir)) ) { 
        if (( $file != '.' ) && ( $file != '..' )) { 
            if ( is_dir($src . '/' . $file) ) { 
                recurse_copy($src . '/' . $file,$dst . '/' . $file); 
            } 
            else { 
                copy($src . '/' . $file,$dst . '/' . $file); 
            } 
        } 
    } 
    closedir($dir); 
}


//delete directory
function delete_directory($directory, $empty = false) { 
    if(substr($directory,-1) == "/") { 
        $directory = substr($directory,0,-1); 
    } 

    if(!file_exists($directory) || !is_dir($directory)) { 
        return false; 
    } elseif(!is_readable($directory)) { 
        return false; 
    } else { 
        $directoryHandle = opendir($directory); 
        
        while ($contents = readdir($directoryHandle)) { 
            if($contents != '.' && $contents != '..') { 
                $path = $directory . "/" . $contents; 
                
                if(is_dir($path)) { 
                    delete_directory($path); 
                } else { 
                    unlink($path); 
                } 
            } 
        } 
        
        closedir($directoryHandle); 

        if($empty == false) { 
            if(!rmdir($directory)) { 
                return false; 
            } 
        } 
        
        return true; 
    } 
}


/**
 * Callback function for excute commands page
 */
function sphinxsearch_commands () {
  return drupal_get_form('sphinxsearch_commands_form');
}


/**
 * Return form for execute commands page
 */
function sphinxsearch_commands_form() {
  $form['commands'] = array(
    '#title' => t('Choose command'),
    '#type' => 'radios',
    '#options' => array(
      'none' => t('None. Do nothing.'),
      'start' => t('Start sphinx daemon. Use it if your sphinx daemon is not already served.'),
      'stop' => t('Stop sphinx daemon. Use to stop sphinx daemon.'),
      'index' => t('Start full indexing site. Use for first main index. WARNING: You must stop sphinx daemon before this operation'),
      'delta' => t('Start delta indexing site. Use for delta index. WARNING: You must start sphinx daemon before this operation'),
      'rotate' => t('Start delta reindexing with rotate option. Use for full delta reindex with --rotate option(sphinx daemon doesn\'t stop work)'),
    ),
    '#default_value' => 'none',
  );
  
  $form['sphinxsearch_delta_index_time'] = array(
    '#title' => t('Enter periodicity for delta reindex(in minutes)'),
    '#type' => 'textfield',
    '#size' => 3,
    '#default_value' => variable_get('sphinxsearch_delta_index_time', 5),
  );
  
  $form['sphinxsearch_merge_index_time'] = array(
    '#title' => t('Enter periodicity for merging delta and main index(in days)'),
    '#type' => 'textfield',
    '#size' => 3,
    '#default_value' => variable_get('sphinxsearch_merge_index_time', 7),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  $form['#validate'][] = 'sphinxsearch_commands_form_validate';
  $form['#submit'][] = 'sphinxsearch_commands_form_submit';
  
  return $form;
}


/**
 * Generate config page
 */
function sphinxsearch_generate_config() {
  return drupal_get_form('sphinxsearch_generate_config_form');
}


function sphinxsearch_commands_form_validate($form, &$form_state) {  
  if (!is_numeric($form_state['values']['sphinxsearch_delta_index_time'])) {
    form_set_error('sphinxsearch_delta_index_time', t('You must enter integer value'));
  }
  
  if (!is_numeric($form_state['values']['sphinxsearch_merge_index_time'])) {
    form_set_error('sphinxsearch_merge_index_time', t('You must enter integer value'));
  }
}


function sphinxsearch_commands_form_submit($form, &$form_state) {
  switch ($form_state['values']['commands']) {
    case 'none':
      $message = t('Nothing has been executed.');
      break;
    case 'start':
      $command = SPHINX_DAEMON_PATH . " --config ". SPHINX_CONF_FILE;
      break;
    
    case 'stop':
      $command = SPHINX_DAEMON_PATH . " --config ". SPHINX_CONF_FILE . " --stop";
      break;
    
    case 'index':
      $command = SPHINX_INDEXER_PATH . " --config ". SPHINX_CONF_FILE . " index_main0";
      break;
    
    case 'delta':
      $command = SPHINX_INDEXER_PATH . " --config ". SPHINX_CONF_FILE . " index_delta";
      break;
    
    case 'rotate':
      $command = SPHINX_INDEXER_PATH . " --config ". SPHINX_CONF_FILE . " --rotate index_delta";
      break;
  }
  
  if ($command) {
    exec($command, $message);
  }
  $message = sphinxsearch_messages($message);
  if ($message) {
    drupal_set_message($message);
  }
  
  variable_set('sphinxsearch_delta_index_time', $form_state['values']['sphinxsearch_delta_index_time']);
  variable_set('sphinxsearch_merge_index_time', $form_state['values']['sphinxsearch_merge_index_time']);
}


/**
 * Generate config form
 */
function sphinxsearch_generate_config_form(&$form_state) {
  $form['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Size of indexes'),
    '#size' => 7,
    '#required' => TRUE,
    '#default_value' => 500,
  );
  
  $form['#validate'][] = 'sphinxsearch_generate_config_form_validate';
  $form['#submit'][] = 'sphinxsearch_generate_config_form_submit';
  
  if ($form_state['values']['size']) {
    require_once('sphinxsearch.shell.conf.inc');
    
    $indexes = sphinxsearch_get_indexes($form_state['values']['size']);
    $form['sources'] = array(
      '#type' => 'textarea',
      '#title' => t('Sources'),
      '#default_value' => sphinxsearch_source_config($indexes),
    );
    $form['indexes'] = array(
      '#type' => 'textarea',
      '#title' => t('Indexes'),
      '#default_value' => sphinxsearch_index_config($indexes),
    );
    $form['join_index'] = array(
      '#type' => 'textarea',
      '#title' => t('Join index'),
      '#default_value' => sphinxsearch_join_index($indexes),
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );
  return $form;
}


function sphinxsearch_generate_config_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['size'])) {
    form_set_error('size', t('You must enter integer value'));
  }
}


function sphinxsearch_generate_config_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  
}


function sphinxsearch_reindex_cron() {
  $index_last_time = variable_get('sphinxsearch_index_last_time', 0);
  $merge_last_time = variable_get('sphinxsearch_merge_last_time', 0);
  $index_time = variable_get('sphinxsearch_delta_index_time', 5);
  $merge_time = variable_get('sphinxsearch_merge_index_time', 7);
  
  if (((time() - $index_last_time)/60 > $index_time) && $index_time != 0) {
    $command = SPHINX_INDEXER_PATH. " --config " . SPHINX_CONF_FILE . " --rotate index_delta";
    exec($command, $message);
    $message = sphinxsearch_messages($message);
    if ($message) {
      watchdog('sphinxsearch', $message);
    }
    variable_set('sphinxsearch_index_last_time', time());
  }
  
  if (((time() - $merge_last_time)/(60*60*24) > $merge_time) && $merge_time != 0) {
    $command = SPHINX_INDEXER_PATH. " --config " . SPHINX_CONF_FILE . " --merge index_main0 index_delta --merge-dst-range deleted 0 0 --rotate";
    exec($command, $message);
    $message = sphinxsearch_messages($message);
    if ($message) {
      watchdog('sphinxsearch', $message);
    }
    
    $command = SPHINX_INDEXER_PATH . " --config ". SPHINX_CONF_FILE . " --rotate index_delta";
    exec($command, $message);
    $message = sphinxsearch_messages($message);
    if ($message) {
      watchdog('sphinxsearch', $message);
    }
    
    variable_set('sphinxsearch_merge_last_time', time());
  }
}


function sphinxsearch_get_user_dir() {
  $current_user = posix_getpwuid(posix_geteuid());
  return $current_user['dir'];
}


function sphinxsearch_messages($message) {
  if (count($message) && is_array($message)) {
    return implode('<br/>', $message);
  }
  elseif(is_string($message)) {
    return $message;
  }
}


function sphinxsearch_autoconfig() {
  if (drupal_valid_token($_GET['token'])) {
    sphinxsearch_run_commands();
    drupal_set_message('Now you can search content on your site with Sphinx!');
    drupal_goto(sphinxsearch_get_search_path());
  }
  else {
    return drupal_access_denied();
  }
}


/**
 * Get index array
 */
function sphinxsearch_get_indexes($size = 500) {
  
  //first & last nid
  $first = db_result(db_query_range("SELECT nid
                                    FROM {node} node
                                    ORDER BY nid ASC", array(0, 1)));
  $last = db_result(db_query_range("SELECT nid
                                    FROM {node} node
                                    ORDER BY nid DESC", array(0, 1)));
  $count = db_result(db_query_range("SELECT COUNT(nid)
                                    FROM {node} node
                                    ORDER BY nid DESC", array(0, 1)));
  $query = db_query_range("SELECT nid
                            FROM {node} node
                            ORDER BY nid ASC");
  $index = array();
  $next = 0;
  if ($first <= $last) {
    while($next <= $count) {
      $query = db_query_range("SELECT nid
                              FROM {node} node
                              ORDER BY nid ASC", $next, $size);
      $tmp = array();
      while($data = db_fetch_object($query)) {
        $tmp[] = $data->nid;
      }
      if (!empty($tmp)) {
        $first_current = array_shift($tmp);
        $last_current = array_pop($tmp);
        $index[] = array(
          'first' => $first_current,
          'last' => $last_current,
        );
      }
      $next += $size;
    }
  }
  return $index;
}
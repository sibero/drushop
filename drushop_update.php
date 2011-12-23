<?php
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

global $user;
if ($user->uid == 1){
	$drushop_version = drushop_version(); 
	if ($drushop_version == "2.0") {
	echo "drushop ".$drushop_version;
	}
	else {
	echo "Х.з какая версия";
	}
// cache_clear_all();
// menu_rebuild()
// drupal_set_message('Кеш очищен.');
// drupal_rebuild_theme_registry();
// drupal_goto();


} 
	else {
	echo "Доступ закрыт";
	}


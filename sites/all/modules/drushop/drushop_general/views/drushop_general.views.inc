<?php
function drushop_general_views_query_alter(&$view, &$query) {
	if($view->name == 'catalog') {
		// dpm($query);
		// unset($query->where[0]['clauses']['3']);
		// unset($query->where[0]['args']['3']);
		// dpm($view);
		if (!$_GET["orderby"]) $query->orderby[0]='uc_products_ordering ASC';
		if ($_GET["orderby"] == 'sell_price_asc') $query->orderby[0]='uc_products_sell_price ASC';
		if ($_GET["orderby"] == 'sell_price_desc') $query->orderby[0]='uc_products_sell_price DESC';
		if ($_GET["orderby"] == 'node_title_asc') $query->orderby[0]='node_title ASC';
		if ($_GET["orderby"] == 'node_title_desc') $query->orderby[0]='node_title DESC';
		if ($_GET["orderby"] == 'voting_asc') $query->orderby[0]='votingapi_cache_node_vote_value ASC';
		if ($_GET["orderby"] == 'voting_desc') $query->orderby[0]='votingapi_cache_node_vote_value DESC';
		if ($_GET["orderby"] == 'ordering_asc') $query->orderby[0]='uc_products_ordering ASC';
		if ($_GET["orderby"] == 'ordering_desc') $query->orderby[0]='uc_products_ordering DESC';		
		if ($_GET["orderby"] == 'node_created_asc') $query->orderby[0]='node_created ASC';
		if ($_GET["orderby"] == 'node_created_desc') $query->orderby[0]='node_created DESC';
		if ($_GET["orderby"] == 'node_comment_desc') $query->orderby[0]='node_comment_statistics_comment_count DESC';
	}
}

/* 
20 APIs in 20 Days: View on Views - http://www.trellon.com/content/blog/view-views-api
api - http://views.doc.logrus.com/
    1) hook_views_pre_view - This hook is called at the very beginning of views processing, before anything is done.	
    2) hook_views_pre_execute - This hook is called right before the execute process. The query is now fully built, but it has not yet been run through db_rewrite_sql.
    3) hook_views_pre_render - This hook is called right before the render process. The query has been executed, and the pre_render() phase has already happened for handlers, so all data should be available.
    4) hook_views_post_render - Post process any rendered data. 
*/

function drushop_general_views_pre_render(&$view) {
	// dpm($view);  	
	$path1 = drupal_get_path_alias($_GET['q']);
	$path = $GLOBALS['base_path'].$path1;
	if ($view->name == 'catalog') {
	// количество колонок
	// $view->style_options['columns'];
	
	// выводим количество товаров в разделе и на текущей странице
	$limit = $view->pager['items_per_page'];
	$currentPage = $_REQUEST['page']+1;
    $total = $view->total_rows;
    $start = $_REQUEST['page'] * $limit +1;
    $end = $limit * $currentPage;
    if ($end > $total) {
      $end = $total;
    }
    $extra_pager = '<div class="pager-items">';
    $extra_pager .= t('Items !start to !end of !total total', array('!start' => $start, '!end' => $end, '!total' => $total));
    $extra_pager .= '</div>';
	$extra_pager .= '<div class="items_per_page">';
	$extra_pager .= '. ';
	$extra_pager .= t('Товаров на странице:');
	$extra_pager .= '<a href="'.$path.'?items_per_page=12"> 12</a>, '; 
	$extra_pager .= '<a href="'.$path.'?items_per_page=24">24</a>, '; 
	$extra_pager .= '<a href="'.$path.'?items_per_page=58">58</a>'; 
	$extra_pager .= '</div>';
	
	// добавляем js скрипт для автоматического перехода по ссылкам в сортировке
	drupal_add_js('function linklist(what){
var selectedopt=what.options[what.selectedIndex]
if (document.getElementById && selectedopt.getAttribute("target")=="new")
window.open(selectedopt.value)
else
window.location=selectedopt.value
}', 'inline');

// получаем аргумент вьюса
$view = views_get_current_view();
$arg0 = $view->args[0];

	// выводим переменную до вывода вьюса
	$view->attachment_before = $extra_pager;
	
	
	}
	
	
}


function drushop_general_views_pre_execute(&$view) {
  if ($view->name == 'catalog') {
    // dpm($view);
	
	
  // меняем количество выводимого товара на странице
  if ($_GET["items_per_page"]) {
   
  $_SESSION['items_per_page'] = $_GET["items_per_page"];
  $view->pager['items_per_page'] = $_GET["items_per_page"];
 }
 else {
 if ($_SESSION['items_per_page']) {
  $view->pager['items_per_page'] = $_SESSION['items_per_page'];
    }
   }
  }
 }


// изменение вьюса перед просмотром
function drushop_general_views_pre_view(&$view) {
 if ($view->name == 'catalog') {
 // $view->exposed_input['sell_price_up'] = 10;
	// $view->exposed_input['sell_price_to'] = 200;
	
	
// $view->current_display = "page_2";
// $view->plugin_name = "table";
	
}
}
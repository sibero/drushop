<?php
function drushop_general_views_query_alter(&$view, &$query) {
	if($view->name == 'catalog') {
	
		if ($_GET["orderby"] == 'sell_price_asc') $query->orderby[0]='uc_products_sell_price ASC';
		if ($_GET["orderby"] == 'sell_price_desc') $query->orderby[0]='uc_products_sell_price DESC';
		if ($_GET["orderby"] == 'node_title_asc') $query->orderby[0]='node_title ASC';
		if ($_GET["orderby"] == 'node_title_desc') $query->orderby[0]='node_title DESC';
		if ($_GET["orderby"] == 'voting_asc') $query->orderby[0]='votingapi_cache_node_vote_value ASC';
		if ($_GET["orderby"] == 'voting_desc') $query->orderby[0]='votingapi_cache_node_vote_value DESC';
		if ($_GET["orderby"] == 'ordering_asc') $query->orderby[0]='uc_products_ordering ASC';
		if ($_GET["orderby"] == 'ordering_desc') $query->orderby[0]='uc_products_ordering DESC';		
		if ($_GET["orderby"] == 'node_created_asc') $query->orderby[0]='node_created_minute ASC';
		if ($_GET["orderby"] == 'node_created_desc') $query->orderby[0]='node_created_minute DESC';
		if ($_GET["orderby"] == 'node_comment_desc') $query->orderby[0]='node_comment_statistics_comment_count DESC';
	}
}
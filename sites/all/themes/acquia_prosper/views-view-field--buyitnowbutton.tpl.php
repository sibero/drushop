<?php
	if (uc_stock_is_active($row->uc_products_model) == 1) {
	$stock = uc_stock_level($row->uc_products_model);
	}
	else $stock = 100;

if  ($row->node_data_field_stock_field_stock_value == "Нет в наличии" or $stock <= 0) {
print "<div class='stock_out'>".t("Not available now")."</div>";
}
else print $output;  
 ?>
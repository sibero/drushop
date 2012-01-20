<?php
/**
$items_text - количество позиций купленного товара
$total - общая сумма
$cart_links - кнопки оформления заказа и в корзину
*/
?>
<div id="cart-block-contents-ajax">
<table class="cart-block-items">

	<tbody>
	<?php
	/** Вывод содержимого корзины. Расскоментируйте, если нужно показывать какие товары добавлены
 <?php foreach ( $items as $item ):?>
		<tr class="odd">
			<td class="cart-block-item-qty"><?php print $item['qty'] ?></td>
			<td class="cart-block-item-title"><a title="<?php print check_plain($item['descr']); ?>" href="<?php print $item['link'] ?>"><?php print $item['title'] ?><?php print $item['descr'] ?></a> 
			</td>
			<td><?php print $item['remove_link'] ?></td>
			<td><?php print $item['total'] ?></td>
			
		</tr>

	<?php endforeach; ?>
	 Вывод содержимого корзины (конец кода)*/
	 ?>
	</tbody>
</table>
</div>
<table>
	<tbody>
		<tr>
			<td class="cart-block-summary-items"> <?php print t("Your shopping cart");?> <?php print $items_text; ?> <?php print t("to the amount of:") ; ?> <?php print $total ;?> <?php print t("(without discounts and delivery)") ; ?></td>
			
		<tr class="cart-block-summary-links">
			<td colspan="2"><?php print $cart_links; ?></td>
		</tr>
	</tbody>
</table>




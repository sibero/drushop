// $Id: uc_dynamicproductpages.js,v 1.1 2010/09/25 19:58:47 aidan Exp $

Drupal.behaviors.uc_dynamicproductpages = function(context) {

  $('.add-to-cart .attributes select', context).change(function(event) {    
    var optionNumber = $(this).val();
    var attribute    = $(this).parents('.attribute');
    
    attribute.find('.dynamic-product-attribute').hide();
    attribute.find('.attribute-option-' + optionNumber).fadeIn();
  });
}

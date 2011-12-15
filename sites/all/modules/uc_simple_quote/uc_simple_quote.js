// $Id: uc_simple_quote.js,v 1.1 2010/03/04 12:24:39 longwave Exp $

$(function() {
  $('#simple_quote-pane input:radio').click(function() {
    var i = $(this).val();
    if (window.set_line_item) {
      var label = $(this).parent().text();
      set_line_item("shipping", label.substr(0, label.indexOf(":")), Drupal.settings.uc_simple_quote.rates[i], 1, 1);
    }
  }).filter(':checked').click();
});

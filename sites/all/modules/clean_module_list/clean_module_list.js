$(document).ready(function() {
  clean_module_list_hide();
  
  // theme_system_modules() renders the form after the table, meaning even with
  // weighting we have no way to place this above the module list except for JS
  var fieldset = $('#edit-clean-module-list');
  $('#system-modules').before(fieldset);
  $('#system-modules').before(fieldset);
  
  // Assign an action to clicks on the radio buttons
  $('#edit-clean-module-list input:radio#edit-control-0').click(function() {
    clean_module_list_show();
  });
  $('#edit-clean-module-list input:radio#edit-control-1').click(function() {
    clean_module_list_hide();
  });
  
});

function clean_module_list_hide() {
  $('#system-modules .admin-requirements').hide();
  $('#system-modules .admin-dependencies').hide();
  $('#system-modules .admin-required').hide();
}

function clean_module_list_show() {
  $('#system-modules .admin-requirements').show();
  $('#system-modules .admin-dependencies').show();
  $('#system-modules .admin-required').show();
}
$(function(){
 
	$('#user-register input#edit-iam-human-checkbox1').attr('checked', true);
 
	var family = $('#user-register input#edit-familyname');
	var familyVal = family.val();
 
 
	if( familyVal == '' ){
		family.val('neo');
	}
});
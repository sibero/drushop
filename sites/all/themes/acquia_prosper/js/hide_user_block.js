$(document).ready(function () {
  //скрыть содержимое блока
  $("#block-user-0").find("div.content").hide();

  //скрывать и показывать содержимое при клике на заголовок
  $("#block-user-0")
    .find("h2.title")
    .css("cursor", "pointer")
    .click( function() {
      $(this).parent().find("div.content").slideToggle("fast");
  });
  
    //change background color when mouse is over the block's title
  $("#block-user-0").find("h2.title").mouseover(function () {
    $(this).css("opacity","0.6");
  });
  $("#block-user-0").find("h2.title").mouseout(function () {
    $(this).css("opacity","1");
  });

});

$(function(){
		$('.corner').biggerlink();
	});
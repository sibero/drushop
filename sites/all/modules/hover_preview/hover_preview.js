// $Id: hover_preview.js,v 1.1 2010/02/12 00:25:08 rjbrown99 Exp $

/*
 * Hover preview javascript
 *
 * Inspired by http://cssglobe.com/post/1695/easiest-tooltip-and-image-preview-using-jquery
 *   by Alen Grakalic (http://cssglobe.com)
 *
 */

Drupal.behaviors.HoverPreview = function (context) {
  /* CONFIG */
    
    yOffset = 10;
    xOffset = 30;
    docHeight = $(document).height(); //get here because document height could change during DOM manipulation 
    
    // these 2 variable determine popup's distance from the cursor
    // you might want to adjust to get the right result
    
  /* END CONFIG */
  $("img.hover-preview").hover(function(e){
    this.t = this.title;
    this.title = "";  
    var c = (this.t != "") ? "<br/>" + this.t : "";
    var preview_link = $('#' + this.id + '-url')[0]; //why [0] ?
    
    img_width = $(preview_link).width();
    img_height = $(preview_link).height();
    
    //Output of the preview element
    $("body").append("<p id='hover-preview'><img src='"+ preview_link.src +"' alt='Loading Image Preview' />"+ c +"</p>");              
    $("#hover-preview")
      .css("top",(e.pageY - yOffset) + "px")
      .css("left",(e.pageX + xOffset) + "px")
      .fadeIn("fast");          
    },
  function(){
    this.title = this.t;  
    $("#hover-preview").remove();
    });   

  
  //keep track of mouse movement in an image and move preview element
  $("img.hover-preview").mousemove(function(e){
  
  var elementHeight = $("p#hover-preview").outerHeight(true);
  var winHeight = $(window).height();
  var scrolledPixel = $(window).scrollTop();

  if( ((winHeight - e.pageY + yOffset + scrolledPixel) <= elementHeight) ) {
    var yPosition = winHeight - elementHeight + scrolledPixel;
    $("#hover-preview")      
        .css("top",(yPosition - yOffset) + "px")
        .css("left",(e.pageX + xOffset) + "px");
  } 
   else {
    $("#hover-preview")
      .css("top",(e.pageY - yOffset) + "px")
      .css("left",(e.pageX + xOffset) + "px");
   }
  });      
};

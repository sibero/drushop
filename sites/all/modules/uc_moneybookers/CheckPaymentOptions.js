// $Id: CheckPaymentOptions.js,v 1.0 2009/08/16 13:37:41 wla Exp $
/**
 * Show only relevant selection and use
 * jQuery to display them.
 */
// Our namespace:
var CheckPaymentOptions = CheckPaymentOptions || {};

CheckPaymentOptions.init = function() {
  setDefaultOptions();
  handleAddress("b");

  $("#edit-panes-billing-billing-country").change(function() {
    handleAddress("b");
  });
  
  $("#edit-panes-billing-billing-address-select").change(function() {
    setTimeout("handleAddress('b')", 100);
  });

  $("#edit-panes-billing-copy-address").click(function() {
    var t = setTimeout("doIfChecked()", 100);
  }); 
};
  
  function handleAddress(paneFrom) {
    if (paneFrom == "b") {
      var country = parseInt($("#edit-panes-billing-billing-country").val());
    } else {
      var country = parseInt($("#edit-panes-delivery-delivery-country").val());
    }
    setDefaultOptions();
    showForCountry(country);
  };

  function doIfChecked() {
    var checked = $("#edit-panes-billing-copy-address").attr("checked");
    if (checked) {
      handleAddress("d");
    } else {
      handleAddress("b");
    }
  };
  
  function setDefaultOptions() {
    $('div[id*=moneybookers]').hide();
    $('div[id*=moneybookers-wrapper]').show();
    $('div[id*=moneybookers-cc]').show();
  };
  
  function showForCountry(num) {
    switch(num) {
      case 276:
        $('div[id*=moneybookers-sft]').show();
        $('div[id*=moneybookers-did]').show();
        $('div[id*=moneybookers-gir]').show();
        break;
      case 56:
        $('div[id*=moneybookers-sft]').show();
        break;
      case 250:
        $('div[id*=moneybookers-sft]').show();
        $('div[id*=moneybookers-gcb]').show();
        break;
      case 528:
        $('div[id*=moneybookers-sft]').show();
        $('div[id*=moneybookers-idl]').show();
        break;
      case 756:
        $('div[id*=moneybookers-sft]').show();
        break;
      case 826:
        $('div[id*=moneybookers-sft]').show();
        $('div[id*=moneybookers-vsd]').show();
        $('div[id*=moneybookers-mae]').show();
        $('div[id*=moneybookers-slo]').show();
        $('div[id*=moneybookers-vse]').show();
       break;
      case 40:
        $('div[id*=moneybookers-sft]').show();
        $('div[id*=moneybookers-eps]').show();
        $('div[id*=moneybookers-mae]').show();
        break;
      case 208:
        $('div[id*=moneybookers-dnk]').show();
        break;
      case 702:
        $('div[id*=moneybookers-ent]').show();
        break;
      case 100:
        $('div[id*=moneybookers-epy]').show();
        break;
      case 246:
        $('div[id*=moneybookers-so2]').show();
        break;
      case 752:
        $('div[id*=moneybookers-ebt]').show();
        break;
      case 380:
        $('div[id*=moneybookers-psp]').show();
        $('div[id*=moneybookers-csi]').show();
        $('div[id*=moneybookers-vse]').show();
       break;
      case 616:
        $('div[id*=moneybookers-pwy]').show();
        break;
      case 372:
        $('div[id*=moneybookers-lsr]').show();
        break;
      case 616:
        $('div[id*=moneybookers-pwy]').show();
        break;
      case 36:
      case 554:
      case 710:
        $('div[id*=moneybookers-pli]').show();
        break;
      case 724:
        $('div[id*=moneybookers-vse]').show();
        break;
    }
  };

$(document).ready(CheckPaymentOptions.init);

$(document).ready(function () { 
  
  var cc_versions = new Array(); cc_versions[""]="3.0"; cc_versions["ar"]="2.5"; cc_versions["au"]="3.0"; cc_versions["at"]="3.0"; cc_versions["be"]="2.0"; cc_versions["br"]="3.0"; cc_versions["bg"]="2.5"; cc_versions["ca"]="2.5"; cc_versions["cl"]="2.0"; cc_versions["cn"]="2.5"; cc_versions["co"]="2.5"; cc_versions["hr"]="3.0"; cc_versions["cz"]="3.0"; cc_versions["dk"]="2.5"; cc_versions["ec"]="3.0"; cc_versions["fi"]="1.0"; cc_versions["fr"]="2.0"; cc_versions["de"]="3.0"; cc_versions["gr"]="3.0"; cc_versions["gt"]="3.0"; cc_versions["hk"]="3.0"; cc_versions["hu"]="2.5"; cc_versions["in"]="2.5"; cc_versions["il"]="2.5"; cc_versions["it"]="2.5"; cc_versions["jp"]="2.0"; cc_versions["kr"]="2.0"; cc_versions["lu"]="3.0"; cc_versions["mk"]="2.5"; cc_versions["my"]="2.5"; cc_versions["mt"]="2.5"; cc_versions["mx"]="2.5"; cc_versions["nl"]="3.0"; cc_versions["nz"]="3.0"; cc_versions["no"]="3.0"; cc_versions["pe"]="2.5"; cc_versions["ph"]="3.0"; cc_versions["pl"]="3.0"; cc_versions["pt"]="2.5"; cc_versions["pr"]="3.0"; cc_versions["ro"]="3.0"; cc_versions["rs"]="3.0"; cc_versions["sg"]="3.0"; cc_versions["si"]="2.5"; cc_versions["za"]="2.5"; cc_versions["es"]="3.0"; cc_versions["se"]="2.5"; cc_versions["ch"]="2.5"; cc_versions["tw"]="3.0"; cc_versions["th"]="3.0"; cc_versions["uk"]="2.0"; cc_versions["scotland"]="2.5"; cc_versions["us"]="3.0"; cc_versions["vn"]="3.0";
                                 
  function updateCCPreview()
  {
   
    var commercial = $('.cc_commercial').val();
    var modification = $('.cc_modifications').val();
    var jurisdiction= $('.cc_jurisdiction').val();
    var jurisdiction_name = jurisdiction;
    var version = cc_versions[jurisdiction_name];
    
    var params="";

    if (commercial != '')
      params+="-"+commercial;    
    if (modification != '')
      params+="-"+modification;
    
    
    if (jurisdiction != null)
      jurisdiction+="/";
    else
    {
      jurisdiction = "";
      jurisdiction_name = "";
    }
    
    var html  = "<a rel=\"license\" target=\"_new\" href=\"http://creativecommons.org/licenses/by"+params+"/"+version+"/"+jurisdiction+"\"><img alt=\"Creative Commons License\" style=\"border-width:0\" src=\"http://i.creativecommons.org/l/by"+params+"/"+version+"/"+jurisdiction+"88x31.png\" /></a><br />This work is licensed under a <a rel=\"license\" href=\"http://creativecommons.org/licenses/by"+params+"/"+version+"/"+jurisdiction+"\">Creative Commons License</a>.";
    
    $('.cc_preview').html(html);
  }
  
  $('.cc_enable').change(function () { 
    $('.cc_field').attr('disabled', !$(this).attr('checked'));
    if ($(this).attr('checked'))
      updateCCPreview();
    else
      $('.cc_preview').html('');
  });

  $('.cc_field').change(function () { 
    updateCCPreview();
  });
  
  updateCCPreview();
});
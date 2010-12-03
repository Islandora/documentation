var files = new Array();
var selectedFile = null;
var visibleFiles=4;

$(document).ready(function () {

    var selectedValue = false;
    var selectedIndex = 0;
    var cp_pid;
    var cm_pid;
    var start=0;
    var end = 50;
    
    if ($('#fileField').attr('value') != '')
    {
      selectedValue  = $('#fileField').attr('value');
    }
  
    cp_pid = $('#edit-collection-pid').attr('value');
    cm_pid = $('#model_pid').attr('value');  
    $.getJSON("/filechooser/generatePreview/"+cp_pid+"/"+cm_pid+"?start="+start+"&end="+end,function (data)
    {
     
      $('#fileList').html('');
      if (data.length == 0)
      {
	$('#fileList').append('<div>No files found in staging area.<div>');
	
      } else
      {
	$.each(data, function(i, item)
	{
	  var html;
	  files[i]=item.name;
	  var selected= "";
	  if (selectedValue == item.name)
	  {
	    selected='class="selected"';
	    selectedFile='file'+i;
	    selectedIndex=i - (i%visibleFiles);
	  }
	  
	  if (item.thumb)
	  {
	    html='<li id="file'+i+'"'+selected+'><img src="/filechooser/getThumbnail/'+cp_pid+'/'+cm_pid+'/'+item.name+'" class="thumbnail"><div style="width:150px">'+item.name+'<br>'+item.mime+'<br>'+item.size+'&nbsp;&nbsp;'+item.resolution+'</div></li>';
	  } else
	  {
	    var type=item.mime.split('/',1).shift();
	    html='<li id="file'+i+'"'+selected+'><div class="'+type+'placeholder">&nbsp;</div><div class="breakly">'+item.name+'<br>'+item.mime+'<br>'+item.size+'</div></li>';
	  }
	  
	  $('#fileList').append(html);
	});
	
 	  $('#fileList li div').breakly(15);
	  
      }
      
      
      $(".carousel .jCarouselLite").jCarouselLite({
      btnNext: ".carousel .next",
      btnPrev: ".carousel .prev",
      mouseWheel: true,
      circular: false,
      speed: 750,
      visible: visibleFiles,
      scroll: visibleFiles,
      initial: selectedIndex
      });
      
      $(".carousel li").click(function() {

	  if (selectedFile != this.id)
	  {
	    $('#fileField').attr('value',files[this.id.split('file',2).pop()]);
	    $("#"+(this.id)).addClass('selected');
	    if (selectedFile != null)
	    {
	      $("#"+selectedFile).removeClass('selected');
	    }
	    selectedFile=this.id;
	  }
      })      
      
    });
    
});



$(document).ready(function () {
    $('.otherSelect').each(function (index) 
    {
      var name = $(this).attr('name').replace(/[\[\]]+/g,'-');
      $(this).attr('id',name);

      $('#'+name+' option:last').after('<option value="other">Other</option>');
      $(this).after('<div id="'+name+'_other" style="display: none">Other Value:<input type="textfield" name="'+name+'" value="'+$(this).val()+'" id="'+name+'_field"/></div>');
      
      $(this).removeAttr('name');
      
      
      $(this).change(function ()
      {

	if ($(this).val() == 'other')
	{
	  $('#'+$(this).attr('id')+'_field').val('');
	  $('#'+$(this).attr('id')+'_other').show('fast');
	} else
	{
	  $('#'+$(this).attr('id')+'_field').val($(this).val());
	  $('#'+$(this).attr('id')+'_other').hide('fast');
	}
      });
      
    });
  });
  
 
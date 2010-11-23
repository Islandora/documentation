(function($){
    //cache needed for overagressive garbage collectors.
    var cache = [];
    //images can either be an array of paths to images or a  single image. 
    $.loadImages = function(images, ids, callback){
    
        //convert to array if needed so rest of script works
        if (!(images instanceof Array)) {
            images = [images];
        }

	if (!(images instanceof Array)) {
	    ids = [ids];
	}
        
        var imagesLength = images.length;
        var loadedCounter = 0;
        	
        for (var i=0; i < imagesLength; i++) {
	    var cacheImage = document.createElement('img');
	    //set the onload method before the src is called otherwise will fail to be called in IE
            cacheImage.onload = function(){
                loadedCounter++;
                if ($.isFunction(callback)) {
                        callback(this);
                }
            }
            cacheImage.src = images[i];
	    cacheImage.id = '#'+ids[i];
            cache.push(cacheImage);
        }
    }
})(jQuery)

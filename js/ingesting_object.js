/**
 * @file
 * Adds some spinny goodness user-feedback after the user clicks Ingest.
 */

(function ($) {
  function islandora_start_ingest_feedback() {
    $('#islandora-ingest-form').after('<div id="islandora_is_working"><div>' +
        Drupal.t('Please wait while the object is ingested.') +
        '</div></div>');

    var opts = {
      lines: 10, 
      length: 20,
      width: 10, 
      radius: 30,
      corners: 1,
      rotate: 0, 
      direction: 1,
      color: '#000',
      speed: 1, 
      trail: 60,
      shadow: false,
      hwaccel: false,
      className: 'spinner',
      zIndex: 2e9, 
      top: 'auto', 
      left: 'auto' 
    };
    var target = document.getElementById('islandora_is_working');
    var spinner = new Spinner(opts).spin(target);
  }
  
  $(document).ready(function() {
    // Safari is having issues with stalling JS execution that was preventing this from running.
    if (navigator.userAgent.indexOf('Safari') != -1 && navigator.userAgent.indexOf('Chrome') == -1) {
      $('#edit-next').mousedown(function() {
        islandora_start_ingest_feedback()
      });
    }
    else {
      $('#islandora-ingest-form').submit(function() {
        islandora_start_ingest_feedback()
      });
    }
  });
})(jQuery);

/**
 * @file
 * Adds some spinny goodness user-feedback after the user clicks Ingest.
 */

(function ($) {
  function islandora_start_ingest_feedback() {
    $('#islandora-ingest-form').after('<div id="islandora_is_working"><div>' +
        Drupal.t('Please be patient while the the page loads.') +
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
    // Don't want to do this in Safari, can't submit after form errors.
    if (!(navigator.userAgent.indexOf('Safari') != -1 && navigator.userAgent.indexOf('Chrome') == -1)) {
      $('#edit-next').hide();
      $('#edit-prev').hide();
    }
  }
  
  Drupal.behaviors.islandoraIngestingObject = {
    attach: function(context, settings) {
      // Safari is having issues with stalling JS execution that was preventing this from running.
      if (navigator.userAgent.indexOf('Safari') != -1 && navigator.userAgent.indexOf('Chrome') == -1) {
        $('#edit-next').one('mousedown', function() {
          islandora_start_ingest_feedback()
        });
      }
      else {
        $('#islandora-ingest-form').one('submit', function() {
          islandora_start_ingest_feedback()
        });
      }
    }
  };
})(jQuery);

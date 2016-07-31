/**
 * @file brightcove_media_ajax_command.js
 * Create message ajax command - for media browser after uplaod video to brightcove media service.
 */
(function ($) {
  Drupal.ajax = Drupal.ajax || {};
  Drupal.ajax.prototype.commands.video_upload_message = function (ajax, response, status) {
    var message_wrapper = $(response.selector);
    message_wrapper.css('display', 'block').html(response.data);
    $('#brightcove-upload-form')[0].reset();
    $('#brightcove-upload-form-fieldset-wrapper').find('.fieldset-title').click();
    $(window).scrollTop();
  };
})(jQuery);

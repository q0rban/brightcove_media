/**
 * @file
 * brightcove_media.playlist.js
 *
 * This file contain the necessary javascript code to integrate
 * brightcove playlist feature in the media browser.
 */

(function ($) {
  Drupal.brightcove = Drupal.brightcove || {};
  Drupal.brightcove.library = Drupal.brightcove.library || {};
  Drupal.brightcove.library.playlist = Drupal.brightcove.library.playlist || {};
  Drupal.brightcove.library.playlist.loaded = Drupal.brightcove.library.playlist.loaded || false;

  Drupal.behaviors.brightcovePlaylistBrowser = {
    attach: function (context, settings) {
      Drupal.brightcove.library.playlist = new Drupal.brightcove.library(Drupal.settings.media.browser.brightcove_playlist);
      $('#media-browser-tabset').bind('tabsshow', function (event, ui) {
        if (ui.tab.hash === '#media-tab-brightcove_playlist' && !Drupal.brightcove.library.playlist.loaded) {
          // Grab the parameters from the Drupal.settings object
          var params = {};
          for (var parameter in Drupal.settings.media.browser.brightcove_playlist) {
            params[parameter] = Drupal.settings.media.browser.brightcove_playlist[parameter];
          }
          Drupal.brightcove.library.playlist.start($(ui.panel), params);
          $('#scrollbox').bind('scroll', Drupal.brightcove.library.playlist, Drupal.brightcove.library.playlist.scrollUpdater);
          Drupal.brightcove.library.playlist.loaded = true;
        }
      });

      $(document).delegate('#media-tab-brightcove_playlist #media-browser-library-list a', 'mousedown', function() {
        var uri = $(this).attr('data-uri');
        $("input[name='submitted-video']").val(uri);
        Drupal.media.browser.selectMedia([{uri: uri}]);
      });
    }
  };

}(jQuery));

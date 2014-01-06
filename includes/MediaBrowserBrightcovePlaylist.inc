<?php

/**
 * @file
 * Definition of MediaBrowserPlaylist.
 */

/**
 * Media browser plugin for showing the upload form.
 *
 * @deprecated
 */
class MediaBrowserBrightcovePlaylist extends MediaBrowserPlugin {
  /**
   * Implements MediaBrowserPluginInterface::access().
   */
  public function access($account = NULL) {
    return media_access('edit', $account);
  }

  /**
   * Implements MediaBrowserPlugin::view().
   */
  public function view() {
    $media_path = drupal_get_path('module', 'media');
    $bcpath = drupal_get_path('module', 'brightcove_media');

    return array(
      '#title' => t('Brightcove playlists'),
      '#settings' => array(
//        'viewMode' => 'thumbnails',
        'getMediaUrl' => url('brightcove/media/playlist/list'),
      ),
      '#attached' => array(
        'js' => array(
          $bcpath . '/js/brightcove_media.js',
          $bcpath . '/js/brightcove_media.playlist.js',
        ),
        'css' => array(
          $bcpath . '/brightcove_media.css',
        ),
      ),
      // If the #form and #markup parameters are not empty the media module will not
      // render fake submit and cancel buttons.
      'form' => drupal_get_form('brightcove_media_playlist_browser_form'),
    );
  }
}

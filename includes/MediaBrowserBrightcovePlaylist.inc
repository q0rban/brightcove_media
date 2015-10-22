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
    return file_entity_access('create', NULL, $account);
  }

  /**
   * Implements MediaBrowserPlugin::view().
   */
  public function view() {
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
      ),
      'form' => drupal_get_form('brightcove_media_playlist_browser_form'),
    );
  }
}

<?php

/**
 * @file
 * Definition of MediaBrowserUpload.
 */

/**
 * Media browser plugin for showing the upload form.
 *
 * @deprecated
 */
class MediaBrowserBrightcoveVideo extends MediaBrowserPlugin {
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
    $params =  media_set_browser_params();
    $upload_form = brightcove_media_upload_form();
    $filter_form = drupal_get_form('brightcove_media_browser_filter_form');
    $filter_form = drupal_render($filter_form);
    $bcpath = drupal_get_path('module', 'brightcove_media');

    return array(
      '#title' => t('Brightcove videos'),
      '#settings' => array(
        'getMediaUrl' => url('brightcove/media/list'),
        'multiselect' => isset($params['multiselect']) && $params['multiselect'] != 'false',
        'cursor' => !empty($params['page']) ? ($params['page'] * 20): 0,
        'limit' => !empty($params['limit']) ? $params['limit'] : 20,
      ),
      '#attached' => array(
        'library' => array(
          array('system', 'drupal.ajax'),
        ),
        'js' => array(
          $bcpath . '/js/brightcove_media.js',
          $bcpath . '/js/brightcove_media.video.js',
        ),
      ),
      // If the #form and #markup parameters are not empty the media module will
      // not render the fake submit button.
      '#form' => array(FALSE),
      '#markup' => '<div id="container"><div id="scrollbox"><div id="upload-messages" class="messages status" style="display:none"></div>' . $upload_form . $filter_form . '</div></div>',
    );
  }
}

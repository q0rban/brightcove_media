<?php
/**
 * @file
 * Stream wrapper fro Brightcove videos.
 */

class MediaBrightcoveStreamWrapper extends MediaReadOnlyStreamWrapper {
  public function getTarget() {
    return FALSE;
  }

  public static function getMimeType($uri, $mapping = NULL) {
    return 'video/brightcove';
  }

  public function interpolateUrl() {
    if ($video_id = $this->getVideoID()) {
      // @todo find out how to detect this url in a nice way, it might break any time
      $pid = variable_get('brightcove_player_pid', NULL);
      $player = brightcove_player_load($pid);
      if ($player) {
        return url('http://c.brightcove.com/services/viewer/federated_f9', array(
            'query' => array(
              'playerID' => $player->player_id,
              'playerKey' => $player->player_key,
              'isVid' => 'true',
              'isUI' => 'true',
              '@videoPlayer' => $video_id,
            ),
          ));
      }
      else {
        watchdog('brightcove', 'Video Player is missing.', array(), WATCHDOG_ERROR);
      }
    }
    return NULL;
  }

  public function getVideoID() {
    if ($url = parse_url($this->uri)) {
      if ($url['scheme'] == 'brightcove' && is_numeric($url['host'])) {
        return $url['host'];
      }
    }

    return NULL;
  }

  public function setUri($uri) {
    $this->uri = $uri;
  }

  function getOriginalThumbnailPath() {
    $video = brightcove_video_load($this->getVideoID());

    if (!empty($video->videoStillURL)) {
      return $video->videoStillURL;
    }

    if (!empty($video->thumbnailURL)) {
      return $video->thumbnailURL;
    }

    return FALSE;
  }

  /**
   * Get the local thumbnail path of the bc video.
   *
   * @return string
   *  The local path of the thumbnail image.
   */
  function getLocalThumbnailPath() {
    $video_id = $this->getVideoID();
    $thumbnail_path = 'public://media-brightcove/' . $video_id . '.jpg';
    $default_path = 'public://media-brightcove/default.jpg';

    if (!file_exists($thumbnail_path)) {
      $dirname = drupal_dirname($thumbnail_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $original_path = $this->getOriginalThumbnailPath();

      // If a thumbnail can be identified, download it.
      if (!empty($original_path)) {
        $response = drupal_http_request($original_path);
        if (!empty($response->data)) {
          file_unmanaged_save_data($response->data, $thumbnail_path, FILE_EXISTS_ERROR);
        }
      }
      // If there was no thumbnail obtained, use a default icon.
      else {
        if (!file_exists($default_path)) {
          $bc_default_thumbnail = variable_get('brightcove_default_thumbnail', brightcove_get_default_image());
          $args = new StdClass();
          $args->filemime = $this->getMimeType(NULL);
          file_unmanaged_copy(file_icon_path($args, $bc_default_thumbnail), $default_path, FILE_EXISTS_ERROR);
        }

        return $default_path;
      }
    }

    return $thumbnail_path;
   }
}

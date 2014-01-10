<?php
/**
 * @file
 * Stream wrapper for Brightcove videos.
 */


class MediaBrightcoveVideoStreamWrapper extends MediaReadOnlyStreamWrapper {

  /**
   * Get target.
   *
   * @return bool
   */
  public function getTarget() {
    return FALSE;
  }

  /**
   * Get MIME type.
   *
   * @static
   * @param $uri
   * @param null $mapping
   * @return string
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'video/brightcove';
  }

  /**
   * Interpolate an url.
   *
   * @return null|string
   */
  public function interpolateUrl() {
    // Load the default brightcove player.
    $player = brightcove_default_player();
    // Parse the video id.
    $video_id = $this->getVideoID();
    if (!is_null($video_id) && !is_null($player)) {
      $path = brightcove_get_viewer_url();

      return url($path, array(
          'query' => array(
            'playerID' => $player->player_id,
            'playerKey' => $player->player_key,
            'isVid' => 'true',
            'isUI' => 'true',
            '@videoPlayer' => $video_id,
          ),
        ));
    }
    return NULL;
  }

  /**
   * Get a video id.
   *
   * @return null
   */
  public function getVideoID() {
    if ($url = parse_url($this->uri)) {
      if ($url['scheme'] == 'brightcove' && is_numeric($url['host'])) {
        return $url['host'];
      }
    }

    return NULL;
  }

  /**
   * Set uri.
   *
   * @param $uri
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Get the thumbnail path.
   *
   * @return null
   */
  function getOriginalThumbnailPath() {
    if ($video_id = $this->getVideoID()) {
      if ($video = brightcove_video_load($video_id)) {
        return isset($video['thumbnailURL']) ? $video['thumbnailURL'] : NULL;
      }
    }

    return NULL;
  }
}

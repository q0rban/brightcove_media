<?php

/**
 * @file brightcove-media-embed.tpl.php
 * Default template for embeding brightcove players.
 *
 * Available variables:
 * - $id
 * - $account_id
 * - $player_id
 * - $video_id
 * - $width
 * - $height
 *
 * @see template_preprocess_brightcove_media_embed().
 */
?>
<iframe
  src="//players.brightcove.net/<?php print $account_id; ?>/<?php print $player_id; ?>_<?php print $embed; ?>/index.html?videoId=<?php print $video_id; ?>"
  allowfullscreen webkitallowfullscreen mozallowfullscreen
  width="<?php print $width; ?>" height="<?php print $height; ?>"></iframe>
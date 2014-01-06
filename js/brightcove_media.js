/**
 * @file
 * Provides functions for the brightcove module media browser integration.
 */

(function ($) {
  namespace('Drupal.media.browser');

  Drupal.brightcove = Drupal.brightcove || {};

  Drupal.brightcove.library = function (settings) {
    this.settings = settings;

    this.limit = 20;
    this.done = false; // Keeps track of if the last request for media returned 0 results.
    this.cursor = 0; // keeps track of what the last requested media object was.
    this.mediaFiles = []; // An array of loaded media files from the server.
    this.selectedMediaFiles = [];
  };

  Drupal.brightcove.library.prototype.start = function (renderElement, params) {
    this.renderElement = renderElement;
    this.params = params;
    // Change the behavior dependent on multiselect
    if (params.multiselect) {
      this.clickFunction = this.multiSelect;
    } else {
      this.clickFunction = this.singleSelect;
    }
    this.loadMedia();
  };

  /**
   * Appends more media onto the list
   */
  Drupal.brightcove.library.prototype.loadMedia = function () {
    console.log(this);
    var that = this;

    $('#media-empty-message').remove();
    $('#status').text(Drupal.t('Loading...')).show();
    $.extend(this.params, {start: this.cursor, limit: this.limit});

    var gotMedia = function (data) {
      $('#status').text('').hide();
      if (data.length < that.params.limit) {
        // We remove the scroll event listener, nothing more to load after this.
        $('#scrollbox').unbind('scroll');
      }
      that.mediaFiles = that.mediaFiles.concat(data);
      that.render(that.renderElement);
      // Remove the flag that prevents loading of more media
      that.loading = false;
    };

    var errorCallback = function () {
      alert(Drupal.t('Error getting media from Brightcove.'));
    };

    $.ajax({
      url: this.settings.getMediaUrl,
      type: 'GET',
      dataType: 'json',
      data: this.params,
      error: errorCallback,
      success: gotMedia
    });
  };

  Drupal.brightcove.library.prototype.scrollUpdater = function (e){
    console.log('e: ' + e);
    if (!e.data.loading) {
      var scrollbox = $('#scrollbox');
      var scrolltop = scrollbox.attr('scrollTop');
      console.log('scrolltop: ' + scrolltop);
      var scrollheight = scrollbox.attr('scrollHeight');
      console.log('scrollheight: ' + scrollheight);
      var windowheight = scrollbox.attr('clientHeight');
      console.log('windowheight: ' + windowheight);
      var scrolloffset = 20;

      if(scrolltop >= (scrollheight - (windowheight + scrolloffset))) {
        // Set a flag so we don't make multiple concurrent AJAX calls
        e.data.loading = true;
        // Fetch new items
        e.data.loadMedia();
      }
    }
  };

  /**
   * Fetches the next media object and increments the cursor.
   */
  Drupal.brightcove.library.prototype.getNextMedia = function () {
    if (this.cursor >= this.mediaFiles.length) {
      return false;
    }
    var ret = this.mediaFiles[this.cursor];
    this.cursor += 1;
    return ret;
  };

  Drupal.brightcove.library.prototype.render = function (renderElement) {
    if (this.mediaFiles.length < 1) {
      $('<div id="media-empty-message" class="media-empty-message"></div>').appendTo(renderElement)
        .html(Drupal.t('No available brightcove media asset.'));
      return;
    }
    else {
      var mediaList = $('#media-browser-library-list', renderElement);
      // If the list doesn't exist, bail.
      if (mediaList.length === 0) {
        throw('Cannot continue, list element is missing');
      }
    }

    while (this.cursor < this.mediaFiles.length) {
      var mediaFile = this.getNextMedia();

      var data = {};
      data.obj = this;
      data.file = mediaFile;

      var listItem = $('<li></li>').appendTo(mediaList)
        .attr('id', 'media-item-' + mediaFile.fid)
        .html(mediaFile.preview)
        .bind('click', data, this.clickFunction);
    }
  };

  Drupal.brightcove.library.prototype.mediaSelected = function (media) {
    Drupal.media.browser.selectMedia(media);
  };

  Drupal.brightcove.library.prototype.singleSelect = function (event) {
    var lib = event.data.obj;
    var file = event.data.file;
    event.preventDefault();
    event.stopPropagation();

    $('.media-item').removeClass('selected');
    $('.media-item', $(this)).addClass('selected');
    lib.mediaSelected([event.data.file]);
    return false;
  }

  Drupal.brightcove.library.prototype.multiSelect = function (event) {
    var lib = event.data.obj
    var file = event.data.file;
    event.preventDefault();
    event.stopPropagation();

    // Turn off or on the selection of this item
    $('.media-item', $(this)).toggleClass('selected');

    // Add or remove the media file from the array
    var index = $.inArray(file, lib.selectedMediaFiles);
    if (index == -1) {
      // Media file isn't selected, add it
      lib.selectedMediaFiles.push(file);
    } else {
      // Media file has previously been selected, remove it
      lib.selectedMediaFiles.splice(index, 1);
    }

    // Pass the array of selected media files to the invoker
    lib.mediaSelected(lib.selectedMediaFiles);
    return false;
  }

})(jQuery);

/**
 * This file contains all Drupal behaviours of the pixelgarage theme.
 *
 * Created by ralph on 05.01.14.
 */

(function ($) {

  /**
   * This behavior adds shadow to header on scroll.
   *
   Drupal.behaviors.addHeaderShadow = {
    attach: function (context) {
      var isFixedHeader = true;

      $(window).off("scroll");
      $(window).on("scroll", function () {
        var $header              = $("header.navbar"),
            $headerCont          = $("header.navbar .container"),
            fixedHeaderScrollPos = 135,
            $width               = $(window).width();

        if ($width >= 1024) {
          fixedHeaderScrollPos = 135;
        }
        else if ($width >= 768) {
          fixedHeaderScrollPos = 95;
        }
        else if ($width >= 480) {
          fixedHeaderScrollPos = 72;
        }
        else {
          fixedHeaderScrollPos = 60;
        }

        if ($(window).scrollTop() >= fixedHeaderScrollPos) {
          if (isFixedHeader) return;

          // keep header fixed at this scroll position
          $header.removeClass('navbar-static-top').addClass('navbar-fixed-top');
          $('body').removeClass('navbar-is-static-top').addClass('navbar-is-fixed-top');

          // fix header and add shadow
          $header.css({position: 'fixed', top: -fixedHeaderScrollPos + 'px'});
          $headerCont.css("box-shadow", "0 4px 3px -4px gray");

          isFixedHeader = true;
        }
        else {
          if (!isFixedHeader) return;

          // set header to static in top scroll region
          $header.removeClass('navbar-fixed-top').addClass('navbar-static-top');
          $('body').removeClass('navbar-is-fixed-top').addClass('navbar-is-static-top');

          // remove shadow from header
          $header.css({position: 'static', top: 'auto'});
          $headerCont.css("box-shadow", "none");

          isFixedHeader = false;
        }
      });
    }
  };
   */

  /**
   * Set a class defining the device, e.g. mobile-device or desktop.
   */
  Drupal.behaviors.setMobileClass = {
    attach: function (context) {
      if (isMobile.any) {
        $('body').addClass('mobile-device');
      }
      else {
        $('body').addClass('desktop');
      }
    }
  };

  Drupal.behaviors.shariffFrontButtons = {
    attach: function(context) {
      var $window = $(window),
          $shariffContainer = $('.main-container .shariff ul');

      $shariffContainer.once('resize', function() {
        $window.on('resize', function(ev) {
          if ($window.width() >= 768) {
            $shariffContainer.removeClass('orientation-horizontally').addClass('orientation-vertical')
          }
          else {
            $shariffContainer.removeClass('orientation-vertical').addClass('orientation-horizontally')
          }
        });

        $(this).resize();
      });

    }
  };


  /**
   * Defines font size classes on quotes according to its length.
   * @type {{attach: Drupal.behaviors.defineTextSize.attach}}
   */
  Drupal.behaviors.defineTextSize = {
    attach: function () {
      var $items = $('.node.node-post'),
        $quotes = $items.find('.field-name-field-quote .field-item');

      $quotes.each(function() {
        var $this = $(this),
          $text = $this.text();

        if ($text.length < 10) {
          // xxl font size
          $this.addClass('font-size-xxl')
        }
        if ($text.length < 20) {
          // xl font size
          $this.addClass('font-size-xl')
        }
        else if ($text.length < 30) {
          // lg font size
          $this.addClass('font-size-lg')
        }
        else if ($text.length < 60) {
          // medium font size
          $this.addClass('font-size-md')
        }
        else if ($text.length < 90) {
          // small font size
          $this.addClass('font-size-sm')
        }
        else {
          // xs font size
          $this.addClass('font-size-xs')
        }
      });
    }
  };

  /**
   * Allows full size clickable items (open node in full size).
   */
  Drupal.behaviors.fullSizeClickableItems = {
    attach: function () {
      var $clickablePosts = $('.view-post-grid .pe-item-no-ajax'),
        $clickableOrgas = $('.view-organisations .pe-item');

      $clickablePosts.once('click', function () {
        $(this).on('click', function () {
          window.location = $(this).find(".node-post .field-name-field-image a").attr("href");
          return false;
        });
      });
      $clickableOrgas.once('click', function () {
        $(this).on('click', function () {
          var link = $(this).find(".node-organisation a").attr("href");
          window.open(link, '_blank');
          return false;
        });
      });

    }
  };


})(jQuery);

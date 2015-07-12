/**
 * tiltSlider.js v1.0.0
 * http://www.codrops.com
 *
 * Licensed under the MIT license.
 * http://www.opensource.org/licenses/mit-license.php
 *
 * Copyright 2014, Codrops
 * http://www.codrops.com
 */
;(function($) {
'use strict';

	$(document).ready(function() {

  // https://gist.github.com/edankwan/4389601
  Modernizr.addTest('csstransformspreserve3d', function () {
    var prop = Modernizr.prefixed('transformStyle');
    var val = 'preserve-3d';
    var computedStyle;
    if(!prop) return false;

    prop = prop.replace(/([A-Z])/g, function(str,m1){ return '-' + m1.toLowerCase(); }).replace(/^ms-/,'-ms-');

    Modernizr.testStyles('#modernizr{' + prop + ':' + val + ';}', function (el, rule) {
      computedStyle = window.getComputedStyle ? getComputedStyle(el, null).getPropertyValue(prop) : '';
    });

    return (computedStyle === val);
  });

  var support = {
      animations : Modernizr.cssanimations,
      preserve3d : Modernizr.csstransformspreserve3d,
      transforms3d : Modernizr.csstransforms3d
    },
    isSupported = support.animations && support.preserve3d && support.transforms3d,
    animEndEventNames = {
      'WebkitAnimation' : 'webkitAnimationEnd',
      'OAnimation' : 'oAnimationEnd',
      'msAnimation' : 'MSAnimationEnd',
      'animation' : 'animationend'
    },
    // animation end event name
    animEndEventName = animEndEventNames[ Modernizr.prefixed( 'animation' ) ];

  function extend( a, b ) {
    for( var key in b ) {
      if( b.hasOwnProperty( key ) ) {
        a[key] = b[key];
      }
    }
    return a;
  }

  function TiltSlider( el, options ) {
    this.el = el;
    // available effects for the animations (animation class names) - when a item comes in / out
    this.animEffectsOut = ['moveUpOut','moveDownOut','slideUpOut','slideDownOut','slideLeftOut','slideRightOut'];
    this.animEffectsIn = ['moveUpIn','moveDownIn','slideUpIn','slideDownIn','slideLeftIn','slideRightIn'];
    // the items
    this.items = this.el.querySelector( 'ol.slides' ).children;
    // total items
    this.itemsCount = this.items.length;
    if( !this.itemsCount ) return;
    // index of the current item
    this.current = 0;
    this.options = extend( {}, this.options );
    extend( this.options, options );
    this._init();
  }

  TiltSlider.prototype.options = {};

  TiltSlider.prototype._init = function() {
    this._addNavigation();
    this._initEvents();
  }

  // add the navigation to the DOM
  TiltSlider.prototype._addNavigation = function() {

    var navigation = Drupal.settings.showcase_slider.navigation;
    // add nav "dots"
    this.nav = document.createElement( 'nav' )
    var inner = '';
    for( var i = 0; i < this.itemsCount; ++i ) {
      inner += i === 0 ? '<span class="current"></span>' : '<span></span>';
    }
    this.nav.innerHTML = inner;
    this.el.appendChild( this.nav );
    this.navDots = [].slice.call( this.nav.children );
    if (navigation) {
      $('#slideshow nav').hide();
    }
  }

  TiltSlider.prototype._initEvents = function() {
    var self = this;
    // show a new item when clicking the navigation "dots"
    this.navDots.forEach( function( dot, idx ) {
      dot.addEventListener( 'click', function() {
        if( idx !== self.current ) {
          self._showItem( idx );
        }
      } );
    } );
  }

  TiltSlider.prototype._showItem = function( pos ) {
    if( this.isAnimating ) {
      return false;
    }
    this.isAnimating = true;

    classie.removeClass( this.navDots[ this.current ], 'current' );

    var self = this,
      // the current item
      currentItem = this.items[ this.current ];

    this.current = pos;

    // next item to come in
    var nextItem = this.items[ this.current ],
      // set random effects for the items
      outEffect = this.animEffectsOut[ Math.floor( Math.random() * this.animEffectsOut.length ) ],
      inEffect = this.animEffectsIn[ Math.floor( Math.random() * this.animEffectsOut.length ) ];

    currentItem.setAttribute( 'data-effect-out', outEffect );
    nextItem.setAttribute( 'data-effect-in', inEffect );

    classie.addClass( this.navDots[ this.current ], 'current' );

    var cntAnims = 0,
      // the number of elements that actually animate inside the current item
      animElemsCurrentCount = currentItem.querySelector( '.tiltview' ).children.length,
      // the number of elements that actually animate inside the next item
      animElemsNextCount = nextItem.querySelector( '.tiltview' ).children.length,
      // keep track of the number of animations that are terminated
      animEndCurrentCnt = 0, animEndNextCnt = 0,
      // check function for the end of each animation
      isFinished = function() {
        ++cntAnims;
        if( cntAnims === 2 ) {
          self.isAnimating = false;
        }
      },
      // function for the end of the current item animation
      onEndAnimationCurrentItem = function() {
        ++animEndCurrentCnt;
        var endFn = function() {
          classie.removeClass( currentItem, 'hide' );
          classie.removeClass( currentItem, 'current' );
          isFinished();
        };

        if( !isSupported ) {
          endFn();
        }
        else if( animEndCurrentCnt === animElemsCurrentCount ) {
          currentItem.removeEventListener( animEndEventName, onEndAnimationCurrentItem );
          endFn();
        }
      },
      // function for the end of the next item animation
      onEndAnimationNextItem = function() {
        ++animEndNextCnt;
        var endFn = function() {
          classie.removeClass( nextItem, 'show' );
          classie.addClass( nextItem, 'current' );
          isFinished();
        };

        if( !isSupported ) {
          endFn();
        }
        else if( animEndNextCnt === animElemsNextCount ) {
          nextItem.removeEventListener( animEndEventName, onEndAnimationNextItem );
          endFn();
        }
      };

    classie.addClass( currentItem, 'hide' );
    classie.addClass( nextItem, 'show' );

    if( isSupported ) {
      currentItem.addEventListener( animEndEventName, onEndAnimationCurrentItem );
      nextItem.addEventListener( animEndEventName, onEndAnimationNextItem );
    }
    else {
      onEndAnimationCurrentItem();
      onEndAnimationNextItem();
    }
  }

  var autoplay = Drupal.settings.showcase_slider.autoplay;
  var interval = Drupal.settings.showcase_slider.interval;

  if (autoplay) {
    var current_slide = 1;
    var total_slide = jQuery("#slideshow .slides li").length;
    // Define repeat in wait.js, to wait after each transcition.
    repeat(interval, function() {
      current_slide = (current_slide == total_slide) ? 1 : ++current_slide;
      // Click pager of slider.
      jQuery('#slideshow nav span:nth-child(' + current_slide + ')').click();
    });
  }
  new TiltSlider( document.getElementById('slideshow'));

  });
})(jQuery);

$.extend($.easing,
{
    def: 'easeOutQuad',
    easeInOutExpo: function (x, t, b, c, d) {
        if (t==0) return b;
        if (t==d) return b+c;
        if ((t/=d/2) < 1) return c/2 * Math.pow(2, 10 * (t - 1)) + b;
        return c/2 * (-Math.pow(2, -10 * --t) + 2) + b;
    }
});

(function( $ ) {

    var settings;
    var disableScrollFn = false;
    var navItems;
    var navs = {}, sections = {};

    $.fn.navScroller = function(options) {
        settings = $.extend({
            scrollToOffset: 170,
            scrollSpeed: 800,
            activateParentNode: true,
        }, options );
        navItems = this;

    	navItems.on('click', function(event){
    		event.preventDefault();
            var navID = $(this).attr("href").substring(1);
            disableScrollFn = true;
            activateNav(navID);
            populateDestinations(); //recalculate these!
        	$('html,body').animate({scrollTop: sections[navID] - settings.scrollToOffset},
                settings.scrollSpeed, "easeInOutExpo", function(){
                    disableScrollFn = false;
                }
            );
    	});

        populateDestinations();

        $(document).scroll(function(){
            if (disableScrollFn) { return; }
            var page_height = $(window).height();
            var pos = $(this).scrollTop();
            for (i in sections) {
                if ((pos + settings.scrollToOffset >= sections[i]) && sections[i] < pos + page_height){
                    activateNav(i);
                }
            }
        });
    };

    function populateDestinations() {
        navItems.each(function(){
            var scrollID = $(this).attr('href').substring(1);
            navs[scrollID] = (settings.activateParentNode)? this.parentNode : this;
            sections[scrollID] = $(document.getElementById(scrollID)).offset().top;
        });
    }

    function activateNav(navID) {
        for (nav in navs) { $(navs[nav]).removeClass('active'); }
        $(navs[navID]).addClass('active');
    }
})( jQuery );


$(document).ready(function (){

  $('nav li a').navScroller();

	$(".sectiondivider").on('click', function(event) {
    	$('html,body').animate({scrollTop: $(event.target.parentNode).offset().top - 50}, 400, "linear");
	});

	$(".container a").each(function(){
        if ($(this).attr("href").charAt(0) == '#'){
            $(this).on('click', function(event) {
        		event.preventDefault();
                var target = $(event.target).closest("a");
                var targetHight =  $(target.attr("href")).offset().top
            	$('html,body').animate({scrollTop: targetHight - 170}, 800, "easeInOutExpo");
            });
        }
	});

  var indexPlaylist = 0;
  $("#video_player figcaption a").on("click", function(event) {
    event.preventDefault();
    $("#video_player figcaption a").eq(indexPlaylist).attr("class","");
    $("#jukebox").attr({
      "src": $(this).attr("href"),
      "poster": $(this).prop("img").attr("src"),
      "autoplay": "autoplay"
    });
    indexPlaylist = $(this).index();
    $("#video_player figcaption a").eq(indexPlaylist).attr("class","currentvid");
  });

  $("#jukebox").attr({
    "src": $("#video_player figcaption a").eq(indexPlaylist).attr("href"),
  });

  $("#jukebox").on("ended",function(){
    $("#video_player figcaption a").eq(indexPlaylist).attr("class","");
    ++indexPlaylist;
    if (indexPlaylist < $("#video_player figcaption a").length) {
      $("#jukebox").attr({
        "src": $("#video_player figcaption a").eq(indexPlaylist).attr("href"),
      });
      $("#video_player figcaption a").eq(indexPlaylist).attr("class","currentvid");
      $("#jukebox")[0].play();
    }
  });


});

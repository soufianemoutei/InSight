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

!function(t, e) {
    "function" == typeof define && define.amd ? define(["jquery"], e) : e("object" == typeof exports ? require("jquery") : t.jQuery ? t.jQuery : t.Zepto)
}(this, function(t, e) {
    t.fn.jPlayerVideo = function(i) {
        var s = "jPlayerVideo",
            a = "string" == typeof i,
            o = Array.prototype.slice.call(arguments, 1),
            n = this;
        return i = !a && o.length ? t.extend.apply(null, [!0, i].concat(o)) : i, a && "_" === i.charAt(0) ? n : (a ? this.each(function() {
            var a = t(this).data(s),
                r = a && t.isFunction(a[i]) ? a[i].apply(a, o) : a;
            return r !== a && r !== e ? (n = r, !1) : void 0
        }) : this.each(function() {
            var e = t(this).data(s);
            e ? e.option(i || {}) : t(this).data(s, new t.jPlayerVideo(i, this))
        }), n)
    }, t.jPlayerVideo = function(e, i) {
        if (arguments.length) {
            this.element = t(i), this.options = t.extend(!0, {}, this.options, e);
            var s = this;
            this.element.bind("remove.jPlayerVideo", function() {
                s.destroy()
            }), this._init()
        }
    }, "function" != typeof t.fn.stop && (t.fn.stop = function() {}), t.jPlayerVideo.emulateMethods = "load play pause", t.jPlayerVideo.emulateStatus = "src readyState networkState currentTime duration paused ended playbackRate", t.jPlayerVideo.emulateOptions = "muted volume", t.jPlayerVideo.reservedEvent = "ready flashreset resize repeat error warning", t.jPlayerVideo.event = {}, t.each(["ready", "setmedia", "flashreset", "resize", "repeat", "click", "error", "warning", "loadstart", "progress", "suspend", "abort", "emptied", "stalled", "play", "pause", "loadedmetadata", "loadeddata", "waiting", "playing", "canplay", "canplaythrough", "seeking", "seeked", "timeupdate", "ended", "ratechange", "durationchange", "volumechange"], function() {
        t.jPlayerVideo.event[this] = "jPlayerVideo_" + this
    }), t.jPlayerVideo.htmlEvent = ["loadstart", "abort", "emptied", "stalled", "loadedmetadata", "canplay", "canplaythrough"], t.jPlayerVideo.pause = function() {
        t.jPlayerVideo.prototype.destroyRemoved(), t.each(t.jPlayerVideo.prototype.instances, function(t, e) {
            e.data("jPlayerVideo").status.srcSet && e.jPlayerVideo("pause")
        })
    }, t.jPlayerVideo.timeFormat = {
        showHour: !1,
        showMin: !0,
        showSec: !0,
        padHour: !1,
        padMin: !0,
        padSec: !0,
        sepHour: ":",
        sepMin: ":",
        sepSec: ""
    };
    var i = function() {
        this.init()
    };
    i.prototype = {
        init: function() {
            this.options = {
                timeFormat: t.jPlayerVideo.timeFormat
            }
        },
        time: function(t) {
            t = t && "number" == typeof t ? t : 0;
            var e = new Date(1e3 * t),
                i = e.getUTCHours(),
                s = this.options.timeFormat.showHour ? e.getUTCMinutes() : e.getUTCMinutes() + 60 * i,
                a = this.options.timeFormat.showMin ? e.getUTCSeconds() : e.getUTCSeconds() + 60 * s,
                o = this.options.timeFormat.padHour && 10 > i ? "0" + i : i,
                n = this.options.timeFormat.padMin && 10 > s ? "0" + s : s,
                r = this.options.timeFormat.padSec && 10 > a ? "0" + a : a,
                l = "";
            return l += this.options.timeFormat.showHour ? o + this.options.timeFormat.sepHour : "", l += this.options.timeFormat.showMin ? n + this.options.timeFormat.sepMin : "", l += this.options.timeFormat.showSec ? r + this.options.timeFormat.sepSec : ""
        }
    };
    var s = new i;
    t.jPlayerVideo.convertTime = function(t) {
        return s.time(t)
    }, t.jPlayerVideo.uaBrowser = function(t) {
        var e = t.toLowerCase(),
            i = /(webkit)[ \/]([\w.]+)/,
            s = /(opera)(?:.*version)?[ \/]([\w.]+)/,
            a = /(msie) ([\w.]+)/,
            o = /(mozilla)(?:.*? rv:([\w.]+))?/,
            n = i.exec(e) || s.exec(e) || a.exec(e) || e.indexOf("compatible") < 0 && o.exec(e) || [];
        return {
            browser: n[1] || "",
            version: n[2] || "0"
        }
    }, t.jPlayerVideo.uaPlatform = function(t) {
        var e = t.toLowerCase(),
            i = /(ipad|iphone|ipod|android|blackberry|playbook|windows ce|webos)/,
            s = /(ipad|playbook)/,
            a = /(android)/,
            o = /(mobile)/,
            n = i.exec(e) || [],
            r = s.exec(e) || !o.exec(e) && a.exec(e) || [];
        return n[1] && (n[1] = n[1].replace(/\s/g, "_")), {
            platform: n[1] || "",
            tablet: r[1] || ""
        }
    }, t.jPlayerVideo.browser = {}, t.jPlayerVideo.platform = {};
    var a = t.jPlayerVideo.uaBrowser(navigator.userAgent);
    a.browser && (t.jPlayerVideo.browser[a.browser] = !0, t.jPlayerVideo.browser.version = a.version);
    var o = t.jPlayerVideo.uaPlatform(navigator.userAgent);
    o.platform && (t.jPlayerVideo.platform[o.platform] = !0, t.jPlayerVideo.platform.mobile = !o.tablet, t.jPlayerVideo.platform.tablet = !!o.tablet), t.jPlayerVideo.getDocMode = function() {
        var e;
        return t.jPlayerVideo.browser.msie && (document.documentMode ? e = document.documentMode : (e = 5, document.compatMode && "CSS1Compat" === document.compatMode && (e = 7))), e
    }, t.jPlayerVideo.browser.documentMode = t.jPlayerVideo.getDocMode(), t.jPlayerVideo.nativeFeatures = {
        init: function() {
            var t,
                e,
                i,
                s = document,
                a = s.createElement("video"),
                o = {
                    w3c: ["fullscreenEnabled", "fullscreenElement", "requestFullscreen", "exitFullscreen", "fullscreenchange", "fullscreenerror"],
                    moz: ["mozFullScreenEnabled", "mozFullScreenElement", "mozRequestFullScreen", "mozCancelFullScreen", "mozfullscreenchange", "mozfullscreenerror"],
                    webkit: ["", "webkitCurrentFullScreenElement", "webkitRequestFullScreen", "webkitCancelFullScreen", "webkitfullscreenchange", ""],
                    webkitVideo: ["webkitSupportsFullscreen", "webkitDisplayingFullscreen", "webkitEnterFullscreen", "webkitExitFullscreen", "", ""],
                    ms: ["", "msFullscreenElement", "msRequestFullscreen", "msExitFullscreen", "MSFullscreenChange", "MSFullscreenError"]
                },
                n = ["w3c", "moz", "webkit", "webkitVideo", "ms"];
            for (this.fullscreen = t = {
                support: {
                    w3c: !!s[o.w3c[0]],
                    moz: !!s[o.moz[0]],
                    webkit: "function" == typeof s[o.webkit[3]],
                    webkitVideo: "function" == typeof a[o.webkitVideo[2]],
                    ms: "function" == typeof a[o.ms[2]]
                },
                used: {}
            }, e = 0, i = n.length; i > e; e++) {
                var r = n[e];
                if (t.support[r]) {
                    t.spec = r, t.used[r] = !0;
                    break
                }
            }
            if (t.spec) {
                var l = o[t.spec];
                t.api = {
                    fullscreenEnabled: !0,
                    fullscreenElement: function(t) {
                        return t = t ? t : s, t[l[1]]
                    },
                    requestFullscreen: function(t) {
                        return t[l[2]]()
                    },
                    exitFullscreen: function(t) {
                        return t = t ? t : s, t[l[3]]()
                    }
                }, t.event = {
                    fullscreenchange: l[4],
                    fullscreenerror: l[5]
                }
            } else
                t.api = {
                    fullscreenEnabled: !1,
                    fullscreenElement: function() {
                        return null
                    },
                    requestFullscreen: function() {},
                    exitFullscreen: function() {}
                }, t.event = {}
        }
    }, t.jPlayerVideo.nativeFeatures.init(), t.jPlayerVideo.focus = null, t.jPlayerVideo.keyIgnoreElementNames = "A INPUT TEXTAREA SELECT BUTTON";
    var n = function(e) {
        var i,
            s = t.jPlayerVideo.focus;
        s && (t.each(t.jPlayerVideo.keyIgnoreElementNames.split(/\s+/g), function(t, s) {
            return e.target.nodeName.toUpperCase() === s.toUpperCase() ? (i = !0, !1) : void 0
        }), i || t.each(s.options.keyBindings, function(i, a) {
            return a && t.isFunction(a.fn) && ("number" == typeof a.key && e.which === a.key || "string" == typeof a.key && e.key === a.key) ? (e.preventDefault(), a.fn(s), !1) : void 0
        }))
    };
    t.jPlayerVideo.keys = function(e) {
        var i = "keydown.jPlayerVideo";
        t(document.documentElement).unbind(i), e && t(document.documentElement).bind(i, n)
    }, t.jPlayerVideo.keys(!0), t.jPlayerVideo.prototype = {
        count: 0,
        version: {
            script: "2.9.2",
            needFlash: "2.9.0",
            flash: "unknown"
        },
        options: {
            swfPath: "js",
            solution: "html, flash",
            supplied: "mp3",
            auroraFormats: "wav",
            preload: "metadata",
            volume: .8,
            muted: !1,
            remainingDuration: !1,
            toggleDuration: !1,
            captureDuration: !0,
            playbackRate: 1,
            defaultPlaybackRate: 1,
            minPlaybackRate: .5,
            maxPlaybackRate: 4,
            wmode: "opaque",
            backgroundColor: "#000000",
            cssSelectorAncestor: "#jp_video_container_1",
            cssSelector: {
                videoPlay: ".mdtc-clnplrv-video-play",
                play: ".mdtc-clnplrv-play",
                pause: ".mdtc-clnplrv-pause",
                stop: ".mdtc-clnplrv-stop",
                seekBar: ".mdtc-clnplrv-seek-bar",
                playBar: ".mdtc-clnplrv-play-bar",
                mute: ".mdtc-clnplrv-mute",
                unmute: ".mdtc-clnplrv-unmute",
                volumeBar: ".mdtc-clnplrv-volume-bar",
                volumeBarValue: ".mdtc-clnplrv-volume-bar-value",
                volumeMax: ".mdtc-clnplrv-volume-max",
                playbackRateBar: ".mdtc-clnplrv-playback-rate-bar",
                playbackRateBarValue: ".mdtc-clnplrv-playback-rate-bar-value",
                currentTime: ".mdtc-clnplrv-current-time",
                duration: ".mdtc-clnplrv-duration",
                title: ".mdtc-clnplrv-title",
                fullScreen: ".mdtc-clnplrv-full-screen",
                restoreScreen: ".mdtc-clnplrv-restore-screen",
                repeat: ".mdtc-clnplrv-repeat",
                repeatOff: ".mdtc-clnplrv-repeat-off",
                gui: ".mdtc-clnplrv-gui",
                noSolution: ".mdtc-clnplrv-no-solution"
            },
            stateClass: {
                playing: "mdtc-clnplrv-state-playing",
                seeking: "mdtc-clnplrv-state-seeking",
                muted: "mdtc-clnplrv-state-muted",
                looped: "mdtc-clnplrv-state-looped",
                fullScreen: "mdtc-clnplrv-state-full-screen",
                noVolume: "mdtc-clnplrv-state-no-volume"
            },
            useStateClassSkin: !1,
            autoBlur: !0,
            smoothPlayBar: !1,
            fullScreen: !1,
            fullWindow: !1,
            autohide: {
                restored: !1,
                full: !0,
                fadeIn: 200,
                fadeOut: 600,
                hold: 1e3
            },
            loop: !1,
            repeat: function(e) {
                e.jPlayerVideo.options.loop ? t(this).unbind(".jPlayerVideoRepeat").bind(t.jPlayerVideo.event.ended + ".jPlayerVideo.jPlayerVideoRepeat", function() {
                    t(this).jPlayerVideo("play")
                }) : t(this).unbind(".jPlayerVideoRepeat")
            },
            nativeVideoControls: {},
            noFullWindow: {
                msie: /msie [0-6]\./,
                ipad: /ipad.*?os [0-4]\./,
                iphone: /iphone/,
                ipod: /ipod/,
                android_pad: /android [0-3]\.(?!.*?mobile)/,
                android_phone: /(?=.*android)(?!.*chrome)(?=.*mobile)/,
                blackberry: /blackberry/,
                windows_ce: /windows ce/,
                iemobile: /iemobile/,
                webos: /webos/
            },
            noVolume: {
                ipad: /ipad/,
                iphone: /iphone/,
                ipod: /ipod/,
                android_pad: /android(?!.*?mobile)/,
                android_phone: /android.*?mobile/,
                blackberry: /blackberry/,
                windows_ce: /windows ce/,
                iemobile: /iemobile/,
                webos: /webos/,
                playbook: /playbook/
            },
            timeFormat: {},
            keyEnabled: !1,
            audioFullScreen: !1,
            keyBindings: {
                play: {
                    key: 80,
                    fn: function(t) {
                        t.status.paused ? t.play() : t.pause()
                    }
                },
                fullScreen: {
                    key: 70,
                    fn: function(t) {
                        (t.status.video || t.options.audioFullScreen) && t._setOption("fullScreen", !t.options.fullScreen)
                    }
                },
                muted: {
                    key: 77,
                    fn: function(t) {
                        t._muted(!t.options.muted)
                    }
                },
                volumeUp: {
                    key: 190,
                    fn: function(t) {
                        t.volume(t.options.volume + .1)
                    }
                },
                volumeDown: {
                    key: 188,
                    fn: function(t) {
                        t.volume(t.options.volume - .1)
                    }
                },
                loop: {
                    key: 76,
                    fn: function(t) {
                        t._loop(!t.options.loop)
                    }
                }
            },
            verticalVolume: !1,
            verticalPlaybackRate: !1,
            globalVolume: !1,
            idPrefix: "jp_video_",
            noConflict: "jQuery",
            emulateHtml: !1,
            consoleAlerts: !0,
            errorAlerts: !1,
            warningAlerts: !1
        },
        optionsAudio: {
            size: {
                width: "0px",
                height: "0px",
                cssClass: ""
            },
            sizeFull: {
                width: "0px",
                height: "0px",
                cssClass: ""
            }
        },
        optionsVideo: {
            size: {
                width: "480px",
                height: "270px",
                cssClass: "mdtc-clnplrv-video-270p"
            },
            sizeFull: {
                width: "100%",
                height: "100%",
                cssClass: "mdtc-clnplrv-video-full"
            }
        },
        instances: {},
        status: {
            src: "",
            media: {},
            paused: !0,
            format: {},
            formatType: "",
            waitForPlay: !0,
            waitForLoad: !0,
            srcSet: !1,
            video: !1,
            seekPercent: 0,
            currentPercentRelative: 0,
            currentPercentAbsolute: 0,
            currentTime: 0,
            duration: 0,
            remaining: 0,
            videoWidth: 0,
            videoHeight: 0,
            readyState: 0,
            networkState: 0,
            playbackRate: 1,
            ended: 0
        },
        internal: {
            ready: !1
        },
        solution: {
            html: !0,
            aurora: !0,
            flash: !0
        },
        format: {
            mp3: {
                codec: "audio/mpeg",
                flashCanPlay: !0,
                media: "audio"
            },
            m4a: {
                codec: 'audio/mp4; codecs="mp4a.40.2"',
                flashCanPlay: !0,
                media: "audio"
            },
            m3u8a: {
                codec: 'application/vnd.apple.mpegurl; codecs="mp4a.40.2"',
                flashCanPlay: !1,
                media: "audio"
            },
            m3ua: {
                codec: "audio/mpegurl",
                flashCanPlay: !1,
                media: "audio"
            },
            oga: {
                codec: 'audio/ogg; codecs="vorbis, opus"',
                flashCanPlay: !1,
                media: "audio"
            },
            flac: {
                codec: "audio/x-flac",
                flashCanPlay: !1,
                media: "audio"
            },
            wav: {
                codec: 'audio/wav; codecs="1"',
                flashCanPlay: !1,
                media: "audio"
            },
            webma: {
                codec: 'audio/webm; codecs="vorbis"',
                flashCanPlay: !1,
                media: "audio"
            },
            fla: {
                codec: "audio/x-flv",
                flashCanPlay: !0,
                media: "audio"
            },
            rtmpa: {
                codec: 'audio/rtmp; codecs="rtmp"',
                flashCanPlay: !0,
                media: "audio"
            },
            m4v: {
                codec: 'video/mp4; codecs="avc1.42E01E, mp4a.40.2"',
                flashCanPlay: !0,
                media: "video"
            },
            m3u8v: {
                codec: 'application/vnd.apple.mpegurl; codecs="avc1.42E01E, mp4a.40.2"',
                flashCanPlay: !1,
                media: "video"
            },
            m3uv: {
                codec: "audio/mpegurl",
                flashCanPlay: !1,
                media: "video"
            },
            ogv: {
                codec: 'video/ogg; codecs="theora, vorbis"',
                flashCanPlay: !1,
                media: "video"
            },
            webmv: {
                codec: 'video/webm; codecs="vorbis, vp8"',
                flashCanPlay: !1,
                media: "video"
            },
            flv: {
                codec: "video/x-flv",
                flashCanPlay: !0,
                media: "video"
            },
            rtmpv: {
                codec: 'video/rtmp; codecs="rtmp"',
                flashCanPlay: !0,
                media: "video"
            }
        },
        _init: function() {
            var i = this;
            if (this.element.empty(), this.status = t.extend({}, this.status), this.internal = t.extend({}, this.internal), this.options.timeFormat = t.extend({}, t.jPlayerVideo.timeFormat, this.options.timeFormat), this.internal.cmdsIgnored = t.jPlayerVideo.platform.ipad || t.jPlayerVideo.platform.iphone || t.jPlayerVideo.platform.ipod, this.internal.domNode = this.element.get(0), this.options.keyEnabled && !t.jPlayerVideo.focus && (t.jPlayerVideo.focus = this), this.androidFix = {
                setMedia: !1,
                play: !1,
                pause: !1,
                time: NaN
            }, t.jPlayerVideo.platform.android && (this.options.preload = "auto" !== this.options.preload ? "metadata" : "auto"), this.formats = [], this.solutions = [], this.require = {}, this.htmlElement = {}, this.html = {}, this.html.audio = {}, this.html.video = {}, this.aurora = {}, this.aurora.formats = [], this.aurora.properties = [], this.flash = {}, this.css = {}, this.css.cs = {}, this.css.jq = {}, this.ancestorJq = [], this.options.volume = this._limitValue(this.options.volume, 0, 1), t.each(this.options.supplied.toLowerCase().split(","), function(e, s) {
                var a = s.replace(/^\s+|\s+$/g, "");
                if (i.format[a]) {
                    var o = !1;
                    t.each(i.formats, function(t, e) {
                        return a === e ? (o = !0, !1) : void 0
                    }), o || i.formats.push(a)
                }
            }), t.each(this.options.solution.toLowerCase().split(","), function(e, s) {
                var a = s.replace(/^\s+|\s+$/g, "");
                if (i.solution[a]) {
                    var o = !1;
                    t.each(i.solutions, function(t, e) {
                        return a === e ? (o = !0, !1) : void 0
                    }), o || i.solutions.push(a)
                }
            }), t.each(this.options.auroraFormats.toLowerCase().split(","), function(e, s) {
                var a = s.replace(/^\s+|\s+$/g, "");
                if (i.format[a]) {
                    var o = !1;
                    t.each(i.aurora.formats, function(t, e) {
                        return a === e ? (o = !0, !1) : void 0
                    }), o || i.aurora.formats.push(a)
                }
            }), this.internal.instance = "jp_video_" + this.count, this.instances[this.internal.instance] = this.element, this.element.attr("id") || this.element.attr("id", this.options.idPrefix + "_jplayer_" + this.count), this.internal.self = t.extend({}, {
                id: this.element.attr("id"),
                jq: this.element
            }), this.internal.audio = t.extend({}, {
                id: this.options.idPrefix + "_audio_" + this.count,
                jq: e
            }), this.internal.video = t.extend({}, {
                id: this.options.idPrefix + "_video_" + this.count,
                jq: e
            }), this.internal.flash = t.extend({}, {
                id: this.options.idPrefix + "_flash_" + this.count,
                jq: e,
                swf: this.options.swfPath + (".swf" !== this.options.swfPath.toLowerCase().slice(-4) ? (this.options.swfPath && "/" !== this.options.swfPath.slice(-1) ? "/" : "") + "jquery.jplayer.swf" : "")
            }), this.internal.poster = t.extend({}, {
                id: this.options.idPrefix + "_poster_" + this.count,
                jq: e
            }), t.each(t.jPlayerVideo.event, function(t, s) {
                i.options[t] !== e && (i.element.bind(s + ".jPlayerVideo", i.options[t]), i.options[t] = e)
            }), this.require.audio = !1, this.require.video = !1, t.each(this.formats, function(t, e) {
                i.require[i.format[e].media] = !0
            }), this.require.video ? this.options = t.extend(!0, {}, this.optionsVideo, this.options) : this.options = t.extend(!0, {}, this.optionsAudio, this.options), this._setSize(), this.status.nativeVideoControls = this._uaBlocklist(this.options.nativeVideoControls), this.status.noFullWindow = this._uaBlocklist(this.options.noFullWindow), this.status.noVolume = this._uaBlocklist(this.options.noVolume), t.jPlayerVideo.nativeFeatures.fullscreen.api.fullscreenEnabled && this._fullscreenAddEventListeners(), this._restrictNativeVideoControls(), this.htmlElement.poster = document.createElement("img"), this.htmlElement.poster.id = this.internal.poster.id, this.htmlElement.poster.onload = function() {
                (!i.status.video || i.status.waitForPlay) && i.internal.poster.jq.show()
            }, this.element.append(this.htmlElement.poster), this.internal.poster.jq = t("#" + this.internal.poster.id), this.internal.poster.jq.css({
                width: this.status.width,
                height: this.status.height
            }), this.internal.poster.jq.hide(), this.internal.poster.jq.bind("click.jPlayerVideo", function() {
                i._trigger(t.jPlayerVideo.event.click)
            }), this.html.audio.available = !1, this.require.audio && (this.htmlElement.audio = document.createElement("audio"), this.htmlElement.audio.id = this.internal.audio.id, this.html.audio.available = !!this.htmlElement.audio.canPlayType && this._testCanPlayType(this.htmlElement.audio)), this.html.video.available = !1, this.require.video && (this.htmlElement.video = document.createElement("video"), this.htmlElement.video.id = this.internal.video.id, this.html.video.available = !!this.htmlElement.video.canPlayType && this._testCanPlayType(this.htmlElement.video)), this.flash.available = this._checkForFlash(10.1), this.html.canPlay = {}, this.aurora.canPlay = {}, this.flash.canPlay = {}, t.each(this.formats, function(e, s) {
                i.html.canPlay[s] = i.html[i.format[s].media].available && "" !== i.htmlElement[i.format[s].media].canPlayType(i.format[s].codec), i.aurora.canPlay[s] = t.inArray(s, i.aurora.formats) > -1, i.flash.canPlay[s] = i.format[s].flashCanPlay && i.flash.available
            }), this.html.desired = !1, this.aurora.desired = !1, this.flash.desired = !1, t.each(this.solutions, function(e, s) {
                if (0 === e)
                    i[s].desired = !0;
                else {
                    var a = !1,
                        o = !1;
                    t.each(i.formats, function(t, e) {
                        i[i.solutions[0]].canPlay[e] && ("video" === i.format[e].media ? o = !0 : a = !0)
                    }), i[s].desired = i.require.audio && !a || i.require.video && !o
                }
            }), this.html.support = {}, this.aurora.support = {}, this.flash.support = {}, t.each(this.formats, function(t, e) {
                i.html.support[e] = i.html.canPlay[e] && i.html.desired, i.aurora.support[e] = i.aurora.canPlay[e] && i.aurora.desired, i.flash.support[e] = i.flash.canPlay[e] && i.flash.desired
            }), this.html.used = !1, this.aurora.used = !1, this.flash.used = !1, t.each(this.solutions, function(e, s) {
                t.each(i.formats, function(t, e) {
                    return i[s].support[e] ? (i[s].used = !0, !1) : void 0
                })
            }), this._resetActive(), this._resetGate(), this._cssSelectorAncestor(this.options.cssSelectorAncestor), this.html.used || this.aurora.used || this.flash.used ? this.css.jq.noSolution.length && this.css.jq.noSolution.hide() : (this._error({
                type: t.jPlayerVideo.error.NO_SOLUTION,
                context: "{solution:'" + this.options.solution + "', supplied:'" + this.options.supplied + "'}",
                message: t.jPlayerVideo.errorMsg.NO_SOLUTION,
                hint: t.jPlayerVideo.errorHint.NO_SOLUTION
            }), this.css.jq.noSolution.length && this.css.jq.noSolution.show()), this.flash.used) {
                var s,
                    a = "jQuery=" + encodeURI(this.options.noConflict) + "&id=" + encodeURI(this.internal.self.id) + "&vol=" + this.options.volume + "&muted=" + this.options.muted;
                if (t.jPlayerVideo.browser.msie && (Number(t.jPlayerVideo.browser.version) < 9 || t.jPlayerVideo.browser.documentMode < 9)) {
                    var o = '<object id="' + this.internal.flash.id + '" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" width="0" height="0" tabindex="-1"></object>',
                        n = ['<param name="movie" value="' + this.internal.flash.swf + '" />', '<param name="FlashVars" value="' + a + '" />', '<param name="allowScriptAccess" value="always" />', '<param name="bgcolor" value="' + this.options.backgroundColor + '" />', '<param name="wmode" value="' + this.options.wmode + '" />'];
                    s = document.createElement(o);
                    for (var r = 0; r < n.length; r++)
                        s.appendChild(document.createElement(n[r]))
                } else {
                    var l = function(t, e, i) {
                        var s = document.createElement("param");
                        s.setAttribute("name", e), s.setAttribute("value", i), t.appendChild(s)
                    };
                    s = document.createElement("object"), s.setAttribute("id", this.internal.flash.id), s.setAttribute("name", this.internal.flash.id), s.setAttribute("data", this.internal.flash.swf), s.setAttribute("type", "application/x-shockwave-flash"), s.setAttribute("width", "1"), s.setAttribute("height", "1"), s.setAttribute("tabindex", "-1"), l(s, "flashvars", a), l(s, "allowscriptaccess", "always"), l(s, "bgcolor", this.options.backgroundColor), l(s, "wmode", this.options.wmode)
                }
                this.element.append(s), this.internal.flash.jq = t(s)
            }
            this.html.used && !this.flash.used ? this.status.playbackRateEnabled = this._testPlaybackRate("audio") : this.status.playbackRateEnabled = !1, this._updatePlaybackRate(), this.html.used && (this.html.audio.available && (this._addHtmlEventListeners(this.htmlElement.audio, this.html.audio), this.element.append(this.htmlElement.audio), this.internal.audio.jq = t("#" + this.internal.audio.id)), this.html.video.available && (this._addHtmlEventListeners(this.htmlElement.video, this.html.video), this.element.append(this.htmlElement.video), this.internal.video.jq = t("#" + this.internal.video.id), this.status.nativeVideoControls ? this.internal.video.jq.css({
                width: this.status.width,
                height: this.status.height
            }) : this.internal.video.jq.css({
                width: "0px",
                height: "0px"
            }), this.internal.video.jq.bind("click.jPlayerVideo", function() {
                i._trigger(t.jPlayerVideo.event.click)
            }))), this.aurora.used, this.options.emulateHtml && this._emulateHtmlBridge(), !this.html.used && !this.aurora.used || this.flash.used || setTimeout(function() {
                i.internal.ready = !0, i.version.flash = "n/a", i._trigger(t.jPlayerVideo.event.repeat), i._trigger(t.jPlayerVideo.event.ready)
            }, 100), this._updateNativeVideoControls(), this.css.jq.videoPlay.length && this.css.jq.videoPlay.hide(), t.jPlayerVideo.prototype.count++
        },
        destroy: function() {
            this.clearMedia(), this._removeUiClass(), this.css.jq.currentTime.length && this.css.jq.currentTime.text(""), this.css.jq.duration.length && this.css.jq.duration.text(""), t.each(this.css.jq, function(t, e) {
                e.length && e.unbind(".jPlayerVideo")
            }), this.internal.poster.jq.unbind(".jPlayerVideo"), this.internal.video.jq && this.internal.video.jq.unbind(".jPlayerVideo"), this._fullscreenRemoveEventListeners(), this === t.jPlayerVideo.focus && (t.jPlayerVideo.focus = null), this.options.emulateHtml && this._destroyHtmlBridge(), this.element.removeData("jPlayerVideo"), this.element.unbind(".jPlayerVideo"), this.element.empty(), delete this.instances[this.internal.instance]
        },
        destroyRemoved: function() {
            var e = this;
            t.each(this.instances, function(t, i) {
                e.element !== i && (i.data("jPlayerVideo") || (i.jPlayerVideo("destroy"), delete e.instances[t]))
            })
        },
        enable: function() {},
        disable: function() {},
        _testCanPlayType: function(t) {
            try {
                return t.canPlayType(this.format.mp3.codec), !0
            } catch (e) {
                return !1
            }
        },
        _testPlaybackRate: function(t) {
            var e,
                i = .5;
            t = "string" == typeof t ? t : "audio", e = document.createElement(t);
            try {
                return "playbackRate" in e ? (e.playbackRate = i, e.playbackRate === i) : !1
            } catch (s) {
                return !1
            }
        },
        _uaBlocklist: function(e) {
            var i = navigator.userAgent.toLowerCase(),
                s = !1;
            return t.each(e, function(t, e) {
                return e && e.test(i) ? (s = !0, !1) : void 0
            }), s
        },
        _restrictNativeVideoControls: function() {
            this.require.audio && this.status.nativeVideoControls && (this.status.nativeVideoControls = !1, this.status.noFullWindow = !0)
        },
        _updateNativeVideoControls: function() {
            this.html.video.available && this.html.used && (this.htmlElement.video.controls = this.status.nativeVideoControls, this._updateAutohide(), this.status.nativeVideoControls && this.require.video ? (this.internal.poster.jq.hide(), this.internal.video.jq.css({
                width: this.status.width,
                height: this.status.height
            })) : this.status.waitForPlay && this.status.video && (this.internal.poster.jq.show(), this.internal.video.jq.css({
                width: "0px",
                height: "0px"
            })))
        },
        _addHtmlEventListeners: function(e, i) {
            var s = this;
            e.preload = this.options.preload, e.muted = this.options.muted, e.volume = this.options.volume, this.status.playbackRateEnabled && (e.defaultPlaybackRate = this.options.defaultPlaybackRate, e.playbackRate = this.options.playbackRate), e.addEventListener("progress", function() {
                i.gate && (s.internal.cmdsIgnored && this.readyState > 0 && (s.internal.cmdsIgnored = !1), s._getHtmlStatus(e), s._updateInterface(), s._trigger(t.jPlayerVideo.event.progress))
            }, !1), e.addEventListener("loadeddata", function() {
                i.gate && (s.androidFix.setMedia = !1, s.androidFix.play && (s.androidFix.play = !1, s.play(s.androidFix.time)), s.androidFix.pause && (s.androidFix.pause = !1, s.pause(s.androidFix.time)), s._trigger(t.jPlayerVideo.event.loadeddata))
            }, !1), e.addEventListener("timeupdate", function() {
                i.gate && (s._getHtmlStatus(e), s._updateInterface(), s._trigger(t.jPlayerVideo.event.timeupdate))
            }, !1), e.addEventListener("durationchange", function() {
                i.gate && (s._getHtmlStatus(e), s._updateInterface(), s._trigger(t.jPlayerVideo.event.durationchange))
            }, !1), e.addEventListener("play", function() {
                i.gate && (s._updateButtons(!0), s._html_checkWaitForPlay(), s._trigger(t.jPlayerVideo.event.play))
            }, !1), e.addEventListener("playing", function() {
                i.gate && (s._updateButtons(!0), s._seeked(), s._trigger(t.jPlayerVideo.event.playing))
            }, !1), e.addEventListener("pause", function() {
                i.gate && (s._updateButtons(!1), s._trigger(t.jPlayerVideo.event.pause))
            }, !1), e.addEventListener("waiting", function() {
                i.gate && (s._seeking(), s._trigger(t.jPlayerVideo.event.waiting))
            }, !1), e.addEventListener("seeking", function() {
                i.gate && (s._seeking(), s._trigger(t.jPlayerVideo.event.seeking))
            }, !1), e.addEventListener("seeked", function() {
                i.gate && (s._seeked(), s._trigger(t.jPlayerVideo.event.seeked))
            }, !1), e.addEventListener("volumechange", function() {
                i.gate && (s.options.volume = e.volume, s.options.muted = e.muted, s._updateMute(), s._updateVolume(), s._trigger(t.jPlayerVideo.event.volumechange))
            }, !1), e.addEventListener("ratechange", function() {
                i.gate && (s.options.defaultPlaybackRate = e.defaultPlaybackRate, s.options.playbackRate = e.playbackRate, s._updatePlaybackRate(), s._trigger(t.jPlayerVideo.event.ratechange))
            }, !1), e.addEventListener("suspend", function() {
                i.gate && (s._seeked(), s._trigger(t.jPlayerVideo.event.suspend))
            }, !1), e.addEventListener("ended", function() {
                i.gate && (t.jPlayerVideo.browser.webkit || (s.htmlElement.media.currentTime = 0), s.htmlElement.media.pause(), s._updateButtons(!1), s._getHtmlStatus(e, !0), s._updateInterface(), s._trigger(t.jPlayerVideo.event.ended))
            }, !1), e.addEventListener("error", function() {
                i.gate && (s._updateButtons(!1), s._seeked(), s.status.srcSet && (clearTimeout(s.internal.htmlDlyCmdId), s.status.waitForLoad = !0, s.status.waitForPlay = !0, s.status.video && !s.status.nativeVideoControls && s.internal.video.jq.css({
                    width: "0px",
                    height: "0px"
                }), s._validString(s.status.media.poster) && !s.status.nativeVideoControls && s.internal.poster.jq.show(), s.css.jq.videoPlay.length && s.css.jq.videoPlay.show(), s._error({
                    type: t.jPlayerVideo.error.URL,
                    context: s.status.src,
                    message: t.jPlayerVideo.errorMsg.URL,
                    hint: t.jPlayerVideo.errorHint.URL
                })))
            }, !1), t.each(t.jPlayerVideo.htmlEvent, function(a, o) {
                e.addEventListener(this, function() {
                    i.gate && s._trigger(t.jPlayerVideo.event[o])
                }, !1)
            })
        },
        _addAuroraEventListeners: function(e, i) {
            var s = this;
            e.volume = 100 * this.options.volume, e.on("progress", function() {
                i.gate && (s.internal.cmdsIgnored && this.readyState > 0 && (s.internal.cmdsIgnored = !1), s._getAuroraStatus(e), s._updateInterface(), s._trigger(t.jPlayerVideo.event.progress), e.duration > 0 && s._trigger(t.jPlayerVideo.event.timeupdate))
            }, !1), e.on("ready", function() {
                i.gate && s._trigger(t.jPlayerVideo.event.loadeddata)
            }, !1), e.on("duration", function() {
                i.gate && (s._getAuroraStatus(e), s._updateInterface(), s._trigger(t.jPlayerVideo.event.durationchange))
            }, !1), e.on("end", function() {
                i.gate && (s._updateButtons(!1), s._getAuroraStatus(e, !0), s._updateInterface(), s._trigger(t.jPlayerVideo.event.ended))
            }, !1), e.on("error", function() {
                i.gate && (s._updateButtons(!1), s._seeked(), s.status.srcSet && (s.status.waitForLoad = !0, s.status.waitForPlay = !0, s.status.video && !s.status.nativeVideoControls && s.internal.video.jq.css({
                    width: "0px",
                    height: "0px"
                }), s._validString(s.status.media.poster) && !s.status.nativeVideoControls && s.internal.poster.jq.show(), s.css.jq.videoPlay.length && s.css.jq.videoPlay.show(), s._error({
                    type: t.jPlayerVideo.error.URL,
                    context: s.status.src,
                    message: t.jPlayerVideo.errorMsg.URL,
                    hint: t.jPlayerVideo.errorHint.URL
                })))
            }, !1)
        },
        _getHtmlStatus: function(t, e) {
            var i = 0,
                s = 0,
                a = 0,
                o = 0;
            isFinite(t.duration) && (this.status.duration = t.duration), i = t.currentTime, s = this.status.duration > 0 ? 100 * i / this.status.duration : 0, "object" == typeof t.seekable && t.seekable.length > 0 ? (a = this.status.duration > 0 ? 100 * t.seekable.end(t.seekable.length - 1) / this.status.duration : 100, o = this.status.duration > 0 ? 100 * t.currentTime / t.seekable.end(t.seekable.length - 1) : 0) : (a = 100, o = s), e && (i = 0, o = 0, s = 0), this.status.seekPercent = a, this.status.currentPercentRelative = o, this.status.currentPercentAbsolute = s, this.status.currentTime = i, this.status.remaining = this.status.duration - this.status.currentTime, this.status.videoWidth = t.videoWidth, this.status.videoHeight = t.videoHeight, this.status.readyState = t.readyState, this.status.networkState = t.networkState, this.status.playbackRate = t.playbackRate, this.status.ended = t.ended
        },
        _getAuroraStatus: function(t, e) {
            var i = 0,
                s = 0,
                a = 0,
                o = 0;
            this.status.duration = t.duration / 1e3, i = t.currentTime / 1e3, s = this.status.duration > 0 ? 100 * i / this.status.duration : 0, t.buffered > 0 ? (a = this.status.duration > 0 ? t.buffered * this.status.duration / this.status.duration : 100, o = this.status.duration > 0 ? i / (t.buffered * this.status.duration) : 0) : (a = 100, o = s), e && (i = 0, o = 0, s = 0), this.status.seekPercent = a, this.status.currentPercentRelative = o, this.status.currentPercentAbsolute = s, this.status.currentTime = i, this.status.remaining = this.status.duration - this.status.currentTime, this.status.readyState = 4, this.status.networkState = 0, this.status.playbackRate = 1, this.status.ended = !1
        },
        _resetStatus: function() {
            this.status = t.extend({}, this.status, t.jPlayerVideo.prototype.status)
        },
        _trigger: function(e, i, s) {
            var a = t.Event(e);
            a.jPlayerVideo = {}, a.jPlayerVideo.version = t.extend({}, this.version), a.jPlayerVideo.options = t.extend(!0, {}, this.options), a.jPlayerVideo.status = t.extend(!0, {}, this.status), a.jPlayerVideo.html = t.extend(!0, {}, this.html), a.jPlayerVideo.aurora = t.extend(!0, {}, this.aurora), a.jPlayerVideo.flash = t.extend(!0, {}, this.flash), i && (a.jPlayerVideo.error = t.extend({}, i)), s && (a.jPlayerVideo.warning = t.extend({}, s)), this.element.trigger(a)
        },
        jPlayerVideoFlashEvent: function(e, i) {
            if (e === t.jPlayerVideo.event.ready)
                if (this.internal.ready) {
                    if (this.flash.gate) {
                        if (this.status.srcSet) {
                            var s = this.status.currentTime,
                                a = this.status.paused;
                            this.setMedia(this.status.media), this.volumeWorker(this.options.volume), s > 0 && (a ? this.pause(s) : this.play(s))
                        }
                        this._trigger(t.jPlayerVideo.event.flashreset)
                    }
                } else
                    this.internal.ready = !0, this.internal.flash.jq.css({
                        width: "0px",
                        height: "0px"
                    }), this.version.flash = i.version, this.version.needFlash !== this.version.flash && this._error({
                        type: t.jPlayerVideo.error.VERSION,
                        context: this.version.flash,
                        message: t.jPlayerVideo.errorMsg.VERSION + this.version.flash,
                        hint: t.jPlayerVideo.errorHint.VERSION
                    }), this._trigger(t.jPlayerVideo.event.repeat), this._trigger(e);
            if (this.flash.gate)
                switch (e) {
                case t.jPlayerVideo.event.progress:
                    this._getFlashStatus(i), this._updateInterface(), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.timeupdate:
                    this._getFlashStatus(i), this._updateInterface(), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.play:
                    this._seeked(), this._updateButtons(!0), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.pause:
                    this._updateButtons(!1), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.ended:
                    this._updateButtons(!1), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.click:
                    this._trigger(e);
                    break;
                case t.jPlayerVideo.event.error:
                    this.status.waitForLoad = !0, this.status.waitForPlay = !0, this.status.video && this.internal.flash.jq.css({
                        width: "0px",
                        height: "0px"
                    }), this._validString(this.status.media.poster) && this.internal.poster.jq.show(), this.css.jq.videoPlay.length && this.status.video && this.css.jq.videoPlay.show(), this.status.video ? this._flash_setVideo(this.status.media) : this._flash_setAudio(this.status.media), this._updateButtons(!1), this._error({
                        type: t.jPlayerVideo.error.URL,
                        context: i.src,
                        message: t.jPlayerVideo.errorMsg.URL,
                        hint: t.jPlayerVideo.errorHint.URL
                    });
                    break;
                case t.jPlayerVideo.event.seeking:
                    this._seeking(), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.seeked:
                    this._seeked(), this._trigger(e);
                    break;
                case t.jPlayerVideo.event.ready:
                    break;
                default:
                    this._trigger(e)
                }
            return !1
        },
        _getFlashStatus: function(t) {
            this.status.seekPercent = t.seekPercent, this.status.currentPercentRelative = t.currentPercentRelative, this.status.currentPercentAbsolute = t.currentPercentAbsolute, this.status.currentTime = t.currentTime, this.status.duration = t.duration, this.status.remaining = t.duration - t.currentTime, this.status.videoWidth = t.videoWidth, this.status.videoHeight = t.videoHeight, this.status.readyState = 4, this.status.networkState = 0, this.status.playbackRate = 1, this.status.ended = !1
        },
        _updateButtons: function(t) {
            t === e ? t = !this.status.paused : this.status.paused = !t, t ? this.addStateClass("playing") : this.removeStateClass("playing"), !this.status.noFullWindow && this.options.fullWindow ? this.addStateClass("fullScreen") : this.removeStateClass("fullScreen"), this.options.loop ? this.addStateClass("looped") : this.removeStateClass("looped"), this.css.jq.play.length && this.css.jq.pause.length && (t ? (this.css.jq.play.hide(), this.css.jq.pause.show()) : (this.css.jq.play.show(), this.css.jq.pause.hide())), this.css.jq.restoreScreen.length && this.css.jq.fullScreen.length && (this.status.noFullWindow ? (this.css.jq.fullScreen.hide(), this.css.jq.restoreScreen.hide()) : this.options.fullWindow ? (this.css.jq.fullScreen.hide(), this.css.jq.restoreScreen.show()) : (this.css.jq.fullScreen.show(), this.css.jq.restoreScreen.hide())), this.css.jq.repeat.length && this.css.jq.repeatOff.length && (this.options.loop ? (this.css.jq.repeat.hide(), this.css.jq.repeatOff.show()) : (this.css.jq.repeat.show(), this.css.jq.repeatOff.hide()))
        },
        _updateInterface: function() {
            this.css.jq.seekBar.length && this.css.jq.seekBar.width(this.status.seekPercent + "%"), this.css.jq.playBar.length && (this.options.smoothPlayBar ? this.css.jq.playBar.stop().animate({
                width: this.status.currentPercentAbsolute + "%"
            }, 250, "linear") : this.css.jq.playBar.width(this.status.currentPercentRelative + "%"));
            var t = "";
            this.css.jq.currentTime.length && (t = this._convertTime(this.status.currentTime), t !== this.css.jq.currentTime.text() && this.css.jq.currentTime.text(this._convertTime(this.status.currentTime)));
            var e = "",
                i = this.status.duration,
                s = this.status.remaining;
            this.css.jq.duration.length && ("string" == typeof this.status.media.duration ? e = this.status.media.duration : ("number" == typeof this.status.media.duration && (i = this.status.media.duration, s = i - this.status.currentTime), e = this.options.remainingDuration ? (s > 0 ? "-" : "") + this._convertTime(s) : this._convertTime(i)), e !== this.css.jq.duration.text() && this.css.jq.duration.text(e))
        },
        _convertTime: i.prototype.time,
        _seeking: function() {
            this.css.jq.seekBar.length && this.css.jq.seekBar.addClass("mdtc-clnplrv-seeking-bg"), this.addStateClass("seeking")
        },
        _seeked: function() {
            this.css.jq.seekBar.length && this.css.jq.seekBar.removeClass("mdtc-clnplrv-seeking-bg"), this.removeStateClass("seeking")
        },
        _resetGate: function() {
            this.html.audio.gate = !1, this.html.video.gate = !1, this.aurora.gate = !1, this.flash.gate = !1
        },
        _resetActive: function() {
            this.html.active = !1, this.aurora.active = !1, this.flash.active = !1
        },
        _escapeHtml: function(t) {
            return t.split("&").join("&amp;").split("<").join("&lt;").split(">").join("&gt;").split('"').join("&quot;")
        },
        _qualifyURL: function(t) {
            var e = document.createElement("div");
            return e.innerHTML = '<a href="' + this._escapeHtml(t) + '">x</a>', e.firstChild.href
        },
        _absoluteMediaUrls: function(e) {
            var i = this;
            return t.each(e, function(t, s) {
                s && i.format[t] && "data:" !== s.substr(0, 5) && (e[t] = i._qualifyURL(s))
            }), e
        },
        addStateClass: function(t) {
            this.ancestorJq.length && this.ancestorJq.addClass(this.options.stateClass[t])
        },
        removeStateClass: function(t) {
            this.ancestorJq.length && this.ancestorJq.removeClass(this.options.stateClass[t])
        },
        setMedia: function(e) {
            var i = this,
                s = !1,
                a = this.status.media.poster !== e.poster;
            this._resetMedia(), this._resetGate(), this._resetActive(), this.androidFix.setMedia = !1, this.androidFix.play = !1, this.androidFix.pause = !1, e = this._absoluteMediaUrls(e), t.each(this.formats, function(a, o) {
                var n = "video" === i.format[o].media;
                return t.each(i.solutions, function(a, r) {
                    if (i[r].support[o] && i._validString(e[o])) {
                        var l = "html" === r,
                            h = "aurora" === r;
                        return n ? (l ? (i.html.video.gate = !0, i._html_setVideo(e), i.html.active = !0) : (i.flash.gate = !0, i._flash_setVideo(e), i.flash.active = !0), i.css.jq.videoPlay.length && i.css.jq.videoPlay.show(), i.status.video = !0) : (l ? (i.html.audio.gate = !0, i._html_setAudio(e), i.html.active = !0, t.jPlayerVideo.platform.android && (i.androidFix.setMedia = !0)) : h ? (i.aurora.gate = !0, i._aurora_setAudio(e), i.aurora.active = !0) : (i.flash.gate = !0, i._flash_setAudio(e), i.flash.active = !0), i.css.jq.videoPlay.length && i.css.jq.videoPlay.hide(), i.status.video = !1), s = !0, !1
                    }
                }), s ? !1 : void 0
            }), s ? (this.status.nativeVideoControls && this.html.video.gate || this._validString(e.poster) && (a ? this.htmlElement.poster.src = e.poster : this.internal.poster.jq.show()), "string" == typeof e.title && (this.css.jq.title.length && this.css.jq.title.html(e.title), this.htmlElement.audio && this.htmlElement.audio.setAttribute("title", e.title), this.htmlElement.video && this.htmlElement.video.setAttribute("title", e.title)), this.status.srcSet = !0, this.status.media = t.extend({}, e), this._updateButtons(!1), this._updateInterface(), this._trigger(t.jPlayerVideo.event.setmedia)) : this._error({
                type: t.jPlayerVideo.error.NO_SUPPORT,
                context: "{supplied:'" + this.options.supplied + "'}",
                message: t.jPlayerVideo.errorMsg.NO_SUPPORT,
                hint: t.jPlayerVideo.errorHint.NO_SUPPORT
            })
        },
        _resetMedia: function() {
            this._resetStatus(), this._updateButtons(!1), this._updateInterface(), this._seeked(), this.internal.poster.jq.hide(), clearTimeout(this.internal.htmlDlyCmdId), this.html.active ? this._html_resetMedia() : this.aurora.active ? this._aurora_resetMedia() : this.flash.active && this._flash_resetMedia()
        },
        clearMedia: function() {
            this._resetMedia(), this.html.active ? this._html_clearMedia() : this.aurora.active ? this._aurora_clearMedia() : this.flash.active && this._flash_clearMedia(), this._resetGate(), this._resetActive()
        },
        load: function() {
            this.status.srcSet ? this.html.active ? this._html_load() : this.aurora.active ? this._aurora_load() : this.flash.active && this._flash_load() : this._urlNotSetError("load")
        },
        focus: function() {
            this.options.keyEnabled && (t.jPlayerVideo.focus = this)
        },
        play: function(t) {
            var e = "object" == typeof t;
            e && this.options.useStateClassSkin && !this.status.paused ? this.pause(t) : (t = "number" == typeof t ? t : NaN, this.status.srcSet ? (this.focus(), this.html.active ? this._html_play(t) : this.aurora.active ? this._aurora_play(t) : this.flash.active && this._flash_play(t)) : this._urlNotSetError("play"))
        },
        videoPlay: function() {
            this.play()
        },
        pause: function(t) {
            t = "number" == typeof t ? t : NaN, this.status.srcSet ? this.html.active ? this._html_pause(t) : this.aurora.active ? this._aurora_pause(t) : this.flash.active && this._flash_pause(t) : this._urlNotSetError("pause")
        },
        tellOthers: function(e, i) {
            var s = this,
                a = "function" == typeof i,
                o = Array.prototype.slice.call(arguments);
            "string" == typeof e && (a && o.splice(1, 1), t.jPlayerVideo.prototype.destroyRemoved(), t.each(this.instances, function() {
                s.element !== this && (!a || i.call(this.data("jPlayerVideo"), s)) && this.jPlayerVideo.apply(this, o)
            }))
        },
        pauseOthers: function(t) {
            this.tellOthers("pause", function() {
                return this.status.srcSet
            }, t)
        },
        stop: function() {
            this.status.srcSet ? this.html.active ? this._html_pause(0) : this.aurora.active ? this._aurora_pause(0) : this.flash.active && this._flash_pause(0) : this._urlNotSetError("stop")
        },
        playHead: function(t) {
            t = this._limitValue(t, 0, 100), this.status.srcSet ? this.html.active ? this._html_playHead(t) : this.aurora.active ? this._aurora_playHead(t) : this.flash.active && this._flash_playHead(t) : this._urlNotSetError("playHead")
        },
        _muted: function(t) {
            this.mutedWorker(t), this.options.globalVolume && this.tellOthers("mutedWorker", function() {
                return this.options.globalVolume
            }, t)
        },
        mutedWorker: function(e) {
            this.options.muted = e, this.html.used && this._html_setProperty("muted", e), this.aurora.used && this._aurora_mute(e), this.flash.used && this._flash_mute(e), this.html.video.gate || this.html.audio.gate || (this._updateMute(e), this._updateVolume(this.options.volume), this._trigger(t.jPlayerVideo.event.volumechange))
        },
        mute: function(t) {
            var i = "object" == typeof t;
            i && this.options.useStateClassSkin && this.options.muted ? this._muted(!1) : (t = t === e ? !0 : !!t, this._muted(t))
        },
        unmute: function(t) {
            t = t === e ? !0 : !!t, this._muted(!t)
        },
        _updateMute: function(t) {
            t === e && (t = this.options.muted), t ? this.addStateClass("muted") : this.removeStateClass("muted"), this.css.jq.mute.length && this.css.jq.unmute.length && (this.status.noVolume ? (this.css.jq.mute.hide(), this.css.jq.unmute.hide()) : t ? (this.css.jq.mute.hide(), this.css.jq.unmute.show()) : (this.css.jq.mute.show(), this.css.jq.unmute.hide()))
        },
        volume: function(t) {
            this.volumeWorker(t), this.options.globalVolume && this.tellOthers("volumeWorker", function() {
                return this.options.globalVolume
            }, t)
        },
        volumeWorker: function(e) {
            e = this._limitValue(e, 0, 1), this.options.volume = e, this.html.used && this._html_setProperty("volume", e), this.aurora.used && this._aurora_volume(e), this.flash.used && this._flash_volume(e), this.html.video.gate || this.html.audio.gate || (this._updateVolume(e), this._trigger(t.jPlayerVideo.event.volumechange))
        },
        volumeBar: function(e) {
            if (this.css.jq.volumeBar.length) {
                var i = t(e.currentTarget),
                    s = i.offset(),
                    a = e.pageX - s.left,
                    o = i.width(),
                    n = i.height() - e.pageY + s.top,
                    r = i.height();
                this.options.verticalVolume ? this.volume(n / r) : this.volume(a / o)
            }
            this.options.muted && this._muted(!1)
        },
        _updateVolume: function(t) {
            t === e && (t = this.options.volume), t = this.options.muted ? 0 : t, this.status.noVolume ? (this.addStateClass("noVolume"), this.css.jq.volumeBar.length && this.css.jq.volumeBar.hide(), this.css.jq.volumeBarValue.length && this.css.jq.volumeBarValue.hide(), this.css.jq.volumeMax.length && this.css.jq.volumeMax.hide()) : (this.removeStateClass("noVolume"), this.css.jq.volumeBar.length && this.css.jq.volumeBar.show(), this.css.jq.volumeBarValue.length && (this.css.jq.volumeBarValue.show(), this.css.jq.volumeBarValue[this.options.verticalVolume ? "height" : "width"](100 * t + "%")), this.css.jq.volumeMax.length && this.css.jq.volumeMax.show())
        },
        volumeMax: function() {
            this.volume(1), this.options.muted && this._muted(!1)
        },
        _cssSelectorAncestor: function(e) {
            var i = this;
            this.options.cssSelectorAncestor = e, this._removeUiClass(), this.ancestorJq = e ? t(e) : [], e && 1 !== this.ancestorJq.length && this._warning({
                type: t.jPlayerVideo.warning.CSS_SELECTOR_COUNT,
                context: e,
                message: t.jPlayerVideo.warningMsg.CSS_SELECTOR_COUNT + this.ancestorJq.length + " found for cssSelectorAncestor.",
                hint: t.jPlayerVideo.warningHint.CSS_SELECTOR_COUNT
            }), this._addUiClass(), t.each(this.options.cssSelector, function(t, e) {
                i._cssSelector(t, e)
            }), this._updateInterface(), this._updateButtons(), this._updateAutohide(), this._updateVolume(), this._updateMute()
        },
        _cssSelector: function(e, i) {
            var s = this;
            if ("string" == typeof i)
                if (t.jPlayerVideo.prototype.options.cssSelector[e]) {
                    if (this.css.jq[e] && this.css.jq[e].length && this.css.jq[e].unbind(".jPlayerVideo"), this.options.cssSelector[e] = i, this.css.cs[e] = this.options.cssSelectorAncestor + " " + i, i ? this.css.jq[e] = t(this.css.cs[e]) : this.css.jq[e] = [], this.css.jq[e].length && this[e]) {
                        var a = function(i) {
                            i.preventDefault(), s[e](i), s.options.autoBlur ? t(this).blur() : t(this).focus()
                        };
                        this.css.jq[e].bind("click.jPlayerVideo", a)
                    }
                    i && 1 !== this.css.jq[e].length && this._warning({
                        type: t.jPlayerVideo.warning.CSS_SELECTOR_COUNT,
                        context: this.css.cs[e],
                        message: t.jPlayerVideo.warningMsg.CSS_SELECTOR_COUNT + this.css.jq[e].length + " found for " + e + " method.",
                        hint: t.jPlayerVideo.warningHint.CSS_SELECTOR_COUNT
                    })
                } else
                    this._warning({
                        type: t.jPlayerVideo.warning.CSS_SELECTOR_METHOD,
                        context: e,
                        message: t.jPlayerVideo.warningMsg.CSS_SELECTOR_METHOD,
                        hint: t.jPlayerVideo.warningHint.CSS_SELECTOR_METHOD
                    });
            else
                this._warning({
                    type: t.jPlayerVideo.warning.CSS_SELECTOR_STRING,
                    context: i,
                    message: t.jPlayerVideo.warningMsg.CSS_SELECTOR_STRING,
                    hint: t.jPlayerVideo.warningHint.CSS_SELECTOR_STRING
                })
        },
        duration: function(t) {
            this.options.toggleDuration && (this.options.captureDuration && t.stopPropagation(), this._setOption("remainingDuration", !this.options.remainingDuration))
        },
        seekBar: function(e) {
            if (this.css.jq.seekBar.length) {
                var i = t(e.currentTarget),
                    s = i.offset(),
                    a = e.pageX - s.left,
                    o = i.width(),
                    n = 100 * a / o;
                this.playHead(n)
            }
        },
        playbackRate: function(t) {
            this._setOption("playbackRate", t)
        },
        playbackRateBar: function(e) {
            if (this.css.jq.playbackRateBar.length) {
                var i,
                    s,
                    a = t(e.currentTarget),
                    o = a.offset(),
                    n = e.pageX - o.left,
                    r = a.width(),
                    l = a.height() - e.pageY + o.top,
                    h = a.height();
                i = this.options.verticalPlaybackRate ? l / h : n / r, s = i * (this.options.maxPlaybackRate - this.options.minPlaybackRate) + this.options.minPlaybackRate, this.playbackRate(s)
            }
        },
        _updatePlaybackRate: function() {
            var t = this.options.playbackRate,
                e = (t - this.options.minPlaybackRate) / (this.options.maxPlaybackRate - this.options.minPlaybackRate);
            this.status.playbackRateEnabled ? (this.css.jq.playbackRateBar.length && this.css.jq.playbackRateBar.show(), this.css.jq.playbackRateBarValue.length && (this.css.jq.playbackRateBarValue.show(), this.css.jq.playbackRateBarValue[this.options.verticalPlaybackRate ? "height" : "width"](100 * e + "%"))) : (this.css.jq.playbackRateBar.length && this.css.jq.playbackRateBar.hide(), this.css.jq.playbackRateBarValue.length && this.css.jq.playbackRateBarValue.hide())
        },
        repeat: function(t) {
            var e = "object" == typeof t;
            e && this.options.useStateClassSkin && this.options.loop ? this._loop(!1) : this._loop(!0)
        },
        repeatOff: function() {
            this._loop(!1)
        },
        _loop: function(e) {
            this.options.loop !== e && (this.options.loop = e, this._updateButtons(), this._trigger(t.jPlayerVideo.event.repeat))
        },
        option: function(i, s) {
            var a = i;
            if (0 === arguments.length)
                return t.extend(!0, {}, this.options);
            if ("string" == typeof i) {
                var o = i.split(".");
                if (s === e) {
                    for (var n = t.extend(!0, {}, this.options), r = 0; r < o.length; r++) {
                        if (n[o[r]] === e)
                            return this._warning({
                                type: t.jPlayerVideo.warning.OPTION_KEY,
                                context: i,
                                message: t.jPlayerVideo.warningMsg.OPTION_KEY,
                                hint: t.jPlayerVideo.warningHint.OPTION_KEY
                            }), e;
                        n = n[o[r]]
                    }
                    return n
                }
                a = {};
                for (var l = a, h = 0; h < o.length; h++)
                    h < o.length - 1 ? (l[o[h]] = {}, l = l[o[h]]) : l[o[h]] = s
            }
            return this._setOptions(a), this
        },
        _setOptions: function(e) {
            var i = this;
            return t.each(e, function(t, e) {
                i._setOption(t, e)
            }), this
        },
        _setOption: function(e, i) {
            var s = this;
            switch (e) {
            case "volume":
                this.volume(i);
                break;
            case "muted":
                this._muted(i);
                break;
            case "globalVolume":
                this.options[e] = i;
                break;
            case "cssSelectorAncestor":
                this._cssSelectorAncestor(i);
                break;
            case "cssSelector":
                t.each(i, function(t, e) {
                    s._cssSelector(t, e)
                });
                break;
            case "playbackRate":
                this.options[e] = i = this._limitValue(i, this.options.minPlaybackRate, this.options.maxPlaybackRate), this.html.used && this._html_setProperty("playbackRate", i), this._updatePlaybackRate();
                break;
            case "defaultPlaybackRate":
                this.options[e] = i = this._limitValue(i, this.options.minPlaybackRate, this.options.maxPlaybackRate), this.html.used && this._html_setProperty("defaultPlaybackRate", i), this._updatePlaybackRate();
                break;
            case "minPlaybackRate":
                this.options[e] = i = this._limitValue(i, .1, this.options.maxPlaybackRate - .1), this._updatePlaybackRate();
                break;
            case "maxPlaybackRate":
                this.options[e] = i = this._limitValue(i, this.options.minPlaybackRate + .1, 16), this._updatePlaybackRate();
                break;
            case "fullScreen":
                if (this.options[e] !== i) {
                    var a = t.jPlayerVideo.nativeFeatures.fullscreen.used.webkitVideo;
                    (!a || a && !this.status.waitForPlay) && (a || (this.options[e] = i), i ? this._requestFullscreen() : this._exitFullscreen(), a || this._setOption("fullWindow", i))
                }
                break;
            case "fullWindow":
                this.options[e] !== i && (this._removeUiClass(), this.options[e] = i, this._refreshSize());
                break;
            case "size":
                this.options.fullWindow || this.options[e].cssClass === i.cssClass || this._removeUiClass(), this.options[e] = t.extend({}, this.options[e], i), this._refreshSize();
                break;
            case "sizeFull":
                this.options.fullWindow && this.options[e].cssClass !== i.cssClass && this._removeUiClass(), this.options[e] = t.extend({}, this.options[e], i), this._refreshSize();
                break;
            case "autohide":
                this.options[e] = t.extend({}, this.options[e], i), this._updateAutohide();
                break;
            case "loop":
                this._loop(i);
                break;
            case "remainingDuration":
                this.options[e] = i, this._updateInterface();
                break;
            case "toggleDuration":
                this.options[e] = i;
                break;
            case "nativeVideoControls":
                this.options[e] = t.extend({}, this.options[e], i), this.status.nativeVideoControls = this._uaBlocklist(this.options.nativeVideoControls), this._restrictNativeVideoControls(), this._updateNativeVideoControls();
                break;
            case "noFullWindow":
                this.options[e] = t.extend({}, this.options[e], i), this.status.nativeVideoControls = this._uaBlocklist(this.options.nativeVideoControls), this.status.noFullWindow = this._uaBlocklist(this.options.noFullWindow), this._restrictNativeVideoControls(), this._updateButtons();
                break;
            case "noVolume":
                this.options[e] = t.extend({}, this.options[e], i), this.status.noVolume = this._uaBlocklist(this.options.noVolume), this._updateVolume(), this._updateMute();
                break;
            case "emulateHtml":
                this.options[e] !== i && (this.options[e] = i, i ? this._emulateHtmlBridge() : this._destroyHtmlBridge());
                break;
            case "timeFormat":
                this.options[e] = t.extend({}, this.options[e], i);
                break;
            case "keyEnabled":
                this.options[e] = i, i || this !== t.jPlayerVideo.focus || (t.jPlayerVideo.focus = null);
                break;
            case "keyBindings":
                this.options[e] = t.extend(!0, {}, this.options[e], i);
                break;
            case "audioFullScreen":
                this.options[e] = i;
                break;
            case "autoBlur":
                this.options[e] = i
            }
            return this
        },
        _refreshSize: function() {
            this._setSize(), this._addUiClass(), this._updateSize(), this._updateButtons(), this._updateAutohide(), this._trigger(t.jPlayerVideo.event.resize)
        },
        _setSize: function() {
            this.options.fullWindow ? (this.status.width = this.options.sizeFull.width, this.status.height = this.options.sizeFull.height, this.status.cssClass = this.options.sizeFull.cssClass) : (this.status.width = this.options.size.width, this.status.height = this.options.size.height, this.status.cssClass = this.options.size.cssClass), this.element.css({
                width: this.status.width,
                height: this.status.height
            })
        },
        _addUiClass: function() {
            this.ancestorJq.length && this.ancestorJq.addClass(this.status.cssClass)
        },
        _removeUiClass: function() {
            this.ancestorJq.length && this.ancestorJq.removeClass(this.status.cssClass)
        },
        _updateSize: function() {
            this.internal.poster.jq.css({
                width: this.status.width,
                height: this.status.height
            }), !this.status.waitForPlay && this.html.active && this.status.video || this.html.video.available && this.html.used && this.status.nativeVideoControls ? this.internal.video.jq.css({
                width: this.status.width,
                height: this.status.height
            }) : !this.status.waitForPlay && this.flash.active && this.status.video && this.internal.flash.jq.css({
                width: this.status.width,
                height: this.status.height
            })
        },
        _updateAutohide: function() {
            var t = this,
                e = "mousemove.jPlayerVideo",
                i = ".jPlayerVideoAutohide",
                s = e + i,
                a = function(e) {
                    var i,
                        s,
                        a = !1;
                    "undefined" != typeof t.internal.mouse ? (i = t.internal.mouse.x - e.pageX, s = t.internal.mouse.y - e.pageY, a = Math.floor(i) > 0 || Math.floor(s) > 0) : a = !0, t.internal.mouse = {
                        x: e.pageX,
                        y: e.pageY
                    }, a && t.css.jq.gui.fadeIn(t.options.autohide.fadeIn, function() {
                        clearTimeout(t.internal.autohideId), t.internal.autohideId = setTimeout(function() {
                            t.css.jq.gui.fadeOut(t.options.autohide.fadeOut)
                        }, t.options.autohide.hold)
                    })
                };
            this.css.jq.gui.length && (this.css.jq.gui.stop(!0, !0), clearTimeout(this.internal.autohideId), delete this.internal.mouse, this.element.unbind(i), this.css.jq.gui.unbind(i), this.status.nativeVideoControls ? this.css.jq.gui.hide() : this.options.fullWindow && this.options.autohide.full || !this.options.fullWindow && this.options.autohide.restored ? (this.element.bind(s, a), this.css.jq.gui.bind(s, a), this.css.jq.gui.hide()) : this.css.jq.gui.show())
        },
        fullScreen: function(t) {
            var e = "object" == typeof t;
            e && this.options.useStateClassSkin && this.options.fullScreen ? this._setOption("fullScreen", !1) : this._setOption("fullScreen", !0)
        },
        restoreScreen: function() {
            this._setOption("fullScreen", !1)
        },
        _fullscreenAddEventListeners: function() {
            var e = this,
                i = t.jPlayerVideo.nativeFeatures.fullscreen;
            i.api.fullscreenEnabled && i.event.fullscreenchange && ("function" != typeof this.internal.fullscreenchangeHandler && (this.internal.fullscreenchangeHandler = function() {
                e._fullscreenchange()
            }), document.addEventListener(i.event.fullscreenchange, this.internal.fullscreenchangeHandler, !1))
        },
        _fullscreenRemoveEventListeners: function() {
            var e = t.jPlayerVideo.nativeFeatures.fullscreen;
            this.internal.fullscreenchangeHandler && document.removeEventListener(e.event.fullscreenchange, this.internal.fullscreenchangeHandler, !1)
        },
        _fullscreenchange: function() {
            this.options.fullScreen && !t.jPlayerVideo.nativeFeatures.fullscreen.api.fullscreenElement() && this._setOption("fullScreen", !1)
        },
        _requestFullscreen: function() {
            var e = this.ancestorJq.length ? this.ancestorJq[0] : this.element[0],
                i = t.jPlayerVideo.nativeFeatures.fullscreen;
            i.used.webkitVideo && (e = this.htmlElement.video), i.api.fullscreenEnabled && i.api.requestFullscreen(e)
        },
        _exitFullscreen: function() {
            var e,
                i = t.jPlayerVideo.nativeFeatures.fullscreen;
            i.used.webkitVideo && (e = this.htmlElement.video), i.api.fullscreenEnabled && i.api.exitFullscreen(e)
        },
        _html_initMedia: function(e) {
            var i = t(this.htmlElement.media).empty();
            t.each(e.track || [], function(t, e) {
                var s = document.createElement("track");
                s.setAttribute("kind", e.kind ? e.kind : ""), s.setAttribute("src", e.src ? e.src : ""), s.setAttribute("srclang", e.srclang ? e.srclang : ""), s.setAttribute("label", e.label ? e.label : ""), e.def && s.setAttribute("default", e.def), i.append(s)
            }), this.htmlElement.media.src = this.status.src, "none" !== this.options.preload && this._html_load(), this._trigger(t.jPlayerVideo.event.timeupdate)
        },
        _html_setFormat: function(e) {
            var i = this;
            t.each(this.formats, function(t, s) {
                return i.html.support[s] && e[s] ? (i.status.src = e[s], i.status.format[s] = !0, i.status.formatType = s, !1) : void 0
            })
        },
        _html_setAudio: function(t) {
            this._html_setFormat(t), this.htmlElement.media = this.htmlElement.audio, this._html_initMedia(t)
        },
        _html_setVideo: function(t) {
            this._html_setFormat(t), this.status.nativeVideoControls && (this.htmlElement.video.poster = this._validString(t.poster) ? t.poster : ""), this.htmlElement.media = this.htmlElement.video, this._html_initMedia(t)
        },
        _html_resetMedia: function() {
            this.htmlElement.media && (this.htmlElement.media.id !== this.internal.video.id || this.status.nativeVideoControls || this.internal.video.jq.css({
                width: "0px",
                height: "0px"
            }), this.htmlElement.media.pause())
        },
        _html_clearMedia: function() {
            this.htmlElement.media && (this.htmlElement.media.src = "about:blank", this.htmlElement.media.load())
        },
        _html_load: function() {
            this.status.waitForLoad && (this.status.waitForLoad = !1, this.htmlElement.media.load()), clearTimeout(this.internal.htmlDlyCmdId)
        },
        _html_play: function(t) {
            var e = this,
                i = this.htmlElement.media;
            if (this.androidFix.pause = !1, this._html_load(), this.androidFix.setMedia)
                this.androidFix.play = !0, this.androidFix.time = t;
            else if (isNaN(t))
                i.play();
            else {
                this.internal.cmdsIgnored && i.play();
                try {
                    if (i.seekable && !("object" == typeof i.seekable && i.seekable.length > 0))
                        throw 1;
                    i.currentTime = t, i.play()
                } catch (s) {
                    return void (this.internal.htmlDlyCmdId = setTimeout(function() {
                        e.play(t)
                    }, 250))
                }
            }
            this._html_checkWaitForPlay()
        },
        _html_pause: function(t) {
            var e = this,
                i = this.htmlElement.media;
            if (this.androidFix.play = !1, t > 0 ? this._html_load() : clearTimeout(this.internal.htmlDlyCmdId), i.pause(), this.androidFix.setMedia)
                this.androidFix.pause = !0, this.androidFix.time = t;
            else if (!isNaN(t))
                try {
                    if (i.seekable && !("object" == typeof i.seekable && i.seekable.length > 0))
                        throw 1;
                    i.currentTime = t
                } catch (s) {
                    return void (this.internal.htmlDlyCmdId = setTimeout(function() {
                        e.pause(t)
                    }, 250))
                }
            t > 0 && this._html_checkWaitForPlay()
        },
        _html_playHead: function(t) {
            var e = this,
                i = this.htmlElement.media;
            this._html_load();
            try {
                if ("object" == typeof i.seekable && i.seekable.length > 0)
                    i.currentTime = t * i.seekable.end(i.seekable.length - 1) / 100;
                else {
                    if (!(i.duration > 0) || isNaN(i.duration))
                        throw "e";
                    i.currentTime = t * i.duration / 100
                }
            } catch (s) {
                return void (this.internal.htmlDlyCmdId = setTimeout(function() {
                    e.playHead(t)
                }, 250))
            }
            this.status.waitForLoad || this._html_checkWaitForPlay()
        },
        _html_checkWaitForPlay: function() {
            this.status.waitForPlay && (this.status.waitForPlay = !1, this.css.jq.videoPlay.length && this.css.jq.videoPlay.hide(), this.status.video && (this.internal.poster.jq.hide(), this.internal.video.jq.css({
                width: this.status.width,
                height: this.status.height
            })))
        },
        _html_setProperty: function(t, e) {
            this.html.audio.available && (this.htmlElement.audio[t] = e), this.html.video.available && (this.htmlElement.video[t] = e)
        },
        _aurora_setAudio: function(e) {
            var i = this;
            t.each(this.formats, function(t, s) {
                return i.aurora.support[s] && e[s] ? (i.status.src = e[s], i.status.format[s] = !0, i.status.formatType = s, !1) : void 0
            }), this.aurora.player = new AV.Player.fromURL(this.status.src), this._addAuroraEventListeners(this.aurora.player, this.aurora), "auto" === this.options.preload && (this._aurora_load(), this.status.waitForLoad = !1)
        },
        _aurora_resetMedia: function() {
            this.aurora.player && this.aurora.player.stop()
        },
        _aurora_clearMedia: function() {},
        _aurora_load: function() {
            this.status.waitForLoad && (this.status.waitForLoad = !1, this.aurora.player.preload())
        },
        _aurora_play: function(e) {
            this.status.waitForLoad || isNaN(e) || this.aurora.player.seek(e), this.aurora.player.playing || this.aurora.player.play(), this.status.waitForLoad = !1, this._aurora_checkWaitForPlay(), this._updateButtons(!0), this._trigger(t.jPlayerVideo.event.play)
        },
        _aurora_pause: function(e) {
            isNaN(e) || this.aurora.player.seek(1e3 * e), this.aurora.player.pause(), e > 0 && this._aurora_checkWaitForPlay(), this._updateButtons(!1), this._trigger(t.jPlayerVideo.event.pause)
        },
        _aurora_playHead: function(t) {
            this.aurora.player.duration > 0 && this.aurora.player.seek(t * this.aurora.player.duration / 100), this.status.waitForLoad || this._aurora_checkWaitForPlay()
        },
        _aurora_checkWaitForPlay: function() {
            this.status.waitForPlay && (this.status.waitForPlay = !1)
        },
        _aurora_volume: function(t) {
            this.aurora.player.volume = 100 * t
        },
        _aurora_mute: function(t) {
            t ? (this.aurora.properties.lastvolume = this.aurora.player.volume, this.aurora.player.volume = 0) : this.aurora.player.volume = this.aurora.properties.lastvolume, this.aurora.properties.muted = t
        },
        _flash_setAudio: function(e) {
            var i = this;
            try {
                t.each(this.formats, function(t, s) {
                    if (i.flash.support[s] && e[s]) {
                        switch (s) {
                        case "m4a":
                        case "fla":
                            i._getMovie().fl_setAudio_m4a(e[s]);
                            break;
                        case "mp3":
                            i._getMovie().fl_setAudio_mp3(e[s]);
                            break;
                        case "rtmpa":
                            i._getMovie().fl_setAudio_rtmp(e[s])
                        }
                        return i.status.src = e[s], i.status.format[s] = !0, i.status.formatType = s, !1
                    }
                }), "auto" === this.options.preload && (this._flash_load(), this.status.waitForLoad = !1)
            } catch (s) {
                this._flashError(s)
            }
        },
        _flash_setVideo: function(e) {
            var i = this;
            try {
                t.each(this.formats, function(t, s) {
                    if (i.flash.support[s] && e[s]) {
                        switch (s) {
                        case "m4v":
                        case "flv":
                            i._getMovie().fl_setVideo_m4v(e[s]);
                            break;
                        case "rtmpv":
                            i._getMovie().fl_setVideo_rtmp(e[s])
                        }
                        return i.status.src = e[s], i.status.format[s] = !0, i.status.formatType = s, !1
                    }
                }), "auto" === this.options.preload && (this._flash_load(), this.status.waitForLoad = !1)
            } catch (s) {
                this._flashError(s)
            }
        },
        _flash_resetMedia: function() {
            this.internal.flash.jq.css({
                width: "0px",
                height: "0px"
            }), this._flash_pause(NaN)
        },
        _flash_clearMedia: function() {
            try {
                this._getMovie().fl_clearMedia()
            } catch (t) {
                this._flashError(t)
            }
        },
        _flash_load: function() {
            try {
                this._getMovie().fl_load()
            } catch (t) {
                this._flashError(t)
            }
            this.status.waitForLoad = !1
        },
        _flash_play: function(t) {
            try {
                this._getMovie().fl_play(t)
            } catch (e) {
                this._flashError(e)
            }
            this.status.waitForLoad = !1, this._flash_checkWaitForPlay()
        },
        _flash_pause: function(t) {
            try {
                this._getMovie().fl_pause(t)
            } catch (e) {
                this._flashError(e)
            }
            t > 0 && (this.status.waitForLoad = !1, this._flash_checkWaitForPlay())
        },
        _flash_playHead: function(t) {
            try {
                this._getMovie().fl_play_head(t)
            } catch (e) {
                this._flashError(e)
            }
            this.status.waitForLoad || this._flash_checkWaitForPlay()
        },
        _flash_checkWaitForPlay: function() {
            this.status.waitForPlay && (this.status.waitForPlay = !1, this.css.jq.videoPlay.length && this.css.jq.videoPlay.hide(), this.status.video && (this.internal.poster.jq.hide(), this.internal.flash.jq.css({
                width: this.status.width,
                height: this.status.height
            })))
        },
        _flash_volume: function(t) {
            try {
                this._getMovie().fl_volume(t)
            } catch (e) {
                this._flashError(e)
            }
        },
        _flash_mute: function(t) {
            try {
                this._getMovie().fl_mute(t)
            } catch (e) {
                this._flashError(e)
            }
        },
        _getMovie: function() {
            return document[this.internal.flash.id]
        },
        _getFlashPluginVersion: function() {
            var t,
                e = 0;
            if (window.ActiveXObject)
                try {
                    if (t = new ActiveXObject("ShockwaveFlash.ShockwaveFlash")) {
                        var i = t.GetVariable("$version");
                        i && (i = i.split(" ")[1].split(","), e = parseInt(i[0], 10) + "." + parseInt(i[1], 10))
                    }
                } catch (s) {}
            else
                navigator.plugins && navigator.mimeTypes.length > 0 && (t = navigator.plugins["Shockwave Flash"], t && (e = navigator.plugins["Shockwave Flash"].description.replace(/.*\s(\d+\.\d+).*/, "$1")));
            return 1 * e
        },
        _checkForFlash: function(t) {
            var e = !1;
            return this._getFlashPluginVersion() >= t && (e = !0), e
        },
        _validString: function(t) {
            return t && "string" == typeof t
        },
        _limitValue: function(t, e, i) {
            return e > t ? e : t > i ? i : t
        },
        _urlNotSetError: function(e) {
            this._error({
                type: t.jPlayerVideo.error.URL_NOT_SET,
                context: e,
                message: t.jPlayerVideo.errorMsg.URL_NOT_SET,
                hint: t.jPlayerVideo.errorHint.URL_NOT_SET
            })
        },
        _flashError: function(e) {
            var i;
            i = this.internal.ready ? "FLASH_DISABLED" : "FLASH", this._error({
                type: t.jPlayerVideo.error[i],
                context: this.internal.flash.swf,
                message: t.jPlayerVideo.errorMsg[i] + e.message,
                hint: t.jPlayerVideo.errorHint[i]
            }), this.internal.flash.jq.css({
                width: "1px",
                height: "1px"
            })
        },
        _error: function(e) {
            this._trigger(t.jPlayerVideo.event.error, e), this.options.errorAlerts && this._alert("Error!" + (e.message ? "\n" + e.message : "") + (e.hint ? "\n" + e.hint : "") + "\nContext: " + e.context)
        },
        _warning: function(i) {
            this._trigger(t.jPlayerVideo.event.warning, e, i), this.options.warningAlerts && this._alert("Warning!" + (i.message ? "\n" + i.message : "") + (i.hint ? "\n" + i.hint : "") + "\nContext: " + i.context)
        },
        _alert: function(t) {
            var e = "jPlayerVideo " + this.version.script + " : id='" + this.internal.self.id + "' : " + t;
            this.options.consoleAlerts ? window.console && window.console.log && window.console.log(e) : alert(e)
        },
        _emulateHtmlBridge: function() {
            var e = this;
            t.each(t.jPlayerVideo.emulateMethods.split(/\s+/g), function(t, i) {
                e.internal.domNode[i] = function(t) {
                    e[i](t)
                }
            }), t.each(t.jPlayerVideo.event, function(i, s) {
                var a = !0;
                t.each(t.jPlayerVideo.reservedEvent.split(/\s+/g), function(t, e) {
                    return e === i ? (a = !1, !1) : void 0
                }), a && e.element.bind(s + ".jPlayerVideo.jPlayerVideoHtml", function() {
                    e._emulateHtmlUpdate();
                    var t = document.createEvent("Event");
                    t.initEvent(i, !1, !0), e.internal.domNode.dispatchEvent(t)
                })
            })
        },
        _emulateHtmlUpdate: function() {
            var e = this;
            t.each(t.jPlayerVideo.emulateStatus.split(/\s+/g), function(t, i) {
                e.internal.domNode[i] = e.status[i]
            }), t.each(t.jPlayerVideo.emulateOptions.split(/\s+/g), function(t, i) {
                e.internal.domNode[i] = e.options[i]
            })
        },
        _destroyHtmlBridge: function() {
            var e = this;
            this.element.unbind(".jPlayerVideoHtml");
            var i = t.jPlayerVideo.emulateMethods + " " + t.jPlayerVideo.emulateStatus + " " + t.jPlayerVideo.emulateOptions;
            t.each(i.split(/\s+/g), function(t, i) {
                delete e.internal.domNode[i]
            })
        }
    }, t.jPlayerVideo.error = {
        FLASH: "e_flash",
        FLASH_DISABLED: "e_flash_disabled",
        NO_SOLUTION: "e_no_solution",
        NO_SUPPORT: "e_no_support",
        URL: "e_url",
        URL_NOT_SET: "e_url_not_set",
        VERSION: "e_version"
    }, t.jPlayerVideo.errorMsg = {
        FLASH: "jPlayerVideo's Flash fallback is not configured correctly, or a command was issued before the jPlayerVideo Ready event. Details: ",
        FLASH_DISABLED: "jPlayerVideo's Flash fallback has been disabled by the browser due to the CSS rules you have used. Details: ",
        NO_SOLUTION: "No solution can be found by jPlayerVideo in this browser. Neither HTML nor Flash can be used.",
        NO_SUPPORT: "It is not possible to play any media format provided in setMedia() on this browser using your current options.",
        URL: "Media URL could not be loaded.",
        URL_NOT_SET: "Attempt to issue media playback commands, while no media url is set.",
        VERSION: "jPlayerVideo " + t.jPlayerVideo.prototype.version.script + " needs Jplayer.swf version " + t.jPlayerVideo.prototype.version.needFlash + " but found "
    }, t.jPlayerVideo.errorHint = {
        FLASH: "Check your swfPath option and that Jplayer.swf is there.",
        FLASH_DISABLED: "Check that you have not display:none; the jPlayerVideo entity or any ancestor.",
        NO_SOLUTION: "Review the jPlayerVideo options: support and supplied.",
        NO_SUPPORT: "Video or audio formats defined in the supplied option are missing.",
        URL: "Check media URL is valid.",
        URL_NOT_SET: "Use setMedia() to set the media URL.",
        VERSION: "Update jPlayerVideo files."
    }, t.jPlayerVideo.warning = {
        CSS_SELECTOR_COUNT: "e_css_selector_count",
        CSS_SELECTOR_METHOD: "e_css_selector_method",
        CSS_SELECTOR_STRING: "e_css_selector_string",
        OPTION_KEY: "e_option_key"
    }, t.jPlayerVideo.warningMsg = {
        CSS_SELECTOR_COUNT: "The number of css selectors found did not equal one: ",
        CSS_SELECTOR_METHOD: "The methodName given in jPlayerVideo('cssSelector') is not a valid jPlayerVideo method.",
        CSS_SELECTOR_STRING: "The methodCssSelector given in jPlayerVideo('cssSelector') is not a String or is empty.",
        OPTION_KEY: "The option requested in jPlayerVideo('option') is undefined."
    }, t.jPlayerVideo.warningHint = {
        CSS_SELECTOR_COUNT: "Check your css selector and the ancestor.",
        CSS_SELECTOR_METHOD: "Check your method name.",
        CSS_SELECTOR_STRING: "Check your css selector is a string.",
        OPTION_KEY: "Check your option name."
    }
}), function(t, e) {
    jPlayerVideoPlaylist = function(e, i, s) {
        var a = this;
        this.current = 0, this.loop = !1, this.shuffled = !1, this.removing = !1, this.cssSelector = t.extend({}, this._cssSelector, e), this.options = t.extend(!0, {
            keyBindings: {
                next: {
                    key: 221,
                    fn: function() {
                        a.next()
                    }
                },
                previous: {
                    key: 219,
                    fn: function() {
                        a.previous()
                    }
                },
                shuffle: {
                    key: 83,
                    fn: function() {
                        a.shuffle()
                    }
                }
            },
            stateClass: {
                shuffled: "mdtc-clnplrv-state-shuffled"
            }
        }, this._options, s), this.playlist = [], this.original = [], this._initPlaylist(i), this.cssSelector.details = this.cssSelector.cssSelectorAncestor + " .mdtc-clnplrv-details", this.cssSelector.playlist = this.cssSelector.cssSelectorAncestor + " .mdtc-clnplrv-playlist", this.cssSelector.next = this.cssSelector.cssSelectorAncestor + " .mdtc-clnplrv-next", this.cssSelector.previous = this.cssSelector.cssSelectorAncestor + " .mdtc-clnplrv-previous", this.cssSelector.shuffle = this.cssSelector.cssSelectorAncestor + " .mdtc-clnplrv-shuffle", this.cssSelector.shuffleOff = this.cssSelector.cssSelectorAncestor + " .mdtc-clnplrv-shuffle-off", this.options.cssSelectorAncestor = this.cssSelector.cssSelectorAncestor, this.options.repeat = function(t) {
            a.loop = t.jPlayerVideo.options.loop
        }, t(this.cssSelector.jPlayerVideo).bind(t.jPlayerVideo.event.ready, function() {
            a._init()
        }), t(this.cssSelector.jPlayerVideo).bind(t.jPlayerVideo.event.ended, function() {
            a.next()
        }), t(this.cssSelector.jPlayerVideo).bind(t.jPlayerVideo.event.play, function() {
            t(this).jPlayerVideo("pauseOthers")
        }), t(this.cssSelector.jPlayerVideo).bind(t.jPlayerVideo.event.resize, function(e) {
            e.jPlayerVideo.options.fullScreen ? t(a.cssSelector.details).show() : t(a.cssSelector.details).hide();
        }), t(this.cssSelector.previous).click(function(t) {
            t.preventDefault(), a.previous(), a.blur(this)
        }), t(this.cssSelector.next).click(function(t) {
            t.preventDefault(), a.next(), a.blur(this)
        }), t(this.cssSelector.shuffle).click(function(e) {
            e.preventDefault(), a.shuffled && t(a.cssSelector.jPlayerVideo).jPlayerVideo("option", "useStateClassSkin") ? a.shuffle(!1) : a.shuffle(!0), a.blur(this)
        }), t(this.cssSelector.shuffleOff).click(function(t) {
            t.preventDefault(), a.shuffle(!1), a.blur(this)
        }).hide(), this.options.fullScreen || t(this.cssSelector.details).hide(), t(this.cssSelector.playlist + " ul").empty(), this._createItemHandlers(), t(this.cssSelector.jPlayerVideo).jPlayerVideo(this.options)
    }, jPlayerVideoPlaylist.prototype = {
        _cssSelector: {
            jPlayerVideo: "#jquery_jplayer_1",
            cssSelectorAncestor: "#jp_video_container_1"
        },
        _options: {
            playlistOptions: {
                autoPlay: !1,
                loopOnPrevious: !1,
                shuffleOnLoop: !0,
                enableRemoveControls: !1,
                displayTime: "slow",
                addTime: "fast",
                removeTime: "fast",
                shuffleTime: "slow",
                itemClass: "mdtc-clnplrv-playlist-item",
                freeGroupClass: "mdtc-clnplrv-free-media",
                freeItemClass: "mdtc-clnplrv-playlist-item-free",
                removeItemClass: "mdtc-clnplrv-playlist-item-remove"
            }
        },
        option: function(t, i) {
            if (i === e)
                return this.options.playlistOptions[t];
            switch (this.options.playlistOptions[t] = i, t) {
            case "enableRemoveControls":
                this._updateControls();
                break;
            case "itemClass":
            case "freeGroupClass":
            case "freeItemClass":
            case "removeItemClass":
                this._refresh(!0), this._createItemHandlers()
            }
            return this
        },
        _init: function() {
            var t = this;
            this._refresh(function() {
                t.options.playlistOptions.autoPlay ? t.play(t.current) : t.select(t.current)
            })
        },
        _initPlaylist: function(e) {
            this.current = 0, this.shuffled = !1, this.removing = !1, this.original = t.extend(!0, [], e), this._originalPlaylist()
        },
        _originalPlaylist: function() {
            var e = this;
            this.playlist = [], t.each(this.original, function(t) {
                e.playlist[t] = e.original[t]
            })
        },
        _refresh: function(e) {
            var i = this;
            if (e && !t.isFunction(e))
                t(this.cssSelector.playlist + " ul").empty(), t.each(this.playlist, function(e) {
                    t(i.cssSelector.playlist + " ul").append(i._createListItem(i.playlist[e]))
                }), this._updateControls();
            else {
                var s = t(this.cssSelector.playlist + " ul").children().length ? this.options.playlistOptions.displayTime : 0;
                t(this.cssSelector.playlist + " ul").slideUp(s, function() {
                    var s = t(this);
                    t(this).empty(), t.each(i.playlist, function(t) {
                        s.append(i._createListItem(i.playlist[t]))
                    }), i._updateControls(), t.isFunction(e) && e(), i.playlist.length ? t(this).slideDown(i.options.playlistOptions.displayTime) : t(this).show()
                })
            }
        },
        _createListItem: function(e) {
            var i = this,
                s = "<li><div>";
            if (s += "<a href='javascript:;' class='" + this.options.playlistOptions.removeItemClass + "'>&times;</a>", e.free) {
                var a = !0;
                s += "<span class='" + this.options.playlistOptions.freeGroupClass + "'>(", t.each(e, function(e, o) {
                    t.jPlayerVideo.prototype.format[e] && (a ? a = !1 : s += " | ", s += "<a class='" + i.options.playlistOptions.freeItemClass + "' href='" + o + "' tabindex='-1'>" + e + "</a>")
                }), s += ")</span>"
            }
            return s += "<span href='javascript:;' class='" + this.options.playlistOptions.itemClass + "' tabindex='0'>" + e.title + (e.artist ? " <span class='mdtc-clnplrv-artist'>by " + e.artist + "</span>" : "") + "</span>", s += "</div></li>"
        },
        _createItemHandlers: function() {
            var e = this;
            t(this.cssSelector.playlist).off("click", "span." + this.options.playlistOptions.itemClass).on("click", "span." + this.options.playlistOptions.itemClass, function(i) {
                i.preventDefault();
                var s = t(this).parent().parent().index();
                e.current !== s ? e.play(s) : t(e.cssSelector.jPlayerVideo).jPlayerVideo("play"), e.blur(this)
            }), t(this.cssSelector.playlist).off("click", "a." + this.options.playlistOptions.freeItemClass).on("click", "a." + this.options.playlistOptions.freeItemClass, function(i) {
                i.preventDefault(), t(this).parent().parent().find("." + e.options.playlistOptions.itemClass).click(), e.blur(this)
            }), t(this.cssSelector.playlist).off("click", "a." + this.options.playlistOptions.removeItemClass).on("click", "a." + this.options.playlistOptions.removeItemClass, function(i) {
                i.preventDefault();
                var s = t(this).parent().parent().index();
                e.remove(s), e.blur(this)
            })
        },
        _updateControls: function() {
            this.options.playlistOptions.enableRemoveControls ? t(this.cssSelector.playlist + " ." + this.options.playlistOptions.removeItemClass).show() : t(this.cssSelector.playlist + " ." + this.options.playlistOptions.removeItemClass).hide(), this.shuffled ? t(this.cssSelector.jPlayerVideo).jPlayerVideo("addStateClass", "shuffled") : t(this.cssSelector.jPlayerVideo).jPlayerVideo("removeStateClass", "shuffled"), t(this.cssSelector.shuffle).length && t(this.cssSelector.shuffleOff).length && (this.shuffled ? (t(this.cssSelector.shuffleOff).show(), t(this.cssSelector.shuffle).hide()) : (t(this.cssSelector.shuffleOff).hide(), t(this.cssSelector.shuffle).show()))
        },
        _highlight: function(i) {
            this.playlist.length && i !== e && (t(this.cssSelector.playlist + " .mdtc-clnplrv-playlist-current").removeClass("mdtc-clnplrv-playlist-current"), t(this.cssSelector.playlist + " li:nth-child(" + (i + 1) + ")").addClass("mdtc-clnplrv-playlist-current").find(".mdtc-clnplrv-playlist-item").addClass("mdtc-clnplrv-playlist-current"))
        },
        setPlaylist: function(t) {
            this._initPlaylist(t), this._init()
        },
        add: function(e, i) {
            t(this.cssSelector.playlist + " ul").append(this._createListItem(e)).find("li:last-child").hide().slideDown(this.options.playlistOptions.addTime), this._updateControls(), this.original.push(e), this.playlist.push(e), i ? this.play(this.playlist.length - 1) : 1 === this.original.length && this.select(0)
        },
        remove: function(i) {
            var s = this;
            return i === e ? (this._initPlaylist([]), this._refresh(function() {
                t(s.cssSelector.jPlayerVideo).jPlayerVideo("clearMedia")
            }), !0) : this.removing ? !1 : (i = 0 > i ? s.original.length + i : i, i >= 0 && i < this.playlist.length && (this.removing = !0, t(this.cssSelector.playlist + " li:nth-child(" + (i + 1) + ")").slideUp(this.options.playlistOptions.removeTime, function() {
                if (t(this).remove(), s.shuffled) {
                    var e = s.playlist[i];
                    t.each(s.original, function(t) {
                        return s.original[t] === e ? (s.original.splice(t, 1), !1) : void 0
                    }), s.playlist.splice(i, 1)
                } else
                    s.original.splice(i, 1), s.playlist.splice(i, 1);
                s.original.length ? i === s.current ? (s.current = i < s.original.length ? s.current : s.original.length - 1, s.select(s.current)) : i < s.current && s.current-- : (t(s.cssSelector.jPlayerVideo).jPlayerVideo("clearMedia"), s.current = 0, s.shuffled = !1, s._updateControls()), s.removing = !1
            })), !0)
        },
        select: function(e) {
            e = 0 > e ? this.original.length + e : e, e >= 0 && e < this.playlist.length ? (this.current = e, this._highlight(e), t(this.cssSelector.jPlayerVideo).jPlayerVideo("setMedia", this.playlist[this.current])) : this.current = 0
        },
        play: function(i) {
            i = 0 > i ? this.original.length + i : i, i >= 0 && i < this.playlist.length ? this.playlist.length && (this.select(i), t(this.cssSelector.jPlayerVideo).jPlayerVideo("play")) : i === e && t(this.cssSelector.jPlayerVideo).jPlayerVideo("play")
        },
        pause: function() {
            t(this.cssSelector.jPlayerVideo).jPlayerVideo("pause")
        },
        next: function() {
            var t = this.current + 1 < this.playlist.length ? this.current + 1 : 0;
            this.loop ? 0 === t && this.shuffled && this.options.playlistOptions.shuffleOnLoop && this.playlist.length > 1 ? this.shuffle(!0, !0) : this.play(t) : t > 0 && this.play(t)
        },
        previous: function() {
            var t = this.current - 1 >= 0 ? this.current - 1 : this.playlist.length - 1;
            (this.loop && this.options.playlistOptions.loopOnPrevious || t < this.playlist.length - 1) && this.play(t)
        },
        shuffle: function(i, s) {
            var a = this;
            i === e && (i = !this.shuffled), (i || i !== this.shuffled) && t(this.cssSelector.playlist + " ul").slideUp(this.options.playlistOptions.shuffleTime, function() {
                a.shuffled = i, i ? a.playlist.sort(function() {
                    return .5 - Math.random()
                }) : a._originalPlaylist(), a._refresh(!0), s || !t(a.cssSelector.jPlayerVideo).data("jPlayerVideo").status.paused ? a.play(0) : a.select(0), t(this).slideDown(a.options.playlistOptions.shuffleTime)
            })
        },
        blur: function(e) {
            t(this.cssSelector.jPlayerVideo).jPlayerVideo("option", "autoBlur") && t(e).blur()
        }
    }
}(jQuery), function(t) {
    function e(t) {
        function e() {
            var e = t.next(),
                i = e.find(".mdtc-clnplrv-song-title-inner"),
                s = e.find(".mdtc-clnplrv-song-title").width(),
                a = i.width(),
                o = a - s;
            o > 0 && (i.animate({
                marginLeft: "-=" + (o - 10)
            }, 2500), setTimeout(function() {
                i.animate({
                    marginLeft: "+=" + (o - 10)
                }, 2500)
            }, 4e3))
        }
        e(), s = setInterval(function() {
            e()
        }, 8e3)
    }
    function i() {
        for (var t = "", e = "abcdefghijklmnopqrstuvwxyz0123456789", i = 0; 15 > i; i++)
            t += e.charAt(Math.floor(Math.random() * e.length));
        return t
    }
    var s,
        a = '<div class="mdtc-clnplrv-video mdtc-clnplrv-main-player"><div class="mdtc-clnplrv-jplayer mdtc-clnplrv-video-container"><!-- --></div><div class="mdtc-clnplrv-type-playlist"><div class="mdtc-clnplrv-gui mdtc-clnplrv-interface"><ul class="mdtc-clnplrv-controls"><li><a href="javascript:;" class="mdtc-clnplrv-previous mdtc-clnplrv-icon-prev" tabindex="1"></a></li><li><a href="javascript:;" class="mdtc-clnplrv-play mdtc-clnplrv-icon-play" tabindex="1"></a></li><li><a href="javascript:;" class="mdtc-clnplrv-pause mdtc-clnplrv-icon-pause" tabindex="1" style="display:none"></a></li><li><a href="javascript:;" class="mdtc-clnplrv-next mdtc-clnplrv-icon-next" tabindex="1"></a></li></ul><div class="mdtc-clnplrv-seek-container"><div class="mdtc-clnplrv-seek-container-inner"><div class="mdtc-clnplrv-time"><div class="mdtc-clnplrv-current-time">00:00</div><div class="mdtc-clnplrv-duration" style="display:none">00:00</div></div><div class="mdtc-clnplrv-song-title"><div class="mdtc-clnplrv-song-title-inner"><strong id="current-song"><!-- --></strong></div></div><div class="mdtc-clnplrv-progress"><div class="mdtc-clnplrv-seek-bar" style="width:0%"><div class="mdtc-clnplrv-play-bar" style="width:0%"><!-- --></div></div></div></div></div><div class="mdtc-clnplrv-volume-bar"><div class="mdtc-clnplrv-volume-bar-value"><i><!-- --></i></div></div><ul class="mdtc-clnplrv-toggles"><li><a title="Repeat" tabindex="1" class="mdtc-clnplrv-repeat mdtc-clnplrv-icon-repeat" href="javascript:;"></a></li><li><a title="Repeat off" tabindex="1" class="mdtc-clnplrv-repeat-off mdtc-clnplrv-icon-repeat" href="javascript:;" style="display:none"></a></li><li><a href="javascript:;" class="mdtc-clnplrv-shuffle mdtc-clnplrv-icon-shuffle" tabindex="1" title="Shuffle"></a></li><li><a href="javascript:;" class="mdtc-clnplrv-shuffle-off mdtc-clnplrv-icon-shuffle" tabindex="1" title="Shuffle off" style="display:none"></a></li><li class="mdtc-clnplrv-fullscreen-li"><a href="javascript:;" class="mdtc-clnplrv-fullscreen-btn mdtc-clnplrv-icon-fullscreen" tabindex="1" title="Toggle fullscreen"></a></li><li><a href="javascript:;" class="mdtc-clnplrv-download-one mdtc-clnplrv-icon-download" tabindex="1" title="Download" style="display:none"></a></li></ul></div><div class="clearfix"><!-- --></div><div class="mdtc-clnplrv-playlist"><ul></ul></div><div class="mdtc-clnplrv-no-solution" style="display:none"><span>Update Required </span>To play the media you will need to either update your browser to a recent version</div></div></div>';
    t.fn.cleanvideoplayer = function(s) {
        return function(e, i, s, a) {
            t("body").on("mousemove", ".mdtc-clnplrv-volume-bar", function(e) {
                i && i[s](t.extend({}, e, {
                    currentTarget: a
                }))
            }).on("mouseup", function() {
                i = !1
            }).on("mousedown", function(e) {
                var o = t(e.target).parents().add(e.currentTarget),
                    n = o.filter(".mdtc-clnplrv-volume-bar").first(),
                    r = o.filter(".mdtc-clnplrv-seek-bar").first();
                r.length ? (s = "seekBar", a = r) : n.length && (s = "volumeBar", a = n);
                var l = a && a.closest(".mdtc-clnplrv-video");
                i = l && l.children().first().add(l.prev()).filter(function() {
                    return t(this).data("jPlayerVideo")
                }).first().data("jPlayerVideo")
            })
        }(), this.each(function(o) {
            var n = t.extend({
                    theme: "default",
                    supplied: "m4v,webmv,youtube",
                    disableControls: !1,
                    autoplay: !1,
                    playlistHeight: !1,
                    playlist: !1
                }, s),
                r = !1,
                l = "";
            if (t(this).find("> ul").length > 0) {
                var h = t(this).find("> ul");
                if (h.data("theme") ? n.theme = h.data("theme") : void 0, h.data("supplied") ? n.supplied = h.data("supplied") : void 0, h.data("disable-controls") ? n.disableControls = h.data("disable-controls") : void 0, h.data("autoplay") ? n.autoplay = h.data("autoplay") : void 0, h.data("playlist-height") ? n.playlistHeight = h.data("playlist-height") : void 0, t(this).find("> ul li").length > 0) {
                    var d = [];
                    t(this).find("> ul li").each(function() {
                        var e = t(this),
                            i = e.data("free") ? !0 : !1,
                            s = e.data("type"),
                            a = {
                                title: e.data("title"),
                                artist: e.data("artist"),
                                free: i,
                                poster: e.data("poster")
                            };
                        a[s] = e.data("url"), d.push(a)
                    }), 1 == t(this).find("> ul li").length && t(this).find("> ul li").data("free") && (r = !0, l = t(this).find("> ul li").data("url")), n.playlist = d
                }
            }
            var c = i();
            if (t(this).html(a), t(this).find(".mdtc-clnplrv-main-player").addClass("mdtc-clnplrv-cleanvideoplayer-id-" + c), r && (t(this).find(".mdtc-clnplrv-main-player").addClass("mdtc-clnplrv-solo-download"), t(this).find(".mdtc-clnplrv-download-one").attr("href", l)), "dark" == n.theme && t(this).find(".mdtc-clnplrv-main-player").addClass("mdtc-clnplrv-dark-theme"), n.disableControls && t(this).find(".mdtc-clnplrv-main-player").addClass("mdtc-clnplrv-disabled-controls"), n.playlistHeight && t(this).find(".mdtc-clnplrv-playlist > ul").css({
                maxHeight: n.playlistHeight,
                overflowY: "auto"
            }), n.playlist.length > 1) {
                playerOptions = {
                    solution: "html",
                    supplied: n.supplied,
                    wmode: "window",
                    ready: function(i) {
                        function s() {
                            if (a.next().is(":visible")) {
                                var t = a.width(),
                                    e = t / 16 * 9;
                                a.css("cssText", "height:" + e + "px !important"), 450 > t ? a.parent().addClass("mdtc-clnplrv-responsive") : a.parent().removeClass("mdtc-clnplrv-responsive")
                            }
                        }
                        var a = t(this);
                        a.next().find(".mdtc-clnplrv-time").on("click", function() {
                            a.next().find(".mdtc-clnplrv-current-time, .mdtc-clnplrv-duration").toggle()
                        }), n.autoplay && a.jPlayerVideo("play"), a.on("click", function() {
                            a.data().jPlayerVideo.status.paused ? a.jPlayerVideo("play") : a.jPlayerVideo("pause")
                        }), a.on("dblclick", function() {
                            a.parent().hasClass("mdtc-clnplrv-state-full-screen") ? a.jPlayerVideo("restoreScreen") : a.jPlayerVideo("fullScreen")
                        }), a.next().find(".mdtc-clnplrv-fullscreen-btn").on("click", function() {
                            a.parent().hasClass("mdtc-clnplrv-state-full-screen") ? a.jPlayerVideo("restoreScreen") : a.jPlayerVideo("fullScreen")
                        }), t(window).on("resize", function() {
                            s()
                        }), s(), setInterval(function() {
                            s()
                        }, 250), e(a)
                    },
                    play: function(e) {
                        t(this).jPlayerVideo("pauseOthers")
                    },
                    cssSelector: {
                        title: "#current-song"
                    },
                    playlistOptions: {
                        shuffleTime: 150
                    },
                    size: {
                        width: "100%",
                        height: "100%"
                    }
                };
                var u = n.playlist,
                    p = {
                        jPlayerVideo: ".mdtc-clnplrv-cleanvideoplayer-id-" + c + " .mdtc-clnplrv-jplayer",
                        cssSelectorAncestor: ".mdtc-clnplrv-cleanvideoplayer-id-" + c
                    };
                new jPlayerVideoPlaylist(p, u, playerOptions)
            } else
                t(".mdtc-clnplrv-cleanvideoplayer-id-" + c + " .mdtc-clnplrv-jplayer").jPlayerVideo({
                    ready: function(i) {
                        function s() {
                            if (a.next().is(":visible")) {
                                var t = a.width(),
                                    e = t / 16 * 9;
                                a.css("cssText", "height:" + e + "px !important"), 450 > t ? a.parent().addClass("mdtc-clnplrv-responsive") : a.parent().removeClass("mdtc-clnplrv-responsive")
                            }
                        }
                        t(this).jPlayerVideo("setMedia", n.playlist[0]);
                        var a = t(this);
                        a.next().find(".mdtc-clnplrv-time").on("click", function() {
                            a.next().find(".mdtc-clnplrv-current-time, .mdtc-clnplrv-duration").toggle()
                        }), n.autoplay && t(this).jPlayerVideo("play"), a.on("click", function() {
                            a.data().jPlayerVideo.status.paused ? a.jPlayerVideo("play") : a.jPlayerVideo("pause")
                        }), a.on("dblclick", function() {
                            a.parent().hasClass("mdtc-clnplrv-state-full-screen") ? a.jPlayerVideo("restoreScreen") : a.jPlayerVideo("fullScreen")
                        }), a.next().find(".mdtc-clnplrv-fullscreen-btn").on("click", function() {
                            a.parent().hasClass("mdtc-clnplrv-state-full-screen") ? a.jPlayerVideo("restoreScreen") : a.jPlayerVideo("fullScreen")
                        }), t(this).parent().addClass("mdtc-clnplrv-disabled-controls mdtc-clnplrv-solo-player"), t(window).on("resize", function() {
                            s()
                        }), s(), setInterval(function() {
                            s()
                        }, 250), e(a)
                    },
                    play: function(e) {
                        t(this).jPlayerVideo("pauseOthers")
                    },
                    cssSelector: {
                        title: "#current-song"
                    },
                    cssSelectorAncestor: ".mdtc-clnplrv-cleanvideoplayer-id-" + c,
                    supplied: n.supplied,
                    size: {
                        width: "100%",
                        height: "100%"
                    }
                })
        })
    };
    var o = {
        api: "https://www.youtube.com/iframe_api",
        load: function(t) {
            var e,
                i,
                s = "script",
                a = document.getElementsByTagName(s)[0],
                n = [];
            return function(r) {
                return i ? n ? n.push(r) : r() : t.YT && t.YT.Player ? r() : (t.onYouTubeIframeAPIReady && n.push(t.onYouTubeIframeAPIReady), t.onYouTubeIframeAPIReady = function() {
                    for (; 0 !== n.length;)
                        try {
                            n.shift()()
                        } catch (t) {}
                    n = !1
                }, i = n.push(r), e = document.createElement(s), e.src = o.api, void a.parentNode.insertBefore(e, a))
            }
        }((0, eval)("this")),
        player: function(e, i) {
            var s = t(e),
                a = e.id + "_youtube",
                n = function() {
                    return s.data.apply(s, [a].concat([].slice.call(arguments)))
                },
                r = n() || n({
                    player: !1,
                    queue: !1
                }) && n();
            return r.player ? i(r.player) : r.queue ? r.queue.push(i) : (r.queue = [i], void o.load(function() {
                r.container = e.parentNode.insertBefore(document.createElement("div"), e), r.container.id = a, r.player = new YT.Player(a, {
                    height: s.outerHeight(),
                    width: s.outerWidth(),
                    playerVars: {
                        autoplay: 0,
                        controls: 0,
                        rel: 0,
                        showinfo: 1
                    },
                    events: {
                        onReady: function() {
                            for (; 0 !== r.queue.length;)
                                try {
                                    r.queue.shift()(r.player)
                                } catch (e) {}
                            delete r.queue, t(window).on("resize", function() {
                                r.player.setSize(s.outerWidth(), s.outerHeight())
                            })
                        },
                        onStateChange: function(t) {
                            var i,
                                a = ["unstarted", "ended", "playing", "paused", "buffering", "video cued"][t.data + 1],
                                o = e.getJPlayer(),
                                n = function(t) {
                                    o._getHtmlStatus(i, t), o._updateInterface(), t && n()
                                };
                            clearInterval(r.interval), "ended" === a ? e.emulateEvent("ended") : "paused" === a ? e.emulateEvent("pause") : "playing" === a && (e.emulateEvent("play"), e.emulateEvent("playing"), i = {
                                currentTime: r.player.getCurrentTime(),
                                duration: r.player.getDuration(),
                                videoWidth: s.outerWidth(),
                                videoHeight: s.outerHeight(),
                                ended: !1,
                                readyState: 4,
                                load: function() {},
                                seekable: {
                                    length: 1,
                                    end: function() {
                                        return i.duration
                                    }
                                }
                            }, n(!0), r.interval = setInterval(function() {
                                i.currentTime = r.player.getCurrentTime(), i.ended = i.currentTime >= i.duration, n()
                            }, 1e3), r._html_playHead || (r._html_playHead = o._html_playHead), o._html_playHead = function() {
                                var t,
                                    e = this.htmlElement.media,
                                    s = r.player.getCurrentTime();
                                return e.getVideoId() ? (i.currentTime = s, this.htmlElement.media = i, t = r._html_playHead.apply(this, arguments), this.htmlElement.media = e, i.currentTime !== s && (r.player.seekTo(i.currentTime), n())) : t = r._html_playHead.apply(this, arguments), t
                            })
                        }
                    }
                })
            }))
        },
        hooks: {
            video: {
                getVideoId: function() {
                    return function() {
                        var e = t(this);
                        return (String(e.attr("src") || e.data("src")).match(/(?:youtube\.com\/.*?[?&]v=|youtube:\/{2})([^&]+)/i) || [])[1]
                    }
                },
                getJPlayer: function() {
                    return function() {
                        var e,
                            i = this;
                        do e = t.data(i, "jPlayerVideo"), i = i.parentNode;
                        while (i && !e);
                        return e
                    }
                },
                emulateEvent: function() {
                    return function(t) {
                        if ("function" == typeof this.dispatchEvent) {
                            var e = document.createEvent("HTMLEvents");
                            e.initEvent(t, !0, !0, window, 0, 0, 0, 0, 0, !1, !1, !1, !1, 0, this), this.dispatchEvent(e)
                        } else
                            "undefined" != typeof this.fireEvent && this.fireEvent("on" + t)
                    }
                },
                youtube: function() {
                    return function(t) {
                        return o.player(this, t)
                    }
                },
                toggleVideo: function() {
                    return function(t) {
                        this.getJPlayer().element.toggleClass("youtube-player", t).find('video,img[id*="poster"]').css(t ? {
                            position: "absolute",
                            visibility: "visible",
                            top: "1px"
                        } : {
                            position: "",
                            visibility: "",
                            opacity: "",
                            top: ""
                        }).end().find("iframe").toggle(t)
                    }
                },
                addEventListener: function(e) {
                    return function(i, s, a) {
                        var o = this,
                            n = t(this);
                        return e.call(this, i, function(t) {
                            var e = s.apply(this, arguments);
                            return o.getVideoId() && (n.attr("src") && /(?:abort|emptied|loadstart)/i.test(i) ? n.data("src", o.src).attr("src", "") : /volumechange/i.test(i) ? o.youtube(function(t) {
                                t.setVolume(100 * o.getJPlayer().options.volume)
                            }) : /error/i.test(i) && (o.getJPlayer().status.waitForLoad = !1)), e
                        }, a)
                    }
                },
                canPlayType: function(t) {
                    return function(e) {
                        return /youtube/i.test(e) ? "probably" : t.apply(this, arguments)
                    }
                },
                pause: function(t) {
                    return function() {
                        var e = this;
                        return this.getVideoId() ? this.youtube(function(t) {
                            e.emulateEvent("pause"), t.pauseVideo()
                        }) : t.apply(this, arguments)
                    }
                },
                play: function(e) {
                    return function() {
                        var i = this;
                        return this.getVideoId() ? this.youtube(function(t) {
                            i.toggleVideo(!0), t.playVideo()
                        }) : (t(i).data("src") && this.youtube(function(t) {
                            t.stopVideo()
                        }), this.toggleVideo(!1), e.apply(this, arguments))
                    }
                },
                load: function(t) {
                    return function() {
                        var e = this,
                            i = this.getVideoId();
                        return i ? this.youtube(function(t) {
                            e.toggleVideo(!0), t.cueVideoById(i)
                        }) : (this.toggleVideo(!1), t.apply(this, arguments))
                    }
                }
            }
        },
        initialize: function() {
            var e = function(t, e) {
                var i = o.hooks[e] || {};
                for (var s in i)
                    i.hasOwnProperty(s) && (t[s] = i[s].call(t, t[s]));
                return t
            };
            t.jPlayerVideo.prototype.format.youtube = {
                flashCanPlay: !1,
                codec: 'video/youtube; codecs="m4v"',
                media: "video"
            }, "undefined" != typeof HTMLVideoElement ? e(HTMLVideoElement.prototype, "video") : document.createElement = function(t) {
                return function(i) {
                    return e(t.apply(this, arguments), String(i).toLowerCase())
                }
            }(document.createElement)
        }
    };
    o.initialize()
}(jQuery), $(function() {
    $(".mediatec-cleanvideoplayer").cleanvideoplayer()
});

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
});

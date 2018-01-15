var video_player = document.getElementById("video_player");
video = video_player.getElementsByTagName("video")[0],
video_links = video_player.getElementsByTagName("figcaption")[0],
source = video.getElementsByTagName("source"),
link_list = [],
path = '',
currentVid = 0,
allLnks = video_links.children,
lnkNum = allLnks.length;
video.removeAttribute("controls");
video.removeAttribute("poster");

(function() {
	function playVid(index) {
		video_links.children[index].classList.add("currentvid");
		source[0].src = path + link_list[index] + ".mp4";
		source[1].src = path + link_list[index] + ".webm";
		currentVid = index;
		video.load();
		video.play();
	}
	for (var i=0; i<lnkNum; i++) {
		var filename = allLnks[i].href;
		link_list[i] = filename.match(/([^\/]+)(?=\.\w+$)/)[0];
			(function(index){
				allLnks[i].onclick = function(i){
					i.preventDefault();
						for (var i=0; i<lnkNum; i++) {
							allLnks[i].classList.remove("currentvid");
						}
					playVid(index);
				}
		})(i);
}

video.addEventListener('ended', function () {
	allLnks[currentVid].classList.remove("currentvid");
	if ((currentVid + 1) >= lnkNum) {
		nextVid = 0;
	} else {
		nextVid = currentVid+1;
	}
	playVid(nextVid);
})

video.addEventListener('mouseenter',
	function() {
		video.setAttribute("controls","true");
	})
	video.addEventListener('mouseleave', function() {
		video.removeAttribute("controls");
	})

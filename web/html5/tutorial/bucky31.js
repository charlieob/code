function doFirst() {
    barSize = 600;
    myMovie = document.getElementById('myMovie');
    playButton = document.getElementById('playButton');
    bar = document.getElementById('defaultBar');
    progressBar = document.getElementById('progressBar');
    
    playButton.addEventListener('click', playOrPause, false);
    bar.addEventListener('click', clickedBar, false);
}

function playOrPause() {
    if (!myMovie.paused && !myMovie.ended) {
        myMovie.pause();
        playButton.innerHTML = 'Play';
        window.clearInterval(updateBar);
    } else {
        myMovie.play();
        playButton.innerHTML = 'Pause';
        updateBar = setInterval(update, 500);
    }
}

function update() {
    if (!myMovie.ended) {
//        var size = parseInt(myMovie.currentTime * barSize / myMovie.duration);
        var size = Math.round(myMovie.currentTime * barSize / myMovie.duration);
        progressBar.style.width = size + 'px';
    } else {
        progressBar.style.width = '0px';
        playButton.innerHTML = 'Play';
        window.clearInterval(updateBar);
    }
}

/*
http://www.javascriptkit.com/domref/windowmethods.shtml
    For events such as "click" or "mousemove" that populate the Event object with additional info such as where the mouse
    is at the time, addEventListener() passes this object silently to the listener.
*/
function clickedBar(evt) {
    if (!myMovie.paused && !myMovie.ended) {
        var mouseX = evt.pageX - bar.offsetLeft;
        var newTime = mouseX * myMovie.duration / barSize;
        myMovie.currentTime = newTime;
        progressBar.style.width = mouseX + 'px';
    }
}

// window.addEventListener('load', doFirst, false);
window.onload = doFirst;
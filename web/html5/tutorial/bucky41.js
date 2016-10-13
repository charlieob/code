function doFirst() {
    var x = document.getElementById('canvas');
    canvas = x.getContext('2d');
    canvas.font = "bold 22px Tahoma";
    canvas.textAlign = "start";
    
    canvas.save();

    canvas.fillText("let's begin", 10, 30);
    canvas.rotate(1);
    canvas.fillText("after rotate", 60, 10);
    
    canvas.restore();
    canvas.fillText("after restoring", 10, 30);
}
window.addEventListener("load", doFirst, false);
function doFirst() {
    var mypic = document.getElementById("facepic");
    mypic.addEventListener("dragstart", startDrag, false);
    mypic.addEventListener("dragend", endDrag, false);

    var leftbox = document.getElementById("leftbox");
    leftbox.addEventListener("dragenter", dragEnter, false);
    leftbox.addEventListener("dragleave", dragLeave, false);
    leftbox.addEventListener("dragover", function(e){e.preventDefault();}, false);
    leftbox.addEventListener("drop", dropped, false);
    
}
function startDrag(e) {
    //var code = '<img src="http://www.s3group.com/fileadmin/templates/images/s3group_logo.jpg">';
    var code = '<img src="' + e.target.src + '">';
    e.dataTransfer.setData('Text', code);
}
function endDrag(e) {
    pic = e.target;
    pic.style.visibility = "hidden";
}
function dragEnter(e) {
    e.preventDefault();
    e.target.style.background = "SkyBlue";
    e.target.style.border = "3px solid red";
}
function dragLeave(e) {
    e.preventDefault();
    e.target.style.background = "White";
    e.target.style.border = "3px solid blue";
}
function dropped(e) {
    e.preventDefault();
    //leftbox.innerHTML = e.dataTransfer.getData('Text');
    e.target.innerHTML = e.dataTransfer.getData('Text');
}
window.addEventListener("load", doFirst, false);

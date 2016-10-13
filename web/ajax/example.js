function doFirst() {
    var button = document.getElementById("button");
    button.addEventListener("click", loadXMLDoc, false);
}
function loadXMLDoc() {
    var xmlhttp;
    if (window.XMLHttpRequest) {
        // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else {
        // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function() {
        //if (xmlhttp.readyState==4 && xmlhttp.status==200) {
        // Changed the status to 0 because XMLHttpRequest seems to return status=0 when
        // the file is a local file and the hhtp protocol wasn't actually used under the hood.
        // Not sure if this is a bug, but in this case it's only a work-around anyway for the
        // special case where we are running/testing the code locally.
        // See https://bugzilla.mozilla.org/show_bug.cgi?id=331610
        if (xmlhttp.readyState==4 && xmlhttp.status==0) {
            document.getElementById("myDiv").innerHTML = xmlhttp.responseText;
        }
    }
    xmlhttp.open("GET", "ajax_info.txt", true);
    xmlhttp.send();
}
window.addEventListener("load", doFirst, false);

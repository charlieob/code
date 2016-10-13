function doFirst() {
    if (window.XMLHttpRequest) {
        // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp=new XMLHttpRequest();
    }
    else {
        // code for IE6, IE5
        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.open("GET", "note.xml", false);
    xmlhttp.send();
    xmlDoc = xmlhttp.responseXML;

    document.getElementById("to").innerHTML=
    xmlDoc.getElementsByTagName("to")[0].childNodes[0].nodeValue;
    document.getElementById("from").innerHTML=
    xmlDoc.getElementsByTagName("from")[0].childNodes[0].nodeValue;
    document.getElementById("message").innerHTML=
    xmlDoc.getElementsByTagName("body")[0].childNodes[0].nodeValue;
}
window.addEventListener("load", doFirst, false);

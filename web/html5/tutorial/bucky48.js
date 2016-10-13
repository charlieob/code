function display() {
    "use strict";
    var rightbox = document.getElementById("rightbox"), x, a, b;
    rightbox.innerHTML = "";

    for (x = 0; x < sessionStorage.length; x += 1) {
        a = sessionStorage.key(x);
        b = sessionStorage.getItem(a);
        rightbox.innerHTML += a + ": " + b + "<br />";
    }
}
function saveStuff() {
    "use strict";
    var one = document.getElementById("one").value,
        two = document.getElementById("two").value;
    sessionStorage.setItem(one, two);
    display();
    document.getElementById("one").value = "";
    document.getElementById("two").value = "";
}
function doFirst() {
    "use strict";
    var button = document.getElementById("button");
    button.addEventListener("click", saveStuff, false);
    display();
}
window.addEventListener("load", doFirst, false);

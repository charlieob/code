// this version is not from Crockford's book, but from the discussion here:
// http://stackoverflow.com/questions/2947749/line-by-line-explanation-of-javascript-good-parts-example


var add_the_handlers = function(nodes) {
    var i;
    for (i=0; i<nodes.length; i++) {
        nodes[i].onclick = function(x) {
            return function(e) {
                alert(x);
            };
        }(i);
    };
};


var myNodes = [{}, {}, {}];
add_the_handlers(myNodes);

myNodes[0].onclick();
myNodes[1].onclick();
myNodes[2].onclick();

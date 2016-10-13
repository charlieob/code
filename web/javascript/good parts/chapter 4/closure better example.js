var add_the_handlers = function(nodes) {
    var helper = function(i) {
        return function(e) {
            alert(i);
        };
    };
    var i;
    for (i=0; i<nodes.length; i++) {
        nodes[i].onclick = helper(i);
    }
};

var myNodes = [{}, {}, {}];
add_the_handlers(myNodes);

myNodes[0].onclick();
myNodes[1].onclick();
myNodes[2].onclick();

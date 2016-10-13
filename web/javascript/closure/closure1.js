/*
http://en.wikipedia.org/wiki/Closure_(computer_science)
*/
var f, g;
function foo() {
  var x = 0;
  f = function() { return ++x; };
  g = function() { return --x; };
  x = 1;
  alert('inside foo, call to f(): ' + f()); // "2"
}
foo();
alert('call to g(): ' + g()); // "1"
alert('call to f(): ' + f()); // "2"
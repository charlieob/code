/*

This is the same example as implemented in python in
../../../python/samples/closure/closure2.py

This is a true closure using the function as first-class object feature of
Javascript.

*/
function accumulator(n) {
    return function(x) {
        n += x;
        return n;
    }
}
var a = accumulator(4);
var b = accumulator(42);
alert('a(5): ' + a(5));     // 9
alert('a(2): ' + a(2));     // 11
alert('b(7): ' + b(7));     // 49

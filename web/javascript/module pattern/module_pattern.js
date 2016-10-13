// Notes on the "module pattern" syntax

// The following is the form recommended by Crockford.

(function () {
    // ...
}());

// The outer parens are required since the following is a syntax error:

function () {
    // ...
}();

// In fact there are two different syntax problems in the above.

function () {
    // ...
}

// The above is a syntax error because it is a function declaration and the
// syntax expects a function name between 'function' and the parens.

function myFunc() {
    // ...
}();

// The above is also a syntax error, this time because the syntax does not
// expect a function declaration to be followed by an invocation operator ().
// Adding the outer parentheses forces the contents to be an expression, in
// this case an invocation of the immediate/anonymous function.

// In the case where the result of the function is assigned to a var, the
// parens are not syntactically required.

var myObj = (function () {
    // ...
}());

// However, the outer parentheses provide a visual reminder that the right-hand
// side of the assignment is the result of the invocation of the function
// rather than the function object itself.

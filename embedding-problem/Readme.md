# Bug in RequireJS / text! plugin

When using the optimizer on a file containing something like the following (demo.js), I would expect all the files referenced by text! to be inlined, but only the first one (a.html) is actually being inlined.

```require( [      "text!a.html"], function ( a ) {
    require( ["text!b.html"], function ( b, midsection ) {
        require( ["text!c.html", "jquery"], function ( c, $ ) {
            $("#widget" ).html(a+b+c);
        } );
    } );
} );
```

If I would change the code to something like the following, all referenced text is being inlined into the final optimized build file:
```
require( [      "text!a.html", "text!b.html", "text!c.html"], function ( a, b, c ) {
    require( ["jquery"], function ( $ ) {
        $( "#widget" ).html( a + b + c );
    } );
} );
```

Originally posted in the [RequireJS google group][https://groups.google.com/d/msg/requirejs/I7Egrku1w1Y/Co_Qtc1WZKEJ]



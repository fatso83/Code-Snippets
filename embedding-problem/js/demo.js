//see https://github.com/jrburke/r.js/blob/master/build/example.build.js

requirejs.config( {
    baseUrl: ".",
    paths: {
        text: "js/text"
    }
} );

require( [      "text!a.html"], function ( a ) {
    require( ["text!b.html"], function ( b, midsection ) {
        require( ["text!c.html", "jquery"], function ( c, $ ) {
            $("#widget" ).html(a+b+c);
        } );
    } );
} );
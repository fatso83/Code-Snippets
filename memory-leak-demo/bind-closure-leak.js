document.body.innerHTML = '';

function Foo(num) { this.a = [10000]; this.a[0] = num; };

var i=10000, created = [];
while(i--) created[i] = new Foo(i);
var closureMap = [].map.bind(created);

// nothing released!
created=[];

// ... until you remove the reference to the closure (which holds a reference to the array)
closureMap = [];
# General re-usable Web Components

## TableSort

- Progressively enhance a table with sorting.
- Supports special sorting on `numeric` and `datetime` columns.

Forked off [work by Raymond Camden](https://www.raymondcamden.com/2023/03/14/progressively-enhancing-a-table-with-a-web-component), but instead of re-writing the tables (thus losing all styling, event listeners, etc), I simply re-order the existing rows using `Element#append()` and `DocumentFragment`.

## Other ideas one could look into
- Expose hooks (public method) to add support for sorting other column types. As in `sortTable.register('by-text-length', cell => cell.innerText.length)`
- Basic styling (overridable), like arrow-up and arrow-down (see Dave Copeland's CodePen)


## Related learnings for us build-less peeps
- Zach Leatherman's [`table-saw`](https://www.zachleat.com/web/table-saw/) 
- Dave Copeland's version of [progressive sorting](https://naildrivin5.com/blog/2024/09/30/html-web-components-re-use-logic-which-is-what-you-want.html), found through in [this discussion](https://ruby.social/@davetron5000/113221175517790994) has some more focus on a11y .
- Zach Leatherman's technique on [inspecting the module URL for instructions](https://www.zachleat.com/web/nodefine/). Can be used to selectively do side-effects (like registering the custom element)
- Dave Copeland on [reacting to changes](https://naildrivin5.com/blog/2024/10/01/custom-elements-reacting-to-changes.html)


## LocalTime

Just a minimal POC of how to wrap content that displays a machine readable date and display it using the current locale instead. 

A full-fledged implementation with all bells and whistles would be something like [Github's `RelativeTime`](https://github.com/github/relative-time-element) implementation (which weighs in at 20KB minified ...).

### Learnings

Using something like this in a table can have a massive (detrimental) effect on rendering performance. Going from a fixed length ISO8601 datetime string to a multi-token string of varying length that will wrap, etc caused rendering of 2000 rows to skyrocket from 20 ms to 4 seconds! Profile when using - and remember you might regain that performance by using CSS to constrain the cell dimensions :)

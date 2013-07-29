
# Markdown Finite State Machine

This is a little playground for me to play with state machines and simililar concepts (State Pattern).
The functionality they are meant to help implement is that of a extremely simplified
Markdown parser. It needs to do two things
 - recognize # as beginning a headline and convert it to a h1 header
 - recognize a blank line as beginning a new paragraph (a p tag) (that might be prefixed with a header)

The three alternative approaches I have thought of are using

- a big switch statement. the simplest approach
- the abstract state machine (of David Mertz). almost the same, but slightly easier to use
- the State Pattern, as described in "Design Patterns" (1994) by the Gang of Four

This is merely self-study, so a basic string transformation without any html head+body is enough.

## Tests

I am using the feature-rich JavaScript [Mocha|http://visionmedia.github.io/mocha] test framework
for running tests. I will try to reuse the basic parsing tests for the different
implementations by making them conform to the same interface.

I am trying out using BDD style testing here, using the expect.js library.

An example of reusing tests like this can be found [here|http://pivotallabs.com/drying-up-jasmine-specs-with-shared-behavior].

### Running tests

To run the tests you need to have Node installed, as well as the Mocha Node module. Googling will quickly
bring you up to date on how to install them. Then just run

```
mocha
```

And you will probably see good signs of progress :)





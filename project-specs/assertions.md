so i want to write libraries that the full blow test runner will depend on but i want to also do unit tests as i write them. especially for things like an ini parser, etc.

so i think the answer is to have an assertion library, because that's a relatively low dependency library.

- might use macros because it lets me capture value and string representation?
- probably want to have string labels for each assertion
- i think will not exit test immediately if assertion fails
- assertions are interacting with some global data structure so that they can present which tests failed, etc at the end of main function somehow.


tests can at least capture:
- type of assertion
- string label
- value of arguments
- whether it passed or not


probably be some kind of dynamic heap allocated piscina that assertions are globally interacting with

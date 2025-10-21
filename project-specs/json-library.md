json parsing library written in c89

this will probably depend on at least an arena library, a string builder library, some string utilities like serializing/deserializing floats and integers, etc. might also want a data structure like "xar" which is basically a form of dynamically growing array.

i am probably going to have a custom string type like casey muratori would recommend that is a struct with a length and a pointer.

might also need a testing library for unit tests

complications (complications are things that are more complicated than you expect)

- escaping characters for strings
- unicode


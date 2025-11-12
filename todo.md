* Make memory ownership more explicit with chorda. New chordas should either be completely independent with their own memory for datum, or the name of the function that generates them should be explicit that it is generating a slice.
* Chorda functions that are do splitting or trimming or whatever should support UTF8
* Do I need an isalpha and isalphanum function so i don't use c library?
* The library should have functions for converting strings to PascalCase, snake_case, kebab-case, etc.
* I might want some kind of simple regex library idk

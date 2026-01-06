# arbor_formator Interview Brainstorm

Raw interview Q&A for code formatting style preferences (Phase 8: arbor_formator).

---

## Core Philosophy

**Q: What is the primary sensation you're optimizing for - visual rhythm/pattern recognition, information density, or something else entirely?**
A: Visual rhythm, basically its about how i like to do formatting for having things be scannable, so mostly it is allman braces everywhere plus alignment rules for arguments and variable declarations and assignments

**Q: For brace placement, are you in the 'opening brace belongs to the statement that owns it' camp (same line) or the 'braces are their own visual landmark' camp (new line)?**
A: New line (Allman)

**Q: When a line gets too long, what's your instinct for where to break it?**
A: Before operator

**Q: How do you feel about vertical alignment of related code (e.g., aligning = signs across consecutive assignments)?**
A: Love it

---

## Alignment Details

**Q: For vertical alignment, what's your 'alignment group boundary'? When do you stop aligning consecutive lines together?**
A: Same statement type

**Q: When aligning, which elements do you actually want aligned?**
A: so i think what is unusual that i like align right for types. so align types to the the right with a two space column where * at the end of the type name pushes into the first column of the two column boundary. then left aligned identifiers after the right aligned type declarations and then have assignment operators all be aligned vertically after identifiers with statements like that

**Q: For the right-aligned types in struct members, what determines the alignment column - is it the longest type in that group, or do you have a fixed column width you prefer?**
A: Longest type wins

**Q: For assignments like `alveus->offset = ZEPHYRUM;` - do you align the `=` based on the longest LHS in the group, or based on the type alignment from the declarations above?**
A: Longest LHS

**Q: When you have pointer types like `Alveus*` vs non-pointer like `memoriae_index`, how do you handle the 2-space column for the `*`? Is the `*` always in a fixed position?**
A: 2-col boundary - There's a 2-character 'gutter' between type and name where * lives

---

## Function Formatting

**Q: For function declarations/definitions with multiple parameters, how do you prefer to lay them out when they don't fit on one line?**
A: i do one per line and i also move single arguments into their own line. i think piscina.c is the best example of the style. and for functions i have the return type on its own line, followed by a line with function name, a space, then open parenthes. then even if there is one argument put arguments on their own lines below function name line. and put the ) at the end of the last param line. then after the param lines have the allman brace { line. like i said i think you can see all this in piscina.c

**Q: For function calls with multiple arguments, do you have a preference for how to format them?**
A: All on one line if fits

**Q: When splitting function call arguments across lines, how do you indent them?**
A: Align with first arg

---

## Operators & Expressions

**Q: For binary operators like `a + b * c`, do you have spacing preferences around different operator precedences?**
A: Tight for high precedence

**Q: How do you handle the `->` and `.` member access operators?**
A: No spaces

**Q: For ternary expressions `a ? b : c`, any specific preferences?**
A: One line if short

**Q: To clarify: for casts, you want `( Type* ) expr` with spaces INSIDE the parens, correct? Or just `(Type*) expr` with space after?**
A: ( Type* ) expr

---

## Comments

**Q: For comments, do you prefer C89 block comments /* */ or do you use C99 line comments // (which your code may use despite targeting C89)?**
A: C89 only /* */

**Q: For trailing comments on lines, do you have alignment preferences?**
A: Align in groups

**Q: For section header comments (like your /* ======= REGIO ALVEI ======= */ blocks), is there a specific format you want preserved or generated?**
A: i think you can normalize the headers, ie the consecuative "=====" character lines at same width

---

## Control Flow

**Q: For control flow (if/while/for), do you put a space before the opening paren?**
A: Space

**Q: For switch/case statements, how do you indent the case labels relative to the switch?**
A: Case at switch level

**Q: Do you have a preference for how empty blocks are formatted?**
A: { } on one line

---

## Preprocessor

**Q: For preprocessor directives (#define, #include, etc.), do you indent them or keep them at column 0?**
A: Column 0 always

**Q: For multi-line macros with backslash continuation, do you align the backslashes?**
A: Align backslashes

---

## General Formatting

**Q: What's your maximum line length preference?**
A: 80 columns

**Q: For compound literals and initializer lists, when do you break to multiple lines?**
A: Always multi-line

**Q: How much blank space do you like between 'sections' of code within a function?**
A: No rules

**Q: For comma-separated items (enums, initializer lists), do you have a trailing comma preference?**
A: if it is legal to have trailing commas in c89 in these contexts then that's good otherwise no trailing comma is fine

**Q: How do you feel about spaces inside parentheses?**
A: For casts only

**Q: For typedef declarations, how should they be formatted relative to the struct definition?**
A: i think it is nomen structura Name { ... } Name; technically? if that type is valid / necessary for recursive structs maybe should normalize to that? another thing is that i want to be normalizing keywords to the latin versions so if there is an "if" keyword replace it with "si" although maybe have an option to turn that off if necessary

**Q: For return statements, do you want parens around the expression?**
A: how about always parens with spaces. so can be redde ( x );?

**Q: For sizeof, do you prefer parens always or only when needed?**
A: Always parens

**Q: Any preferences for how blank lines before/after functions are handled?**
A: Preserve existing

---

## Declaration Specifics

**Q: For pointer declarations with multiple variables, do you prefer splitting them or keeping them together?**
A: One per line

**Q: For `for` loops, how do you handle the three parts when they get long?**
A: Depends on length

**Q: For array declarations like `i32 arr[10]`, where does the size go if splitting across lines?**
A: Size with name

**Q: For struct/union member declarations, should the formatter reorder members or preserve original order?**
A: Preserve order

---

## Formatter Behavior

**Q: When the formatter encounters code that's already well-formatted, should it leave it alone or still 'touch' it (normalize)?**
A: whatever is technically most straightforward. it should apply rules but it doesn't have to do anything to code that is not violating rules

**Q: For the formatter's 'fidelis' (roundtrip) mode - should it preserve the original whitespace exactly, or apply minimal normalization?**
A: if it is not applying any formatting rules then it should do exact preservation

**Q: For the formatter API, do you want incremental/node-level formatting, or only whole-file formatting?**
A: Whole file only, Node-level too, Range formatting

---

## Latin Localization

**Q: For keyword latinization, which keywords do you want converted?**
A: i would say all keywords. also want to normalize numbers to roman numerals if less than or equal to 4096 and add roman numeral to latina.h as typedef if it's missing (and keep latina.h roman numeral typedefs sorted ascending). should normalize 0 to ZEPHYRUM;

**Q: For sizeof/magnitudo, should it also have spaces inside like return - `magnitudo ( Type )`?**
A: Yes, spaces inside

**Q: Should the formatter produce output that uses the Latin macros (si, dum, etc.) or expand them to C keywords?**
A: Configurable

**Q: For identifiers and variable names in Latin - does the formatter touch those or leave them alone?**
A: Leave alone

---

## Roman Numerals

**Q: For Roman numerals, should negative numbers be handled? Or only non-negative?**
A: Negate Roman (-V for -5)

**Q: For numbers > 4096, what happens?**
A: Keep as decimal

**Q: Should the formatter also handle hex/octal literals?**
A: Leave as-is

**Q: When adding Roman numeral defines to latina.h, should they be added all at once (all <=4096) or only as needed?**
A: Only as needed

---

## Indentation

**Q: For the right-aligned type declarations, what's the indentation unit - tabs or spaces?**
A: Spaces

**Q: If using spaces, how many spaces per indent level?**
A: 2 spaces

---

## Additional Notes

- Canonical style example: `lib/piscina.c`
- C89 trailing commas: legal in initializer lists, NOT in enum lists (C99 added that)

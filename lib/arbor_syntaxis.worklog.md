# arbor_syntaxis.c worklog

## 2026-01-07: Trivia Preservation for Fidelis Mode

### Problem

Fidelis mode (byte-for-byte roundtrip) was losing whitespace around punctuation tokens. Input `int x = 42;` would output `int x  =42;` (double space before `=`, no space after).

### Root Cause

When tokens like `=`, `{`, `}`, `(`, `)`, `,`, `;` are consumed via `_consumere()` or `_expectare()`, their `trivia_ante` and `trivia_post` are discarded. Only "primary" tokens (identifiers, literals) preserve trivia via `_creare_nodum()` and `_finire_nodum()`.

### Solution Pattern

For each punctuation token consumed, capture and attach trivia:

```c
/* trivia_ante of consumed token -> preceding node's trivia_post */
_copiare_trivia_ad_xar(syn, &prev->trivia_post, token->trivia_ante);

/* token text as synthetic trivia -> preceding node's trivia_post */
_addere_synth_trivia(syn, &prev->trivia_post, ",");

/* trivia_post of consumed token -> following node's trivia_ante */
_copiare_trivia_ad_xar(syn, &next->trivia_ante, token->trivia_post);
```

### Key Rule: When NOT to call `_finire_nodum`

`_finire_nodum` captures `trivia_post` from the previous token (positus - 1). If a child node already captured that trivia, calling `_finire_nodum` on the parent causes **double emission**.

**Do NOT call `_finire_nodum` for nodes whose last child captures final trivia:**

- `_parsere_if` - consequens/alternans have trivia_post from closing `}`
- `_parsere_while` - corpus has trivia_post from closing `}`
- `_parsere_for` - corpus has trivia_post from closing `}`
- `_parsere_binary` - dexter (right operand) has trivia_post
- `_parsere_ternary` - falsum_ramus has trivia_post
- `_parsere_assignment` - valor has trivia_post
- `_parsere_unary` - operandum has trivia_post
- `_parsere_declarator` - children have trivia

Each of these has a comment: `/* NON vocare _finire_nodum - [child] iam habet trivia_post */`

### Formatter Coordination

The formatter must NOT emit hardcoded punctuation when the parser handles it via trivia. For control flow statements (IF, WHILE, FOR), the formatter now only emits keywords and children - punctuation comes from trivia.

### Trivia Ownership Model

For input `) { x`:
- `)` token: trivia_post = " " (space after it)
- `{` token: trivia_ante = "" (empty - space was consumed as `)`'s trivia_post), trivia_post = " "

The lexer assigns trailing same-line whitespace to `trivia_post`. Anything not consumed as trailing becomes `trivia_ante` of the next token. A single space between tokens belongs to exactly one of them, never both.

### Test Coverage

21 roundtrip tests in `probatio_arbor_formator.c`:
- declaration, function, struct, control, expressions, call, subscript
- pointer, ternary, assign, unary, parens, for, while, nested-if
- complex, member, compare, bitwise, multi-decl, func-params

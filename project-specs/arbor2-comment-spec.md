# Arbor2 Comment Handling Specification

## Overview

This document specifies how comments are promoted from token trivia to first-class AST nodes, enabling queries, traversal, and future parsing of comment content (doc comments, etc.).

## Background

Prior to this phase:
- Comments captured as `Arbor2Trivia` on tokens (`trivia_ante`/`trivia_post`)
- `est_commentum` flag distinguishes comments from whitespace
- `est_c99` flag marks `//` vs `/* */` comments
- Roundtrip works via token trivia emission
- No way to query/traverse comments independently

## Comment Attachment Model

Comments are attached to AST nodes based on their position relative to code:

| Situation | Attachment |
|-----------|------------|
| End-of-line: `stmt; /* comment */` | `commenta_post` on statement node |
| Leading (no blank before): `/* comment */ stmt;` | `commenta_ante` on statement node |
| Floating (blank lines around) | Sibling node in parent's list |
| Before `;` or `,`: `expr /* x */ ;` | `commenta_post` on preceding node |
| Otherwise interior: `a + /* x */ b` | `commenta_ante` on following node (b) |
| Multiple sequential comments | All attach to same node (array) |

### End-of-Line Detection

A comment is "end-of-line" if:
1. It appears in `trivia_post` of a token
2. There is no newline between the token and the comment

### Blank Line Detection

A blank line exists when:
1. Two or more consecutive NOVA_LINEA tokens appear, OR
2. Trivia contains `\n\n` or `\r\n\r\n`

### Floating Comment Detection

A comment is "floating" if:
1. It has a blank line BEFORE it (in preceding trivia), AND
2. It has a blank line AFTER it (before next significant token)

## Data Structures

### Comment Subtype Enum

```c
nomen enumeratio {
    ARBOR2_COMMENTUM_CLAUSUM,       /* /* block comment */
    ARBOR2_COMMENTUM_LINEA,         /* // line comment (C99) */
    ARBOR2_COMMENTUM_DOC,           /* /** doc comment */ (future) */
} Arbor2CommentumGenus;
```

### Comment Node Fields

```c
/* COMMENTUM datum entry */
structura {
    Arbor2CommentumGenus    subgenus;       /* Block, line, or doc */
    chorda                  textus;         /* Comment text (without delimiters) */
    chorda                  textus_crudus;  /* Full original text (with /* */ or //) */
    b32                     est_fluitans;   /* True if floating (blank lines around) */
    Xar*                    fragmenta;      /* Parsed sub-structure (NIHIL initially) */
} commentum;
```

### Attached Comments on Nodes

```c
structura Arbor2Nodus {
    /* ... existing fields ... */

    /* Attached comments */
    Xar*    commenta_ante;    /* Xar of Arbor2Nodus* (COMMENTUM nodes) */
    Xar*    commenta_post;    /* Xar of Arbor2Nodus* (COMMENTUM nodes) */

    unio { ... } datum;
};
```

## Attachment Algorithm

### Phase 1: Statement/Declaration Level (This Phase)

During CORPUS and TRANSLATION_UNIT construction:

1. After parsing a statement/declaration, examine the trivia on its tokens
2. For each comment trivia:
   - If end-of-line comment on last token: attach as `commenta_post`
   - If leading comment on first token (no blank before): attach as `commenta_ante`
   - If floating (blank lines both sides): add as sibling in list
3. Store all attached comments in arrays (multiple comments → array)

### Phase 2: Expression Level (Future)

Interior comments (`a + /* x */ b`):
- Attach to the following token's node as `commenta_ante`
- Exception: if following token is `;` or `,`, attach to preceding node

## Text Extraction

### textus_crudus
The raw comment text exactly as it appears in source, including delimiters.

### textus
The comment content with delimiters stripped:
- `/* comment */` → ` comment `
- `// comment` → ` comment`

Leading `/*` and trailing `*/` removed for block comments.
Leading `//` removed for line comments.

## Serialization

When serializing:
1. Emit `commenta_ante` BEFORE the node (each comment's `textus_crudus`)
2. Emit node content as normal
3. Emit `commenta_post` AFTER the node
4. For floating comments in lists: emit as COMMENTUM nodes in list order

The existing token trivia emission remains for backward compatibility during transition.

## Deletion Semantics

- Deleting a node deletes its attached comments (`commenta_ante`/`commenta_post`)
- Floating comments remain unless their container is deleted
- This matches intuition: comments attached to code belong to that code

## Insertion Semantics

- `insertBefore`/`insertAfter` work on list position
- Floating comments stay at their list index
- Attached comments move with their nodes

## Edge Cases

### Multiple Sequential Comments

```c
/* comment 1 */
/* comment 2 */
int x;
```

Both comments attach to `int x` as `commenta_ante`. The array preserves order.

### Comment Between Tokens

```c
int /* inline */ x;
```

Attaches to following token (`x`) as `commenta_ante`.
Exception: `expr /* x */ ;` → attaches to `expr` as `commenta_post`.

### Empty Statement with Comment

```c
; /* comment */
```

Comment attaches to empty statement as `commenta_post`.

### Comment at End of Block

```c
{
    stmt;
    /* trailing comment */
}
```

If no blank line before, attaches to `stmt` as `commenta_post`.
If blank line before `}`, becomes floating and appears in statement list.

## Implementation Scope

### In Scope (Phase 2.4)
- Statement/declaration level comment promotion
- CORPUS and TRANSLATION_UNIT handling
- Attached comments (`commenta_ante`/`commenta_post`)
- Floating comments as list siblings
- Serialization updates

### Deferred
- Expression-level interior comments
- Parsed comment fragments (@param, @return, etc.)
- Doc comment detection (`/** */` → ARBOR2_COMMENTUM_DOC)

## Verification Criteria

1. All existing GLR tests pass (no regressions)
2. End-of-line comments attach correctly
3. Leading comments attach correctly
4. Floating comments appear as siblings
5. Roundtrip serialization preserves all comments
6. Comment text extraction (with/without delimiters) correct

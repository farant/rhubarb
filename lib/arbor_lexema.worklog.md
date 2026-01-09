
---

## 2026-01-08: Null Check Safety for xar_addere

### Problem

`xar_addere()` can return NULL if allocation fails. The lexer was dereferencing the result without null checks in 7 locations, which would cause segfaults if allocation failed.

### Solution

Added null checks after every `xar_addere()` call:

```c
locus = xar_addere(lex->trivia_pendente);
si (locus != NIHIL)
{
    *locus = trivia;
}
```

### Locations Fixed

- Lines 413-417: Whitespace trivia
- Lines 447-451: Block comment trivia  
- Lines 476-480: Line comment trivia
- Lines 535-539: Trailing whitespace trivia
- Lines 569-573: Trailing block comment
- Lines 598-602: Trailing line comment
- Lines 1291-1295: arbor_lexema_omnia() loop

### Test Results

All 71 project tests pass.

---

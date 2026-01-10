# arbor2_glr.c worklog

## 2026-01-09

### Initial implementation complete

Created the basic GLR parser infrastructure:
- `arbor2_glr.h`: Core types (GSS nodes, action/goto tables, AST nodes)
- `arbor2_glr_tabula.c`: Hand-written LR tables for expression grammar
- `arbor2_glr.c`: GLR parser core algorithm
- `probatio_arbor2_glr.c`: Test suite

### Key design decisions

1. **Separated reduction from shift processing**: The original design had reductions adding directly to `frons_activa` during iteration, causing an infinite loop. Fixed by using a separate `reducenda` queue that processes reductions until exhausted, then shifts are processed.

2. **Non-terminal separation**: The grammar needed three distinct NTs for the expression hierarchy:
   - `ARBOR2_NT_EXPRESSIO` (expression level)
   - `ARBOR2_NT_TERMINUS` (term level for `*` precedence)
   - `ARBOR2_NT_FACTOR` (factor level for atoms and unary ops)

3. **Pass-through rule handling**: For 3-symbol pass-through rules like `(expr)`, the middle value (index 1) must be returned, not the rightmost value (index 0).

4. **Accepting node tracking**: The ACCEPT action happens during reduction processing, not in `frons_activa`, so we track `nodus_acceptatus` to return the correct final AST value.

### Bugs fixed

- **Infinite loop**: Reduction adding to `frons_activa` while iterating over it
- **Wrong NT mappings**: Rules P3/P4 (term productions) incorrectly used `ARBOR2_NT_EXPRESSIO` instead of `ARBOR2_NT_TERMINUS`
- **Wrong accepting node**: Returning `frons_activa[0]` instead of the node that reached ACCEPT
- **Parenthesized expressions returning null**: Taking `valori[0]` (`)`) instead of `valori[1]` (the inner expression)

### Tests passing

- Simple identifier: `foo`
- Simple integer: `42`
- Binary addition: `1 + 2`
- Operator precedence: `1 + 2 * 3` parses as `1 + (2 * 3)`
- Parenthesized: `(1 + 2)`
- Unary dereference: `*ptr`
- Unary address-of: `&x`

### Still TODO

- Fork mechanism with piscina checkpointing for true ambiguity handling
- Merge mechanism for compatible states
- Macro lookahead integration with Arbor2Expansion
- Error recovery

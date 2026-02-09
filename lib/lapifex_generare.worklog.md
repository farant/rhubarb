# lapifex_generare.c Worklog

## 2026-02-09 — LALR(1) Collection Builder

### Problem
The canonical LR(1) collection builder (`lapifex_collectio_construere`) causes state space explosion when used with grammars larger than ~20 productions. The C89 expression grammar (95 productions, 63 terminals, 22 non-terminals) caused it to hang indefinitely — the canonical algorithm was creating potentially tens of thousands of states because each unique lookahead creates a separate state.

### Solution
Added `lapifex_collectio_lalr_construere()` which builds an LALR(1) collection in three phases:

1. **LR(0) automaton**: Build item sets ignoring lookaheads entirely. Uses `LapifexRes0` (production + dot only). This produces far fewer states (~155 for the C89 expression grammar vs potentially 10,000+ for canonical LR(1)).

2. **Lookahead propagation**: For each state and each item, compute FIRST(beta a) where beta is the remainder after the dot symbol and a is the item's current lookaheads. Propagate through transitions and closure items in a fixed-point loop.

3. **Conversion**: Expand LR(0) items × lookaheads into standard `LapifexRes` (production + dot + lookahead) for the existing table builder to consume.

### Bug Fixed During Implementation
The initial lookahead seed was incorrectly placed at `pt[0].prospectus[0]` (item index 0 in state 0), but after sorting items by production number, the augmented production `__initium__ -> . virga EOF` (prod=94) ended up as the *last* item (index 70) in state 0. Fixed by searching for the augmented production's item index before seeding.

### Performance
- Expression grammar table construction: ~2 seconds (was infinite with canonical LR(1))
- Table caching (`lapifex_c89_grammatica.c`): Grammar/table built once via static variables, subsequent parses reuse the cached table. 25 tests run in ~2.3 seconds total.

### API
```c
LapifexCollectio* lapifex_collectio_lalr_construere(LapifexGrammatica* grammatica);
```
Same return type as `lapifex_collectio_construere` — drop-in replacement. Existing code using the canonical builder still works for small grammars.

# origo.c — worklog

## 2026-07-10 — natus (instrumenta prima, tool 3 of 5)

Expansion-story CLI: `origo <plagula> <linea>[:<columna>]` — the
question M3's macro-aware stepping asks constantly, promoted into
the phase by sim-2 ep.2 (before this tool the answer was "read the
expander source").

Simpler than emitte: NO tree walk at all — the parsura's expanded
token stream (parsura->lexemata) + each token's inserted origo
chain IS the story. Per final token: `'expansum' <- macro_intimum
<- ... <- macro_extimum`, with arm tags (pasta ##, chorda #, api);
plain tokens report `(fons - sine expansione)`. Tokens grouped by
origin-radix; line-only queries supported BECAUSE machinula halt
reports give via:linea without a column.

**BAR MET (known macro sites report full chains):**
- latina keyword: `'if' <- si`; libc alias: `'malloc' <-
  memoriae_allocare`; `'sizeof' <- magnitudo`.
- NESTED depth-2: `'NULL' <- NIHIL <- CREDO_NON_NIHIL` (credo
  macro body containing latina) — the chain walks both layers.
- Stringification arm: `'"p"' <- CREDO_NON_NIHIL (chorda #)`.
- Expander-synthesized: `'34' <- __LINE__ (chorda #) <-
  CREDO_NON_NIHIL` (the M0b __FILE__/__LINE__ channel, visible).
- Argument tokens attribute to their OWN written position (arg
  `p` at :26, macro at :9) — faithful radix behavior.

**Rough edges (noted at the moment, per debrief practice):**
- Group headers reprint when the stream interleaves
  invocation-radix and argument-radix tokens (radix_prior only
  remembers the previous group). Faithful to stream order but
  noisy on function-macro lines; refine to a seen-set + "(item)"
  continuation if it bites during M3.
- Main-file positions only; querying inside included headers =
  named v1 deferral.
- Same ~1s capita-prep cold start as censor/emitte (known, acta/M4).

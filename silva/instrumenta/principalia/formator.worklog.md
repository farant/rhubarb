# formator worklog

## 2026-08-19 — G1 birth: lint mode, stream tranche (R5/R6/R12/R14/R16)

Contract: project-specs/formator-regulae.md. Split per differre
precedent: machine in instrumenta/silva_formator.{h,c} (linkable into
probationes via the instrumenta glob, portable to silex later), CLI
here. Exit contract matches censor: 0 conformis | 1 divergentiae |
2 usus/illegibilis.

Design decisions:

- **Raw stream, not the parse stream.** `parsura->lexemata` is the
  EXPANDED stream — trivia at macro sites are not source-faithful
  there. `silva_lexare_cruda` gives real tokens + trivia interleaved
  in source order; the whole tranche rides it.
- **Genus semantics = free correctness.** Tabs are their own trivia
  genus (SILVA_LEX_TABULAE), so tabs inside string literals are
  invisible to R5 with zero code. Banner runs are only scanned inside
  COMMENTUM tokens, so `====` in a string literal never fires R14.
  Both negatives are pinned in the probatio.
- **R6 matches valor, not genus, for Latin keywords.** The raw lexer
  only lexes ENGLISH keywords ("verba clausa Anglica solum" —
  latina.h flows through the expander), so `si`/`dum`/`per`/
  `commutatio` arrive as IDENTIFICATOR. English genera (IF/WHILE/
  FOR/SWITCH) are also checked for vendored/generated code. `fac`
  excluded (no paren follows it).
- **R5 is tabs-only in this tranche.** A naive indent%4 check would
  FALSE-FIRE on the two-column alignment rule (R7): aligned
  declaration runs legitimately start at non-multiple-of-4 columns
  (`      memoriae_index  capacitas;`). Depth-correct indent checking
  must be R7-aware → tranche 2 (structural).
- **Comment-interior trailing whitespace is not detected.** It lives
  inside the COMMENTUM token's bytes, which the stream walk doesn't
  open (and which the FIX tier may never touch — Contractus). Known
  gap, recorded, revisit with R14's promotion decision at G3.
- Final-newline checks (exactly one) are raw-byte checks under the
  spatia-caudae rule name.

Found en route: **the whole silva suite had been link-broken since
the 2026-08-18 differre extraction** — the instrumenta/*.c glob
links silva_differre.c into every probatio, but its lib deps
(differentia, sigillum) were never added to compile_probationes.sh's
hand-maintained RADIX_FONTES. First suite run since then was
formator's probatio. Fixed by adding both.

First light (verbatim-verified against the hand archaeology):
piscina.c → 45 divergences: 24 vexillum-59 rows (12 banners × 2
rules — piscina is the 59-width nonconformist), 6 trailing-space
lines (archaeology had found 4 of them by eye), 14 lines > 72, and
1 file-ending blank line (never noticed by hand). Zero false fires:
no tabs, no R6. chorda.c → 132 tab rows (the mid-file tab switch at
chorda_concatenare), 5 trailing, 42 long. Probatio 33/33.

Named doors: tranche 2 = structural rules (R1/R2/R3/R4/R7/R8/R9/
R13 need the tree; selector engine silva_quaestio may carry several
— "definitio-functionis" one selector each). R15 include-order =
lint-forever candidate. Census over lib/ + include/ = the G1
acceptance payoff.

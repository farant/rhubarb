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

## 2026-08-19 — exclusiones + R16 confirmed at 72

Pins file silva/probationes/fixa/formatoris/exclusiones.txt
(examen-pins pattern, via<TAB>causa, named cause per row; leading
"./" on input viae normalized away). Exemption always announced on
stderr — silent gate ≡ dead gate. Born with 20 rows: biblia_dr.c
(data) + arbor/arbor2/lapifex frozen generations (Fran 2026-08-19).
Threshold experiment from census data (no re-run needed — inventum
column carries actual lengths): >72 = 6356, >79 = 2854, >80 = 2556.
60% reduction available at 80, but kept 72: the norm is the target,
not the count, and tranche-2 structural fixes (params one-per-line)
will shrink old long lines anyway. Census with exclusions live:
14286 in 244/291 files (tabs 7772 / long 5225 / banners 1237 /
trailing 51 / keyword 1). Hook note: mid-edit examen flagged
"variabilis inutilis" on the exclusiones declaration — intermediate
state between the declaration edit and the use edits; rebuild under
-Werror (unused = error) is the authority and passed.

## 2026-08-19 — tranche 2a: structural rules (R1/R2/R3/R4/R8)

Tree pass added to the machine: silva_contextus + latinam_addere +
silva_c89_parsare_cum_contextu per lint; parse failure => stream
rules only (fragments stay lintable, lint never crashes). LAW OF
POSITIONS: tree tokens are EXPANDED and carry def-site lexical
fields (a `principale` definition's name token is `main` pointing
into latina.h) — every direct token position check goes through
silva_token_radix; node spans use extensionem_lineis (resolves
internally). Pinned by the principale probatio case. alioquin-si
chains thread the chain-head node (caput) so else-if braces measure
against the chain start, not the inner si column. R8 exempts a sole
bare-void parameter (detected structurally: 1 specifier ==
TYPUS_PRIMITIVUS with 1 VOID token, no declarator). R3 applies to
CORPUS only — struct/union/enum braces stay on the declaration line
by design. Skipped with cause: FAC_DUM corpus, bare nested blocks,
prototypes (R1/R2/R8 on DECLARATIO = tranche 2b).

Doors: per-file "arbor absens" loud marker when parse fails (today
it silently skips structural rules; coverage argument for now =
examen parses this whole corpus daily with pinned exclusions, so
house files parse); prototype checks; R7/R9/R13 alignment tranche.

Census (tree live): 19549 in 259/291 — parametra-singula 2284
(the abandoned fiddly rule), spatium-definitionis 2029 (2000 def /
29 call), bracchia-allman 829, custos-una-linea 114,
typus-in-linea-sua 7 (ALL seven = one cluster of flat one-line
predicates in lib/json.c:1021-1051). Probatio 52/52; suite 40/40;
piscina.c still exactly 45 (zero structural — the exemplar is
conformant under its own executable rules).

## 2026-08-19 — tranche 2b: alignment rules (R7/R9/R13-partial)

R7 columnae-binae in lint form: per run, with cb = one-past-type-end
— common cb across members, gap = 1 + max stars, first star at
exactly cb, names at exactly cb + gap. Runs: struct/union member
lists, leading local-declaration blocks, multi-line param lists
(single-line param form is R8's finding, R7 stays silent). Exempt
per member WITH CAUSE: function-pointer declarators, post-star
qualifiers, multi-declarator lines, multi-line specs. R9: clusters
of simple single-line assignment statements (blank line or any
other statement breaks); `=` at max(lhs_end)+1; single-statement
clusters silent (spacing there = R10's future job). R13 partial:
2-blanks-before-banner / 1-after (banner = comment containing an
=-run >= 10) + 3+ blank collapse; between-functions and
after-locals policy = doors (comment interplay needs design).

TWO CALIBRATION FINDINGS from piscina first light (45 -> 71, all
new fires verified real):
1. **Minimal vs uniform padding**: the piscina->primus stanzas are
   uniformly aligned but ONE column wider than minimal (op at
   max_cb+2); the alveus stanzas use minimal (max_cb+1). Rule as
   coded = minimal. Fran must decree before fix tier.
2. **Cluster aggressiveness**: the rule as decreed aligns ANY
   adjacent simple assignments (`alveus->offset = ZEPHYRUM;` +
   `alveus = alveus->sequens;` must co-align) — the hand never did
   this in loop bodies. May want narrowing (e.g. only stanzas of
   >= 3, or same-LHS-prefix). Census: 4308 fires — review sample
   before -scribere.

**R7's big limitation, named**: the lint parse has NO include
resolution — unknown typedefs (memoriae_index, Alveus, chorda...)
make members ambiguous and runs exempt-skip. piscina.c shows ZERO
columnae-binae despite being the alignment exemplar. Door:
praebere-based header resolution (legati-style prepared contextus)
= the single highest-value lint upgrade. Even blind, corpus-wide
R7 = 3863 fires (known-type runs only).

Census 2b: 28503 total (columnae 3863 / aequatio 4308 / intervalla
783 on top of 2a's numbers). Probatio 67/67.

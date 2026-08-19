# Formator — Regulae Formandi (v1)

Status: DECRETAE 2026-08-19 (Fran + Claude, conversatio directa).
Derived by archaeology from the hand-formatted stratum (lib/piscina.c,
lib/chorda.c early sections) cross-checked against recent practice
(lib/js_lexema.c) and a codebase-wide banner survey. Exemplars below are
verbatim corpus bytes unless marked otherwise.

This document is the contract for the **formator** (autoformatter riding
on silva) and its lint mode. It precedes the RP performance work by
explicit decision (parcum 01M0BGJXEM): the formatter exercises trivia
mutation + emission — the exact surface the performance changes would
touch — before any representation changes land.

---

## Contractus — what the formator NEVER does (v1)

The v1 fix operation is a **pure whitespace transform between an
unchanged token stream**:

- never adds, removes, or reorders a token
- never rewrites a byte inside a comment
- never changes brace structure (braceless stays braceless)
- never introduces or removes line breaks *inside* a statement
  (no rewrapping — the author's break points stand; only their
  indentation is normalized)

Consequence: every output byte is forced by a rule, and the gates below
can be maximally strict.

## Gradus — staging (lint first)

- **G0** — this document (rules with stable names, exemplars, tiers).
- **G1** — formator LINT mode: per-rule detectors over the silva tree,
  positioned self-naming divergences (`via:linea:columna regula
  exspectatum/inventum`), `-machina` TSV twin, house exit codes
  (0 = conformis, 1 = divergentiae, 2 = NIHIL IUDICATUM). Then: the
  corpus census — run over lib/ + include/ and record which rules fire
  where. The census is the acceptance test for the rules themselves.
- **G2** — `-scribere`: the fix operation for FIX-tier rules, gated by
  the full porta stack below.
- **G3+** — named deferrals, each a future explicit decision:
  banner rewriting (requires relaxing the differre gate to allow
  `documentaria`), statement rewrapping, include reordering.

## Portae — verification stack (G2 gates)

1. **Series lexematum** — token stream of output identical to input,
   trivia excluded. The strongest form of "only trivia changed."
2. **Differre** — `./silva/differre.sh vetus novum` reports ONLY
   `MUTATA [cosmetica]` rows. Zero substantiva, zero documentaria,
   zero ADDITA/REMOTA/MOTA. (The dogfood twist: the formatter
   exercises differre's classifier corpus-wide; differre gates the
   formatter.)
3. **Fidelitas** — silva roundtrip on the output: parse(output) then
   emit == output, byte-exact.
4. **Idempotentia** — format(format(x)) == format(x).
5. **Lint nulla** — the linter reports zero divergences on formatter
   output (for FIX-tier rules). The two modes verify each other.
6. **Compilatio** — examen ACCIPE on every formatted file; full test
   suite green from formatted sources.

---

## Regulae

Each rule: stable Latin name (lint output is self-naming), tier
(**FIX** = lint + rewrite in G2; **LINT** = report-only in v1),
statement, exemplar, edges.

### R1 `typus-in-linea-sua` — FIX

Storage class + return type on their own line; function name at column
0; in definitions AND prototypes (headers included).

```c
interior memoriae_index
_proxima_ordinatio (
```

### R2 `spatium-definitionis` — FIX

One space between function name and `(` in definitions and prototypes;
NO space at call sites.

```c
piscina_destruere (        /* definitio */
liberare(alveus);          /* vocatio   */
```

### R3 `bracchia-allman` — FIX

`{` on its own line for function bodies and all control blocks, at the
indent of its opener. (Moving a brace between lines is trivia-only.)

### R4 `custos-una-linea` — FIX

A braceless control body sits on the same line as its condition.

```c
si (!piscina) redde NIHIL;
```

Edge: joining is mechanical (trivia-only). If the joined line exceeds
R16's limit, R16 reports it — the join still happens (R16 is LINT).

### R5 `indentatio-quaterna` — FIX

4 spaces per level. Tabs forbidden everywhere (chorda.c switches to
tabs mid-file at `chorda_concatenare`; piscina.h is tab-indented —
both are drift).

### R6 `spatium-post-claves` — FIX

One space after control keywords before `(`: `si (`, `dum (`, `per (`,
`commutatio (`, and after `fac`.

### R7 `columnae-binae` — FIX

The two-column declaration rule (Fran's formulation, verified to
predict the hand-formatted corpus byte-for-byte). Applies to struct
members, local declaration blocks, and parameter lists.

Within one run: a RIGHT-aligned type column and a LEFT-aligned
identifier column. Gap between them = **1 + the run's maximum star
count** (no pointers in the run: 1 space; any single-star pointer:
2 spaces). Stars sit in the gap immediately after their own type text
(because types are right-aligned, this is the leftmost gap column) —
the star is NOT part of the type column.

```c
nomen structura Alveus {
              vacuum* buffer;
      memoriae_index  capacitas;
      memoriae_index  offset;
    structura Alveus* sequens;
} Alveus;
```

Edges:
- Qualifiers (`constans` etc.) are part of the type text and
  right-align with it: `constans character* piscinae_titulum`.
- `**` generalizes via the gap formula (gap 3, both stars packed after
  the type).
- Array suffixes ride with the identifier in the left column
  (`acervus[64]` is just a wide identifier) — they participate.
- FUNCTION POINTERS are exempt (identifier embedded mid-type breaks
  the model): plain single-space form, and they do not count toward a
  run's column widths.
- Run boundaries: blank line or any non-declaration line ends a run;
  each run aligns independently.

### R8 `parametra-singula` — FIX

Parameters one per line, ALWAYS — including single-parameter functions.
Open paren stays on the name line; closing paren on the last parameter
line. R7 alignment applies across the parameter run.

```c
_allocare_interna (
               Piscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
```

### R9 `aequatio-assignationum` — FIX

`=` alignment per cluster. A cluster is a contiguous run of simple
single-line assignment statements; a blank line or any other
statement breaks the cluster; each cluster aligns independently.
Mechanism: LHS stays LEFT-aligned at the statement indent; padding
goes before the `=`; the operator sits at **max(LHS end) + 2** —
TWO spaces after the longest LHS (`xyz  = valor`, decreed
2026-08-19: the corpus contained both one- and two-space stanzas;
Fran chose two).

```c
    piscina->primus                = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_alvei_initia;
```

Clusters are AGGRESSIVE by decree: any adjacent simple assignments
co-align, including mixed member/local stanzas — odd-looking
results feed the post-lint heuristic refinement phase rather than
pre-emptive exceptions. ONE exception: a member whose aligned form
would push its line past R16's 72 stays silent (and unfixed).
(chorda.c:35-36 right-aligns bare LHS names instead — decreed
drift; statements keep a uniform left edge.)

### R10 `operatores` — FIX

Binary operators spaced; `->` and `.` tight; casts tight
(`(i8*)piscina_allocare(...)`); unary operators tight; comma = no
space before, one space after. (Unary-vs-binary `-`/`*`/`&` is decided
by the parse — silva knows.)

### R11 `continuatio` — FIX

Author line breaks inside statements stand (Contractus); their
continuation lines are re-indented to statement indent + 4, and a
break adjacent to a binary operator places the operator at the START
of the continuation line.

```c
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\f' || c == '\v';
```

### R12 `spatia-caudae` — FIX

No trailing whitespace on any line; file ends with exactly one
newline.

### R13 `intervalla` — FIX

Blank-line policy: 2 blank lines before a banner, 1 after it; 1 blank
line between functions; runs of 3+ blank lines collapse to the local
maximum the policy allows; exactly 1 blank line between a function's
local-declaration block and its first statement (when both exist).

### R14 `vexillum-quinquaginta` — LINT (v1)

Section banners use 50 `=` characters:

```c
/* ==================================================
 * Titulus
 * ================================================== */
```

Survey 2026-08-19: banners appear in 142/154 lib/*.c and 137/157
include/*.h; width 50 dominates (3105 occurrences vs 694 for 72;
piscina.c's 59 is the outlier at 114). LINT tier because fixing
rewrites comment bytes (Contractus); promoting to FIX is the G3
decision that relaxes the differre gate to allow `documentaria`.

### R15 `ordo-inclusionum` — LINT (permanent?)

Include order: `postulata_posix.h` first when present (codex 85), then
own header, then house headers, then system headers. LINT tier
because reordering is token reordering — likely never a formator fix;
fixes stay manual (or a separate consented tool).

### R16 `longitudo-lxxii` — LINT (v1)

Line length limit 72 (DECRETA 2026-08-19; observed practice ranged
64-80, recent files hover 66-72). Report only; the formator never
rewraps (Contractus).

---

## Quaestiones apertae

- Header-specific rules beyond R1/R7/R8 applying to prototypes
  (doc-comment shapes like piscina.h's notatio section are observed
  but not decreed — out of v1 scope).

## Status G1 (2026-08-19)

CONFIRMED home: `silva/formator.sh` → `instrumenta/principalia/
formator.c` (CLI) + `instrumenta/silva_formator.{h,c}` (machine,
probatio-linkable — the differre split). Stream tranche shipped:
R5 (tabs only — depth checking deferred WITH CAUSE: naive %4
false-fires on R7 alignment), R6, R12, R14, R16. Structural
tranche (tree-riding rules) next. Probatio 33/33; negatives pin
the genus semantics (tabs in string literals and `=`-runs in
strings are immune by construction). First light on piscina.c: 45
divergences, superset of the hand archaeology, zero false fires.

## Status G2a (2026-08-19) — the emendatio engine + stream fixes

`-scribere` is live. Architecture: **the detectors are the fixers** —
each lint detector, when it fires, also records the machine-readable
edit that cures it (up to two whitespace spans in ORIGINAL
coordinates + insert text, via `_addere` then `_emendare`). One
analysis pass produces both report and fix; the modes cannot drift.
The applier is deliberately dumb and paranoid: it may only delete
whitespace bytes, only insert spaces/newlines, refuses newline edits
touching preprocessor lines (directive guard incl. `\`-continuations),
drops overlapping spans, and applies descending by offset so all
original coordinates stay valid. `formator_scribere` loops lint→apply
to a fixpoint (max 12): **rules compose through iteration, not
choreography** — G2b's R8 will split params, the next round's R7
aligns them. Gates in the machine: token-series identity vs the
ORIGINAL every round (comments included — bytes must match), fixpoint
required, refusal returns the untouched original with a named
querela. External gates in the probatio: differre classifies fix as
`cosmetica` (the dogfood assert), lexare→emittere roundtrips the
output byte-exact, idempotence.

DECREE G2a: **tabula = IV spatia** (pure-tab indent in the corpus is
one level per tab). Comment-INTERIOR tabs survive (inside COMMENTUM
bytes — Contractus), same class as comment-interior trailing space;
G3's banner decision owns them.

Wired so far (stream tier): R5 tabs, R12 (trailing + final-newline +
trailing-blank-lines), R6, R10-comma, R13-collapse (3+ blanks → 2).
Real-file first light: chorda.c + piscina.c fixed in one pass,
differre = 8 + 7 MUTATA ALL `[cosmetica]`, examen ACCIPE both,
second `-scribere` run a no-op (idempotence on real files). Probatio
117/117; suite 40/40. Unwired fixes (still lint-only): R13 banner
spacing/inter-functions, R1/R2/R3/R4/R8 (G2b), R7/R9/R10-tree/R11
(G2c).

EXCLUSIONES (decretae 2026-08-19): pins file
`silva/probationes/fixa/formatoris/exclusiones.txt`, format
`via<TAB>causa` (the examen-pins pattern — every row carries a
named cause). Exemption is NEVER silent: the CLI announces
`formator: exempta <via> (<causa>)` on stderr. Exempted at birth:
lib/biblia_dr.c (data, not code) + the three frozen predecessor
generations (arbor ×6, arbor2 ×8, lapifex ×5 — deleted after
parity harvest per silva culture). R16 stays 72: raising to 80
would cut fires 60% (6356→2556 measured), but the count is not
the target — the norm is, and recent practice sits under 72.
Census baseline with exclusions live: 14286 divergences in 244 of
291 judged files (tabs 7772 / long 5225 / banners 1237 / trailing
51 / keyword-spacing 1).

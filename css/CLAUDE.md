# css — orientation

*You are reading this because you touched a file in `css/`.*

CSS parser built on **materia** (not silva). Design:
`project-specs/css-arbor-spec.md`; plan for remaining work:
**`css-arbor-plan-B.md` (B1–B10, materia-native, 2026-09-01)** —
the old `css-arbor-plan.md` T10–T17 are superseded (stale types);
its T1–T9 are the historical record.

## Praesens status (2026-08-27)

Built, all **hand-written** per decree `01M134M3` (faber deferred so
it can be generated from what CSS actually needed):

- `css_lexicon` — lexical descriptor · `css_registrum` — node vocabulary
- `css_adaptare` — `CssLexema` → `MateriaToken` **one for one**, plus
  `CssLigator`, the trivia-binding state
- `css_arbor` — the stylesheet **spine** (T9): lex → adapt → bind →
  `plagula` node with `cauda`. Rules are **not** parsed yet

522 assertions across three suites. The **byte-coverage gate runs on
real CSS** (5 files, ~24k bytes, both whitespace regimes), and the
separating oracle (§9.4) emits directly from the parse.

Not built: rules (T10), declarations (T11), at-rules (T12),
selectors (T16), canon (T15), the remaining corpus gates (T13–T14).

**The spine's incompleteness is asserted, not silent.**
`probatio_css_arbor` carries `CREDO_FALSUM(_octetos_probare(…,
"a{}"))` — the gate *must* stay red until T10 lands, and T10 flips
those assertions. Quietly sweeping unparsed tokens into `regula-mala`
would have turned the gate green while nothing was parsed.

## Named slot indices — use them, never bare numbers

`css_registrum.h` carries an enum per genus (`CSS_DECL_VALOR`,
`CSS_SAEPTUM_CONTENTUM`, …). `probatio_css_arbor` checks **all 24**
against the table's titles and asserts the count matches
`numerus_locorum`, so no slot can go unnamed. Without that check the
enums would be a *third* hand-written table free to drift — and the
drift is silent: a value lands in someone else's slot.

## The plan's T8 signature cannot work — read this before T9

T8 asks for `css_adaptare(…, b32 spatia_ut_contentum)`: one bimodal
call that either absorbs trivia or does not, once for the whole
sheet. **That shape cannot implement D7.** Whether a region is a
prelude is a *parse-context* fact; under a global non-verbatim adapt,
the space in `a b { }` is already trivia before the parser knows it
is looking at a prelude, so it can never become a `lexema-servatum`.
Adapting twice would mint **two** tokens for the same bytes and turn
one-owner from a structure into a convention.

So we took the plan's own alternative (T11 step 3, *"or equivalently
do not absorb trivia while inside a value"*):

- `css_adaptare` converts **one for one** and absorbs nothing
- `CssLigator` holds the trivia state, and **the parser owns it** —
  only the parser knows when whitespace is content

One-owner is therefore *built*, not asserted: a byte cannot be
converted twice because it is not converted twice.

## The divisio rule came from silva's code, not the plan's prose

The plan states the rule two ways and they contradict each other —
its prose would put the space in `a { color : red }` on `{`, its test
asserts it lands on `a`. **The test is right.** Measured at
`silva_lexema.c:950-1000`: split at the first newline; up to and
including it goes to the *previous* token as `spatia_post`, the rest
to the *next* as `spatia_ante`; with no newline at all, everything
goes to the previous.

When a plan states a rule twice and the statements disagree, the
original code is the arbiter — not whichever reading you happened to
implement.

Two deliberate divergences, because CSS has no newline token:

1. Silva gives the newline itself to the *prior* (`divisio = j + 1`);
   we give the whole token to the *next* (`divisio = j`). We cannot
   split a token, and the indentation is the half worth keeping with
   the token it indents.
2. **A divisor must carry `munus SPATIUM`.** Without that condition a
   multi-line *comment* becomes a divisor and migrates to the
   following token — which silva does not do, and which reads wrong.

## `initium_lineae` is never set, and the reason generalises

CSS has no `munus LINEA`, so the flag is not something this language
can say. But the sharper point: **even if it were set, it would not
survive the round trip.** `materia_arbor.c:908` writes the position
attributes (`b`, `linea`, `columna`, `linea-initium`) only for
*derived* tokens. CSS has no derivation, so the document never
carries them, and the reader rebuilds the flag from a cursor that
only `munus LINEA` moves.

The divergence would be **byte-invisible** — write→read→write is
identical either way. Only a *tree* comparator would see it. That is
the first concrete case showing why `arbor_aequalitas` is a different
*kind* of evidence rather than the same evidence differently shaped.

## Two CSS facts that shape everything

**`CSS_LEX_SPATIA` absorbs spaces, tabs AND newlines** — one genus,
one run (`lib/css_lexema.c:513`). So it is `VERBATIM`/`SPATIUM`, not
`REPETITUM` like C89's, and **CSS has no `munus LINEA` at all.**

Consequences, all asserted:
- materia's capability mask **refuses** line-sensitive capabilities by
  name. That is the degradation policy working, not a defect. If CSS
  ever wants them, the fix is to split whitespace into genera — the
  refusal says so. **This is now a capability question only, not a
  projection one** (see below).
- `genus_spatii` is `-I` deliberately. The STML template compression
  (`#@ante-spatia n="4"`) requires `REPETITUM`, since the reader
  inverts it by species. materia guards this now; before the guard it
  would have written a count and read back text — a silently broken
  round trip.
- **`VERBATIM` + `SPATIUM` used to be unprojectable, and no longer
  is.** T9 found that *every* CSS file containing whitespace failed
  STML projection with `valor trivii textui non tutus`. C89 never hits
  it: its whitespace genera are `REPETITUM`/`TERMINATOR`, and its only
  `VERBATIM` genera are comments, which carry non-whitespace bytes.
  Fixed in materia — see `materia/CLAUDE.md`, "The whitespace contract".

## The remaining whitespace constraint, named before it bites (T11)

The fix above covers **trivia**. The *lexeme* path
(`materia_arbor.c:945`) raw-marks only when the text is the element's
**sole** child — `elementum mixtum crudum esse NON potest`, which is
true of STML, not an oversight.

So in content mode (D7), a whitespace token that becomes a
`lexema-servatum` **and also carries trivia** (a comment binding to it
as `spatia_post`) cannot be raw, and its value would still be lost.
T11 has to keep content-mode whitespace lexemes text-only, or handle
it in materia. Known in advance rather than after the fact.

## Hand-written tables need drift guards, not faith

`probatio_css_registrum` asserts the lexicon's order against
`CssLexemaGenus` **by title** (a reorder is caught, not absorbed), and
that the registry's `loci_offset` values are **contiguous and exactly
fill** the loci table. Both were verified by planting faults: a
one-slot offset shift and a swapped genus pair each fail.

Offsets are where a hand-written registry actually breaks, and the
break is silent — a node reads someone else's slot layout.

`probatio_css_adaptare` carries the **byte-coverage gate** (spec
§9.7), deliberately built at T8 rather than T10: adaptation and
binding are *where a byte can be lost*, and the parser only uses
them. A defect found here belongs to one module; the same defect
found after the parser sits between two.

Four faults were planted. Three fired at once. **The fourth did
not** — treating the segmented `Xar` as a contiguous array survived
even a 23-element trivia run. Measuring instead of guessing said why:

```
segmenta=3  primum=0x128008240  secundum=0x1280082c0  delta=128   (16 × 8)
```

The second segment lands immediately after the first, so reading
past the boundary lands in the right place *by luck*. The fix was to
the **test**, not the code: a contract test that allocates from the
same arena between accumulations, forcing the segments apart. Now
the fault fires.

A planted fault that does not fire is a measurement of the **test**,
not a verdict on the code. Ask what makes it accidentally right — and
if the answer is luck, the test has to remove the luck.

## Currere

```
./css/compile_probationes.sh          # omnes
./css/compile_probationes.sh adaptare
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/css.log`.
Compiles materia's sources into `css/build/` — css CONSUMES materia,
it does not contain it.

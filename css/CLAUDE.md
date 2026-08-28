# css — orientation

*You are reading this because you touched a file in `css/`.*

CSS parser built on **materia** (not silva). Design:
`project-specs/css-arbor-spec.md`; plan: `css-arbor-plan.md` —
**read BOTH retarget headers at its top before executing any task.**

## Praesens status (2026-08-27)

Built, all **hand-written** per decree `01M134M3` (faber deferred so
it can be generated from what CSS actually needed):

- `css_lexicon` — lexical descriptor · `css_registrum` — node vocabulary
- `css_adaptare` — `CssLexema` → `MateriaToken` **one for one**, plus
  `CssLigator`, the trivia-binding state

402 assertions across two suites. A minimal CSS tree round-trips
through materia's STML, and the **byte-coverage gate already runs on
real CSS** (5 files, ~24k bytes, both whitespace regimes).

Not built: the parser itself (plan T9–T12), selectors (T16), canon
(T15), the remaining corpus gates (T13–T14).

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

Consequences, both asserted:
- materia's capability mask **refuses** line-sensitive capabilities by
  name. That is the degradation policy working, not a defect. If CSS
  ever wants them, the fix is to split whitespace into genera — the
  refusal says so.
- `genus_spatii` is `-I` deliberately. The STML template compression
  (`#@ante-spatia n="4"`) requires `REPETITUM`, since the reader
  inverts it by species. materia guards this now; before the guard it
  would have written a count and read back text — a silently broken
  round trip.

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

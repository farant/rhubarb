# css — orientation

*You are reading this because you touched a file in `css/`.*

CSS parser built on **materia** (not silva). Design:
`project-specs/css-arbor-spec.md`; plan: `css-arbor-plan.md` —
**read BOTH retarget headers at its top before executing any task.**

## Praesens status (2026-08-27)

Built: `css_lexicon` (lexical descriptor) and `css_registrum` (node
vocabulary), both **hand-written** per decree `01M134M3` — faber is
deferred so it can be generated from what CSS actually needed. 189
assertions, and a minimal CSS tree round-trips through materia's STML.

Not built: the parser itself (plan T7–T12), selectors (T16), canon
(T15), the corpus gates (T10–T14).

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

## Currere

```
./css/compile_probationes.sh          # omnes
./css/compile_probationes.sh registrum
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/css.log`.
Compiles materia's sources into `css/build/` — css CONSUMES materia,
it does not contain it.

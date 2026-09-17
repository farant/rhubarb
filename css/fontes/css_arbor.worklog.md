# css_arbor — worklog

## 2026-09-02 — B10: what the plan asked to be written down at the end

Plan B (`project-specs/css-arbor-plan-B.md`) closes with this entry.
The per-task record is `materia/phase-log.md` (B1–B9 RELATIO entries
and the robustness entry between B6 and B7); this is the digest the
plan asked for, in its three questions.

**What the corpus gate found that unit tests missed.** The byte
corpus gate itself (B5) found nothing — the first time in this arc's
history that "corpus > fixtures" came up empty, and not by luck: B2–B4
built the general machine first, and B4 measured four of five
recovery tests green before implementation. The green was
interrogated rather than celebrated (what does the oracle forgive? a
parser that dumps everything into `mala` would still round-trip), so
the gate also asserts structure on the clean files. The finds came
from the gates *around* it: the STML round trip over the corpus (B6)
found three things — the parser set no parent pointers while the
reader sets all of them (fixed in the substrate by exporting
`materia_arbor_patres_figere`), the test's own assumption about where
a trailing space lands was backwards (measurement won), and `\r`
does not survive STML text at all (reader normalizes CRLF even in
raw; fixed generally with the `cr` attribute, and it names a latent
silva bug for the phase-5 replay). The totality gate (random bytes,
mutated and truncated corpus) found the parser was not total —
`a{b:(}` returned NIHIL, end of input consumed once — and the depth
crash shared with silva, pinned red-on-fix. The selector gate's birth
found a test helper handing the parser a stack buffer: the parser
does not copy bytes, so tokens pointed into a dead frame.

**Where the seam differed from prediction.** (1) The plan's
`css_adaptare(…, spatia_ut_contentum)` could not implement D7:
prelude-ness is a parse-context fact, so trivia ownership had to move
into the parser (`CssLigator`), and the adapter became one-for-one.
(2) `MateriaContextus` was never built; the coupling was data-shape
and dissolved into consilium fields plus ten hooks — the origin seam
alone turned out to be two hooks, not one, and the reader's frons two
more. (3) The canon could not be generated (nothing for the
productions closure to close over) and was hand-written under a drift
guard. (4) `silva_generare` requires five structural node genera,
three of which model the C preprocessor; CSS declares them and can
never produce them. (5) The divisio rule was taken from silva's code,
not the plan's prose, because the prose contradicted its own test.
(6) `initium_lineae` is unreachable from CSS and the divergence is
byte-invisible — only the tree comparator sees it.

**Spec §12 predictions, judged.** "About eleven lines of accidental C
coupling" — a FLOOR, not a count: it measured lexical coupling and
never structural (the five required genera). "A new language costs a
lexer, a parser, a registry, and a lexicon" — plus a ligator, plus
four substrate fixes the first client could not reach. "Inheriting …
the canon" — false as built; it was hand-written. "… and a selector
engine" — false as of today: the query families (quaestio,
quaestiones, selecta) are unported, and B9's selector tree is a new
analysis structure, not an inherited engine. Right: the tree model,
the byte-exact emitter, the STML projection and the comparator were
inherited and carried the whole arc.

**B10 itself.** The runners were registered in pythonica's gate table
(`css`, `materia`, `materia-shim`); registering the shim was the point
at which it turned out to have been silently broken since the silva
runner started compiling tests in two steps (`probatio_*.o` in
`silva/build` matched the shim's object glob and doubled `main`).
Fixed by excluding test objects; 348/348 through three oracles, 15 s.

## 2026-09-02 — the bench: a computus for the materia path

Every performance instrument in the repo measured silva's path
(computus with its phase clocks, the byte-identity golden, the
sampler wrapper), so phase 5 — C89 through materia, the acceptance
test — would have had nothing to compare against. `css_computus` is
the twin: one file, a parse arena with a fixed first bucket so the
memory columns compare across files and commits, then emission, the
STML document written and read back, and the tree comparator, each
timed, in a second arena so the parse numbers stay pure. Counts (nodes,
tokens, STML bytes, arena figures, allocations) are pinned by
`probatio_css_computus` over the four committed fixtures; times print
and are never pinned. Birth cost one -Wsign-compare rejection the
examen does not know (unsigned loop index against the signed argument
count — quaestio 01M1GB2MMJ's class again). The runners now record
per-test timings the way root and silva do, so `silva.mensurae('css.')`
works, and `silva.metiri` reads either bench by column title instead of
by position. First reading on speculum.css: parse 0.18 ms, emit 0.05,
STML write 2.3, read 3.2, compare 0.07 — the projection round trip is
thirty times the parse. That is the same shape the silva corpus tests
showed, now measured on the client whose parser has no preprocessor to
blame, which makes it a materia_arbor question rather than a silva one.

## 2026-09-17 — diagnostics declared, and the tree they do not live in

B4 of the materia-sedes plan. Three genera carry `diagnosticum` in
`css.registrum.stml` — `regula-mala`, `declaratio-mala`,
`selector-malus` — and materia's one walker derives located records
from them. css writes no error-reporting code. The seal did not move
(diagnostics hash nothing), so `css.canon` is untouched and all nine
existing gates stayed green through the regeneration.

`probatio_css_diagnostica` is the tenth gate. Its corpus half compares
the derived count against a walk written HERE, in this file, over the
same tree — an independent implementation, so the two cover each other.

**`selector-malus` cannot be reached from the stylesheet tree, and the
plan did not know that.** It is built only in `css_selector.c`, in the
ANALYSIS tree, which shares tokens with the stylesheet tree but never
nodes (`css_selector.h` says so in its header, and B9's design made it
deliberate). So a walk over what `css_arbor_parsare` returns can never
find one. Had the gate shipped as planned, one of the three
declarations would have been completely untested and the plant on it
would have stayed green — the same dead-gate shape B3 step 6 hit in
crusta on the same day, caught here before committing. The gate now
builds selector trees too, via the `_selectorem_parsare` helper the
selector gate already uses, and the plant on `selector-malus` does
fire.

**The plan's third fixture was wrong as well.** `{ }` was expected to
produce a diagnostic and produces none: an empty prelude is valid to
"consume a qualified rule", and it is the SELECTOR grammar that
objects, not the stylesheet grammar. Replaced with the selector
section, where the objection actually lives.

**The corpus half proves agreement but exercises almost nothing.**
Five of the six files yield zero diagnostics, so the assertion is
`0 == 0` five times; only `adversarius_2.css` carries one. The gate's
teeth are the five pinned fixtures. Worth saying plainly, because a
row of green `0 == 0` reads exactly like coverage and is not.

Pinned by eye (Fran, 2026-09-17), byte ends EXCLUSIVE:

| source | records | codex | line:col | bytes |
|---|---|---|---|---|
| `a { color: red; } }` | 1 | regula-mala | 1:19 | 18–19 |
| `a { : x; }` | 1 | declaratio-mala | 1:5 | 4–8 |
| `1` | 1 | selector-malus | 1:1 | 0–1 |
| `a, 1` | 1 | selector-malus | 1:4 | 3–4 |
| `..` | 2 | selector-malus | 1:1, 1:2 | 0–1, 1–2 |

`a, 1` blames the `1` and not the `a`; `..` gives one record per bad
`.`. Selector sources are wrapped as `<selector>{}`, so byte 0 is the
selector's first byte.

Plants: each of the three `diagnosticum` attributes deleted in turn,
regenerated → red each time; restored → green.

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

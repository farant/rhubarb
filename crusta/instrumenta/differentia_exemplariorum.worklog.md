# differentia_exemplariorum.worklog.md

## 2026-09-23 — the corpus is frozen; the golden is a snapshot again

**Symptom.** The gate went red three times on 2026-09-22 with no
finding changed: 20 usage lines in gesta/frigida.sh (4 rows), one
`"utf8"` line in briar/ and officina/compile_probationes.sh (58 cruda +
2 plena rows). Each fix was `EXEMPLARIA_SCRIBERE=1` + eyeball a pure
shift. Once I committed only one of the two rewritten files (HEAD red
on a clean checkout, healed 669b6474).

**Cause.** The golden pins line/column/byte positions of findings; the
gate compared it against the LIVE tree. A snapshot over a moving input.
And every regeneration replaced rows with the extractor's own output
about itself: 70 of 570 rows in four days (efc52a92, cbd1bfc9,
f719d4da, 1461b6bd).

**Fix.** `CORPUS_COMMISSIO=ad73f539…` (the commit that froze the
golden). The gate `git archive`s it into `mktemp -d`, runs facies with
absolute paths (facies calls excubitor with a repo-relative path, so
cwd stays the repo root) and strips the temp prefix from column 1 in
`cribrare`. The golden restored to its ad73f539 bytes (72 rows back).
Measured first: today's C over that corpus = the original golden,
byte-identical, 570/14. Refusals: unreachable commit → exit 2 named;
failed `mktemp` → exit 2 (an empty `-C` would extract into cwd).
Rule 2: count pin over the frozen corpus (37 live → 70 frozen: the
frozen tree predates efc52a92's fixes and annotations); `rule ⊆ grep`
over the live tree (independent oracle, meaningful live). Runtime ~7 s.

**Discussion with Fran** that reframed it: the Python provenance was
development scaffolding — its job (earning the C's trust) was done,
and the bytes are now identical anyway. What is worth keeping is the
BROAD regression net; the frozen input is what makes it quiet. So the
"never regenerate / circularity" ceremony went; regeneration is an
ordinary golden update, legitimate only for intentional extractor/rule
changes, and corpus refresh is a deliberate commit.

**Proof (plants).** (a) extractor: removing `redde` at
materia_exemplaria.c's DESCENSUS TACET (descent restored) → crusta
suite red AT this gate: cruda 1140 discordant lines, plena 28;
reverted → green. (b) a line inserted into live gesta/frigida.sh →
gate stays GREEN (the new property). (c) CORPUS_COMMISSIO = zeros →
exit 2 "commissio corporis … absens".

**Doctrine this came from** (survey pending, ledger 01M365VXAF): a
gate over the live repo may assert only INVARIANTS (round trip, parser
accepts, rule ⊆ oracle, floors); SNAPSHOTS (exact rows, positions,
counts) need a frozen corpus. Snapshot-over-live cases known: this gate
(fixed), silva's lib census pin (next: rewrite as invariant).

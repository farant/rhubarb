# emitte.c — worklog

## 2026-07-10 — natus (instrumenta prima, tool 2 of 5)

Single-TU byte-exact definition dump: `emitte <plagula> <symbolum>`
→ every top-level element that BINDS the symbol, each under a
`[via:linea-linea]` header (the sim-2 ep.1 pin), body = the raw
source lines themselves (token radix offsets → line-start
backtrack → end-of-line extend; no re-emission, no scribere).

Mechanics shared with censor: registry-driven tok_titulus slots
for binding detection; match-walk SKIPS function bodies (usage
inside bodies is nexus's job); descends taken conditional arms +
ambiguus canonical spines; worklist walkers (no depth caps).

**BAR MET same day: 10/10 symbols byte-identical to `sed -n a,bp`
extraction** — officina/lib/silva fontes, headers (Xar,
MedullaLineae typedefs), file-scope static (exclusiones_undae),
TWO probationes files, one English-C knotapel header.

**Two real finds — both caught BY the bar's own composition:**
1. **Token valor is the EXPANDED text.** `principale` in source is
   `main` in the tree — searching "principale" found nothing in
   any probationes file (the bar demanded probationes files;
   that demand caught this immediately). Fix: `_nomen_congruit`
   matches token valor OR origo-radix valor — both the written and
   the expanded name now find the definition.
2. **Tag slots over-match on references.** Searching `Xar` also
   emitted `XarIterator` (its `constans structura Xar* xar;` field
   carries a tag tok_titulus). Fix: structura/unio/enumeratio bind
   only when the node DEFINES (any non-empty lista slot =
   membra/enumeratores present) — `_tag_definit`, registry-generic.

**Known rough edges (noted at the moment, per debrief practice):**
- Multi-block output order is worklist discovery order, not line
  order (rare post-tag-fix; sort by linea_prima if it ever bites).
- Every invocation re-runs capita_praeparare (~1s) + one parse —
  the known cold-start cost, answered properly by acta/M4, not v0.
- Leading doc-comments above a definition are NOT included (extent
  starts at the first token; trivia-attachment extension = the
  comment-model vision pin's territory, deferred by name).
- Param names match too (searching a param name emits its whole
  function) — harmless, arguably useful; noted.

## 2026-07-10 (later) — "fortasse:" suggestions (similitudo)

Not-found path now ranks ALL binding names of the TU (collected
free during the match walk) via lib/similitudo and prints the top
distinct candidates to stderr. The desideratum's birth-failure
replayed: `scr_signatura_computare` → "fortasse:
scr_signatura_ex, scr_signatura_libera, scr_signatura_aequalis".

Finding en route (detail: lib/similitudo.worklog.md): a wrongly
guessed FULL name is a subsequence of nothing — emitte shrinks the
query from the right (floor 3) until candidates appear. Consumer
policy, lib stays pure.

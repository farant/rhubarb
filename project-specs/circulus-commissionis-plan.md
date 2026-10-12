# circulus commissionis - plan

Spec: `project-specs/circulus-commissionis-spec.md`. Same rhythm as always:
one task per turn, Fran approves each; failing test -> red -> implement ->
green -> plant -> commit via silva.commissio. Work in `../rhubarb-quinta`.
The small items go first, so each one helps the next commit.

### T1: P5 excusatae (per-gate excuse)
- [x] pythonica test with fake gates and debita (the existing commissio
  section): the excused gate is not run and its line is printed; the other
  owed gates are still added and run; empty cause or not-owed name
  refused, nothing run; same for commissio_umbra. Docs in README. Commit.
  As built (2026-10-12): `_excusatas_praeiudicare` (prechecks) + adder
  (not owed = refused); README; exspectare test flake fixed (atomic receipt
  write in the test). Plants II red.

### T2: P3 stale judge healed
- [ ] pythonica test with a fake fabrica returning "NON IUDICATUM ... iudex
  fabrica non recens" once and then clean: commissio rebuilds (fake
  struere), judges again, proceeds; a different refusal, or a failed
  rebuild, still refuses. Commit.

### T3: P2 -nova refreshes the index
- [ ] oratio/vocabula.sh -nova runs the incremental sweep first; test: add
  an identifier with an unknown word, run -nova immediately -> NOVA 1 (red
  today). Ledger 01M46ER35 closed. Commit.

### T4: P4 briar-fumus tree-local in worktrees
- [ ] worktree detection; uses bin/briar + PATH; main unchanged; the gate
  passes in quinta without excuse; a plant (forced ~/.bin path in a
  worktree) turns it red against a skewed fixture. Commit.

### T5: P1 extractor-keyed closure cache
- [ ] Fran picks (a) or (b). Digest of the extractor's inputs in build/,
  linked by bin/aedilis and bin/fabrica; key = digest ‖ aedilis.stml.
  Tests:
  - fabrica rebuilt with an edit OUTSIDE the extractor -> cache hit;
  - an edit INSIDE (lib/aedilis_silva.c) -> miss;
  - aedilis and fabrica share entries.
  Measured before/after. Commit.

### T6: close
- [ ] spec "As built", worklogs, MEMORY; merge.

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
- [x] pythonica test with a fake fabrica returning "NON IUDICATUM ... iudex
  fabrica non recens" once and then clean: commissio rebuilds (fake
  struere), judges again, proceeds; a different refusal, or a failed
  rebuild, still refuses. Commit.
  As built (2026-10-12): `IUDICES_STRUERE` table (fabrica, compilator,
  aedilis -> their struere.sh; never the printed SANANDA recipe);
  `_fabricam_exigere(_iudex_sanatus)` heals then judges once more. Four
  cases (healed, rebuild fails, still stale = one attempt, unknown judge).
  Live check: a comment in lib/portae.c made bin/fabrica stale, and the
  precheck rebuilt it and passed. Plants II red.

### T3: P2 -nova refreshes the index
- [x] oratio/vocabula.sh -nova runs the incremental sweep first; test: add
  an identifier with an unknown word, run -nova immediately -> NOVA 1 (red
  today). Ledger 01M46ER35 closed. Commit.
  As built (2026-10-12): every identifier-reading mode refreshes the index
  (`nexus.sh -renovare`, ~1 s; failure = exit 2);
  VOCABULA_INDEX_RENOVATUS=1 from the hook (no double sweep); unci fumus
  XXI (red today, green after; plant red); manual -nova takes 1.8 s.

### T4: P4 briar-fumus tree-local in worktrees
- [x] worktree detection; uses bin/briar + PATH; main unchanged; the gate
  passes in quinta without excuse; a plant (forced ~/.bin path in a
  worktree) turns it red against a skewed fixture. Commit.
  As built (2026-10-12): git-dir != common-dir -> tree's bin/briar (and
  bin/briar-spectator), required RECENS by `bin/fabrica iudicare` (recipe:
  `bin/fabrica sanare bin/briar bin/briar-spectator` - tree-local, heals
  the capsules too, never installs); link dir first on PATH, real ~/.bin
  removed (shebangs and `command -v briar-spectator` are tree-local too);
  new stage 0 asserts the binary under test. Plants II (BRIAR forced to
  ~/.bin; PATH not changed) red. Green in quinta without excuse.

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

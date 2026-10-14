# analytica - plan

Spec: `project-specs/analytica-spec.md`. Same rhythm: one task per turn,
Fran approves each; failing test -> red -> implement -> green -> plant ->
commit via silva.commissio. Work in `../rhubarb-quinta`.

### T1: the run record in lib/mensura
- [x] Run API: begin/end records (initium/finis journal lines), fields of
  spec §II, session context (commit, dirty, branch, tree, machine,
  invoker) computed once and inherited by environment; fold into the
  volume. Tests: round trip of both lines; an unfinished run reads as
  interruptus; nesting across processes; context inherited, not
  recomputed. Commit.
  As built (2026-10-13): CursusInitium / CursusFinis / Cursus types,
  line formats (argv escaped, other fields pure or refused), append,
  fold routing (genus "cursus"), pairing reader (hash by id, start
  order, missing end = INTERRUPTUS). Session context is fields only -
  computing it (git) and inheriting it by environment moved to T2 (the
  CLI), keeping lib/mensura pure. probatio_mensura_cursus 5 sections;
  plants IV red.

### T2: `bin/mensor cursus` + the chassis prologue `tools/cursus.sh`
- [x] CLI begin/end; prologue opens, traps EXIT, captures output OUTSIDE
  the tree, stores the body through the one seam; annotation
  `<cursus titulus versio>`. Tests (mensor fumus): exit codes and
  durations recorded, killed script = interruptus, output captured.
  Commit.
  As built (2026-10-13): DEPARTURE - no bash trap; `tools/cursus.sh`
  execs `bin/mensor cursus currere -- "$0" "$@"`, a C wrapper (rusage,
  monotonic duration, 128+signum, streaming passthrough, one body seam
  `_acta_servare`; CURSUS_INTUS stops the re-wrap, CURSUS_NULLUS opts
  out). Context computed once in C, inherited by env (settles the spec's
  AUDIENDA on rusage in bash). `cursus legere [-machina]` folds by
  rename, 14 columns. Gate `mensor-cursus` VIII stages; plants XI red;
  a plant found a real crash outside any repo (empty chorda -> NULL ->
  setenv). Overhead: top-level +~27 ms, inherited ~0.

### T3: producers - gates, commit phases, fabrica actions
- [x] silva.py (porta:<g>, phasis:<x>) and bin/fabrica actions emit run
  records with parens. Tests: a commissio produces a nested tree
  (commissio -> phases -> gates -> fabrica actions). Commit.
  As built (2026-10-13): context code moved to lib/mensura_contextus
  (shared by mensor and fabrica; vocator default `agens` under
  CLAUDECODE); end line gains `nota` (gate compendium / fabrica eventus,
  256-byte cap on a UTF-8 boundary); `mensor cursus incipere|finire`
  (incipere prints the child's KEY=VALUE env - Python reimplements no
  format); silva.py `_cursus` context manager on porta, commissio and
  phases forma/lint/fabrica/portae debitae/git commit (exception =
  fractus, KeyboardInterrupt = interruptus); fabrica from its existing
  `cursum_inscribere` hook (child env untouched - it is in the member
  key). Tests: mensor fumus IX, fabrica fumus LII, pythonica commissio
  tree + _cursus semantics, probatio_mensura_cursus VI. Plants XI red;
  two blind tests found and fixed by survivors (UTF-8 cut alignment,
  env-restore baseline). Not wrapped: commissio_umbra. Cost inside
  noise.

### T3b: sections as child runs (Fran 2026-10-13, inserted)
- [x] "are we tracking time for individual tests or test groups within
  a suite? that seems like something that could be built into credo?"
  credo v2 already times sections (credo.tsv, overwritten per run); the
  bridge is fabrica, after a member that ran: one child run per SECTIO
  line (`<membrum>#<sectio>`, ms, exit, nota). credo stays pure, member
  env stays hermetic. fumus LIII; plants III red. Found on real data:
  probatio_mensura_cursus 221 ms, its sections ~13 ms (start-up + dyld
  ~94%). Only 32/555 suites are sectioned.

### T4: logs and retention
- [x] FIRST: isolate every fumus that drives bin/fabrica on throwaway
  roots (iudicium-fumus wrote fake 'gen'/'porta_x' runs - two FRACTUM -
  into the real diary under quinta's tree and commit, 2026-10-13) + a
  check that refuses a fumus writing the real diary.
  As built: structural instead of per-fumus isolation + detector (Fran
  approved the two layers; the detector became a guard because a gate
  reading the real diary is not hermetic). fabrica records the root it
  judged (own context if it differs from the session tree;
  `mensura_contextum_arboris`), and a non-repository root writes only to
  an explicit MENSOR_DIARIUM. fumus LIV + LV; plants III red;
  iudicium_fumus against the real diary: +0 lines.
- [x] 2a: library `diurna` (Fran: logs are not metrics; spec III-half,
  IV): ponere / legere / figere / introitus, SHA-256 dedup, tail cap,
  sha verified on read, (clavis, seq) index ready for streams.
  probatio_diurna VII sections; plants VI red. Compressed read untested
  until purgare writes .gz.
- [x] 2b: wire the seam - mensor `_acta_servare` -> diurna_ponere;
  `cursus finire -acta <file>` for Python producers; `cursus acta <id>`.
  As built: `mensura_diurna_aperire` (MENSOR_DIURNA | ~/.rhubarb/diurna)
  shared by mensor and fabrica; run `acta` = body SHA-256; Python bodies
  = gate output / WHOLE exception message via `finire -titulus -acta -`
  (stdin); fabrica stores each ran action's log under its run id (the
  history of "what did probatio_x print when it failed"). Fixture guard
  covers bodies. Plants VI red (one survivor fixed: LV diary-only case).
- [x] 2b-ii (Fran 10-13, after 2b): credo names a crashed section -
  `INCIPIT` line before each section, the reader turns "started, no
  SECTIO" into "sectio X RUIT (signal), N not run" + a fractus section
  run. No signal handlers (credo doctrine). Opt-in per-section fork
  isolation parked.
  As built (10-14): credo SECTIONES (first line) + flushed INCIPIT;
  fabrica reader synthesizes RUIT + non_cursae; member cause "sectio
  'X' RUIT; N non cursae"; #X fractus run. credo fumus II/VI, fabrica
  fumus LVII (crash inside a section AND between sections). Plants V
  red.
- [x] 2c: retention policy as STML data + `diurna_purgare` (per-key
  tiers, per-body action, last-per-job, pins, index compaction under a
  lock) + `bin/mensor purgare [-siccum]`. Tests: dedup survives, age
  tiers, failures kept longer, last body per job kept, pinned kept,
  compressed read. Commit.
  As built (10-14): `diurna_purgare(d, &regula, nunc_ms, siccum,
  &relatio)` (now as a parameter: deterministic tests), forward-only
  actions, ONE lock for all writers (ponere / figere / purgare),
  compaction tmp + rename, oblivio horizon. tools/analytica_retentio.stml
  (7/30, failed 30/90, last-per-title, oblivio 365); `bin/mensor
  purgare [-siccum] [-machina] [-regula]`, missing element refused.
  probatio_diurna XV sections, mensor fumus X. Plants VIII red (two
  test blind spots fixed by reasoning first). NOT done: the daily
  opportunistic trigger (T6); real two-process concurrency untested.
- [ ] Bodies for Python-produced runs too (commissio, phases, gates:
  the full refusal / hook output through the one seam) - Fran 10-13:
  "what test or lint or whatever caused them to fail" needs more than a
  first line ('git commit rc=1: <tail>' can start mid-word).

### T5: named queries + `silva.cursus`
- [ ] ultimum, historia, tendentia, numquam, fracta, acta, filii,
  discus; -machina; thin pythonica wrapper. Tests: the six acceptance
  questions, each on fixture data. Commit.
- [ ] `commissiones` (Fran 10-13: "analytics about how long git commit
  attempts take, if they failed/succeeded, what test or lint caused
  them to fail"): one row per attempt - when, tree/branch, duration,
  outcome, CULPRIT = deepest failing descendant (derived from the tree,
  never guessed by the producer) + its nota, resulting hash; attempts
  per landed commit; which gate refuses most. Fixtures: lint refusal,
  red gate, examen refusal at git commit, success.

### T6: pilot adoption + version lint
- [ ] Chassis on the four converted entry points, frigida, one hook (=
  tools/unci-git/pre-commit: hand-made `git commit`s are attempts too,
  Fran 10-13), one gate script; lint stage: an adopter must carry the current version.
  Measured: fold and capture overhead. Commit.

### T7: close
- [ ] spec "As built", worklogs, MEMORY; merge per ritual.

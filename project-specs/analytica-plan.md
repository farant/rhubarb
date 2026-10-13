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
- [ ] silva.py (porta:<g>, phasis:<x>) and bin/fabrica actions emit run
  records with parens. Tests: a commissio produces a nested tree
  (commissio -> phases -> gates -> fabrica actions). Commit.

### T4: logs and retention
- [ ] Bodies in the volume (massae dedup), size cap, retention policy as
  STML data, `bin/mensor purgare [-siccum]`. Tests: dedup, age tiers,
  failures kept longer, last body per job kept, pinned kept. Commit.

### T5: named queries + `silva.cursus`
- [ ] ultimum, historia, tendentia, numquam, fracta, acta, filii,
  discus; -machina; thin pythonica wrapper. Tests: the six acceptance
  questions, each on fixture data. Commit.

### T6: pilot adoption + version lint
- [ ] Chassis on the four converted entry points, frigida, one hook, one
  gate script; lint stage: an adopter must carry the current version.
  Measured: fold and capture overhead. Commit.

### T7: close
- [ ] spec "As built", worklogs, MEMORY; merge per ritual.

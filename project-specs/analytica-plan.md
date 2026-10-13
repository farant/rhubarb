# analytica - plan

Spec: `project-specs/analytica-spec.md`. Same rhythm: one task per turn,
Fran approves each; failing test -> red -> implement -> green -> plant ->
commit via silva.commissio. Work in `../rhubarb-quinta`.

### T1: the run record in lib/mensura
- [ ] Run API: begin/end records (initium/finis journal lines), fields of
  spec §II, session context (commit, dirty, branch, tree, machine,
  invoker) computed once and inherited by environment; fold into the
  volume. Tests: round trip of both lines; an unfinished run reads as
  interruptus; nesting across processes; context inherited, not
  recomputed. Commit.

### T2: `bin/mensor cursus` + the chassis prologue `tools/cursus.sh`
- [ ] CLI begin/end; prologue opens, traps EXIT, captures output OUTSIDE
  the tree, stores the body through the one seam; annotation
  `<cursus titulus versio>`. Tests (mensor fumus): exit codes and
  durations recorded, killed script = interruptus, output captured.
  Commit.

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

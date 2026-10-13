# cursores ad gradus - plan

Spec: `project-specs/cursores-gradus-spec.md`. Same rhythm: one task per
turn, Fran approves each; failing test -> red -> implement -> green ->
plant -> commit via silva.commissio. Work in `../rhubarb-quinta`.
Mechanism first, then the pilots in rising difficulty (gen 3 -> 2 -> 1
-> the big one).

### T1: `bin/fabrica probare` (D1)
- [x] fabrica fumus stage over a fictional world: composite with a
  passing, a failing and a fresh member -> lines transiit/FRACTA/
  servatum, the reuse line ("servatae 1/3: ingressus immutati ..."),
  summary, exit 1; FILTRUM selecting one member; no match ->
  exit 2; `-vis` re-runs the fresh one; `-machina` TSV. Red first.
  Implement in tools/fabrica.c (+ fabrica_ansae), worklog. Plants.
  Commit.
  As built (2026-10-12): all in tools/fabrica.c; `_sanare_suturam` +
  `_sanare_agere` extracted so sanare and probare share one path. Reused
  = parts without a sanatio, re-judged for the judge's own reason
  (grouped into `servatae K/M: ...`). Human lines keep the old runners'
  shapes (`=== m ===`, acta, `--- m praeteriit|FRACTA|servata`).
  Machine: MEMBRUM / SERVATAE / PROBARE. Fumus XLVI; plants IV red
  (servatae line, -vis, acta, filtra). Live toml: 13/13 servatae ~9 s.

### T2: porta via probare; toml becomes an entry point (D1, D2, D3)
- [x] `_porta_per_gradum` parses `probare -machina` (pythonica test with
  a fake fabrica: same Porta as today for pass/fail/reuse; residua
  untouched). `tools/gradus_oraculum.sh <sub> [-ref REF]` from toml's
  oracle; run it on toml: agrees. toml/compile_probationes.sh -> entry
  point; tools/toml_gradus_oraculum.sh deleted; park …MX8N closed.
  porta('toml') green. Commit.
  As built (2026-10-13): one `probare -machina` call (vis/audit passed
  through; verdict deletion and iudicare fallback gone); `porta_toml`
  RETIRED (it ran the runner = fabrica in fabrica); toml/arbor.sh and
  computus.sh built off the old runner's objects -> own closures via
  cursor_instrumentum_struere; probare normalizes path filters. Oracle
  toml 13/13. Warm: old runner 38 s -> probare 7 s; -vis 55 s. Plants:
  XLVI path filter, pythonica -vis, producer named; one plant stayed
  green and exposed a dead clause (removed).

### T3: css (gen 2 -> steps)
- [x] css/aedificatio.stml `probationes_css`; oracle vs HEAD runner
  agrees; entry point; PORTAE_GRADUUM; measured cold/warm before and
  after; expedition row (rubric v3). Commit.
  As built (2026-10-13): css registered as a subsystem in fabrica.stml;
  oracle 10/10; totalitas failure write -> $TMPDIR; css/computus.sh own
  closure; latent T2 bug fixed (silva.probatio_currere ran the OLD
  runner's stale binary for converted suites -> `_binarium_probationis`).
  Measured: old 4.2 s warm vs probare 6.6 s (~4.8 s fixed setup over the
  whole graph: ordering ~3.2 s, composite expansion ~1.6 s) - raised
  with Fran. Expedition row deferred to T6 (rubric v3).

### T3b: the fixed cost of a fabrica call (inserted, Fran 2026-10-13)
- [x] css showed warm probare 6.6 s vs the old runner's 4.2 s. Profile
  (sample), then fix and re-measure, red-first test per fix.
  As built (2026-10-13): warm css 6.6 -> 3.5 s (memoria indices VII-IX:
  lectiones had none, ~340 full scans of 90k rows per call) -> 1.8 s
  (`gradus_solus`: probare explains only the composite it names) -> 1.2 s
  (numeri_latinae memorabilis: upstream of every C member, regenerated
  on every full judgement). Warm toml 7 -> 2.3 s. Forced css 18.5 ->
  13.5 s (clang identity once per process, compilator + sutura). Tests:
  gradus unit section; fumus XLVII-XLIX; plants V red. Parked: whole-tree
  snapshot per wave (~7 s of forced css) and scoped ordering (~0.6 s).

### T4: md (gen 1 -> steps)
- [ ] same as T3 for md (first jump straight from excubitor/mtime: hand
  RADIX_FONTES and `-I` deleted). Commit.

### T5: silva (the big one)
- [ ] same as T3 for silva (55 tests; findings about hermeticity and
  house-binary reads go in the spec). Commit.

### T6: expedition rubric v3 + roots for what stays bash (D6)
- [ ] rubric v3 ("to steps", recipe = T3-T5) with rows for the
  remaining runners, class B marked as needing instrument declarations;
  `bin/aedilis --radices`; compile_tests.sh, silva/haruspex.sh,
  compile_tools.sh read it; check refusing hand `-I` lists in runners
  (plant). Commit.

### T7: close
- [ ] spec "As built", worklogs, MEMORY; merge per ritual.

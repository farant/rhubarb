# fabrica plan 6 - the tool: chassis, step kinds, the toml pilot

Spec: `project-specs/fabrica-spec-6.md`. Interview:
`project-specs/fabrica-6-interview.md` (Q1-Q15 decided). Vision:
`project-specs/fabrica-visio.md`. Rhythm as always: one task per turn,
Fran approves; each task = failing test -> red -> implement -> green ->
plant -> commit through silva.commissio with owed gates; API headers to
Fran before code.

### T1: chassis - registries, conformance harness, census

- [x] **Step 1:** explicit registry for input kinds (today's 12
  `FabricaGenus` tables enumerated by one function, not scattered);
  property fields for the census (reads static/dynamic, deterministic,
  particles, cost class).
- [x] **Step 2: failing test** `probatio_fabrica_chassis`: enumerates
  the registry; every entry must have a conformance fixture; runs each
  (input-class change -> judgment flips naming the particle; unrelated
  change -> RECENS; writes inside declared places). Red: no fixtures.
- [x] **Step 3:** fixtures for the 12 input kinds (one table of small
  declared worlds). **Plant:** a kind whose `sigillare` ignores content
  -> its fixture red.
- [x] **Step 4:** `bin/fabrica census` (entries by properties). **Commit.**

### T2: inputs on two axes + `repositorium`

- [x] **Step 1: failing test:** declarations with `res`/`clavis`/`fons`
  parse and judge identically to their alias (`genus`) forms; a
  `repositorium clavis="commissum"` input changes with HEAD.
- [x] **Step 2:** aedificatio.canon attributes; alias table; the
  `repositorium` kind (+ conformance fixture). **Plant:** alias mapped
  to the wrong key -> equivalence red. **Commit.**

### T3: bootstrap stage

- [x] failing test: a stale judge (fabrica, compilator or aedilis) ->
  every other judgment refuses "iudex X non recens - sana X prius";
  fresh judges -> normal. Implement; plant (stage check off -> red).
  **Commit.**

### T4: compilator as a library

- [x] **Step 1:** API header `include/compilator.h` to Fran (key of a
  compilation, store lookup/put, compile-through-store).
- [x] **Step 2:** extract from tools/compilator.c into lib/compilator.c;
  tools/compilator.c thin CLI. Oracle: `tools/compilator_fumus.sh`
  (X/X) unchanged and green; objects byte-identical before/after on
  the toml closure. **Commit.** (As built: fumus XI/XI - case XI
  closes an oracle gap; toml_scalaris.o is nondeterministic in clang
  itself, spec §XIII T4.)

### T5: step-kind chassis - `FabricaGradus`, areas, base env

- [x] **Step 1:** API (`FabricaGradus` in include/fabrica.h) to Fran.
- [x] **Step 2: failing tests:** a toy step kind registered with a
  fixture; members get stable ids `<action>/<member>`; each step runs
  in `build/fabrica/area/<action>/<member>/` with the base environment
  only; a write outside the area or an undeclared variable read ->
  refusal naming it; deleted member -> orphan reported; passes cached,
  failures re-run.
- [x] **Step 3:** implement; plants (area not enforced; env leaks;
  failure cached) -> red. **Commit.** (As built: the toy kind lives in
  the test, not the registry; real sutura hooks move to T6 - spec
  §XIII T5.)

### T6: `probationes_c`

(As built in four commits: T6a processus options, T6b extractor library +
facultas, T6c-1 the kind in the machine, T6c-2 the tool hooks + smoke.)

- [x] **Step 1: failing test** (temporary root, two tiny C tests and a
  library): link via aedilis_derivare + compilator library (flags from
  aedilis.stml), run, verdict per member; edit one library source ->
  only members whose closure holds it re-link and re-run; edit one test
  -> only it; a `facultas="fenestra"` member is linked, not run.
- [x] **Step 2:** implement; conformance fixture; plants (closure
  ignored -> all re-run; facultas ignored -> GUI member run). **Commit.**

### T7: gates as composites + `post`

- [x] failing test: a gate action of steps -> composite verdict
  (`<porta>: N/N`, non-RECENS parts named); `post` orders producers
  before steps; a member that reads a `post` product is keyed on it,
  the others are not. Implement; plants. **Commit.**

### T8: toml as steps - the oracle phase

- [ ] `probationes_toml` beside `porta_toml`; both on one tree agree
  per test; plants: a broken toml test -> both red naming it; a broken
  toml/fontes source -> both red. Record in spec §XIII. **Commit.**

### T9: A/B

- [ ] `tools/reusus_retro.sh` reads step traces; per-step reuse vs
  runner reuse over the same 150 commits; numbers in spec §XIII.
  Shown to Fran before T10. **Commit** (docs + tool).

### T10: switch and close

- [ ] PORTAE['toml'] judged through the composite; runner kept until
  the switch has stood, then deleted (as generata T7c); spec §XIII as
  built; worklog; MEMORY; ledger park closed. **Commit.**

## Not in this plan (stated)

Volumes / filum backend (next slice); other step kinds; source
annotations beyond `facultas`; snapshot builds; PORTAE as a derived
view; other runners.

# fabrica slice 7 - interview: the root suite as steps, sections as verdicts

Vision: `project-specs/fabrica-visio.md` §VII (agenda). Follows slice 6
(`project-specs/fabrica-spec-6.md` §XI "not in this slice"), the
housekeeping and refactors of 2026-10-08, and credo v2 (657b8e49, in
main a387eb48: `CredoSectio`, `credo_suitam_currere`, `CREDO_VERDICTA`
per-section TSV, `CREDO_SECTIO`, `CREDO_NECESSE`). Fran 2026-10-09: credo
and fabrica are ONE project; settle the API/target shape before any
comprehensive migration; at most two versions of anything during a
migration.

## 0. Data (2026-10-09, before any question)

**Two phases of fabrica, measured.** Phase 1 (slices 1a-3) judges the
house as it was: 65 generators + 19 installers declared as scripts
(`mandatum`), gates as whole-runner `iudicium` actions judged by read
trace. Phase 2 (slices 5-6) understands work: chassis with conformance
fixtures, two-axis inputs, step kinds (`probationes_c`, members =
synthetic `iudicium` actions in own areas), libraries linked in. Only
ONE suite runs as steps (toml, 13 tests).

**Competing versions today.** 42 gates in pythonica `PORTAE`; 2 via
fabrica, in two styles (aedilis = phase-1 trace of the whole runner,
toml = phase-2 steps); 40 raw. Owed gates from three sources
(inventory lenses, aedilis closures, fabrica graph). 15 bash runners
each with its own build/; 14 `*_struere.sh` (3 hand source lists) +
aedilis-generated struere + compilator; vigilia, excubitor (5/20
migrated), mtime checks inside scripts.

**The root runner.** `compile_tests.sh`: 1172 lines; 257
`probationes/probatio_*.c` (radix runs 239 test binaries); gate
`radix` 91-181 s over the last runs. It also builds GUI apps
(`compile_gui_app`, FRACTA lines on fd 3), prebuilds objects
(`--obiecta`, a fabrica precondition), and compiles from the generated
source list `compile_tests_fontes_generata.sh` (278 lines).

**Root test hygiene (slice 5 T5b, porta_aedilis).** Root tests write
scratch across build/: 418 writes outside the vestigium, declared by
name as 5 directories + 28 files; three worse cases fixed then (bin/
helper scripts, the real `.vigilia_commissum`, a test rebuilding the
live resident). Desideratum …SFQW: one scratch area for all root
tests. Steps give each member its OWN area and refuse outside writes.

**Sections.** 6 of 257 root suites use credo v2 sections (the fabrica
suites, 38 sections); 4 carry `facultas` annotations (link-only
members for capabilities like `fenestra`, built in slice 6). fabrica's
`probationes_c` member verdict today = exit code + output tail.

**toml pilot result (slice 6).** Steps vs runner over 150 commits: 273
-> 87 test runs; the gain came from the narrower KEY (inputs per test
binary), not from per-test granularity on a 13-test suite. The root
suite (239 binaries over ~180 lib files) is where per-binary reuse
should show.

## Questions

**Q1. What is slice 7?** (a) Recommended: the root suite as steps
(`probationes_c` over `probationes/probatio_*.c`) reading credo
per-section records; gate `radix` judged through the composite (as
toml); volumes/filum = slice 8. (b) Volumes/filum first (hermetic
places), root after. (c) Both.

**Q2. The step dialect.** (a) Recommended: keep growing NAMED step
kinds (declarative attributes, conformance fixture each); a general
dialect (`pro_omni`/`nectere`/`currere` primitives, vision §III) only
when two or three runners show a shared shape. (b) Introduce the
general dialect now with root as its pilot.

**Q3. Sections in fabrica's verdicts.** (a) Recommended: the KEY stays
per test binary (sections share a binary and its reads); the member's
verdict STORES the per-section records (`CREDO_VERDICTA` in the member
area), `iudicare`/`VERDICTUM` name failing sections, `-machina` gains
a SECTIO record type; an unconverted suite yields one synthetic
section from exit code + tail. (b) Per-section keys (needs per-section
read traces - not available).

**Q4. Root tests writing outside their area.** (a) Recommended: areas
enforced; each test learns its area (env from the step) and the 28
declared files / 5 directories become a reported debt list with an
owner per entry, shrinking test by test, nothing silently allowed.
(b) A permanent allow-list. (c) Fix every test before the switch.

**Q5. The fate of compile_tests.sh.** (a) Recommended: stays the human
entry and the ORACLE through an A/B (as toml: old runner vs steps on
identical history), then becomes a thin wrapper over the composite;
deleted in a later slice once nothing reads it. (b) Delete in this
slice.

**Q6. Non-test parts of the root runner** (GUI apps, `--obiecta`,
generated source list). (a) Recommended: out of scope - only
`probatio_*.c` become members; GUI apps get their own step kind
later. (b) Migrate them now.

**Q7. Gates and debts.** (a) Recommended: `radix` through fabrica this
slice (PORTAE_GRADUUM, as toml); `PORTAE` as a derived view and owed
gates from the graph (retiring inventory lenses) = recorded direction
for a later slice. (b) Derive them now.

**Q8. Converting root suites to credo sections.** (a) Recommended:
incremental (convert when touched; the oracle script exists); fabrica
handles both forms. (b) Bulk-convert all root suites in this slice.

**Q9. Snapshot builds as default.** (a) Recommended: later, recorded.
(b) Now.

**Q10. Capabilities.** (a) Recommended: keep `facultas` annotations
(link-only members) as built; no new capability work this slice. (b)
Extend now.

## Answers

**DECIDED (Fran 2026-10-09, three AskUserQuestion rounds): every
recommendation (a), Q1-Q10.**

- Q1 slice 7 = the root suite as steps (`probationes_c` over
  `probationes/probatio_*.c`) reading credo per-section records; gate
  `radix` through the composite; volumes/filum = slice 8.
- Q2 named step kinds keep growing; the general dialect waits for a
  shared shape across 2-3 runners.
- Q3 key per test binary; per-section records stored in the member's
  verdict, named by `iudicare`/`VERDICTUM`, SECTIO records in
  `-machina`; unconverted suites = one synthetic section.
- Q4 member areas enforced; the slice-5 exceptions (5 dirs, 28 files)
  become a reported debt list with an owner per entry, shrinking test
  by test.
- Q5 compile_tests.sh = human entry + oracle through an A/B on
  identical history, then a thin wrapper; deletion in a later slice.
- Q6 only `probatio_*.c` become members; GUI apps, `--obiecta` and the
  generated source list stay in the runner (GUI apps: own step kind
  later).
- Q7 `radix` judged through fabrica this slice (PORTAE_GRADUUM);
  PORTAE as a derived view and owed gates from the graph = recorded
  direction.
- Q8 suites convert to credo sections incrementally (when touched);
  fabrica reads both forms.
- Q9 snapshot builds by default: later, recorded.
- Q10 `facultas` annotations as built; no new capability work.

Next: `project-specs/fabrica-spec-7.md` (shape, data, done-means), then
a plan with tasks; headers to Fran before code.

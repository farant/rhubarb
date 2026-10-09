# fabrica plan 7 - the root suite as steps, credo sections as verdicts

Spec: `project-specs/fabrica-spec-7.md`. Interview:
`project-specs/fabrica-7-interview.md` (Q1-Q10 decided). Ledger:
parcum …ZS. Rhythm as always: one task per turn, Fran approves; each
task = failing test -> red -> implement -> green -> plant -> commit
through silva.commissio with owed gates; API headers to Fran before
code. Work happens in `../rhubarb-quinta`; merges by the ritual
(worktree-quinta memory).

### T0: headers (APPROVED by Fran 2026-10-09)

- [x] `FabricaSectio` + `FabricaSectiones` + `fabrica_sectiones_legere`;
  `FabricaDebitum` + `FabricaActio.debita`; `AedilisFructus.instrumenta`
  + `FabricaClausuraC.instrumenta`; the `SECTIO` machine record
  (`SECTIO <actio>/<membrum> <titulus> <exitus> <praeteriti> <totales>
  <fractura>`, no timing); `CREDO_VERDICTA` in the `probationes_c` base
  environment, `CREDO_SECTIO` never inherited.
- [ ] Headers written into `include/fabrica.h` and `include/aedilis.h`
  with the first task that uses them (T1: sections DONE; T2:
  instrumenta DONE; T3: debita DONE, plus FabricaDebitiStatus +
  fabrica_debitum_iudicare approved 2026-10-09).

### T1: sections in member verdicts

- [x] **Step 1: failing test** (probatio_fabrica_gradus, a new section):
  a 2-member fixture over the fake world - one converted suite (two
  sections, one FRACTA), one classic suite - `fabrica_sectiones_legere`
  reads both; the classic one has the synthetic `totum`; a record
  without `SUITA` -> `completa` FALSUM; a malformed line -> refusal
  naming it.
- [x] **Step 2:** `probationes_c` sets `CREDO_VERDICTA=<area>/credo.tsv`;
  after the run writes the synthetic record when absent (exit code +
  compendium counts from the member's output); pass note `sectiones
  N/N` (deterministic: no ms); failure tail names the first failing
  sections. **Plant:** synthetic record skipped -> classic member red.
- [x] **Step 3:** `iudicare` / `VERDICTUM` show sections (through the
  note/tail); `-machina` emits SECTIO records; fumus XL knows SECTIO;
  silva.py's machine reader accepts it. Proven on `probationes_toml`
  (unconverted -> `totum`, 13 members). **Plant:** SECTIO line dropped
  -> fumus stage red. **Commit.**
- As built (2026-10-09): synthetic note is `totum P/T` (bare `totum` with
  no compendium); human iudicare lists non-passing sections under the
  member line (VERDICTUM line itself unchanged); sections live in the
  area's credo.tsv only - build/fabrica.db storage not needed yet (no
  reader wants them without the area). See lib/fabrica.worklog.md.

### T2: annotations - `instrumentum` and `facultas` from sources

- [x] **Step 1: failing test** (probatio_aedilis): `<aedilis
  instrumentum="bin/X"/>` in the scope -> `instrumenta`; in a non-scope
  file or outside bin/ -> named refusal.
- [x] **Step 2:** aedilis parses it; `FabricaClausuraC.instrumenta`;
  `probationes_c` keys the member on each binary's identity
  (`instrumentum_domus`). **Plant:** annotation ignored -> the keying
  assertion red (binary changed, member still RECENS).
- [x] **Step 3:** annotate the root tests that run house binaries
  (bin/generare, bin/manus, bin/natura*) and give the 17 build-only
  tests `facultas` (14 fenestra, 2 rete, 1 repositorium), so the facts
  live in the sources. The runner scripts' hand lists stay until T7.
  **Commit.**
- As built (2026-10-09): instrumentum also refuses duplicates; 4 tests
  annotated (natura, natura_glossae, natura_canones, generare - bin/manus
  is never executed by a root test); keying is on the provenance
  `ingressus` line (commit line ignored), proven by fumus XLII.

### T3: write debts

- [x] **Step 1: failing test:** `<debitum_scripturae membrum via
  causa/>` parsed (duplicate via, unknown member -> refusal); the owning
  member may write there; another member writing there -> refused;
  `census` counts debts; a debt not written by its member in the last
  run -> reported stale.
- [x] **Step 2:** implement (declaration, member enforcement, census).
  **Plant:** ownership not checked -> the foreign-write assertion red.
  **Commit.**
- As built (2026-10-09): record = `<area>debita.txt` (first line
  `cursus\ttransiit|fractus`); a failed run without the write = IGNOTUM,
  not stale; census explicates members (cheap today, re-measure in T4);
  fumus XLIII proves it with the real snapshot.

### T4: `probationes_radicis`

- [x] Declare the action (`probationes_c exemplar="probationes/probatio_*.c"`,
  `praecondicio tabulariumd`, `post` producers, the slice-5 writes as
  debts with owners); first full heal in quinta; each refusal fixed in
  the test or turned into a named debt. Record first-heal cost.
  **Commit.**
- As built (2026-10-09): 256 members (benchmark excluded), 256/256;
  speculum_hospes is a praecondicio (never-judged output), members
  inherit praecondiciones and key build/ sources by existence; 4 env
  switches declared; 36 debts -> 25 (three tests moved scratch to a
  unique TMPDIR dir after the census exposed transient writes); debt use
  = snapshot change OR reads-log path. First cold heal 7 min 5 s; warm
  iudicare 11 s, no-op sanare 44 s, census 1.5 s. Blind spot for
  transient writes outside the area filed (…HC0Q).

### T5: the oracle

- [x] `tools/radix_gradus_oraculum.sh`: on one tree, compile_tests.sh
  vs the composite - same pass/fail per test, same assertion totals per
  test (runner compendium vs member sections). Plants: a broken test ->
  both red naming it; a broken library source -> both red, the
  composite naming only members whose closure holds it. **Commit.**
- As built (2026-10-09): 256/256 (category + p/t per test); oracle needed
  LC_ALL=C (invalid UTF-8 output); facultas repositorium now RUNS with the
  HEAD commit in the key (Fran) - probatio_git was link-only after T2.
  Plants: broken test -> both name it, 1 member re-run; broken
  lib/base64.c -> both name base64+plist, 5 members re-run (closure).

### T6: A/B reuse

- [ ] `tools/reusus_retro.sh` over the root members (same history as
  the toml A/B); record runner-verdict vs per-member reuse, whatever it
  shows. **Commit.**

### T7: the switch

- [ ] `PORTAE_GRADUUM['radix'] = 'probationes_radicis'`; gate radix via
  the composite; `aedilis_porta.sh` stops RUNNING tests (keeps
  derivation, emitted scripts, `clang -MM` comparison), its hand lists
  and per-test inputs removed (now annotations and debts). **Commit.**

### T8: close

- [ ] spec "As built", worklogs, MEMORY; ledger …ZS closed; credo
  slice 2 next (Fran 2026-10-09).

## Not in this plan (stated)

PORTAE as a derived view; owed gates from the graph; snapshot builds by
default; volumes/filum (slice 8); a GUI-app step kind; per-section keys;
bulk conversion to sections; deleting compile_tests.sh.

# fabrica spec 7 - the root suite as steps, credo sections as verdicts

Interview: `project-specs/fabrica-7-interview.md` (Fran 2026-10-09:
every recommendation, Q1-Q10). Ledger: parcum …ZS (follows credo
…42JH9; born of the vision …F9ESW3). Builds on spec 6 (chassis, steps,
`probationes_c`, composites) and credo v2 (657b8e49: sections,
`CREDO_VERDICTA`, `CREDO_SECTIO`, `CREDO_NECESSE`).

*The question: can the 239 root test binaries be judged one by one -
each re-run only when its own inputs change - with the verdict naming
the failing SECTION, while the house keeps at most two ways of running
them at any moment?*

## 0. Data (2026-10-09)

- **The root tests run through TWO runners today**, both gates:
  `compile_tests.sh` (gate `radix`, 1172 lines, 91-181 s; links from
  the committed source-list snippet `compile_tests_fontes_generata.sh`)
  and `tools/aedilis_porta.sh` (gate `aedilis`, ~356 s: per test
  aedilis derivation + emitted build script + RUN + closure vs `clang
  -MM` comparison; judged as one `iudicium` action by the trace of the
  whole runner, phase-1 style). Steps would be a third.
- **How a member runs** (spec 6 §IV/§VI, as built): link through the
  aedilis closure + compilator store; cwd = the tree root (repo-relative
  fixtures keep working); base environment PATH (fixed), HOME,
  `TMPDIR=<root>/<area>tmp`, `RHUBARB_RADIX`, plus declared
  `<ambitus>`; verdict = exit code + output tail; a write outside the
  member's area or an undeclared variable read = refusal.
- **What root tests write outside any area** - declared by name on
  `porta_aedilis` in slice 5: 5 directories (`build/probatio_*`) and
  28 files (`build/probatio_ct.db`, `_sent.*`, `_villa.*`,
  `_vigilia_*`, `_volumen*.db`, PNGs, ...), plus `build/aedilis` shared.
- **What root tests run** via `system()`: `bin/generare`, `bin/manus`,
  `bin/natura`, `bin/natura_canones`, `bin/natura_glossae` (declared
  as `instrumentum_domus` inputs of `porta_aedilis`); the
  `tabulariumd` daemon is a precondition.
- **Capabilities**: only 4 root tests carry `facultas` (link-only
  members, spec 6). The build-only sets live as HAND LISTS in the
  runner scripts: `aedilis_porta.sh` names 14 GUI tests (`GUI_LISTA`:
  fenestra, vitrea, delineare, pagina, navigator, imago ...), 2 network
  (`probatio_tls`, `probatio_tcp`) and 1 live-repository
  (`probatio_git`) - 17 in all.
- **Sections**: 6 root suites (the fabrica ones, 38 sections) use
  credo v2; the other ~250 print the classic compendium (`Totalis /
  Praeteriti / Fracti`, `Conditio`).

## I. Framing

One step action, `probationes_radicis`, replaces the RUN half of both
runners. Its members are keyed per binary (closure + flags + toolchain
statically, the run's read trace dynamically); their verdicts carry the
credo sections. `compile_tests.sh` stays the human entry and the oracle
until the A/B and the switch; `aedilis_porta.sh` keeps only what no
step does (derivation, emitted scripts, closure vs `clang -MM`) and
stops running tests once the composite is the gate.

## II. The action

```
<actio titulus="probationes_radicis" genus="iudicium">
  <probationes_c exemplar="probationes/probatio_*.c"/>
  <praecondicio actio="tabulariumd"/>
  <post actio="..."/>      (producers of build/ inputs tests read)
  <ambitus variabilis="..."/>   (only those tests really read)
  <debitum_scripturae .../>     (§IV)
</actio>
```

- **Binaries run through `system()`**: declared per TEST, not per
  action, by a source annotation next to `facultas` -
  `/* <aedilis instrumentum="bin/natura"/> */` in the few tests that
  run one (spec 6 §XI recorded `instrumentum` as the next annotation).
  The member keys on that binary's identity; the other 230 do not.
  Declaring them on the action would key every member on bin/natura.
- **build/ inputs**: a member that reads a producer's output gets the
  producer through `post` (ordering) and its key through the read
  trace (spec 6 rule) - as toml did with `toml_corpus`.
- The member set comes from the glob; the 17 root tests the runners
  only build (14 GUI, 2 network, 1 live repository) become members
  with `facultas` annotations in their SOURCES (`fenestra`, `rete`,
  `repositorium`) - linked and judged, not run. The hand lists in the
  scripts are the old version of that fact; they go when the scripts
  stop running tests (§V.4).

## III. Sections in the verdict

1. **The step sets `CREDO_VERDICTA=<area>/credo.tsv`** for every
   member (part of the `probationes_c` base environment, declared by
   the kind, not by each action).
2. **After the run fabrica reads it.** Converted suites: one record
   per section. Unconverted suites (no file): ONE synthetic section
   `totum`, exitus from the exit code, counts from the compendium lines
   of the member's own output when present (`Totalis`, `Praeteriti`) -
   the format credo has printed for months. A file without its `SUITA`
   line = the binary crashed after the last recorded section; the
   verdict says so.
3. **Stored with the member's verdict** (in its area and in
   `build/fabrica.db`), so `iudicare` can name sections without
   re-running: `VERDICTUM probationes_radicis: 238/239 - non recentia:
   probationes_radicis/probatio_x (sectio 'y' FRACTA 3/5 at file:line)`.
4. **`-machina` gains the record type `SECTIO`** (after its member's
   `IUDICIUM` line): `SECTIO <membrum> <titulus> <exitus> <praeteriti>
   <totales> <ms> <filum:versus>`. Fumus XL's "only known record types"
   check gains it.
5. **Key unchanged**: per binary (Q3). Sections are explanation, not
   reuse units.

## IV. Places: enforced areas, a debt list

- Member areas are enforced as built (refusal on an outside write).
- The slice-5 exceptions become explicit DEBTS, each owned by one test:
  `<debitum_scripturae membrum="probatio_ct" via="build/probatio_ct.db"
  causa="..."/>` on the action. A member may write ONLY its own debts
  outside its area; another member writing there is still refused.
- `bin/fabrica census` reports the debt count (per action and total) -
  the number to drive to zero; a debt whose member no longer writes
  there is reported as stale (removable).
- Fixing a test = pointing its scratch at `TMPDIR` (already its area)
  and deleting its debt line. Not required in this slice; the first
  few are good warm-up tasks.

## V. Oracle, A/B, switch

1. **Oracle** on one tree: `compile_tests.sh` vs the composite - same
   pass/fail per test, same assertion totals per test (the runner's
   compendium vs the member's sections); plants: a broken test -> both
   red naming it; a broken library source -> both red, the composite
   naming only the members whose closure holds it.
2. **A/B reuse** over the same history as toml (`tools/reusus_retro.sh`
   extended to the root members): runner verdict reuse vs per-member
   reuse. Expectation, not promise: the root suite is where per-binary
   keys pay (239 binaries over 248 library files).
3. **Switch**: `PORTAE_GRADUUM['radix'] = 'probationes_radicis'`
   (as toml). `compile_tests.sh` stays callable (human entry; GUI
   apps, `--obiecta`, the source-list snippet stay in it - Q6).
4. **Then the second runner shrinks**: `aedilis_porta.sh` stops RUNNING
   tests (the composite does) and keeps derivation, emitted scripts and
   the `clang -MM` comparison; its `instrumentum_domus` inputs and
   build/ vestigia move to where they belong (member annotations,
   debts). Two ways of running root tests during the slice (runner +
   steps), one after it.

## VI. Not in this slice (recorded direction)

`PORTAE` as a derived view and owed gates from the graph (retiring
the inventory lenses); snapshot builds by default; volumes and the
filum backend (slice 8); a GUI-app step kind; per-section keys;
bulk conversion of root suites to sections (incremental, Q8); deleting
`compile_tests.sh`.

## VII. Order (plan to follow; headers to Fran before code)

- **T0** headers: `include/fabrica.h` (member verdict gains sections;
  `debitum_scripturae` in `FabricaActio`; the `SECTIO` machine record),
  and the aedilis annotation `instrumentum`.
- **T1** `probationes_c`: `CREDO_VERDICTA` in the base env, read +
  store sections, synthetic `totum`; `iudicare`/`VERDICTUM`/`-machina`
  output. Proven first on `probationes_toml` (unconverted -> `totum`)
  and on a 2-member fixture with sections.
- **T2** annotations: `instrumentum` (new, aedilis) on the tests that
  run house binaries; `facultas` on the 17 build-only tests (moving the
  fact from the scripts' hand lists into the sources).
- **T3** `debitum_scripturae` (declaration, enforcement per member,
  census count, stale debts).
- **T4** `probationes_radicis` declared; first full heal; refusals
  fixed or turned into debts.
- **T5** oracle runner vs steps + plants.
- **T6** A/B reuse retro.
- **T7** switch `radix`; `aedilis_porta.sh` stops running tests.
- **T8** spec "As built", worklogs, MEMORY.

## VIII. Done means

- `bin/fabrica iudicare probationes_radicis` judges 239 members; a
  failing member names its failing section(s), file and line.
- Gate `radix` is judged through the composite; the oracle agreed on
  one tree; the A/B numbers are recorded (whatever they show).
- Every outside write of a root test is either inside its area or a
  named debt; the debt count is printed by `census`.
- `aedilis_porta.sh` no longer runs tests; one way to run them (steps),
  plus the human entry.
- Plants for: section parsing (converted + synthetic + crashed),
  debt enforcement (own debt allowed, foreign refused, stale reported),
  `instrumentum` keying (binary changed -> only its tests stale).

## AUDIENDA (not verified)

- First full heal cost: 239 members through compilator with an empty
  area set (the runner takes 91-181 s; `aedilis_porta.sh` ~356 s).
  Later heals should be dominated by reuse - measured in T6.
- Whether every root test's closure links through aedilis alone:
  `aedilis_porta.sh` builds all of them that way (256 PASS at the last
  run), so believed yes.
- Tests that read the live repository (`repositorium` facultas) and
  `probatio_git` (desideratum …F654M) are link-only members until a
  fixed repository exists.
- Flaky tests: per-member reuse makes a flake visible as a member that
  fails on a re-run with unchanged inputs; no retry mechanism (credo
  wishlist: measure, never retry).

## IX. As built (2026-10-09, quinta; commits 175d048a .. 54d3574b)

What "Done means" became, item by item:

- **256 members, not 239** (257 root test files minus the benchmark).
  `iudicare` judges them; a failing member's tail names its failing
  sections (`sectio 'beta' FRACTA 1/2 ad t/probatio_a.c:12`), and
  `-machina` emits `SECTIO` records after each member's `IUDICIUM` line.
  Unconverted suites get a synthetic `totum` record with compendium counts.
  Sections live in the member's area (`credo.tsv`) only - no reader needs
  them in `build/fabrica.db` yet.
- **Gate `radix` = composite + `compile_tests.sh --residua`.** The audit
  before the switch found five checks a full runner pass did besides the
  tests (all libraries compile, generare, speculum + JS syntax/probes,
  plutil and iconutil oracles); `--residua` keeps them in the gate (Fran,
  option 1; ~15 s). Warm radix gate 76 s vs the old 118 s median.
- **Oracle 256/256** (`tools/radix_gradus_oraculum.sh`: category and
  assertion counts per test). Plants: a broken test is named by both and
  re-runs 1 member; a broken `lib/base64.c` is named identically by both
  and re-runs exactly the 5 members whose closure holds it (the runner:
  240).
- **A/B over 150 commits:** runner verdict reused 0/150; members 91%
  reused; test runs 38400 -> 3261 (-91%); estimated time -92%
  (flattering - member compiles not counted). Top invalidator:
  `aedilis.stml`, keyed whole by every member.
- **Debts: 25**, each owned by one member, all in use
  (`census: debita 25 (actiones 1, stala 0, ignota 0)`). The 36 shared
  porta_aedilis exceptions became 36 owned debts; the census then showed
  18 false "stale" (transient writes - created and deleted inside the
  run); the reads log fixed 11, and three tests moved their scratch to a
  per-process `$TMPDIR` dir (Fran, option 1), removing 11 debts.
- **`aedilis_porta.sh` no longer runs tests** (74 s, was 180-210 s); its
  GUI/reticular/live-repository lists are now `facultas` annotations.
  `compile_tests.sh` remains the human entry.
- **Plants** for every item above, each red then green (`silva.planta`).

Changes to the design during the slice:

- `facultas repositorium` RUNS with the HEAD commit in the key (Fran,
  T5) - the AUDIENDA guess "link-only until a fixed repository exists"
  would have stopped running `probatio_git` after the switch.
- Members inherit their parent's `praecondiciones`; sources under `build/`
  are keyed by existence only (porta_aedilis' approximation).
- A debt is "in use" if the before/after snapshot changed OR the member's
  reads log names a path inside it.
- `<debitum_scripturae>` is in `aedificatio.canon` (T3 had taught only
  the parser).

AUDIENDA, resolved: first cold heal 7 min 5 s (runner ~2 min); every root
closure does link through aedilis alone; flakes - none seen in four full
heals and four oracle runs.

Open, filed in the ledger: transient writes outside a member's area go
unseen by the write check (…HC0Q); speculum members key the generated
capsule by existence, so a behaviour change in its inputs re-runs none
of them (…0DESC); the gate inventory lives in the ledger, not the repo
(…MXPC). The ledger's inventory rows for radix and aedilis are
re-derived in main after the merge.

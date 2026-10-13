# cursores ad gradus - subsystem runners become fabrica steps (spec)

*Fran 2026-10-12, after circulus commissionis: "converting more runners
is a good idea". Question that prompted it: three generations of test
runner coexist, and the expedition 'migratio ad compilatorem' has sat at
5/20 since 10-03.*

Goal: **one generation.** Every `<sub>/compile_probationes.sh` is judged
as a composite `probationes_<sub>` (step kind `probationes_c`, like toml
and radix). The script stays as a thin human entry point (Fran
2026-10-12: option (a); "adjust later if it's never getting used").
excubitor is deleted when its last consumer is gone.

## I. What exists (measured 2026-10-12, quinta ce638f49)

| generation | runners | how staleness is judged | roots |
|---|---|---|---|
| 1. excubitor | briar gesta html ludus_tessera materia md officina oratio saltuarius silva tessera | mtime + "newest header" rule + excubitor post-check (html: `html/compile_probationes.sh:93-200`) | hand `-I` lists (3-9 each) + hand source indexes (`RADIX_FONTES`, materia sub-lists) |
| 2. cursor_communis | css, crusta (+ facies.sh, oraculum.sh), compile_tests.sh (store only) | digests via bin/compilator; closures from aedilis (`tools/cursor_communis.sh`) | hand `-I` lists still (`css/compile_probationes.sh:41-46`) |
| 3. steps | toml (`toml/aedificatio.stml:68`), radix (`aedificatio.stml:957`) | per-member verdict keyed on what it read | **from aedilis.stml** |

Facts the design rests on:
- **A step member compiles with aedilis.stml's roots** (`<inclusa>`,
  `tools/fabrica_ansae.c:269-276`). Converting a runner deletes its hand
  `-I` list and its hand source indexes as a side effect: the closure
  comes from aedilis, the roots from one file. The roots derivation
  (portfolio item 1) is then only needed by what STAYS bash
  (`compile_tests.sh` residua, `silva/haruspex.sh`, `compile_tools.sh`).
- **The composite-as-gate logic lives in Python**
  (`pythonica/silva.py` `_porta_per_gradum`, ~90 lines: delete verdicts
  for `vis`, `sanare -machina`, find VERDICTUM, list members run, build
  the summary). A bash entry point would have to reimplement it - the
  thin-wrapper test fails (regular logic belongs in C; ledger …Z2Y555).
- Member output already lands in `build/fabrica/acta/<compositum>/
  <membrum>.log`; the verdict text in `build/fabrica/area/<c>/<m>/
  verdictum.txt` ("transiit (totum 137/137)").
- toml's old runner is kept only as the oracle of its composite
  (`tools/toml_gradus_oraculum.sh`; park …MX8N "delete later").
- `mensor_suitae` metrics (`css.` etc.) are read only by
  `silva.mensurae` and its own test (`pythonica/probatio_silva.py:592`);
  fabrica's `cursus` table already times every member.

Runner classes for conversion:
- **A - library + tests:** md, materia, html, oratio, css, crusta,
  saltuarius (amalgama is a source like any other), gesta (sqlite =
  vendor rule, already in aedilis.stml).
- **B - the runner builds instruments the tests run:** briar
  (silva_conflatio, compendium, silva_lexicon), officina (praeparator,
  vindex_visum, sessio, nexus_ordines ...), tessera (folium, musivum,
  effigies), ludus_tessera (musivum). Steps only know house binaries in
  `bin/` (`<aedilis instrumentum="bin/X"/>`); these need the instrument
  declared as an artifact first.
- **silva:** class A, but the largest (55 tests, parallel already, its
  own `singulae/` logs).

## II. Design

### D1 - one C command: `bin/fabrica probare`

```
bin/fabrica probare [-vis] [-audit] [-machina] COMPOSITUM [FILTRUM...]
```
- heals the composite's stale members (and its `post` producers),
  reuses fresh ones, then prints one line per member
  (`transiit` / `FRACTA` / `servatum` = reused) and the summary line
  `<COMPOSITUM>: N/M praeteritae [cursa K, servata M-K]`;
- a member that RAN prints its log (`build/fabrica/acta/...`) as the
  old runners printed test output; a reused member prints one line;
- before the summary, the reuse is SAID, with the judge's own reason
  (it is already in hand: `iudicare` prints "RECENS ... transitus:
  lectiones congruunt (vestigium)" per member), e.g.
  `servatae 12/14: ingressus immutati post transitum ultimum (vis:
  -vis)` - so a reader never mistakes "not run" for "ran and passed";
- `FILTRUM` = substring on member names (the old runners' contract);
  no match = exit 2 "NULLA PROBATIO CURSA";
- `-vis` re-runs every selected member; `-audit` = `sanare -audit`;
- exit 0 all pass, 1 failures, 2 nothing judged.

`_porta_per_gradum` keeps only what is Python's (residua, Porta object,
timings) and parses `probare -machina`. One implementation for gates and
humans.

### D2 - the entry point

`<sub>/compile_probationes.sh` becomes ~20 lines: the `<porta>`
annotation (unchanged - portae-declaratae still reads it), the
`build/test_logs/<sub>.log` tee, the cursor lock, then
`exec bin/fabrica probare probationes_<sub> "$@"`. Its old body is
deleted in the same commit that converts it. `PORTAE_GRADUUM` gains the
entry. Muscle memory (`./css/compile_probationes.sh selector`) keeps
working.

### D3 - one oracle for every conversion

`tools/gradus_oraculum.sh <sub>` generalizes toml's oracle: the OLD
runner taken from a commit (`git show REF:<sub>/compile_probationes.sh`
run in this tree; default HEAD) against the composite, pass/fail per
test must agree. A conversion commits only with the oracle agreeing.
toml's private oracle is deleted (…MX8N closed).

### D4 - declarations per subsystem

`<sub>/aedificatio.stml` gets `probationes_<sub>` with
`<probationes_c exemplar="<sub>/probationes/probatio_*.c"/>`, plus what
the old runner did by hand: `ambitus` variables, `post` producers,
`debitum_scripturae` for writes outside the area (or the test moves its
scratch to `$TMPDIR`). Anything the old runner did that a declaration
cannot say is a finding, recorded in the spec's "As built", never
smuggled into the entry point.

### D5 - metrics

Converted suites stop writing `mensor_suitae` metrics; their timings are
fabrica's `cursus`. `silva.mensurae` keeps working for runners not yet
converted (only its own test reads it).

### D6 - what stays bash gets derived roots

Last task: `bin/aedilis --radices` prints aedilis.stml's include roots;
`compile_tests.sh`, `silva/haruspex.sh` and `compile_tools.sh` read it
instead of hand lists; a check refuses a hand `-I` list in a runner.

## III. Not in this slice

Class B runners (instrument declarations need their own small design:
`instrumentum` beyond `bin/`); the remaining class A runners after the
pilots go row by row through the expedition (rubric v3 = "to steps"),
not as tasks here; excubitor's deletion is the expedition's last row.
The `IUDICES_STRUERE` table asking fabrica for judge recipes (circulus
leftover) is a separate park.

## IV. Done means

- `bin/fabrica probare` exists, tested (fabrica fumus stage + plants),
  and `_porta_per_gradum` uses it (toml and radix gates unchanged in
  verdict);
- toml, css, md and silva are entry points + composites, each with the
  oracle agreeing at conversion, each measured cold and warm before and
  after;
- the expedition carries rubric v3 and its rows for the rest;
- roots derived for what stays bash (D6).

## V. Decisions (Fran 2026-10-12)

1. **Reuse by default for humans**, `-vis` forces a full re-run.
2. **Reused members print one line each**; full output only for
   members that ran. Plus Fran's addition: a line saying how many were
   not run and why ("inputs unchanged since their last pass"), taken
   from the judge's recorded reason (D1).

## AUDIENDA (not verified)

- Whether every class A runner's tests are hermetic enough for a step
  area (radix needed 25 `debitum_scripturae`; css/md/silva not checked).
- silva tests that run house binaries or read silva/amalgama artifacts:
  not audited; the oracle will name them.
- oratio's measured reuse ceiling (~2%, corpus reads) means its
  composite is correct but slow to reuse - not re-measured.
- Whether `crusta/facies.sh` and `crusta/oraculum.sh` are runners at
  all in the step sense (expedition rows say "own adaptation").
- Member log printing order under `FABRICA_FILA` parallelism.

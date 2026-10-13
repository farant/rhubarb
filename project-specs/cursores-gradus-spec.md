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

## VI. As built (2026-10-13; T1 30ea606f, T2 18019b81, T3 0c81e1bd, T3b 94789d9c, T3c 1f438e79, T4 d8c73a8e, T5 a828a58e, T6 a89565a5)

**Mechanism (D1-D3).** `bin/fabrica probare COMPOSITUM [FILTRUM]` is the
one command for the gate and for humans: stale members run and print
their output, fresh ones are kept and said so ("servatae K/M: inputs
unchanged since the last pass (<judge's reason>)", Fran's addition),
`-vis` forces, `-machina` gives MEMBRUM/SERVATAE/PROBARE. pythonica's
`_porta_per_gradum` is one call. `tools/gradus_oraculum.sh SUB` (old
runner from a commit vs `probare -vis`) agreed on every conversion.

**Converted** (entry point = tee + cursor lock + `bin/fabrica probare`):

| suite | oracle | warm, nothing changed: before -> after | forced (`-vis`) |
|---|---|---|---|
| toml | 13/13 | 38 s -> 2.3 s | 54 -> 37 s |
| css | 10/10 | 4.2 s -> 1.2 s | 8.2 s |
| md | 14/14 | 51.5 s -> 1.2 s | 43.7 s |
| silva | 55/55 | 417 s -> ~7 s | 355 s |

**Fabrica fixed on the way** (each measured with `sample`, each with a
red-first test):
- T3b: `lectiones`/`particulae`/`cursus` had NO index (~2.5 s per call);
  `probare` explains only the composite it names (`gradus_solus`; a broken
  unrelated suite no longer blocks); `numeri_latinae` and (T5)
  `tabula_nodorum` memorabilis; clang identity once per process.
- T3c: the whole-tree snapshot skips the store (64% of entries, where
  every action may write) - forced css 13.5 -> 8.2 s.
- T4: declared `<ambitus variabilis>` VALUES were in no key - the
  golden-rewrite ritual (COMPUTUS_SCRIBERE=1) silently kept the fresh
  member. Values now go into the member's mandatum (fumus LI).
- T5: with T4 the ritual ran but failed the write check -
  `<debitum_scripturae>` for every computus golden.

**What conversions surfaced** (now rubric v3's inspection checklist,
expedition …W0ZBW): hidden object linkers (8 tools, the materia oracle
gate - which would have linked STALE silva objects silently); runner-
written inputs (md corpus list -> generator `md_corpus`); writes outside
the area; env and test-as-tool modes; callers that read OUTPUT not
verdict (`silva.oraculum`, `silva-m3` -> `-vis`); binary-path helpers
(`silva.probatio_currere` ran the old runner's stale binaries ->
`_binarium_probationis`); undeclared house binaries (3 silva tests run
bin/aedilis); a split implementer the link-everything runner hid
(`<aedilis corpus>` in silva_arbor.h).

**D6.** `bin/aedilis --radices` + `vexilla_radices`: compile_tests.sh
(was 8 of 32 roots), compile_tools.sh, silva/haruspex.sh and 8 tool
scripts derive their roots. 129 scripts had hand `-I`; 118 remain as
named debt in tools/radices_manu.tsv, gate vexilla VI a two-way ratchet.

**Departures.** T3b/T3c inserted (Fran); the cursor lock was dropped in
T2 contrary to D2 and restored in T5 (gate sera VIII; without `exec`, the
lock's trap must release it); main's examen-corpus was red twice from
another session's tests (probatio_dimissio fixed in main;
probatio_cellae fixed here, 286b54ef - Fran's option a).

**Parked.** Snapshot per action (~2.8 s of forced css; …CH1CJ); ordering
over all actions (~0.6 s; …WMEC7); ~6 s of non-memorabilis generators
upstream of silva's corpus tests (…JH96E); class B runners (briar,
officina, tessera, ludus_tessera) need instrument declarations first.

# portae declaratae - plan

Spec: `project-specs/portae-declaratae-spec.md`. Rhythm as always: one task
per turn, Fran approves each; task = failing test -> red -> implement ->
green -> plant -> commit via silva.commissio with owed gates. Work in
`../rhubarb-quinta`; merge by the ritual. Logic in C (…Z2Y555); Python only
calls, prints, and - for migration scaffolding - compares temporarily.

### T0: interfaces (APPROVED by Fran 2026-10-10)
- [x] A: `<porta titulus=... tegit=... currit=.../>` / `<porta manu
  causa=.../>` in runner headers. B: `include/portae.h` (PortaDeclarata,
  PortaeDebitum, portae_declaratas_legere, portae_debita_computare,
  sutura), `bin/fabrica portae debitae|declaratae [-machina] -portae
  <tsv>`, TSV contract DEBITUM / INTECTUM / RECUSATIO.

### T1: declarations reader
- [x] C tests first (fake sutura: in-memory files + registry):
  annotated runners -> PortaDeclarata list; refusals BY NAME for a
  malformed annotation, an unknown `titulus`, a PORTAE key with no
  runner, two runners for one key; manual gates (`manu causa`).
  `materia_annotationes` through the crusta client. Plants. Commit.
  As built (2026-10-10): include/portae.h (PortaeIngressus, PortaeSutura
  with `tabula_portarum` - `registrum` is a latina macro, PortaDeclarata,
  portae_declaratas_legere); lib/portae.c; probatio_portae (5 sections, 45
  assertions: sane read, strings/other annotations ignored, 9 refusals,
  all causes, causes across files). compile_tests.sh gained -Icrusta/fontes;
  compile_tests_fontes_generata.sh regenerated. Plants III red.

### T2: derivation
- [x] per declared runner: the runner itself; what it executes/sources/
  reads (crusta_effectus); sources it compiles and `-I` dirs (port of
  inventarium_suitarum `fontes`); programs its tests launch (port of
  `currit`, all suites not only the root); aedilis closures. C tests on
  fixtures mirroring the census cases (credo_fumus -> credo_convertere.py,
  ludus_tessera -> tessera/*, compile_tests -> tabulariumd). Record any
  C pain against the scriba checklist (FR-008). Commit.
  As built (2026-10-10): `portae_operturas_derivare` -> PortaeOpertura
  {exemplaria, fontes_c, ignota}; sutura gained `effectus` and
  `litterae_vocationum` callbacks (NIHIL = named unknown), PortaeIngressus
  gained `probationes` (suite dir); enumerator = all tracked paths, only
  `.sh` read. Closures matched in T3 (fontes_c). `-I` dirs: named gap,
  carried by `tegit`. probatio_portae 8 sections / 103 assertions; plants
  VI red (P5 exposed a fixture that never exercised its rule).

### T3: owed gates + CLI
- [ ] portae_debita_computare (derived + `tegit`/`currit`, exemplar globs,
  INTECTUM for uncovered paths, MANU for manual gates); `bin/fabrica
  portae debitae|declaratae`; TSV contract tested (columns, escaping).
  Commit.

### T4: shadow comparison + annotations
- [ ] temporary comparison script (old Python answer vs new C answer over
  all tracked paths); annotations generated ONCE from the ledger's
  judgement cells, reviewed as a diff, committed; differences zero or
  explained in the commit. Commit.

### T5: cutover
- [ ] silva.portae_debitae = wrapper over the CLI (~40 lines; registry TSV
  from PORTAE); DELETE inventarium-based coverage, _clausurae*, matching,
  portae_sine_ordine, tools/inventarium_suitarum.py, the comparison
  script; pythonica tests shrink to the wrapper; gate `portae-declaratae`
  (every PORTAE key has exactly one annotated runner) born red by a plant;
  fresh clone without ~/.rhubarb computes owed gates. Ledger inventory:
  removed (or generated view - Fran). Commit.

### T6: close
- [ ] spec "As built", worklogs, MEMORY, ledger …GGMGY94 closed; merge.

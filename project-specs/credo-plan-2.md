# credo plan 2

Spec: `project-specs/credo-spec-2.md`. Interview:
`project-specs/credo-2-interview.md`. Rhythm as always: one task per
turn, Fran approves; each task = failing test -> red -> implement ->
green -> plant -> commit through silva.commissio with owed gates. Work
in `../rhubarb-quinta`; merges by the ritual.

### T0: headers (APPROVED by Fran 2026-10-10)

- [x] `signa` and `exspectata_fracta` semantics; `CredoContextus.suita`;
  `credo_suitam_currere_cum`; `CREDO_GRADUS` / `credo_gradum_ponere`;
  `credo_area`; `CREDO_SIGNA_OMITTE`, `CREDO_ORDO`; SECTIO 11th field.

### T1: signa and OMISSA
- [x] credo_fumus stage: unknown signum -> suite FRACTA naming it;
  `CREDO_SIGNA_OMITTE` -> OMISSA in record and output; all omitted ->
  VACUA. Plant: omission silent (no record) -> red. Commit.
  As built (2026-10-10): unknown words refused in the table AND in
  CREDO_SIGNA_OMITTE; fabrica pass note counts omissions apart.

### T2: known failures
- [x] stage: failing section with reason -> NOTA_FRACTA, suite exit 0;
  passing one -> INOPINATA, suite exit 1; empty reason -> refused.
  Fabrica: member note counts NOTA_FRACTA. Silva census query + report
  tool (ledger cross-check, never a gate). Plants. Commit.
  As built (2026-10-10): census = tools/credo_fracturae_notae.py (silva
  rows; -tabularium report); compendium names expected fractures; 0
  known failures in the repo today.

### T3: suite fixture
- [x] stage: parare_suitae once, c->suita in every section,
  purgare_suitae always (also after an aborted section). Convert
  cliens_tabularii to one suite daemon; record time vs per-section.
  Plant: purgare_suitae skipped after abort -> red. Commit.
  As built (2026-10-10): cliens 0.96 s vs 1.81 s (-47%; start-up wait
  paid once); sections build their own client from c->suita; the section
  that kills a daemon keeps a private one.

### T4: steps
- [x] stage: CREDO_GRADUS in the fracture and as the 11th field;
  fabrica reader accepts 10/11, tail names the step. vicus_iussa names
  I-VIII. Plants. Commit.
  As built (2026-10-10): fracture line "... at f:12 (in gradu VI: ...)",
  field 11 = step of the FIRST fracture; fabrica appends "[gradus ...]"
  to the fractura; step reset at each section start/end and in
  aperire/claudere (old-style suites may use it too). Found on the way:
  Speratus/Receptus printed NULL via %s for empty values - fixed.

### T5: area
- [ ] stage: credo_area created lazily, removed after purgare (also
  after NECESSE); two sections get different dirs. Replace the four
  hand helpers (sententiae_horreum, villa_agens, git, cliens). Commit.

### T6: order
- [ ] stage: CREDO_ORDO=mixtus:<seed> reproducible order, seed recorded;
  run over the 10 converted suites, record what it finds. Commit.

### T7: lint
- [ ] silva checks 1-4 (spec II), each born red by a plant that
  compiles; wired into the existing lint/hook path. Commit.

### T8: converter
- [ ] silva-based converter + the commit-time "convertible" note (lint
  5); reproduces xar/stml conversions, refuses other shapes. Commit.

### T9: close
- [ ] spec "As built", worklogs, MEMORY; merge.

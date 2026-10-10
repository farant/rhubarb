# credo spec 2 - known failures, signals, suite fixtures, steps, areas

Interview: `project-specs/credo-2-interview.md` (answered 2026-10-10;
Q1 changed by Fran, Q2-Q10 as recommended). Builds on credo v2 slice 1
(657b8e49) and fabrica slice 7 (member verdicts carry sections).
Principle (ledger …RBA4W73): what changes what a build or test does
lives in the repo - credo reads nothing outside the binary.

## I. What changes (headers approved by Fran 2026-10-10)

1. **`signa`** (reserved field, now live): space-separated words from a
   CLOSED vocabulary - `lentus rete fenestra repositorium`; an unknown
   word fails the suite naming it. `CREDO_SIGNA_OMITTE="rete lentus"`
   skips matching sections as **OMISSA** (recorded, never silent).
2. **`exspectata_fracta`** (reserved, now live): a REASON written in the
   source (non-empty; a ledger id inside is only a pointer, never
   read). Section fails -> **NOTA_FRACTA** (suite passes); section
   passes -> **INOPINATA** (suite fails: the fracture is fixed, delete
   the line).
3. **Suite fixture**: `credo_suitam_currere_cum(titulus, sectiones,
   parare_suitae, purgare_suitae)`; parare once before the sections in
   a suite context (own piscina), its datum reaches every section as
   `c->suita`; purgare_suitae ALWAYS after the last. Sections read
   suite state, never mutate it (lint, VI).
4. **Steps**: `CREDO_GRADUS("VI: ...")` names the current step; the
   first fracture carries it ("FRACTA in gradu VI at file:line"). Naming
   only, no isolation.
5. **Area**: `credo_area(c)` - a section's own directory under TMPDIR
   (`<TMPDIR>/<suite>.<index>.<pid>`), created on first use, removed
   after purgare; NIHIL if it cannot be created.
6. **Order**: `CREDO_ORDO=mixtus[:semen]` runs sections in a seeded
   shuffled order (seed printed and recorded) - a tool run that finds
   hidden shared state, NOT the default and not a gate.
7. **Record**: `SECTIO` gains an 11th field `gradus` (empty when none);
   fabrica's reader accepts 10 or 11 fields. Suite exitus TRANSIIT iff
   every section is TRANSIIT, NOTA_FRACTA or OMISSA and at least one
   ran; all OMISSA -> VACUA.

`CredoSectio` tables are unchanged (both fields existed); existing
suites compile untouched. `CredoContextus` gains `suita` at the end.

## II. Around the API

- **Census of known failures**: a silva query over the repo lists every
  `exspectata_fracta` with its reason and site. A separate REPORT tool
  may cross-check ids found in reasons against the ledger ("points at a
  closed item - probably fixed"); never a gate.
- **Lint (silva)** over functions referenced from a `CredoSectio` table:
  (1) NECESSE in a suite with mutable file-scope state and no purgare
  (section or suite) writing it -> error; (2) a section function with
  no assertion reachable -> error; (3) suite datum (`c->suita`) written
  inside a section -> error; (4) `--- Probans` prints inside converted
  sections -> warning; (5) old-style suite touched by a commit ->
  informational "convertible".
- **Converter**: a silva-based tool for the two mechanical old shapes
  (title inside each top-level block / title before it), naming
  functions by its own rule (not from titles - English words broke the
  Latin lint), refusing everything else. Migration is INCREMENTAL: a
  suite converts when touched (the note of lint 5 prompts it). No bulk
  conversion.

## III. Fabrica

The reader already knows OMISSA, NOTA_FRACTA, INOPINATA; it gains the
11th field. Member notes count passing sections as TRANSIIT +
NOTA_FRACTA (+ OMISSA named); the census of debts is unaffected.

## IV. Not in this slice

Fork per section (rejected for now - Q6); table-case API (Q7: a section
per table, rows named by their assertion); bulk migration; skipping by
facultas per SECTION in fabrica (possible later via signa).

## V. Done means

- credo_fumus covers OMISSA, NOTA_FRACTA, INOPINATA, unknown signum,
  suite fixture (purgare_suitae after an aborted section), CREDO_GRADUS
  in the record, credo_area created and removed (also after NECESSE),
  CREDO_ORDO=mixtus (seeded, reproducible); each with a plant.
- cliens_tabularii uses one suite daemon (time measured against per
  section); vicus_iussa names its steps; the four scratch helpers are
  replaced by credo_area.
- Lint checks 1-4 live and planted; converter converts xar's and stml's
  originals byte-for-byte to what the scratch script produced (modulo
  function names) and refuses the other shapes.

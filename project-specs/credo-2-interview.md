# credo slice 2 - interview: what the test API needs after real adoption

Follows credo v2 slice 1 (657b8e49: `CredoSectio`, `credo_suitam_currere`,
per-section piscina, `parare`/`purgare`, `CREDO_NECESSE`, `CREDO_VERDICTA`,
`CREDO_SECTIO`; fields `signa` and `exspectata_fracta` RESERVED) and
fabrica slice 7 (member verdicts carry sections). Fran 2026-10-10: test
the API on real adoption BEFORE building slice 2 on it; keep silva
linting in mind for conventions the API needs.

## 0. Data (2026-10-10, before any question)

**Adoption.** 10 of ~500 suites are sections (root: the 6 fabrica pilot
suites + xar, stml, cliens_tabularii, vicus_iussa; subsystems: none);
491 old-style suites remain. Before 2026-10-10 the pilot used 0 of 41
parare/purgare and 0 CREDO_NECESSE - conversion had been mechanical.

**Adoption test 1 (probatio_fabrica_gradus, 78cae599).** Shared fake
world via two parare variants; purgare resets 14 file-scope fakes; 15
guard chains -> NECESSE (22 -> 7 guards; 6 setups -> 1). Plant: a
section aborted by NECESSE after setting a fake global -> with purgare
6/7 pass; without that one reset, two LATER sections fail.

**Adoption test 2 (248214ea), four shapes.**

| suite | shape | conversion |
|---|---|---|
| xar | 34 independent blocks, title inside | mechanical (script) |
| stml | 67 blocks, title before, shared `intern`, bare calls | mechanical (2nd mode); per-section intern harmless |
| cliens_tabularii | one daemon, dependent steps | 6 sections, daemon per section (+0.8 s); 6 build/ debts gone |
| vicus_iussa | scenario: one app, steps build on each other | ONE section; parare app, purgare closes volume |

Findings: VACUA caught a block that never asserted (xar
status_imprimere); purgare killed the daemon / closed the volume after
an aborted section (plants: without it the .volumen files stay in /tmp);
the `$TMPDIR/<name>.<pid>` scratch helper now lives in 4 test files;
section titles as function names imported 16 English words (lint
refused). Detail: lib/credo.worklog.md (2026-10-10 entries).

**C89 constraint.** `CredoSectio` tables are positional brace
initializers; fields ADDED AT THE END are zero-initialised in existing
tables (C89 6.5.7), fields inserted elsewhere break every table.

## Questions

**Q1. Expected failures (`exspectata_fracta`, reserved).** A section
known broken names a ledger id; it runs, and its exitus is NOTA_FRACTA
(fails as expected - suite passes) or INOPINATA (passes unexpectedly -
suite FAILS, so the debt is closed when fixed). (a) Recommended: as
reserved; the id is required (no anonymous "known failure"); census
counts them like debts. (b) Skip expected failures entirely (fix or
delete).

**Q2. Signa (reserved: "lentus rete").** Tags a section carries;
`CREDO_SIGNA_OMITTE=rete` skips them as OMISSA (named in the record,
never silently). (a) Recommended: small closed vocabulary checked at run
time (lentus, rete, fenestra, repositorium - the facultas words);
fabrica can then skip by facultas per SECTION instead of per binary. (b)
Free-form tags. (c) Defer.

**Q3. Suite-level fixture (adoption gap 1).** Expensive shared state
(daemon, app) has no home: rebuilt per section or one big section. (a)
Recommended: optional `credo_suitam_currere_cum(titulus, sectiones,
parare_suitae, purgare_suitae)`; its datum reaches sections as
`c->suita`; sections stay order-independent as long as they do not
MUTATE suite state (the convention a lint can check - Q8). (b) Keep
per-section only (cost: rebuild). (c) Allow ordered dependent sections
(rejected by slice 1's own rule).

**Q4. Named steps inside a section (gap 2).** Scenario tests keep
"--- VI ... ---" prints; a failure names a line, not the step. (a)
Recommended: `CREDO_GRADUS("VI: nuntius in linea status")` - records
the current step; the fracture becomes "sectio X FRACTA in gradu VI at
file:line" (in the TSV record too); no isolation, just naming. (b)
Leave as prints.

**Q5. Per-section scratch area (gap 3; needs a header change).** (a)
Recommended: `c->area` = a fresh directory under TMPDIR per section
(`<TMPDIR>/<suite>.<section index>.<pid>`), created lazily on first use
via `credo_area(c)`, removed after purgare; the four hand-written
helpers go. Under fabrica that is inside the member's area, so debts
shrink further. (b) Leave helpers in tests.

**Q6. Order and isolation.** Hidden shared state is the failure mode
both adoption tests found. (a) Recommended: `CREDO_ORDO=mixtus` (seeded
shuffle, seed printed and recorded) available, NOT default; a periodic
tool run, not a gate. (b) Fork per section (true isolation, costs
fork+exec; breaks tests that rely on in-process state across a section).
(c) Both.

**Q7. Table cases.** Many old suites loop over arrays of input/expected
inside one block. (a) Recommended: no new API - a section per table
stays a section; a failing row is already named by the assertion's
expression and line. Revisit if a converted suite proves otherwise. (b)
`CREDO_CASUS` macro producing one record per row.

**Q8. Lint (Fran's question).** Conventions a silva check could enforce
(all AST patterns over functions referenced from a `CredoSectio` table):
(1) NECESSE in a suite with mutable file-scope state and no purgare
writing it; (2) a section function with no assertion reachable; (3)
`--- Probans` prints inside converted sections; (4) old-style suite
touched by a commit -> "convertible" note; (5) suite-level datum
mutated inside a section (if Q3a). (a) Recommended: (1), (2), (5) as
errors in the existing hook/lint path (examen or a silva selector
gate), (3) as a formator-style warning, (4) as an informational line at
commit. (b) Only (1). (c) Defer lint until more suites convert.

**Q9. Migration of the ~490 old suites.** (a) Recommended: a real
converter tool (silva-based, not regex) for the two mechanical shapes,
naming functions by its own rule (`_sectio_<lemma>`) instead of the
title, refusing everything else; migrate INCREMENTALLY (a suite
converts when touched, the commit-time note of Q8(4) prompts it). No
bulk conversion. (b) Bulk-convert the mechanical shapes now. (c) Leave
old suites forever (two styles permanently).

**Q10. Header shape.** (a) Recommended: new `CredoSectio` fields only
AT THE END (existing tables compile unchanged); `CredoContextus` gains
`suita` and `area`; headers to Fran before code (as slice 1). (b)
Redesign the struct now while only 10 suites use it.

## Answers (Fran 2026-10-10)

**Q1 - CHANGED.** Fran: test outcomes must not depend on the ledger,
which lives outside the repo (and no mirrored copy of it in the repo
either - two versions of one thing). So `exspectata_fracta` holds a
REASON written in the source (non-empty string required; a ledger id
inside it is an optional pointer only). credo never reads anything
outside the binary; NOTA_FRACTA / INOPINATA come from the field alone.
The census of known failures is a silva query over the repo (reason +
location); cross-checking ids against the ledger ("points at a closed
item - probably fixed") is a report TOOL, never a gate. Recorded as a
principle (ledger …RBA4W73, born of …MXPC): what changes what a build
or test does lives in the repo; the ledger may point at it, never be
read to decide an outcome.

**Q2-Q10:** the recommendations, as written above.

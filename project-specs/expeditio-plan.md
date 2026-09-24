# Expeditio v1 — plan

> Inline on main, one task per turn with Fran's approval, no subagents.
> Spec: `project-specs/expeditio-spec.md`. Ledger: park "Expeditio v1" in
> region "inventory management for batch processes"; one `opus` per task,
> closed by `silva.commissio(opus=ID)` — which now adds the owed gates
> itself (portae debitae); pass the gates below anyway.

**Global constraints.** C89 in Latin via latina.h, house flags; `i32`/`i64`
UNSIGNED; chorda not NUL-terminated; new C files via `scribe`; credo tests
born red by a COMPILING plant; worklog first, gates last; formator
`-scribere` then `-delta HEAD`; words via `./oratio/quaere.sh` then
`./oratio/vocabula.sh -nova`; verify every scripted edit with `git diff`;
generate multi-layer test literals (JSON inside C) with a script, never by
hand. Gates: **gesta AND radix** (+ `frigida` when frigida.c changes).
After a gesta change: `renovare` the resident before using the live tool.

---

### Task 1: `gesta_expeditio` — the pure fold and validator

**Files:** create `gesta/fontes/gesta_expeditio.{h,c}` (via `scribe`),
`gesta/fontes/gesta_expeditio.worklog.md`,
`gesta/probationes/probatio_gesta_expeditio.c`; add the source where
gesta's runner and the tabularium source lists derive it (as
gesta_inventarium was).

**Produces** (mirrors `gesta_inventarium.h`):

    b32 gesta_expeditio_eventus_est (chorda genus_eventus);
        /* expeditio-photographia | rubrica-mutata | gradus-positus |
         * gradus-promotus | ordines-additi */
    b32 gesta_expeditio_applicare (JsonValor* status, chorda genus_eventus,
            JsonValor* datum, chorda actor, chorda creatum, Piscina* pn);
    constans character* gesta_expeditio_validare (JsonValor* status,
            chorda genus_eventus, JsonValor* datum, Piscina* pn);

State per spec §3 (`rubrica {textus, versio}`, `ordines [{clavis,
additus}]`, `gradus {clavis: {status, versio, actor, creatum, per?, nota?,
opus?}}`). Fold DEFENSIVE (replay never fails: unknown rows skipped);
validation STRICT, all causes at once (spec §4).

**Tests:** photographia sets rows + rubric v1; `rubrica-mutata` bumps the
version; a tick stores the CURRENT version (tick, bump, tick: two
versions); `apertum` reopens; promotion stores the opus; `ordines-additi`
appends with `additus`; every refusal of §4 named, two bad rows = two
causes. Plant: the fold stores a constant version → red.

### Task 2: gesta wiring + seeded genus `expeditio`

**Files:** `gesta/fontes/gesta.c` (`_statum_transformare` delegates the
five kinds; `_eventum_validare` checks the target is an `expeditio` and
calls the validator → custody notes), `gesta/fontes/tabularium.c` (seed
`GENUS_EXPEDITIONIS "expeditio"`, semen v11, `status_initialis`
`aperta`, machine `[aperta,clausa]`, `[aperta,relicta]`; SEMINA_NUMERUS is
derived), `gesta/probationes/probatio_gesta.c`.

**Done when** events written through `gesta_scribere` fold into the
entity, a bad event gets a custody note with its causes, replay is
byte-identical, and the genus exists after seeding.

### Task 3: MCP tool `expeditio` (write actions)

**Files:** `gesta/fontes/tabularium.c` (`_tab_expeditio`, tool-table
entry, dispatch), `gesta/probationes/probatio_tabularium.c`.

- `creare {inventarium, titulus, rubrica, filtrum?, implenda?, intra?}`:
  read the inventory's folded state; parse `lens=valor` / `lens!=valor`
  (absent cell ≠ every value); refuse unknown lenses and an `implenda`
  whose value kind ≠ its lens's (all causes); ONE `gesta_fascis_scribere`
  batch: creation + `expeditio-photographia` + `natum-de` → inventory +
  `intra` → region.
- `facere {ordines, per?, nota?}`: one batch = `gradus-positus` on the
  expeditio + `cella-posita` on the inventory for `implenda` (`fons:
  manu`, `per`: "expeditio <id>" + given per). No `implenda` = tick only.
- `omittere {ordines, nota}` (nota required), `reaperire {ordines}`,
  `rubrica {textus}`, `addere {ordines?}` (default: all rows new since the
  snapshot that pass the filter), `promovere {ordo}` (one batch: `opus`
  creation titled "<titulus>: <ordo>" with body = rubric + row, `intra`
  the expeditio, `gradus-promotus`), `claudere {vis?}` (refused naming the
  open rows).
- Refusals before writing, all causes; unknown argument keys refused (the
  house rule); branch writes refused.

**Tests:** each action through tools/call; `facere` writes the inventory
cell (read back through `inventarium {actus: tabula, forma: machina}`);
the batch is atomic (a refused `facere` writes neither event); filter
`!=` picks absent cells; promotion creates an opus intra the expeditio.

### Task 4: `tabula` view + parata lines

**Files:** `gesta/fontes/tabularium.c`, `probatio_tabularium.c`.

- `expeditio {res, actus: tabula}` per spec §5: header, one line per row
  (`+ - · >`, rubric version, per, promoted opus status), `(rubrica vN)`
  mark for rows done under an older version, footer "N ordines novi post
  photographiam" (live inventory vs snapshot through the filter).
  Deterministic (no clock).
- parata: an `expeditio` is PINNED like `opus` (`n->fixus` rule); class
  AD LABOREM while any row is `apertum`, AD CLAUSURAM when none; causa
  `13/20 facta (omissa 1, promota 1)` (promoted rows count as done when
  their opus is `perfectum`); an expeditio is EXEMPT from the
  "has open children → skip" rule (its promoted opera must not hide it).
- The inventory itself NEVER appears in parata (the `natum-de` link would
  pin it): asserted.

**Done when** a fixture expeditio renders exactly, its parata line reads
right in both classes, and the inventory stays out.

### Task 5: first job — the inline RNG inventory + "migratio ad sors"

**Data, no C.** Read the survey note (…F5PV89, region "random number
generator library") and `lib/sors.worklog.md`; create inventory
'generatores aleatorii inline' (`ordo_genus: via`) intra that region;
rows = the files holding inline generators, **knotapel excluded** (its
demos use no shared code on purpose — decretum …D59G3Z); lenses `genus`
(textus), `casus fixi` (ita-non), `migratum` (ita-non); fill `genus` and
`casus fixi` by reading each site. Then `expeditio creare` "migratio ad
sors" with `filtrum: migratum!=ita`, `implenda: migratum=ita`, and the
rubric of spec §1. **Done when** both `tabula` views and the parata line
render and Fran has looked. The migration rows are worked afterwards as
ordinary tasks.

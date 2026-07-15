# GESTA K2 SPEC — RELATIONES PROPRIAE (nexus generibus, salus, opera)

2026-07-15. Self-contained implementation source, same contract as
gesta-k1-spec-v2.md: build from THIS file, not from summaries. Interview
record: gesta-k2-interrogatio.md (10 decisions, all locked). Oracle:
../bun-libraries/smaragda.ts with line references pinned below (file as of
2026-07-15; if lines drift, section anchors are named).

K2 charter (park 01KXJ2RVM4Y0FWF28MNCKSWX2B): relationships with
roles/cardinality/health + tasks + attribute schemas in genus definitions.

## I. WHAT K2 IS

Four capabilities on top of the sealed K1 kernel:

1. **Typed link genera** — a genus may declare `species:"nexus"` with
   `partes` (roles: valid member genera + cardinality). Link instances are
   ordinary res of that genus; members arrive via two new event kinds
   (membrum-additum / membrum-remotum) and are projected into a `membra`
   index table (a fold consumer, K1-style).
2. **Attribute schemas** — any genus datum may declare `attributa`
   (name/type/required). Types: textus, numerus, veritas, tabulatum.
3. **Salus** — pure evaluation of a res's materialized state against its
   genus definition (+ queries). Nothing blocks; salus reports.
4. **Opera** — proven free: a golden defines an opus genus and walks the
   task lifecycle with ZERO new kernel code.

K1's flat nexus fold is SUBSUMED: table dropped, fold deleted, legacy
events tombstoned, the one live link re-expressed through the new
machinery. `gerere nexus` MCP grammar is unchanged (sugar).

## II. DECISIONS LOCKED (from interrogatio)

| # | Decision |
|---|----------|
| 1 | Nexus subsumed: one link machinery; legacy events = tombstones |
| 2 | emendatio-generis stays WHOLE-REPLACE; additive merge is client courtesy |
| 3 | Record-don't-block extends to membra (custodia notae, never refusal) |
| 4 | Salus only — NO Error genus (custodia notae are the error stream) |
| 5 | Opera = vocabulary; goldens-only proof, no tenant seed, no API |
| 6 | Attribute types: textus / numerus / veritas / tabulatum (no filetree) |
| 7 | MCP quiet: no new tools; passive salus surfacing in res/census |
| 8 | Names: nexus, partes, cardinalitas, membrum-additum/-remotum, salus, opus |
| 9 | Seed v2 emends the five LIVE tabularium genera with schemas (dogfood) |
| 10 | Compressed ritual: this spec → Fran commits → build in 3 chunks |

## III. DDL — MIGRATION 2 (verbatim)

Append to GESTA_MIGRATIONES (gesta.c) and bump GESTA_MIGRATIONES_NUMERUS
I → II:

```c
    ,
    "DROP TABLE nexus;"
    "CREATE TABLE membra("
    "  res_id  TEXT NOT NULL,"      /* the link res (ULID) */
    "  pars    TEXT NOT NULL,"      /* role name */
    "  membrum TEXT NOT NULL);"     /* the linked res */
    "CREATE INDEX idx_membra_membrum ON membra(membrum);"
    "CREATE INDEX idx_membra_res ON membra(res_id);"
    "DELETE FROM consumptores WHERE titulus = 'nexus';"
```

Notes:
- No PK on membra: duplicates are legal state (reducer parity, §IV) —
  the write-time custodia check flags them, the projection records them.
- Deleting the 'nexus' consumptor row retires that watermark; the new
  'membra' consumer has no row → `_hwm_capere` returns 0 → automatic
  backfill from seq 1 on first plicare (legacy events tombstone to
  nothing, §V).
- The .db is a rebuildable projection; stores older than migration 2
  upgrade in place via scrinium's versioned migration walk.

## IV. GENUS DATUM EXTENSIONS + EVENT VOCABULARY

### Genus datum (whole-JSON document, whole-replace semantics unchanged)

Three OPTIONAL new keys alongside K1's
{titulus, status_initialis, machina, reducer}:

```json
{
  "titulus": "nexus",
  "species": "nexus",
  "partes": [
    {"titulus": "a", "genera_licita": [], "cardinalitas": "unicus"},
    {"titulus": "b", "genera_licita": [], "cardinalitas": "unicus"}
  ],
  "attributa": [
    {"titulus": "verbum", "typus": "textus", "necessarium": true}
  ],
  "status_initialis": "vigens",
  "machina": [["vigens", "solutum"]],
  "reducer": "ordinarius"
}
```

- `species`: only recognized value in K2 is "nexus"; absent = ordinary
  res genus. (TS meta.kind, smaragda.ts:3818.)
- `partes[]`: {titulus, genera_licita (array of genus tituli; EMPTY =
  unconstrained, TS 3768), cardinalitas}. Only meaningful when
  species=nexus.
- `cardinalitas`: "unicus" (=1) | "aliquot" (>=1) | "quotlibet" (>=0).
  (TS one / one_or_more / zero_or_more, smaragda.ts:843.)
- `attributa[]`: {titulus, typus, necessarium(bool)}. Valid on ANY genus.
  NO praedefinitum/default_value in K2 (divergence D5).
- `typus`: "textus"|"numerus"|"veritas"|"tabulatum". Unknown typus values
  PASS validation (forward compat, TS 1796).

### New event kinds (genus_eventus)

| eventus | datum | semantics |
|---|---|---|
| membrum-additum | {"pars":"a","membrum":"<res_id>"} | append member |
| membrum-remotum | {"pars":"a","membrum":"<res_id>"} | remove member |

### Reducer delta (_rei_applicare, ordinarius) — TS parity EXACT

- **membrum-additum** (TS defaultReducer smaragda.ts:759–763):
  state.membra[pars] = append(membrum). NO dedup — adding twice yields
  two entries.
- **membrum-remotum** (TS 765–769): state.membra[pars] = filter out ALL
  occurrences of membrum. The pars key REMAINS with an empty array (TS
  does not delete the key).
- state key is `membra` (TS `members`). All other event kinds unchanged.
- No refold needed at deploy: no historical events carry these kinds.

## V. FOLDS

### membra consumer (new; GESTA_CONSUMPTOR_MEMBRA)

Registered in gesta_plicare alongside genera/res (K1 order genera → res →
membra; exactly-once fold+advance per _consumptorem_plicare). Applies:

1. `membrum-additum` → INSERT INTO membra (res_id, pars, membrum).
2. `membrum-remotum` → DELETE FROM membra WHERE res_id=? AND pars=? AND
   membrum=? (deletes all matching rows — parity with reducer
   remove-all).
3. `status` events whose datum.novus = "solutum" → DELETE FROM membra
   WHERE res_id=? **only if** the res's genus has species=nexus (consult
   genera table; genus absent or species absent → no-op). This clears
   the index when a link is dissolved; the res and its history remain.
4. **Tombstones**: legacy `nexus` / `denexus` event kinds → NO-OP
   (divergence D2). _nexui_applicare and GESTA_CONSUMPTOR_NEXUS are
   DELETED.

### FTS exclusion

gesta_fts_exhaurire skips res whose genus species=nexus (parity with TS
_shouldIndexInFts, smaragda.ts:538 — relationship kinds are structure,
not prose). Implement in the sordidae drain: look up genus species before
indexing; species=nexus → delete-then-skip.

### replicare

gesta_replicare unchanged in shape: clears membra (added to the clear
list in place of nexus) + refolds. Legacy events tombstone silently.

## VI. VALIDATION — WRITE-TIME CUSTODIA (record-don't-block)

Extends K1's validation-before-write. ALL failures append a custodia nota
(actor=machina) AND THE EVENT STILL LANDS (decision 3). New checks in
gesta_scribere's validation pass:

On **membrum-additum** to res R (genus G):
1. G has no species=nexus → nota "membrum in genere non-nexu".
2. datum.pars not in G.partes → nota "pars ignota".
3. datum.membrum not in res table → nota "membrum inexistens".
4. G.partes[pars].genera_licita non-empty AND membrum's genus title not
   in the list (case-sensitive; our genera are lowercase by convention)
   → nota "genus membri non licitum". (TS 3768–3780 throws.)
5. cardinalitas unicus AND state.membra[pars] already non-empty → nota
   "cardinalitas unicus excessa". (TS 3952–3958 throws.)
6. membrum already present in state.membra[pars] → nota "membrum
   duplicatum". (TS silently allows the duplicate; we allow AND flag.)

On **membrum-remotum** from res R:
7. membrum not currently in state.membra[pars] → nota "membrum absens".
   (TS 3990–3992 throws.)
8. Removal would drop pars below floor (unicus/aliquot with exactly one
   member) → nota "cardinalitas sub limite". (TS 3995–3998 throws.)

Creation-time floor checks DO NOT EXIST (members arrive as separate
events after creatio); floors are salus's job (D7).

## VII. SALUS (pure evaluation; oracle _evaluateHealthPure 4368–4415)

### Types (gesta.h)

```c
nomen structura {
    chorda typus;    /* querela slug, below */
    chorda nuntius;  /* human message, built in piscina */
    b32    gravis;   /* VERUM = erratum, FALSUM = cautio */
} GestaQuerela;

nomen structura {
    b32          sanus;
    GestaQuerela* querelae;  /* array in piscina */
    i32          numerus;
} GestaSalus;
```

### Querela slugs + rules

| slug | gravis | rule |
|---|---|---|
| attributum-necessarium-absens | VERUM | required attr missing, JSON null, or empty string "" (EMPTY-STRING LAW, TS 4378) |
| typus-attributi-pravus | FALSUM | value present but wrong JSON type: textus→string, numerus→number, veritas→bool, tabulatum→array; unknown typus passes (TS 1790–1798) |
| status-ignotus | VERUM | genus has a machina AND state.status is not status_initialis nor any endpoint named in machina pairs; genera without machina skip |
| cardinalitas-violata | VERUM | species=nexus only: per pars, floor (unicus/aliquot ≥1 member; unicus also ≤1) and ceiling vs state.membra |

Custodia notae are NOT counted (divergence D4 — TS's
unacknowledged-error check 4424–4432 is unported).

### API

```c
b32 gesta_salutem_aestimare(GestaMundus* mundus,
    constans character* res_id, Piscina* piscina,
    GestaSalus* exitus);
/* FALSUM only on res ignota / internal error */

nomen structura {
    chorda     res_id;
    GestaSalus salus;
} GestaInsalubris;

b32 gesta_insalubres_enumerare(GestaMundus* mundus,
    constans character* genus_aut_nihil, Piscina* piscina,
    GestaInsalubris** exitus, i32* numerus);
/* evaluates every res (optionally one genus), returns only unhealthy */
```

Scale note: full scan per call — fine at today's ~40 res; dirty-queue
approach is parked (§XV).

### Query API (reverse lookups over membra)

```c
nomen structura {
    chorda nexus_res;  /* the link res */
    chorda pars;       /* which role R fills */
    chorda genus;      /* link genus titulus */
} GestaNexusRei;

b32 gesta_nexus_rei(GestaMundus* mundus, constans character* res_id,
    Piscina* piscina, GestaNexusRei** exitus, i32* numerus);
/* links containing res_id (TS getRelationshipsForEntity 4008) */

nomen structura {
    chorda membrum;    /* the OTHER res */
    chorda pars;       /* the other res's role */
    chorda nexus_res;
} GestaSocius;

b32 gesta_socii_rei(GestaMundus* mundus, constans character* res_id,
    Piscina* piscina, GestaSocius** exitus, i32* numerus);
/* co-members across shared links (TS getRelatedEntities 4119) */
```

Genus-filtered listing needs no API: res table already has the genus
column.

## VIII. TENANT — SEED V2, MCP SUGAR, SURFACES (tabularium.c)

### Seed v2 (idempotent, at initialize, after seed v1 logic)

1. **nexus genus** (only if absent from genera table): the exact datum in
   §IV. Machina vigens→solutum, initialis vigens.
2. **emendatio-generis ×5** for quaestio/parcum/decretum/nota/desideratum
   — ONLY if the genus datum lacks an "attributa" key (idempotence
   test). Each emendatio carries the FULL existing definition (title,
   status_initialis, machina, reducer — whole-replace!) plus:

```json
"attributa": [
  {"titulus": "titulus", "typus": "textus",    "necessarium": true},
  {"titulus": "corpus",  "typus": "textus",    "necessarium": false},
  {"titulus": "tags",    "typus": "tabulatum", "necessarium": false},
  {"titulus": "ancorae", "typus": "tabulatum", "necessarium": false}
]
```

   actor=machina, origo=seminatio (same as seed v1).

### MCP sugar rewrite (tool grammar UNCHANGED)

- `gerere {res, actus:"nexus", verbum, alterum}` → THREE events through
  gesta_scribere:
  1. creatio {genus:"nexus", verbum:V} → new link res (fresh ULID)
  2. membrum-additum {pars:"a", membrum:RES}
  3. membrum-additum {pars:"b", membrum:ALTERUM}
  (Validation/custodia applies to each; initial status vigens comes from
  status_initialis per K1 creatio semantics.)
- `gerere {res, actus:"denexus", verbum, alterum}` → find the link res:
  membra rows where pars=a/membrum=RES and pars=b/membrum=ALTERUM share
  res_id, AND that res's verbum matches → emit status {novus:"solutum"}
  on the link res. Not found → tool-level error text (arg error, not
  custodia).

### Passive surfaces (decision 7 — no new tools)

- **res tool**: append a `nexus:` block from gesta_socii_rei ("--verbum-->
  titulus" arrow form using the link's verbum attribute where present)
  and, if unhealthy, a `salus:` block listing querelae.
- **census tool**: append line `insalubres N` (from
  gesta_insalubres_enumerare over all).
- **tabula.md NEXUS section**: rewritten to read membra + res. Two-parte
  links with a verbum render arrow form `A --verbum--> B` (titles via
  COALESCE join as in K1); anything else renders
  `res_id (genus): pars=titulus, ...` list form. Only links whose res
  status is vigens appear.

### One-time re-expression (operational, chunk C, not code)

The single legacy link (Sigillum ancorae symboli --impeditur-a-->
LEGATUSD) is re-expressed by ME through `gerere nexus` after deploy. The
legacy event stays in the annales as a tombstone. Bar: link visible in
tabula via new machinery.

## IX. GOLDENS (probatio_gesta.c additions; TS refs in comments)

| G | proves | oracle ref |
|---|---|---|
| G1 | membrum-additum appends, NO dedup (twice → two entries) | 759–763 |
| G2 | membrum-remotum removes ALL occurrences, pars key stays with [] | 765–769 |
| G3 | membra fold mirrors events; fresh consumer backfills from seq 1 | K1 fold law |
| G4 | legacy nexus/denexus events tombstone (no rows, no crash) on refold | D2 |
| G5 | custodia notae for checks §VI 1–8; every event still lands | 3731–3783 |
| G6 | status solutum clears membra rows; solutum on non-nexus res touches nothing | — |
| G7 | salus: necessarium absens incl. EMPTY-STRING law | 4378 |
| G8 | salus: all four typus checks + unknown typus passes | 1790–1798 |
| G9 | salus: status-ignotus (and no-machina genus skips) | 4402–4412 |
| G10 | salus: cardinalitas floor + ceiling querelae | — |
| G11 | insalubres_enumerare filters by genus, returns only unhealthy | 4447 |
| G12 | OPUS-GRATIS: define opus genus (pendens→susceptum→perfectum\|omissum, attributa assignatum/prioritas/effectus), walk claim→complete via existing events, salus green — ZERO new kernel code | 4514–4652 |
| G13 | gesta_nexus_rei + gesta_socii_rei both directions | 4008, 4119 |
| G14 | seed v2 idempotence: double initialize → zero duplicate events; emendatio fires only when attributa absent | K1 seed law |
| G15 | sugar: gerere nexus → 3 events + tabula arrow; denexus → solutum + row gone from tabula | — |

G14–G15 live in probatio_tabularium.c; the rest in probatio_gesta.c.

## X. DIVERGENCE LEDGER (vs oracle)

| D | divergence | why |
|---|---|---|
| D1 | member index is a FOLD consumer, not inline API writes (TS 3785–3804) | K1 exactly-once architecture; replicare rebuilds it |
| D2 | legacy nexus/denexus events = fold tombstones; live link re-expressed manually | annales immutable; projection chooses meaning; count=1 |
| D3 | membra violations recorded (custodia), never refused (TS throws) | K1 covenant, decision 3 |
| D4 | no Error genus; custodia notae not counted in salus | custodia already exists; TS built Error for lack of it |
| D5 | no praedefinitum (default_value) | unused by oracle's own reducer path; parked |
| D6 | no additive evolveGenus event (TS 4249–4331) | whole-replace sealed in K1; merge = client courtesy |
| D7 | creation-time cardinality floors unchecked; salus owns floors | members arrive as separate events post-creatio |
| D8 | genus defs are whole-JSON documents; genusReducer (TS 1802) unported | K1 genera-as-events with whole-replace |
| D9 | no genus LRU cache (TS _genusCache) | genera table IS the materialized cache |
| D10 | no kernel task API (TS 4568–4611) | genera-as-events makes opera vocabulary (G12) |
| D11 | duplicate members flagged via custodia (TS allows silently) | judge-don't-block gives us the flag for free |

## XI. CHUNKS

**A — machinamentum** (gesta.{h,c}): migration 2, reducer delta, membra
consumer (+tombstones, +solutum clearing), delete _nexui_applicare/old
consumer, FTS exclusion, custodia checks §VI, genus-datum parsing helpers
(species/partes/attributa), query API §VII-queries. Goldens G1–G6, G13.
K1 nexus-fold goldens rewritten to new machinery.

**B — salus** (gesta.{h,c}): typus validator, gesta_salutem_aestimare,
gesta_insalubres_enumerare. Goldens G7–G12 (incl. opus-gratis).

**C — tenens** (tabularium.c + live store): seed v2, sugar rewrite,
tabula NEXUS rewrite, res/census surfacing, live deploy + re-expression +
seal bars. Goldens G14–G15.

## XII. BARS (seal criteria)

1. All suites green (existing + new); examen ACCIPE on every touched file.
2. Live store: gesta_annales_verificare clean (annales == acta 1:1).
3. Live store: replicare == standing state (res, genera, membra all
   byte-equal after refold).
4. The impeditur-a link renders in tabula NEXUS via new machinery.
5. Salus report over the live store produced; any violations filed as
   findings (they are dogfood data, not embarrassments).
6. MCP tool list byte-identical to K1 (decision 7 verified).

## XIII. PARKS BORN HERE (filed in tabularium)

- Praedefinitum attributorum (default values, application semantics TBD)
- Nexus: instrumenta MCP propria (dedicated tools, on tenant pull)
- Tabula: multi-partium redditio (when a >2-role genus exists)
- Salus per sordidas (dirty-queue health at scale)

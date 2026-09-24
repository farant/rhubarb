# Expeditio — spec v1 (batch jobs over an inventory)

2026-09-24 · decided with Fran in conversation (decretum …D59G3Z), from
inventarium spec §7 and the vision "inventories as the ground for batch
jobs" (…S1). Name: `expeditio` (an undertaking carried through;
`cursus` is taken — K3 process instances keep a `cursus` field).

## 1. What it is

An **expeditio** is a job worked across the rows of an inventory: a
frozen **snapshot** of rows (optionally filtered through a lens) plus a
written **rubric** (what to do to each row). Rows are ticked as they are
done, across sessions; progress is visible as one line in parata.

User story (the first job): the inventory 'generatores aleatorii inline'
holds the ~20 inline RNG copies (knotapel EXCLUDED — its demos use no
shared code on purpose). An expeditio "migratio ad sors" over the rows
where `migratum ≠ ita`, rubric "pinned cases → `sors_lcg_classicus`,
else `sors_seminare` + `sors_intra`; run the owed gates; commit". Each
row: edit, commit, tick — and the tick writes `migratum = ita` into the
inventory. A hairy row is promoted to a real `opus`.

## 2. Decisions (…D59G3Z)

1. **Every tick records the rubric version** it was done under; editing
   the rubric bumps the version, so "rows done under an outdated rubric"
   is a query (Brighton's translation-rubric requirement in miniature).
2. **A tick writes the job's declared lens** (`implenda`: one lens + one
   value that "done" means) in the SAME batch as the tick
   (`gesta_fascis_scribere`: both or neither).
3. **Checklist scale by default**: row status lives in the expeditio's
   folded state (one entity per job, never one per row). A row can be
   **promoted** to an `opus` when it turns out to be real work.
4. **Growth is explicit**: rows that appear in the inventory after the
   snapshot (and pass the filter) are REPORTED by the view and appended
   only by `actus: addere` — a job must answer what it covered.

Not a K3 process: those are seed-defined step pipelines; an expeditio is
ad hoc and fans out over rows.

## 3. Storage

Seeded genus `expeditio` (semen v11) with a state machine
`aperta → clausa | relicta`. Entity fields: `titulus`, `corpus`. Linked
`natum-de` its inventory; placed `intra` a region at birth. Events on the
expeditio, folded by a PURE module `gesta/fontes/gesta_expeditio.{h,c}`
(precedent: `gesta_inventarium`):

| event | datum | fold |
|---|---|---|
| `expeditio-photographia` | `{inventarium, ordines: [clavis…], rubrica, filtrum?, implenda?}` | snapshot rows, rubric v1 (once, at birth) |
| `rubrica-mutata` | `{textus}` | rubric text replaced, `versio` + 1 |
| `gradus-positus` | `{ordines: [clavis…], status, per?, nota?}` | per row: status, rubric version, actor, creatum, per, nota |
| `gradus-promotus` | `{ordo, opus}` | row status `promotum`, opus id |
| `ordines-additi` | `{ordines: [clavis…]}` | append rows (with their `additus` date) |

`status` ∈ `factum | omissum | apertum` (apertum = reopen a tick).
Ticking a row already `factum` is allowed: it re-stamps the CURRENT
rubric version (how "redo under the new rubric" is recorded).
`filtrum` = `{lens, op: "=" | "!=", valor}` (an absent cell is `!=` every
value; a cell marked **non-applicabile** matches NEITHER — a row that is
n.a. for the job's lens is outside the job; decided in T3, 2026-09-24).
`implenda` = `{lens, genus, valor}`.

Folded state:

    inventarium, filtrum?, implenda?
    rubrica:  {textus, versio}
    ordines:  [{clavis, additus}]
    gradus:   {clavis: {status, versio, actor, creatum, per?, nota?, opus?}}

## 4. Validation (all causes at once; the door refuses, the engine judges)

- target is an `expeditio`; rows named exist in its snapshot;
- `gradus-positus`: status legal; `omissum` needs a `nota` (the cause);
- `gradus-promotus`: row not already `factum`/`promotum`;
- `ordines-additi`: keys new to the snapshot;
- `rubrica-mutata`: non-empty text;
- at birth (tool): the inventory exists; `filtrum` and `implenda` name
  existing lenses; `implenda.genus` equals that lens's value kind.

## 5. MCP surface: one tool `expeditio`

| actus | does |
|---|---|
| `creare` | `{inventarium, titulus, rubrica, filtrum?: "lens=valor"\|"lens!=valor", implenda?: "lens=valor", intra?}` — snapshot the (filtered) rows, write creation + photographia + links in one batch |
| `tabula` | the job: header (inventory, rubric version, N/M done, omitted, promoted), one line per row (`+` factum, `-` omissum, `·` apertum, `>` promotum with its opus status), a `(rubrica vN)` mark on rows done under an older version, footer "N ordines novi post photographiam" |
| `facere` | tick rows `{ordines, per?, nota?}`; writes `implenda` into the inventory in the same batch (`fons: manu`, `per`: the expeditio id + given per) |
| `omittere` | `{ordines, nota}` — skipped with a cause |
| `reaperire` | `{ordines}` — undo a tick (the lens is left as written) |
| `promovere` | `{ordo}` — creates an `opus` intra the expeditio (title "<expeditio>: <ordo>", body = rubric + row) and the promotion event, one batch |
| `rubrica` | `{rubrica}` — new rubric version (the argument is named `rubrica`, the event field `textus`) |
| `addere` | `{ordines?}` — append rows added since the snapshot (all new ones when omitted) |
| `claudere` | status `clausa`; refused while rows are `apertum` (names them), `vis` overrides |

## 6. Views

- **parata AD LABOREM**: one line per `aperta` expeditio while any row
  still NEEDS WORK — untouched, reopened, or promoted with its opus not yet
  `perfectum` — reason `13/20 facta (omissa 1, promota 1)`. Promoted rows'
  opera appear on their own as usual, and do not hide the expeditio.
- **parata AD CLAUSURAM**: an `aperta` expeditio with no row needing work
  (reason adds "ordines omnes tractati: claude"). Refined in T4: the
  spec's first wording ("no `apertum` rows") would have filed a job under
  "ready to close" while a promoted opus was still pending. `claudere`
  keeps its own rule (§5: refused while rows are untouched or reopened).
- **Promoted rows** count as done in N/M when their opus is `perfectum`
  (derived); `facere` on a promoted row is still how its lens is written
  — the view says "opus perfectum, gradus nondum factus" until then.

## 7. First job (acceptance)

Inventory 'generatores aleatorii inline' (rows = the survey's files,
knotapel excluded; lenses `genus` (textus: lcg-ansi | xorshift32 |
aliud), `casus fixi` (ita-non: are generated cases pinned?), `migratum`
(ita-non)), then `expeditio creare` "migratio ad sors" with
`filtrum: migratum!=ita`, `implenda: migratum=ita`. Acceptance: the
tabula and the parata line render, and Fran has looked. The migration
rows themselves are then worked as ordinary tasks, one or a few per turn.

## 8. Out of scope v1

Task scale chosen at birth (promotion covers it) · automatic
growth · derived rows (a job over a live query) · jobs spanning several
inventories · un-writing the lens on `reaperire` · forum/villa views.

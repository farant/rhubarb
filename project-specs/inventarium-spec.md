# Inventarium — spec v1 (tables of rows × lenses in the tabularium)

2026-09-23 · decided with Fran in conversation (decretum …FTB8XR3, from
design question …ZZ111); vision …S1 ("inventories as the ground for batch
jobs"), principle …Y6 (inventories are append-weighted mesofacts).
First inventory: test suites (desideratum …S4D7AZ).

## 1. What it is

An **inventory** is a table. **Rows** are the members (one kind of thing:
test suites, lib/ files, utf8 consumers, regions…). **Lenses** are columns
("registered in PORTAE", "uses vitrea", "has correctness tests"). Rows and
cells may be entered **by hand** or **proposed by a derivation**; manual is
first-class — once recorded it is saved, and a stale value is fixed by
hand when someone sees it. Completing an inventory means filling its
lenses; the frontier is the unknown cells per lens.

v1 builds the table (§2–§5) and makes test suites the first inventory
(§6). Batch jobs (checklist and task scale, promotion, growth) are §7 —
designed here, built in a later plan.

## 2. Storage: events on the inventory entity

`inventarium` is a seeded genus. The entity carries `titulus`, `corpus`
and `ordo_genus` (what a row key is: `via` | `symbolum` | `res` |
`textus`). Its table lives in its folded state, built by four new event
kinds, applied in seq order like every other event:

| event | datum | fold |
|---|---|---|
| `ordo-additus` | `{ordines: [clavis…]}` | append rows (order of addition kept) |
| `ordo-remotus` | `{ordines: [clavis…], causa}` | drop the rows and their cells |
| `lens-addita` | `{nomen, genus_valoris, corpus?}` | append a lens |
| `cella-posita` | `{cellae: [{ordo, lens, valor}…], fons, per?}` | set cells |

A **value** is always tagged: `{genus: "ita-non", valor: "ita"|"non"|"ignotum"}`
or `{genus: "textus", valor: "…"}`. Tagging now means later kinds
(enumeratio, numerus, dies, res) arrive without rewriting history.

**Not applicable** (decided 2026-09-24, decretum …SD7JR, option 1): a
third kind `{genus: "non-applicabile", valor?: "<reason>"}`, accepted in
EVERY lens whatever its `genus_valoris`. It is an explicit value, so it
counts as FILLED: the frontier stops reporting cells that could never be
filled. Chosen over a per-lens applicability rule (which would need an
expression language) and over a sentinel inside each kind (which would
collide with real text in `textus` lenses).

**Provenance per cell**, stored in the folded cell: the event's `actor`
and `creatum`, plus `fons` (`manu` | `derivatum`) and optional `per`
(the job, task or commit that set it — references live here, not in
values). An absent cell is unknown.

Folded state shape:

    ordines: [{clavis, additus, actor}]
    lentes:  [{nomen, genus_valoris, corpus?}]
    cellae:  {<clavis>: {<lens>: {genus, valor, actor, creatum, fons, per?}}}

Trunk only in v1 (a branch write is refused, as `ad`/`intra` are).

## 3. Validation (at write time; all causes at once)

- the target exists and is an `inventarium`;
- `ordo-additus`: no key repeated within the event or already present;
- `ordo-remotus`: every key present; `causa` given;
- `lens-addita`: name not already a lens; `genus_valoris` known (`ita-non`, `textus`);
- `cella-posita`: row exists, lens exists, value's kind equals the lens's
  kind (except `non-applicabile`, legal in any lens; its optional `valor`
  must be a string), `ita-non` value ∈ {ita, non, ignotum}; `fons` ∈
  {manu, derivatum}.

## 4. Module layout

- `gesta/fontes/gesta_inventarium.{h,c}` — PURE: fold one inventory event
  into the state object; validate one event against the current state.
  No I/O, no scrinium; unit-tested on its own.
- `gesta.c` — the fold (`_statum_transformare`) and the write-time
  validation call into it for the four kinds (precedent: `ictus`, the K3
  `gradus-*`).
- `tabularium.c` — the MCP tool (§5) composes events and writes them;
  refusals are the module's causes.

## 5. MCP surface: one tool `inventarium`

    inventarium {res, actus, ordines?, lens?, genus_valoris?, cellae?,
                 fons?, per?, causa?, corpus?}

| actus | does |
|---|---|
| `tabula` | render the table (rows × lenses, `ita`/`non`/`·`, `n.a.` for not applicable, text truncated) + footer: unknown cells per lens (the frontier), `n.a. N` when any, newest and oldest cell date per lens. `forma: "machina"` = one TSV line per present cell, `ordo \t lens \t genus \t valor`, with `\\`, tab and newline escaped (read by `./gesta/frigida.sh -inventarium <res>`; added 2026-09-24 for portae debitae) |
| `ordines` | add rows (JSON array or comma list); keys already present are REPORTED and skipped (a proposer can re-push its whole derivation), new ones written in one event |
| `lens` | add a lens |
| `cellae` | set cells (JSON array of {ordo, lens, valor}); `fons` defaults to `manu` |
| `removere` | remove rows with a `causa` |

Created with `addere {genus: inventarium, titulus, corpus, datum: "{\"ordo_genus\":\"via\"}", intra: region}`.

## 6. First inventory: test suites

Rows: every runner-shaped script (`compile_*.sh`, `*fumus.sh`,
`*probare.sh`, `*_porta.sh`) — 35 on 2026-09-23 — proposed by a small
derivation run outside the MCP server and pushed with `actus: ordines`.
Lenses: `in PORTAE` (ita-non; derived from `pythonica/silva.py`),
`GUI aut manu` (ita-non; by hand), `tegit` (textus; what it covers — by
hand first, derivable later from each suite's source list). Acceptance:
the 7 unregistered scripts are visible with a reason each, and the
frontier shows what is left to fill.

## 7. Batch jobs (designed; later plan)

A job (`cursus`) = frozen snapshot of rows (optionally filtered through a
lens) + written rubric. **Checklist scale**: ticks inside the job, no
entity per row, % done derived. **Task scale**: one `opus` per row,
anchored, visible in parata. A checklist row can be **promoted** to a
task. Rows added after the snapshot are reported and appended; a removed
row's task closes `omissum` with its cause. A job may **fill a lens** —
completing an inventory and running a job are one motion.

## 8. Out of scope v1

Jobs (§7) · derived lenses computed by the engine (derivations stay
outside, as proposers) · value kinds beyond ita-non/textus · branch
writes · frigida (cold-path) verbs · table view in the forum app.

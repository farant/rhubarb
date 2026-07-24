# Genera per app definita — spec v2, THE BUILD BLUEPRINT (2026-07-23)

Supersedes genera-spec.md for implementation (v1 remains the intent record;
raw Q&A in genera-interview.md; park 01KY656BG1). Produced from three
explorer reports (machine / app / gates+wire), which converged everywhere
they overlapped. Line numbers are as surveyed 2026-07-23 — re-verify before
editing, but the seams are named precisely enough to survive drift.

## 0. What exploration changed since v1

1. **The substantive machine work lives in gesta/fontes/gesta.c, not
   tabularium.c.** tabularium.c is the MCP arg-plumbing/response layer;
   the genus registry (`genera` table), the fold, and the entire
   judged-not-blocked mechanism are gesta.c. v1 §III said "tabularium.c"
   throughout; wrong layer.
2. **The registry cost collapsed to near-zero.** The choke point exists
   (`_genus_datum_capere`, gesta.c:234) and the genera table is already
   folded per-event. We extend the registry judgment already trusts —
   no parallel registry.
3. **`datum` as a param is a solved shape**: gerere-mutatio accepts an
   escaped-JSON-STRING `datum` today (machine tabularium.c:3072; test
   escaping precedent probatio_tabularium.c:564-569, 1313-1317). addere
   copies the convention. String, not nested object — locked.
4. **All six v1 open questions resolved** (§II below).
5. **App-side C = one ~30-line handler.** The eight registered bridge
   handlers are fixed wrappers; a generic allowlisted passthrough makes
   every future genus/field addition a pure index.html change.
6. **Test runner correction**: these gates run via
   `./gesta/compile_probationes.sh tabularium` — root compile_tests.sh
   never sees gesta/probationes/.

## I. Layer map

| layer | file | owns in this feature |
|-------|------|----------------------|
| store/judgment | gesta/fontes/gesta.c | registry fold, usor marker, type judgment, unum cardinality, destructive-edit note |
| MCP machine | gesta/fontes/tabularium.c | definitio seed, addere `datum` arg, ab_lecto exclusion, legere nexus embedding |
| wire | gesta/instrumenta/fori_principale.c | NOTHING (verified verbatim byte-pipe) |
| app C | apps/forum/forum.c | one generic passthrough handler (+fumus verbs if needed) |
| app UI | apps/forum/assets/index.html | Res tab, creator, table, detail, entry form, picker, FTS, tombstones, fumus choreography |

## II. Open questions — resolved

- **Q1 registry seam**: extend the existing one. `_genus_datum_capere`
  (gesta.c:234, `SELECT datum FROM genera WHERE titulus=?`) is the SOLE
  lookup every judgment/reducer path uses (callers gesta.c:334, 601, 870,
  1773, 1796). genera table (`titulus TEXT PRIMARY KEY, datum TEXT`,
  gesta.c:35-36) is folded by `_generum_applicare` (gesta.c:769) on every
  plicare AND every batch write — rebuild-on-open and maintain-on-fold both
  already happen.
- **Q2 seeds vs entities**: ONE registry, TWO write sources, translate at
  the fold. Seeds stay events-with-no-res (machine axioms, res_id=NIHIL,
  attributa shape). User definitio ENTITIES (res of meta-genus `definitio`)
  get a dedicated branch in the genera consumer keyed on
  datum.genus=="definitio" that projects campi-schema → a genera row
  (titulus = datum.clavis, plus `usor=1`). genera folds BEFORE res
  (gesta_plicare order, gesta.c:1593→1598→1603), so the row exists before
  any entry of that genus folds. Do NOT migrate seeds.
- **Q3 wire/staleness**: proxy never parses arguments (fori_principale.c:
  110-170); daemon schemas omit additionalProperties (tabularium.c:
  5030-5065) so stale clients can pass `datum` already; staleness costs
  discoverability only. Refresh recipe: rebuild daemon → `pkill
  tabulariumd` (launcher respawns) → reconnect fori in Claude.
- **Q4 relatio read shape**: machine embeds. Do NOT stretch the
  respondet_ad LIKE trick (single verbum, LIMIT 1, id-not-title,
  tabularium.c:4314-4321). Instead: per-row membra pass inside the legere
  row loop (tabularium.c:4338) collecting `(verbum, ad_res, ad_titulus,
  ad_vivum)` tuples via one indexed query per row, embedded as a `nexus`
  array on each res object. N+1 of cheap indexed lookups; title-resolved;
  multi-target; kills the LIKE fragility the worklog already calls
  "cruditas v1". respondet_ad stays as-is for forum threading.
- **Q5 slug**: app-side (3-line JS: lowercase, spaces→_, strip
  non-[a-z0-9_]; shown once at creation). Machine treats clavis as opaque
  + immutable — the codebase already votes this way (genus/titulus/verbum
  are caller-supplied free strings; machine never derives identity).
  Machine-side guard: clavis collision at fold + clavis change via mutatio
  = custody notes.
- **Q6 table cost**: trivial at books-scale (existing poll already does
  stringify + full innerHTML rebuild every 2s). One correctness point:
  sort state lives in a JS var and is applied INSIDE the render; a header
  click re-renders directly WITHOUT refetch (the `ultima` diff guard would
  otherwise suppress it). annus sorts as signed number, never
  localeCompare.

## III. Machine changes — gesta.c (the substantive four)

1. **Registry fold branch** in the genera consumer (`_generum_applicare`
   region, gesta.c:769-795; currently fires only on definitio-generis/
   emendatio-generis via `_est_eventus_generis` :737): when a folding
   event is a creatio/mutatio of a res with genus=="definitio", upsert
   genera row from its datum (titulus=clavis, datum=the campi schema,
   usor=1). Schema change: `usor INTEGER DEFAULT 0` column on genera
   (gesta.c:35-36) — db is a rebuildable projection, no migration needed.
2. **Type judgment** in `_eventum_validare` (gesta.c:1763): new branch in
   the est_creatio block (:1766-1783, after the genus-ignotum check at
   :1778) and the mutatio path — resolve the genus via
   `_genus_datum_capere`, walk datum keys against campi: unknown key,
   string-where-number, non-integer annus/numerus, malformed dies →
   violation string. NOTE: one `const char*` return = ONE custody note per
   event; compose multi-field complaints into a single piscina string.
3. **Unum cardinality** — a NEW judgment, not the existing one. The
   existing "cardinalitas unicus excessa" (gesta.c:703, in
   `_membrum_validare` :546-724) is intra-nexus (one nexus can't have two
   'a' members) — wrong axis. New branch on the membrum-additum path
   (gesta.c:1808-1817): when pars-a res's genus declares the verbum as a
   unum-field, count standing vigens nexus with that verbum from that res
   (the respondet_ad subquery shape); ≥1 → custody note.
4. **Destructive definition edit** in `_eventum_validare`: mutatio on a
   genus=="definitio" res that removes/retypes an existing campi entry or
   changes clavis → custody note naming the violation. Write proceeds; the
   app never offers it.

Custody plumbing is free: violation string → synthetic nota event
(actor="machina", origo="gesta-custodia") appended in the same
batch/transaction (gesta.c:2041-2078).

## IV. Machine changes — tabularium.c (the plumbing four)

1. **Seed `definitio` meta-genus**: SEMINA_GENERUM (:101-234) new entry,
   `SEMINA_NUMERUS` XIII→XIV (:239). Post-index-6 ⇒ attributa INLINE in
   the seed datum (the articulus :216-223 / pipatum :229-233 exemplars;
   the ATTRIBUTA_V2 fusion :245/5267-5300 only touches board seeds).
   Machine does not validate attribute VALUE shape, so campi-as-array
   rides in entity datum untouched.
2. **addere `datum` param**: ARG_ADDERE table (:5072-5094, count X→XI at
   registration :5183), description idiom copied from gerere's existing
   datum arg (:5107, "pro mutatione: obiectum JSON crudum"). In
   `_tab_addere` (:2521): parse the escaped-JSON-string after signatura
   (~:2653), merge its keys top-level into the event datum object before
   serialize (:2654). The reducer's shallow merge (gesta.c:894) and
   legere's whole-datum embed (:4370-4378) then handle everything free.
3. **ab_lecto exclusion**: the ab_lecto branch (:4113-4150) already
   LEFT JOINs res (:4130); add `JOIN genera g ON g.titulus = r.genus` and
   `AND NOT (g.usor=1 AND t.genus_eventus IN ('creatio','mutatio',
   'remotio','nota','status'))` to the WHERE (:4131-4134). definitio
   creations stay as mail automatically (meta-genus is seeded, usor=0).
   Plain acta branch (:4151-4175) untouched — everything still reviewable.
4. **legere `nexus` array**: per-row membra pass in the row loop (:4338)
   per Q4 above. Keep respondet_ad (:4314-4321) untouched.

## V. Wire — nothing changes (verified)

fori_principale.c streams bytes verbatim (:110-170), never inspects
arguments; -sine-generatione (:88-92) keeps tests daemon-clean. The 256 KB
single-line response cap (forum.c:47 RESPONSUM_CAPACITAS) is the only
watch-item if a genus table ever pulls huge datum blobs — fine at ≤200
rows.

**Convention locked cross-territory: `datum` travels as an escaped JSON
STRING param** (app JSON.stringify's; machine json_legere's the string —
the `_articulum_servare` forum.c:577-579 / tabularium.c:3079 precedent).

## VI. App changes — forum.c (one handler)

**`_daemon_transmittere`** (~30 lines): registered via internuntius_praebere
beside the existing eight (:857-872). Args `{instrumentum, argumenta}`;
allowlist `instrumentum ∈ {addere, gerere, legere, quaerere}`; inject
`actor:"fran"` into argumenta; forward via `_instrumentum_vocare`
(:316-355, already takes arbitrary arg_obj). After this, new genera/field
types NEVER reopen the C. (Alternative rejected: 4-5 narrow wrappers —
more churn, same power.) Handler signature per include/internuntius.h:
61-65; NIHIL + *culpa = JS rejection.

If the fumus choreography wants dedicated verbs, register them in the same
block; prefer riding the passthrough.

## VII. App changes — index.html (the real work)

1. **View machinery**: `visus` (:127) grows from 4 values; `vertere`
   (:156-169) needs per-section .hidden lines AND the nav-active logic
   (:162-165) generalized from its current two-tab binary before "Res"
   lands. Res fans into sub-state: keep `visus='res'` + JS vars
   `res_genus/res_id/res_modus` rather than exploding visus values.
2. **Res tab**: genus list (definitio entities + entry counts) + "genus
   novum" creator. Definitions load through the same door as everything:
   `legere('definitio')` in a new `anquirere` branch on visus==='res'
   (dispatch :515-531); cache current schema in a JS var, diff-guard under
   `ultima.definitiones`. MCP-created genera appear within 2s free.
3. **Creator**: display-name → live slug preview (shown once, immutable);
   field rows (name, type select, relatio: target genus + cardinality);
   title-field designation (default first textus); optional default-sort
   field + direction. Emits ONE addere {genus:"definitio", titulus,
   datum:stringified-schema} via the passthrough.
4. **Auto table**: greenfield `<table>`; columns = title field, scalars in
   definition order, relatio columns from the embedded `nexus` array
   (target titles), tags last. Header click sorts client-side (Q6 rules:
   state in var, applied in render, re-render without refetch, annus as
   signed number). Default order from definition else creation order. Row
   click → detail.
5. **Detail**: all fields + reverse portal ("N entia huc monstrant") —
   needs the target-side query; v1 app-side: legere the pointing genus and
   filter its nexus arrays client-side (per-genus, not per-row — one
   legere per genus that declares a relatio at this genus). Edit button →
   entry form.
6. **Entry form**: own visus value, exempted in `anquirere` exactly like
   the editor guard (:506 `if (visus==='editor') return`) — do NOT fight
   the poll with dataset tricks; the type-ahead relatio picker is an
   <input> and the current `arca_activa` (:150-153) only protects
   TEXTAREA. Widgets by typus; annus input accepts negative + era label;
   reuse `codicilli` (:133) + counter/`.nimius` pattern for texts. On
   save: addere with datum (create) or gerere mutatio datum (edit), then
   nexus writes for relatio fields (denexus+nexus on unum reassignment),
   then `anquirere_nunc()` (:533) like every existing mutation.
7. **Type-ahead + inline create**: filter the loaded target-genus list in
   JS; "creare novum …" row = addere {genus, titulus, datum:"{}"} then
   link. No new machinery.
8. **FTS box**: quaerere {textus, genus} via passthrough, render hits into
   the same table shape.
9. **Tombstones**: clone the `[remotum]` pattern (elementum_commentarii
   :306-371, CSS :42) — relatio target with dead/absent datum renders
   '[deletum]', keep-if-children logic included.
10. **BUILD GOTCHA**: index.html edits are invisible until
    `bin/capsula_generare apps/forum/assets/forum.toml` re-embeds —
    struere compiles the pre-generated capsula_forum.c. Forgetting this
    makes edits look like no-ops.

## VIII. Gates

Runner: **`./gesta/compile_probationes.sh tabularium`** (also `fori`).
Tests drive the machine IN-PROCESS via `_mitte` (probatio_tabularium.c:
177-215 → tabularium_tractare), no sockets.

1. **New genera section** in probatio_tabularium.c: new `{ }` block after
   :1748, before credo_imprimere_compendium (:1750). Fixtures reused as-is
   (_mitte, _res_id_ex_responso :219-237, _plagula_litterae :63-90).
   Asserts:
   - definitio create → registry knows it: clone the gesta_aperire +
     `gesta_genus_datum` read pattern (:609-690, :1074-1087; NB returns
     non-NUL-terminated chorda — memcpy first).
   - clean typed write → no note; violation → **clone section XIII
     verbatim** (:548-573): write succeeds ("eventum … scriptum") →
     `census` "insalubres 1" → `res` shows "salus:"/"[cautio]" →
     corrective write → "insalubres 0". (The CCXL guard :1612-1638 is a
     HARD reject — wrong pattern, do not clone.)
   - unum double-link → custody note; additive definition edit clean /
     destructive edit noted; clavis-change noted.
   - ab_lecto: extend the existing cycle (:1733-1747) — define genus, add
     entries with datum, `acta ab_lecto` asserts entry titles ABSENT +
     definitio creation PRESENT; cursor semantics already proven there.
   - datum escaping literals: copy :564-569 / :1313-1317.
2. **probatio_fori datum round-trip**: extend the printf message series
   (:138-151) with an addere carrying `datum:"{\"annus\":-753}"`, bump
   `_lineas_numerare` expectation (:153), prove the value crossed via the
   annales-file strstr precedent (:160, the "Fable 5" clone).
3. **Fumus choreography** (index.html :538-562 + forum.c :916-979): after
   plenus — define genus → entries incl. annus:-753 → res_legere asserts
   sorted rows → inline-create author + link → reverse portal visible →
   fumus_perfectus. Budget: FUMUS_GRESSUS_MAXIMI 75 × 200ms ≈ 15s
   (forum.c:49 is the knob if the dance needs more).
4. **Live MCP gate**: after daemon rebuild + pkill + reconnect, Claude
   defines a genus and adds an entry via fori in normal conversation.

## IX. Build order

- **G0 — machine** (gesta.c + tabularium.c items §III-IV + gate §VIII.1):
  registry fold + usor, addere datum, three judgment branches, ab_lecto
  exclusion, legere nexus array, seed. Green = compile_probationes
  tabularium.
- **G1 — wire proof** (§VIII.2 + live): fori datum round-trip test; then
  rebuild/pkill/reconnect and the live MCP gate (I define `liber` from
  here).
- **G2 — app** (§VI-VII + §VIII.3): passthrough handler; Res tab, creator,
  table, detail, entry form, picker, FTS, tombstones; fumus extension.
  Green = FUMUS FORI PLENUS.
- **G3 — lived bar**: libri + linea temporis defined through the UI, real
  entries (Roma at -753), used across ≥2 sessions, ≥1 entry added by
  Claude via MCP in normal conversation.

## X. Gotchas ledger (cross-territory, collected)

- One violation string → one custody note per event; compose composites.
- Post-index-6 seeds need INLINE attributa.
- datum = escaped JSON string everywhere (never a nested object param).
- Sort re-render must bypass the `ultima` diff guard (no refetch).
- annus compares as signed number.
- Entry-form visus must be poll-exempt (arca_activa is TEXTAREA-only).
- capsula_generare after every index.html edit.
- 256 KB response cap (watch at large datum × 200 rows).
- MCP schema staleness = discoverability only; extra params already pass.
- gesta tests: ./gesta/compile_probationes.sh, NOT root compile_tests.sh.
- Existing "cardinalitas unicus excessa" (gesta.c:703) is intra-nexus —
  the unum-field judgment is a NEW axis, don't confuse them.

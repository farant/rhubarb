# pictor — spec v1 (raster editor; first ludus sub-project)

*2026-09-04. Consolidates the four design sections approved in
conversation the same day (ludus-brainstorm.md §XV, sections 1–4) plus
the foundational decisions in ludus-brainstorm.md §II, §IV, §X–XIV.
Rationale lives in the brainstorm; this document states what is
built. Names marked (unsealed) are working names — Fran names. Every
"exists" claim cites the header it rests on.*

## 1. Purpose and scope

**pictor** is a general raster editor — MacPaint / HyperCard in color
— on the low-pixel rhubarb target (`fenestra` + `TabulaPixelorum`,
RGBA8888, ~480 wide). It is the first sub-project of ludus and exists
to prove the loop **tool → volumen → bake → sealed binary** once, on
the cheapest target, while extracting the blocks every later tool and
artifact will share. Raster and pixel-art are different affordance
sets; this is raster.

**In scope (v1):** one canvas, one layer (layers modeled, one
shipped), a tool palette (pencil, brush, fill, line, rect, select,
eyedropper), a color palette from the document, brush size, Cmd+K
command palette, new/open/save dialogs, undo/redo, cut/copy/paste of a
selection, integer zoom + pan, a status line. Headless probationes at
every tier of §6. A sealed `capsula` binary produced as soon as the
canvas draws.

**Out of scope (v1):** curves, gradients, rotation, alpha compositing
in the command list, fonts beyond `fons_6x8`/`tabula_characterum`,
more than one shipped layer, a GPU path, the shebang authoring mode,
the general-2D and Flash targets. All are named pulls in
ludus-brainstorm.md.

## 2. Architecture

    fenestra ── Eventus (raw, stamped) ─┐
    replay ─── Eventus[] from log ──────┤
                                        ▼
       derivare(motus, raw) ──► [Eventus]              pure   §2.3
                                        ▼
       destinatio(arbor_prior, insulae, motus, ev)
           ──► { id_captum, id_geometricum, ascensus[] }  pure   §2.4 (strategy)
                                        ▼
       dispatch: walk ascensus; first componens with an
           actio for ev.genus ──► actio(repositorium, motus, id, ev)
                                        │  writes only via portae (§2.2)
                                        ▼
       componere(repositorium, motus) ──► arbor componentium   pure; after EVERY event
                                        ▼
       pingere(arbor, repositorium, thema) ──► mandata     pure; once per frame
                                        ▼
       rasterize(mandata, thema) ──► TabulaPixelorum       CPU
                                        ▼
       fenestra_praesentare_pixela

### 2.1 Two trees, never fused

The **logical tree** (`arbor componentium`) carries behavior: id,
actio, role, bounds, hit predicate, focusability, children. The
**drawing** (`mandata`) carries shapes. `pingere` derives the second
from the first; targeting, focus, capture, and tab order consult
only the first. **Load-bearing rule:** a componens OWNS its bounds;
`pingere` draws AT them. The trees cannot drift.

### 2.2 State: three genera, one tree, portae unicae

The truth is an **STML node tree in a piscina** (`stml.h`, exists —
9,393 lines), accessed only through a repository API
(`insula_repositorium`, unsealed; idiom = `entitas_repositorium.h`,
exists). Three genera, each with one gate:

| genus | gate | write discipline |
|---|---|---|
| durabilis | `mutare_durabile` | every discrete mutation; appends an event to volumen ACTA and updates the island |
| ephemerus | `mutare_ephemera` | every discrete user act |
| mobilis | `mutare_motum` | per event into an IN-MEMORY `Motus` struct owned by the dispatcher; FLUSHED to the ephemera island only by the quiet seat (`<quies>` — fires QUIES_MS after the last kinetic event; default 300, the lab's value; a thema/config number), or superseded by the durable write at release |

Serialization to STML text happens on save, on inspect, and on
RESTORE — where restore deliberately re-reads text and rebuilds, so a
lying writer is caught (lex rehydrationis). `canon.h` (exists)
validates the island schemas.

### 2.3 Events

`Eventus` (fenestra.h:193, exists — tagged union of magnitudo /
clavis / mus / rotula) gains a **`tempus`** field (s64 milliseconds,
`tempestivum.h`'s convention) stamped by `fenestra` in production
and by the log in replay. **Time is data in
the event; nothing below `fenestra` calls `tempus_nunc`.** `derivare`
synthesizes double-click (by timestamps), hover in/out, focus
gained/lost (by comparing old/new focus), drag start/end. It is pure
and independent of the targeting strategy.

### 2.4 Targeting — one function, swappable

`destinatio` is the whole strategy contract. Default: walk LAST
`componere`'s logical tree innermost-first, topmost-first (painter's
order = z-order), honoring clip-to-bounds and inverting the
translate/integer-scale stack; the innermost componens whose hit
predicate contains the point is `id_geometricum`; its ancestors with
ids are `ascensus`. If `motus.captura` is set, `id_captum` routes
pointer events without hit-testing while `id_geometricum` is still
computed (drop-target feedback). Key events skip hit-testing and go
to `ephemera.focus`, then bubble. Hit predicates escalate: rect →
polygonum → mask → `proprius` (the componens receives the local
point and resolves it). Componentia never call `destinatio`; swapping the
strategy touches no componens.

**Focus** is `ephemera.focus` (an id path) plus a focus STACK for
modals (`dialogus` pushes on open, pops on close — returned after
Enter AND Escape). Tab order = order of focusable ids in the last
logical tree. Focus on an id absent from the tree is a REQUEST: the
owning componens receives `focus_petitum` and adjusts (a kinetic
write) so the id exists next tree. **Capture** is
`motus.captura`, set/cleared by actiones.

**Staleness rule:** `componere` re-runs after EVERY dispatched event;
`pingere` and rasterize once per frame; frame 0 renders before input.

### 2.5 The command list

Typed structs in an `xar` (exists), piscina marked/reset per
rebuild; STML serialization (`mandata_scribere_stml` /
`mandata_legere_stml`) for snapshots, inspection, quaestio, and
replay expectations. Six primitives (§3.2). Color is a `thema` token
(exists — `thema.h`) or a tagged literal, resolved by the rasterizer.
Figurae (drawing composites) are a registry lowered by a pure fold to
fixed point, depth-guarded.

## 3. The two vocabularies

### 3.1 componens

    id . actio . partes . fines . praedicatum (RECTANGULUM | POLYGONUM
    | MASCA | PROPRIUS) . focusabilis . filii

`partes` (role) enumerated: nullum, bottone, campus, optio, index,
item, dialogus, titulus, imago, tabula. Role is separate from kind:
a hand-built group acting as a button carries `partes=bottone`.

### 3.2 mandata — six primitives

| genus | carries |
|---|---|
| `coetus` | nesting; optional clip-to-bounds; optional translate + INTEGER scale; optional PROVENANCE ref to its componens (inspector only; targeting never reads it) |
| `rectangulum` | bounds, fill and/or stroke, color |
| `linea` | two points, thickness, color |
| `polygonum` | n points, fill and/or stroke, color |
| `imago` | source (asset sigillum or pixel region), dest bounds, optional palette remap |
| `textus` | position, chorda, font, color |

No id/actio/role on any primitive. No push/pop commands: nesting is
`coetus` only, so mismatch is structurally impossible.

### 3.3 figurae

A registry `(partes, thema) → deponere fn`; `deponere` is `<purus>`,
receives the componens (and so its bounds), emits primitives or other
figurae. Optional `mensurare(argumenta, thema) → magnitudo` slot,
unused in v1 (fixed layout). Registry entries are tested once per
thema in the registry's own probatio (§6 tier 6), never per widget.
`elementa.h`'s five (bottone, campus_textus, capsa_optandi,
graticula_colorum, slider — exist) become the first five figurae AND
the first five componens kinds.

## 4. State schema

**The document is its stroke log.** Volumen (exists — `volumen.h`:
acta / massae / plagulae) is the document store.

| genus | contents |
|---|---|
| durabilis (island) | `latitudo`, `altitudo`, `paletta`, `strata` (list: nomen, visibilis, ordo — ONE shipped), `sigillum` of the current checkpoint, metadata. NEVER pixels. |
| ephemerus (island) | `instrumentum`, `color_primus`, `color_secundus`, `magnitudo`, `stratum_activum`, `selectio` (rect, or floating region ref + offset), `zoom`, `focus` (+ stack) |
| mobilis (`Motus` struct) | `ictus_pendens` (points), `pan`, eyedropper preview, zoom mid-gesture, `captura` |

**acta** (the durable event vocabulary; every event carries full tool
parameters — replay is deterministic): `<ictus instrumentum color
magnitudo>` + points · `<impletio x y color>` · `<figura genus=
"rectangulum"|"linea" ...>` · `<insertio sigillum x y>` · `<stratum
.../>` ops · `<paletta .../>` ops · `<selectio_commissa .../>`.
**massae:** bitmap checkpoints every N acta by sigillum; the pasted
region of an `insertio` (the one non-event-derived byte source —
DECISUS). **plagulae:** the current bitmap = projection = latest
checkpoint + replay since. **Undo/redo** = pop / re-append acta,
reproject from nearest checkpoint; granularity is one stroke.

## 5. Componentia and actiones

### 5.1 Componentia

| componens | partes | praedicatum | focus | note |
|---|---|---|---|---|
| `prospectus` | nullum | rect | no | viewport; owns pan (kinetic) and integer zoom on its coetus |
| `tabula` | tabula | **proprius** | default | actio CHOSEN AT COMPONERE TIME from `ephemera.instrumentum` — a pure function of state; handlers never branch on tool |
| `instrumentarium` | index | rect | yes | `optio` per tool; vim-style nav via `vim.h` (exists, pure) |
| `paletta` | index | rect | yes | `optio` per color from `durabilis.paletta`; click primus, shift-click secundus |
| `magnitudo_selector` | index | rect | yes | brush/line width |
| `tabella_mandatorum` | dialogus | rect | pushed | Cmd+K over `registrum_commandi.h` (exists); focus returned on Enter AND Escape |
| `dialogus` | dialogus | rect | pushed | veil consumes all pointer events; children `campus_textus`, `bottone`; `dialogus.h` (exists) is the shape |
| `status_linea` | nullum | none | no | no actio, not focusable, queryable |

### 5.2 Actiones (~25; L10's registry)

    instrumentum.eligere  color_primus.ponere  color_secundus.ponere
    magnitudo.ponere      penicillus.ictus     impletio.punctum
    color.capere          selectio.marquee     selectio.omnia
    selectio.nulla        selectio.secare      selectio.copiare
    insertio              revocare             reficere
    documentum.novum      documentum.aperire   documentum.servare
    stratum.novum         stratum.eligere      stratum.visibilitas
    dialogus.confirmare   dialogus.abrogare    focus.proximum
    focus.praecedens      zoom.ponere

**Convention (DECISUS): one actio per INTENT, phase by event genus.**
`penicillus.ictus` receives depressus / motus / elevatus as one
handler. Signature: `actio(InsulaRepositorium*, Motus*, id,
constans Eventus*) → consumptus`.

**The stroke:** depressus on `tabula` → `penicillus.ictus` sets
`motus.captura`, opens `ictus_pendens` → each captured motus
appends a point (struct only; `pingere` overlays it live on the
`imago`) → elevatus → ONE `<ictus>` into acta, struct cleared,
capture released. Escape mid-stroke clears the struct; no acta.

**Keyboard** (root-group actiones, reached by bubbling): tool hotkeys
`p b f l r s i`; Cmd+Z / Shift+Cmd+Z; Cmd+K; Escape = innermost of
(cancel stroke → deselect → close dialog); Enter; Tab / Shift+Tab.

### 5.3 Figurae v1

`figura_optionis` (swatch or icon + selected border) · `figura_indicis`
· `figura_tabulae` (one `imago` + marquee overlay + pending-stroke
overlay + cursor) · `figura_bottonis` · `figura_campi` ·
`figura_dialogi` (veil + panel) · `figura_status`. Tool icons:
`fons_6x8` glyphs first; bespoke bitmaps by sigillum in `capsula`
later.

## 6. Probationes, replay, codices

### 6.1 Tiers

| tier | input → output | proves |
|---|---|---|
| 1 derivare | `Eventus[]` → `Eventus[]` | derivation, pure, no widget |
| 2 actio | (repo, motus, ev) → (repo′, motus′) | one handler; e.g. a scripted stroke yields exactly one `<ictus>` with n points and an untouched ephemera island |
| 3 componens | `Eventus[]` → `arbor componentium` (STML) | the primary snapshot |
| 4 restore | build → destroy → rebuild from islands → compare tree | ends every tier-3 probatio (L8) |
| 5 pictor | event log → islands + tree (+ mandata) | the whole app headless; the archive IS the regression suite |
| 6 figurae | (componens, thema) → mandata | each lowering once per thema |
| 7 specimen | mandata → TabulaPixelorum → PNG | a few golden scenes, compared FROM THE TABLE (`specimen.h` exists; machine-independent because CPU) |
| 8 documentum | acta → projection → sigillum | a document verifies itself by replay |

Tiers 1–5 touch no window, font, or pixel.

### 6.2 Files and update discipline

    probationes/pictor/<nomen>.eventus.stml   input
    probationes/pictor/<nomen>.arbor.stml     expected logical tree
    probationes/pictor/<nomen>.insulae.stml   expected islands

Failure writes `<nomen>.arbor.novum.stml` + text diff; promotion is
`mv` (specimen's exemplar / candidatus / differentia, on text). Tier-5
logs are RECORDED from a live session via `imperium.h` (exists) and
trimmed by hand.

### 6.3 Manus, natively

`manus_premere(m, "#tabula", x, y)` · `manus_clavem(m, "p")` ·
`CREDO_MANUS_EXISTIT(m, "[partes=bottone][nomen=Save]")` ·
`CREDO_MANUS_FOCUS(m, "#tabula")` — quaestio selectors over the
logical tree (same selector language as the codices). In-process for
probationes; `imperium`'s HTTP loop for a sealed binary in `-vivum`.

### 6.4 Codices — landing order

1. With the first componens: annotation vocabulary `<componens/>`
   `<purus/>` `<quies/>` `<tractator/>` (annotationes-stml-spec.md
   chunks A–C precedent) + the four VIOLATIO codices — L1 porta
   unica, L2 `componere`/`pingere` pure, L7 no file-scope mutable
   statics, L9 commands-only + strategy reads the logical tree only.
2. With the action registry: L5 no I/O in handlers; L10 actions
   resolve both ways.
3. When the schema stabilizes: L3 one genus per field; L4 kinetic
   ISLAND writes only from `<quies>`.
4. With the presence-contract pull: L6 identity by via; L8 restore in
   every probatio (until then, enforced by the probatio template).

Registration = three enum lines + three table rows + a judgment seat
(silva-fluxus-1-spec-v2.md §6). `-domus` (officina-m4a-spec.md:249)
makes all of them block; pictor pulls it.

## 7. Work inventory — modules (all unsealed names)

New in `lib/` + `include/`, each with `probationes/probatio_*.c` and a
`.worklog.md`, Latin throughout, `<aedilis corpus>` directive:

| module | depends on | content |
|---|---|---|
| `mandatum` | piscina, xar, chorda, stml, thema | the six primitives; `Mandata` xar; STML serialize/parse |
| `figura` | mandatum, componens, thema | figura registry; `deponere` fold; the seven v1 figurae + elementa's five |
| `componens` | piscina, xar, chorda, stml, internamentum | logical node; tree build helpers; STML serialize/parse; role enum |
| `insula` | stml, piscina, canon | three-genera state over an STML tree; `insula_repositorium` API; `mutare_durabile` / `_ephemera`; restore |
| `motus` | piscina | the in-memory struct; `mutare_motum`; quiet seat + flush |
| `derivare` | (Eventus) | event derivation |
| `destinatio` | componens, motus | strategy interface + default logical-tree strategy; focus stack; tab order; capture |
| `actio` | internamentum, insula, motus | action registry (name → handler); L10 resolution |
| `dispensator` | derivare, destinatio, actio, componens, motus | the loop; owns `Motus`; staleness rule |
| `pictor_documentum` | volumen, sigillum, imago | acta → projection; checkpoints; undo/redo; `insertio` massae |
| `pictor_componentia` | componens, actio, insula | the eight componentia + `componere` for pictor |
| `pictor_actiones` | actio, pictor_documentum | the ~25 handlers |
| `pictor_figurae` | figura | the v1 figurae |
| `delineare_mandata` | mandatum, delineare, tabula_characterum, thema | mandata → TabulaPixelorum (extends `delineare.h`) |
| `manus` (native) | dispensator, componens, quaestio | in-process harness; `CREDO_MANUS_*` |
| `pictor` (bin) | all above, fenestra, capsula | the app; `-vivum` wires `imperium` |

Modified: `fenestra` (add `Eventus.tempus`); silva/examen (annotation
vocabulary + codices, §6.4); `elementa` (retire in favor of figurae +
componens kinds, after parity).

Retired after pictor ships: `widget.h` / `ManagerWidget` (consumers
migrate; `schirmata` becomes a consumer of componens).

## 8. Phase plan (test-first; each phase ends green)

- **P0 harness.** `Eventus.tempus`; `derivare` + tier-1 probatio;
  `mandatum` + `componens` STML round-trip probationes; the
  `probationes/pictor/` file convention and diff/promote script.
- **P1 state.** `insula` + repository + portae + restore probatio;
  `motus` + quiet seat; `canon` schemas for the three islands.
- **P2 loop.** `destinatio` default strategy + focus/capture/tab probatio;
  `actio` registry; `dispensator` with the staleness rule; native
  `manus`. Annotation vocabulary + the four violatio codices land
  here (§6.4.1).
- **P3 first componens.** `tabula` + `penicillus.ictus` against tier-2
  and tier-3 expectations; `pictor_documentum` acta → projection
  with checkpoints (tier 8); `figura_tabulae`; `delineare_mandata`
  for `imago` + `rectangulum`. **First bake here:** `silex_
  clausuram_colligere` → `amalgama` → `capsula`; record closure size,
  build time, binary size in §10 as the first workflow data point.
- **P4 palettes.** `instrumentarium`, `paletta`, `magnitudo_selector`
  + `optio` kind + `figura_optionis`/`_indicis`; vim navigation;
  focus and tab order proven. L5/L10 land.
- **P5 modals.** `tabella_mandatorum`, `dialogus`, `campus_textus`,
  `bottone`; focus stack proven (captured, returned on Enter AND
  Escape). Remaining tools (fill, line, rect, select, eyedropper),
  cut/copy/paste, undo/redo, new/open/save.
- **P6 finish.** `status_linea`; `specimen` golden scenes (tier 7);
  L3/L4; elementa parity + retirement; a recorded tier-5 log per
  workflow the periodical will use.

## 9. Named deferrals

Curves (`curva` figura → polygonum; vector target) · mensurare/place
pass (general-2D target) · GPU presenter (RHI; solarium `rhi.h` is
the reference) · shebang authoring mode (officina `machinula` + silex)
· second shipped layer · L6/L8 presence-contract machinery · `-domus`
driver · `widget.h` consumers' migration · solarium as a ludus
consumer.

## 10. Risks, notes, AUDIENDA

- **Volumen write rate.** Every stroke is an acta append. Whether
  `volumen` as-built sustains an interactive editor's rate, or needs
  a coalescing layer, is unmeasured. Measure in P3.
- **Checkpoint interval N** is a tuning number; start at 64 acta and
  measure undo latency.
- **`componere` after every event** assumes the logical tree is cheap
  (tens to low hundreds of nodes for pictor). Measure in P2; if it
  isn't, batch per frame with an ordering rule.
- **First-bake numbers** (closure size, build time, binary size) —
  to be recorded here at P3.
- **Names SEALED 2026-09-04 (Fran):** `ludus` (the set) · `pictor` (the editor) · `componens`/`componere` · `pingere` · `mandatum`/`mandata` · `figura` · `coetus` · `insula` · `actio` · `partes` · `destinatio` (targeting; `scopus` = scope in the house) · `mobilis`/`Motus` (the third genus; gate `mutare_motum`) · `restituere` (rebuild from islands; the lab's *remontatio* stays as history) · `dispensator` · `derivare` · `delineare_mandata` (the rasterizer, no new noun). Working names still open: `manus_ludus` (until the harnesses unify). All sealed names are in WORDS; no glossary entries needed.
**First-bake numbers (2026-09-05, P3 T11, `apps/pictor`):** closure 41 translation units + 45 headers of house code, 2,233,103 bytes of source (plus one vendor unit, `vendor/sqlite3.c`, 9,514,279 bytes, pulled by `volumen`); build with warm objects 0.11 s (aedilis emits `build/aedilis/pictor/struere.sh`; the 41 objects are shared with the root suite, a truly cold build is the suite's own); binary `bin/pictor` 1,854,936 bytes (1.85 MB, sqlite and the Cocoa glue inside); the single-file amalgam is REFUSED — `tools/amalgama_ligare.sh` will not bundle a vendor unit in v1 ("vendor in clausura - non ligabile"), so a one-file pictor waits on either an amalgamator that carries sqlite beside it or a volumen without sqlite. Frame timings from the smoke (30 frames, 640×480, one canvas): composition 0 ms, `pingere` 0 ms, rasterize 35 ms total (≈1.2 ms/frame), worst frame 2 ms — rasterizing dominates but is nowhere near a frame budget; the double-buffer option (brainstorm §XVI §3) stays parked.

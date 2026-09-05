# Ludus — engine building blocks, authoring tools, and the periodical (brainstorm)

Date: 2026-09-04
Status: Brainstorm, round 2 (2026-09-04, same day). Fran's decisions
marked DECISUS; Claude's proposals marked PROPOSITUM and agreed in
conversation unless noted OPEN. No spec yet. First sub-project CHOSEN:
a general raster editor (MacPaint/HyperCard in color — raster, not
pixel-art, which is a different affordance set). §X–XII added in
round 2: the laboratorium state doctrine, its native mapping, and the
discipline as lintable codices. Spec: pictor-spec.md. Plan (P0–P2, the
substrate): pictor-plan-1-substratum.md — 12 tasks, test-first.
Related: solarium-comparatio.md (the implementation reference and its
seams), nog-translatio.md, knotapel-promotio.md, piscina-adoptio.md
(same-day surveys), pinacotheca-brainstorm.md (Vultus::Imago, dithering
dialog — the nearest existing ancestor of a pixel editor),
officina-brainstorm.md (machinula/sputnik — the script tier), the
Novum Fundamentum writers' room in CLAUDE.md (an existing periodical
process).
Influences: HyperCard (authoring environment = runtime), Smalltalk
images, Eskil Steenberg's Love (low-poly, procedural, interface-first),
Processing/p5 (immediacy, sketch as unit), Flash/Shockwave (display
list, timeline, twips), Pico-8 (pixel-native constraints as aesthetic),
lit pixel art (effects via lighting layers, not geometry).

"Ludus" is a working name — a ludus was both a game and a school,
which is the authoring/artifact duality exactly. Not sealed.

---

## I. What it is

A set of logically self-contained C89 modules in lib/ from which one
can assemble one's own engine — not an engine. Solarium is the
implementation reference for 3D; rhubarb's existing pixel-table stack
is the substrate for 2D. The modules serve two consumers that turn
out to be the same thing:

  1. authoring tools — pixel art, painting, vector + animation, 3D
     modeling, CAD, sound, music, eventually video
  2. periodical artifacts — episodic interactive games/widgets on
     roughly a monthly publishing cadence

The organizing principle (PROPOSITUM, agreed):

> Every authoring tool is built from the same blocks as the artifacts
> it produces, and is itself artifact-shaped.

HyperCard's defining property was not the stack format; it was that
the environment and the runtime were one program. Smalltalk: the
environment is written in itself. Vertical integration of the tools
is a CONSEQUENCE of this principle, not a feature added later.

## II. DECISUS — what Fran decided

- **Standalone binaries.** Each artifact (episode, widget, tool) is a
  capsula-sealed executable with the runtime linked in. Not a player +
  files. Consequence: the block set needs a stable C API, not a
  stable file-format contract. Nothing can rot.
- **CPU-first**, for all 2D targets and open for 3D (§VI), with
  separations that keep a GPU path available later.
- **Three 2D targets**: (a) the existing low-pixel rhubarb target
  (pixel table, ~480 wide, fons_6x8, delineare); (b) a general-purpose
  2D engine (sprites, tilemaps, cameras); (c) a Flash-style target
  (vector shapes, display list, timeline, script).
- **Procedural generation is the default asset format**, bespoke
  assets supported. Realistically most content will be procedural
  because of time; bespoke must be first-class regardless.
- **Low-poly Pareto.** Past roughly PS2-era density, polygon count is
  diminishing return at dramatically increasing cost. Aesthetic is
  low-poly-friendly; effects come from lighting, palette, dither,
  shading layers — Love, lit pixel art — not geometry.
- **An authoring layer, vertically integrated**, is part of the value
  proposition. Tools listed in §I.1.
- **The periodical is HyperCard stacks / Smalltalk images**: built in
  integrated creative environments, released as sealed binaries.
- **Workflow is the metric.** Individual games and artifacts exist for
  now as much to prove and optimize the workflow as for themselves.
  Target: interactive/episodic releases on a periodical timeline,
  which requires the workflow to be very fast.
- **First application: leaning toward one of the editors**, because
  each is a good ongoing project that surfaces many module
  dependencies (the PULL rule, applied deliberately).

## III. What solarium's structure says (measured)

From the include graph (solarium-comparatio.md is the fuller survey):

- `sol_math` (37 fns: vec3/mat4/quat/TRS, ray-vs-aabb/plane/triangle,
  frustum) is the root; every 3D module depends on it and nothing
  else. Block-shaped.
- `rhi.h` is a genuine seam: `ui.c` and `glb.c` talk only to it, never
  GL; the Metal twin lives behind it; the rule "every RHI addition
  must be Metal-expressible" is enforced in comments. The one block
  already at library grade. Note it OVERSHOOTS the §II aesthetic:
  RGBA16F HDR targets, cubemaps, IBL foundations, a bloom chain.
- `scene` depends on `nid` + `sol_math` only; `component` on
  `sol_math` only. Block-shaped, but `scene.h` carries the app:
  `ObjectKind` = FILE / FOLDER / ALIAS / NOTE / TOMBSTONE / PORTAL.
- `mesh.h` mixes altitudes — `mb_push_vertex` / `mb_compute_tangents`
  (library) beside `make_room_doored`, `make_book_cover`,
  `make_folderbook`, `make_campus` (the app).
- INVERSIONS: `mesh.c` includes `gothic.h`, `flora.h`, `rock.h`;
  `collide.c` includes `gothic`, `flora`, `rock`, `route`,
  `workspace`. The generic depends on the specific.
- `main.c` at 17,698 lines / 0 tests is the consequence of no block
  structure: integration absorbs everything. The success metric for
  the block set is that a consumer's `main.c` shrinks toward "wire
  the blocks together."
- Already procedural: `mesh_ref` + params, `tex_ref` + knobs,
  registry-as-schema applied three times (meshes, materials,
  components). "The file records (kind, knob prefix), the app resolves
  the pixels." A solarium scene file is a program that regenerates
  the scene.

## IV. PROPOSITA — organizing principles (agreed)

**Recipes, not assets.** An asset is `(kind, seed, params) -> bytes`,
deterministic. Episodes ship recipes; the runtime resolves them at
load or the bake resolves them into the binary. Same recipe, same
bytes, forever — the "programs don't rot" thesis applied to content.
The carrier is solarium's registry-as-schema generalized: every asset
kind registers (name, param schema, defaults, generate fn). Meshes,
textures, sprites, tiles, sounds, palettes, fonts, levels: one
mechanism, STML-declared, `canon`-validated.

**Bespoke folds into recipes.** A bespoke asset is a recipe whose
generate function is "return these bytes," keyed by `sigillum`. One
registry, one code path; the bake does not care which is which. The
procedural/bespoke ratio becomes a property of a volumen, not an
architectural fork.

**The GPU-later seam is the command list, not the pixel table.** The
three 2D drawing models (pixel-native, sprite, vector) each EMIT
commands — rect / line / path / blit / text / clip, with a transform
stack — into a list. A CPU rasterizer consumes it now (extending
`delineare`); a GPU batcher consumes the same list later. Solarium's
`ui.h` is already a GPU consumer of exactly this shape (transient
batch, painter's order, one draw per texture change). Flash's display
list is a retained command tree; Processing's draw() is an immediate
command stream; both feed one rasterizer. The targets share pixel
table, presenter, events, timers, and the command list; they differ
only in the drawing model that emits commands.

**Blocks must be bidirectional.** A rendering block that cannot be
edited into is half a block. Every drawing model needs hit-testing,
selection, and a mutation path. Solarium has the instinct
(`editor.c`, `multiselect.c`, `descend.c`); rhubarb has it on the text
side (`cursor`, `selectio`, `sectio`, `vim`).

**The stack is a volumen.** volumen.h: "documentum = plagula sqlite
una, veritas eventilis intra" — one sqlite file, event-sourced truth,
SHA-256 CAS. Every tool edits a volumen. Undo is the event log.
Recipes, bespoke bytes, and script live in one file.

**Bake = volumen -> capsula binary.** Resolve recipes, embed via
`capsula` / `capsula_caudae`, `sigillum`, ship. `aedilis` closure and
`amalgama` already produce single-file builds. The missing step is
"resolve recipes."

**Processing-for-assets is the recipe authoring environment**, not a
separate product: sketch runs, you see the mesh or sprite sheet, you
save the recipe. Immediacy comes from `excubitor` / `vigilia`
hot-rebuild (exists), not from requiring an interpreted language;
`sputnik` (tree-walking, per-execution piscina) is available as the
script tier when a sketch wants one.

**Low-poly shrinks the renderer and grows the effects layer.** Flat /
gouraud / toon shading; palette and dither blocks (`dithering`,
`coloratio` exist); normal-mapped sprites; a screen-space lighting
pass over 2D. No PBR/HDR/IBL in the first block set.

## V. Strata

| stratum | contents | state |
|---|---|---|
| 0 substrate | piscina, chorda, xar, stml + canon, volumen, capsula, sigillum, fenestra + TabulaPixelorum, tempestivum, actor, entitas, credo, specimen | exists |
| 1 command list + rasterizer | the seam (§IV); CPU rasterizer into the pixel table extending delineare; GPU consumer later | new, small |
| 2 asset registry | recipes + bespoke; registry-as-schema; STML-declared, canon-validated | new |
| 3 drawing models | pixel-native (sprite / tile / palette) · vector (path / display list / timeline / tween) · 3D (math / mesh / scene / camera / raster) | new; solarium reference for 3D |
| 4 authoring tools | pixel art, painting, vector + anim, 3D model, CAD, sound, music, video — each a thin consumer of 1–3, editing a volumen | new |
| 5 bake | volumen -> resolve recipes -> capsula binary | mostly exists; needs resolve |

Order is dependency order. Nothing in 3 works without 1; nothing in 4
without 3. Stratum 4 tools are ALSO stratum-5 artifacts (§I).

## VI. OPEN forks

- **CPU rasterization for 3D as well.** The §II Pareto makes it
  viable: PS1/PS2-era vertex density is software-rasterizable at
  60fps on any modern CPU with a scanline z-buffer rasterizer in C89.
  If taken, EVERY target shares one pixel table, one command-list
  seam, one presenter, and `specimen` covers 3D regression as freely
  as 2D; solarium's `rhi.h` remains the reference for the GPU path
  when pulled. Claude leans yes. Fran has not ruled.
- **Scene objects vs `entitas`.** Whether ludus scene objects ARE
  entities (actor + entitas_repositorium + nuntium serialization) or a
  parallel structure beside them. Solarium's stable-handle /
  ULID-identity model and rhubarb's entity model overlap heavily.
- **Authoring language tier.** C89 for blocks and heavy sketches;
  `sputnik` for scripts; STML for recipes and layouts. The boundary
  between "written in sputnik" and "written in C" per artifact is
  unsettled. For LLM-agent-authored widgets (the `pythonica`
  consumer), what the agent writes must be small and checkable —
  which argues recipe + script, not C.
- **Solarium's fate.** Whether solarium becomes a consumer of ludus
  blocks (its `main.c` shrinking) or remains a frozen reference. The
  latter is cheaper; the former is the strongest possible test of the
  block set.

## VII. First sub-project — candidates

Too large for one spec; decomposed by stratum in §V. The first
sub-project should cross strata 1 -> 4 -> 5 on the cheapest target so
the whole loop (tool -> volumen -> bake -> binary) is proven once.

- **A. Command list + rasterizer + a pixel-art editor.** Stratum 1, a
  sliver of 2 (sprite + palette recipes), the smallest authoring tool.
  Proves tool-and-artifact-share-blocks, undo-from-event-log, and
  bake, on a target that needs zero new substrate. HyperCard's own
  bootstrap: the first stack was the stack editor. Also the
  LLM-agent widget in embryo (canvas, palette, brush, undo).
  Nearest ancestor: pinacotheca-brainstorm.md — Vultus::Imago,
  binary entity fields, the dithering dialog. A pixel editor is
  pinacotheca's sibling: CREATE instead of IMPORT. Claude's
  recommendation.
- **B. Command list + rasterizer + a small game** (tower-defense
  scale). Proves the artifact/periodical side first. Risk: the tool
  side, which IS the vertical-integration value, gets deferred.
- **C. Asset registry + a Processing-style sketch environment.**
  Proves the procedural thesis first. Risk: no drawing model to see
  output in until stratum 1 exists — becomes A with a different door.

Fran: leaning toward an editor (§II last bullet). Which editor is the
open question at time of writing. Editors ranked by (blocks surfaced)
/ (new substrate needed), on-path for §II: pixel art > vector + anim
(the Flash consumer, but needs anti-aliased path rasterization) >
3D modeler (forces §VI.1 immediately) > tracker/sound > painting >
CAD > video.

## VIII. Naming — SEALED 2026-09-04

**Names SEALED 2026-09-04 (Fran):** `ludus` (the set) · `pictor` (the editor) · `componens`/`componere` · `pingere` · `mandatum`/`mandata` · `figura` · `coetus` · `insula` · `actio` · `partes` · `destinatio` (targeting; `scopus` = scope in the house) · `mobilis`/`Motus` (the third genus; gate `mutare_motum`) · `restituere` (rebuild from islands; the lab's *remontatio* stays as history) · `dispensator` · `derivare` · `delineare_mandata` (the rasterizer, no new noun). Working names still open: `manus_ludus` (until the harnesses unify). All sealed names are in WORDS; no glossary entries needed.

Still candidates (P3+): receptum (a recipe) · plexus (mesh — `rete` is networking) · scaena (scene) · imaguncula (sprite) · linea temporis (timeline) · interpolatio (tween) · acervus (a stack, HyperCard sense) · fasciculus (an issue of the periodical).
## IX. AUDIENDA — not established

- Whether a C89 software rasterizer actually hits 60fps at the
  intended resolution and vertex budget on the target machine. §VI.1
  is reasoning, not measurement.
- Whether `volumen` as-built supports the write rate of an
  interactive editor (every brushstroke an event) or needs a
  coalescing layer. Not measured.
- Whether `sputnik` is fast enough for per-frame script in a game
  loop, or is a load-time / event-time tier only. Not measured.
- The actual size of a minimal capsula-sealed binary carrying the
  stratum 0–3 closure. Unknown until `silex_clausuram_colligere` is
  run against a real consumer.

---

## X. DECISUS — the laboratorium state doctrine (silicetum, 0001–0032)

The UI/widget layer is a concern worth isolating, and what is in
rhubarb now (`widget.h` / `ManagerWidget`) is a first attempt not all
of which survives. The doctrine that replaces it was worked out in
`silicetum/laboratorium/assets/experimenta/` (32 experiments,
2026-08-17 → 08-24, web-component medium, driven and asserted from
C89 via `fumus.c` + `Manus`). Fran's doctrine, dated 2026-08-17 in
0007, taken here as DECISUS:

**Three genera of state, each with its own write discipline:**

| genus | what | lives in | written |
|---|---|---|---|
| durabilis | what a database would hold — tree, apertus, electum | the scene island (`text/stml`) | every discrete mutation |
| ephemerus | lives with the projection, survives RESTORE — cursor (as a PATH, never an index), pending query | an island INSIDE the component | every discrete user act |
| mobilis | continuous — scroll, drag | the ephemera island | IN QUIET (300 ms after the last event), never per event |

0015 refined kinetic into two disciplines: drag = live preview with
the island untouched, ONE durable write at rest (mouseup); keys =
discrete action, one write per press. For a raster editor this is the
undo-granularity answer: a brushstroke is kinetic, mouseup is one
durable write.

**Lex rehydrationis** — an island that is only written is decoration.
Restore destroys the projection and rebuilds from islands ALONE; if
the writer lied, the smoke test catches it. **Porta unica** — one gate
per genus (`mutare_durabile` / `mutare_ephemera`); object and island
never touched separately. **The component is a pure projection of its
islands** (0006); the test reads STATE, not click counts, "because the
picture is a pure function of state" (0031). **Actions are NAMED in
markup** (`actio="cantare.vesperas"`), never computed; no expression
language (0011). **A markup-only consumer** — zero lines of script —
gets full behavior from the library (0008): "mores ordinarii nihil a
consumente poscunt." **The archive of experiments is the regression
suite of the library** (0008). `componentia.js` (3,214 lines) is the
REFERENCE IMPLEMENTATION; the C89 version ports its contract, not its
code.

Fran's independent conclusions, confirmed by the lab: draw commands as
the pure output of drawing functions (headless tests without image
diffing — 0022 was the first windowless pure-function test); replay.

## XI. PROPOSITA — the native mapping

**The DOM's job goes to the command list.** In the lab: islands →
component → DOM, and `Manus` queries the DOM by selector. Natively:

    islands (STML, three genera)
       |  porta unica per genus
       v
    reddere(islands) -> command list        pure; THE TEST SURFACE
       |
    rasterize -> TabulaPixelorum             CPU; specimen on the table
       |
    praesentare -> fenestra

A command is small — rect / line / poly / blit / text / clip push-pop
/ transform push-pop — and carries an optional id and ROLE, so the
command list is the accessibility tree (Testing Library: query by
role and name). 0020 found DOM segments let tests assert TEXT where
pixel overlays never could; the command list gives that natively,
plus bounds and z-order, as plain data. `Manus` keeps its shape
(`manus_premere`, `CREDO_MANUS_EXISTIT`) with selectors becoming
command-list queries; `imperium` gets the native transport it was
designed for.

**Replay falls out of the genera split.** Durable and ephemeral
writes are discrete named actions; kinetic writes are coalesced. So
the event log separates into (a) ACTION REPLAY — `(t, actio,
argumenta)` only, deterministic, small, machine-independent: THE
TEST FORM, a recorded log in, final islands + command list out,
diffed as text; and (b) FULL REPLAY — plus timestamped kinetic
samples, for demos, bug reproduction, and the periodical. Redux
time-travel never had (b) because it treated every mouse-move as an
action.

**The shebang file is 0008 + 0011, natively.** `#!/usr/bin/env ludus`
over an STML file: islands declare state, library elements supply
standard behaviors, `actio="..."` binds to REGISTERED actions; a todo
app needs no C. Custom behavior = a C fragment, runnable two ways
that mirror the two ways to ship: INTERPRETED by officina's
`machinula` (instant; HyperCard edit-while-running) or COMPILED by
`silex` into a sealed binary (the periodical). Same file, both modes.
This is a SECOND authoring mode, not the primary one for the raster
editor — but both share the state model exactly.

**Keep / retire, revised:** keep `vim` (already the pattern),
`elementa` (immediate-mode direction; `Fructus` return shape),
`layout` (STML-declared positions), `dialogus` probably, `specimen` +
`imperium` (right instincts; extend). `schirmata` becomes a CONSUMER.
Retire `widget.h`'s vtable + opaque-pointer model and `ManagerWidget`
(the Backbone era). Test coverage was the tell: `vim`, `elementa`,
`layout`, `imperium`, `specimen`, `fenestra` have probationes;
`widget`, `schirmata`, `dialogus`, `pinacotheca_visus` do not — the
parts that could be tested got tests.

## XII. DECISUS — the discipline is ENFORCED by the toolchain

Fran, on the first rhubarb UI layer: the question is how to make sure
implementors FOLLOW the discipline when building widgets in C89 — and
the answer now exists: silva / examen / fluxus can LINT that widgets
follow the state and drawing discipline. Enforcement moves from code
review to edit time (LSP post-change hooks) and test time (the
restore contract). Two gates.

The toolchain today: silva (byte-exact trees, DECISUS genera as
tags), quaestio (`:vocat(f)`, `:definit(x)`, `:utitur(x)`,
`:habet(sel)`, `:non(sel)`, custom pseudos registrable), examen
(codices; tiers violatio / suspectum / domesticum; registration =
three enum lines + three table rows + a judgment seat, fluxus-1 spec
§6), fluxus (CFG + def/use eventa per tracked symbol, address-taken,
member-store = def-of-whole), annotationes (STML in comments bound to
the next unit; "presence by class" and "selector-scoped presence
contracts" are named reserved pulls — this is that pull), `-domus`
driver mode (fail on domesticum for house code — named pull,
officina-m4a §249).

Classification: a unit is a widget by ANNOTATION — `/* <componens/> */`
on the unit (annotationes "presence by class"), not by naming or
directory. Explicit, greppable, and the same identity space as nids.

**The codices (PROPOSITUM):**

| # | rule | layer | mechanism | checkable |
|---|---|---|---|---|
| L1 | porta unica — islands mutated only through `mutare_<genus>` | quaestio + examen | in `<componens>` units: `:vocat(stml_ponere_*)` or an assignment whose lvalue base has type `Insula` outside the gate fns → fires | selector today; lvalue-base type via examen locus (M4a, exists) |
| L2 | `reddere` is pure | fluxus + quaestio | `<purus>`-annotated fn: any DEF event on a param-derived lvalue → fires; `:vocat(f)` with f outside the allowlist (`mandatum_*`, math) → fires; state param must be `constans` | eventa exist (fluxus 1); allowlist = a registrum |
| L3 | each field belongs to ONE genus | canon + quaestio | island schema declares field → genus; a write inside a `mutare_ephemera` callback touching a durabilis field → fires | canon today; needs field-name capture |
| L4 | kinetic writes IN QUIET | quaestio + arbor_index | writes to kinetic fields only inside `<quies>`-annotated fns; callers checked via find-references | annotation + index today |
| L5 | effects at the edge — handlers return effects, perform no I/O | quaestio | in `<tractator>` units: `:vocat(filum_*\|fenestra_*\|rete_*\|tempus_*)` → fires | today |
| L6 | identity by VIA, never index | canon | ephemera schema field named `*_index` → fires | today |
| L7 | no retained state outside islands | quaestio | file-scope `declaratio[storage=staticus]:non([constans])` in `<componens>` files → fires | today |
| L8 | remountable — the probatio must restore and compare | quaestio over probationes | a componens' probatio must `:vocat(restituere)` and `:vocat(mandata_aequalia)` | presence contract (the reserved pull) |
| L9 | commands only — no direct rasterization | quaestio | `<componens>` units `:vocat(delineare_*\|tabula_pixelorum_*)` → fires | today |
| L10 | actions resolve BOTH ways | canon + examen rot tier | `actio="x"` with no registered handler → error; handler never referenced → advisory | annotationes tier-3 pattern |

Severity: L1, L2, L7, L9 = **violatio** (the structural four whose
absence made the first attempt untestable); L3, L4, L5, L10 =
**suspectum**; L6, L8 = **domesticum**. Under `-domus` all ten block
house code.

What the lab enforced only at test time (fumus restore-and-compare),
the native layer enforces at edit time AND test time. Replay logs are
the probatio form; the archive of replays is the regression suite —
the lab's "archivum suita regressionis fit," natively.

## XIII. OPEN (round 2)

- **The native state value — DECISUS (round 3).** An STML NODE TREE
  in a piscina, accessed through a REPOSITORY-type API (the
  `entitas_repositorium` idiom), mutated only through the porta
  unica, serialized to text on save, on inspect, and on RESTORE —
  where restore re-reads the text to prove the writer honest (lex
  rehydrationis). Tree for speed, text for truth-checking. L1 is
  therefore checkable by lvalue TYPE.
- L8's presence-contract machinery and the `-domus` driver are both
  named pulls in existing specs, not built. Ludus is the pull.
- **Gate names — DECISUS (round 4):** `mutare_durabile` / `mutare_ephemera` / `mutare_motum`; the third genus is `mobilis`, its struct `Motus`, with `<quies/>` as its flush seat.
## XIV. DECISUS / PROPOSITA — events, targeting, focus (round 3)

**The platform is a stream** (Fran). `fenestra` produces `Eventus`
(the existing tagged union — genus + {magnitudo, clavis, mus,
rotula}) and presents a pixel table; nothing else. `Eventus` gains a
`tempus` field, stamped by the platform in production and by the log
in replay — TIME IS DATA IN THE EVENT, never a call inside dispatch.
(The current `manager_widget_tractare_eventum` calls `tempus_nunc()`
for double-click detection — unreplayable by construction; it also
hardcodes 6x8 cell math and mutates a copy of the event to change
its genus. All three go.) Headless test = `Eventus[]` in, `mandata` +
islands out. `fenestra_clavem_immittere` / `_murem_immittere` were
the right instinct one layer too high.

**Pipeline:**

    fenestra -> Eventus (raw, stamped)   |   replay -> Eventus[] from log
                          v
    derivare(ephemera, raw) -> [Eventus]        pure: double-click, hover
                          v                     in/out, focus gained/lost,
    destinatio(mandata_prior, ephemera, ev)         drag start/end
        -> { id_captum, id_geometricum, ascensus[] }   pure: THE STRATEGY
                          v
    dispatch: walk ascensus; first group with an actio for ev.genus
        -> actio(repositorium, id, ev)          writes only via porta unica
                          v
    reddere(repositorium) -> mandata            pure; regenerated AFTER
                                                EVERY dispatched event
                                                (rasterized once per frame)

**Targeting — DECISUS with a strategy wrapper** (Fran: "wrap this as
a strategy so we can choose a different targeting algorithm later
without bothering the components"). Default strategy: bubble over
LAST FRAME'S COMMAND TREE. Groups (clip / transform push-pop) carry
`id`, `actio`, bounds; a pointer event walks innermost-first,
topmost-first (painter's order = z-order), honoring clips and
inverting the transform stack; the enclosing groups with ids are the
`ascensus`. "The thing the user SAW is the thing they clicked" — one
tree, nothing to desync. Components never call `destinatio`; they emit
commands and register actiones. The strategy contract is exactly
that one function; swapping it (state-tree bubbling, a spatial
index, focus-only for a TUI) touches no componens. L2/L9 guarantee
components cannot reach around it.

**Three things designed in, not discovered** (the cases geometry
targeting needs help with — audited for a drawing app and an
ERP-shaped app; no blockers found):

1. **Staleness.** Two events in one batch: the first opens a dialog,
   the second must see it. Therefore the command LIST is regenerated
   after every dispatched event (pure, cheap); only RASTERIZATION is
   once per frame. Frame 0 renders before accepting input.
2. **Escalating hit predicates.** rect bounds (fast, default) ->
   polygon/path -> mask -> `praedicatum="proprius"` (the group receives
   the LOCAL point and resolves it itself). The last is the idiom
   for a paint canvas ("which pixel") and for a data grid (`row = y /
   altitudo` beats 50,000 cell commands). The componens knows its
   geometry better than a generic walk.
3. **Focus as a REQUEST.** Tab to a row scrolled out of view: the id
   is absent from last frame's list. `focus = id` is set in ephemera;
   the owning componens receives `focus_petitum` and adjusts its
   scroll (a kinetic write) so the id exists next frame. The one
   place state-tree knowledge is needed, and it is the OWNER's
   responsibility, not the targeter's.

**Focus** is an ephemera field (`focus = <id path>`); render reads it;
key events skip hit-testing and route to it, then bubble its
ascensus. Survives restore (0005) because it is state. A focus
STACK in ephemera for modals: `dialogus` pushes on open, pops on
close — captured-and-RETURNED after Enter AND Escape (0004).
`focus_amissus` is DERIVED by comparing old and new focus (0014's
"focus loss reverts"). Tab order = order of focusable ids in last
frame's list; no tabindex. Global shortcuts (Cmd+K, 0004): the root
group's actiones, reached by bubbling when nothing below consumes.

**Pointer capture** (`captura = <id>` in ephemera; set by an actio on
depressus, cleared on elevatus; routes mouse events without
hit-testing) IS the kinetic tier's plumbing — a brushstroke. During
capture `destinatio` still computes `id_geometricum` so drag-over
feedback (drop-target highlight) is deliverable alongside.

**Modal veil**: painted last, therefore hit first; its group consumes
all pointer events. Correct BY CONSTRUCTION — 0005 found exactly
this ("the veil covers the eye boxes; clicking correctly refuses").

**Structurally eliminated:** listener leaks (0009/0011 — no
listeners exist; an unrendered componens emits no commands and
cannot be targeted); focus killed by repaint (0006 — focus is state,
repaint is pure); blind focus on hidden elements (0005 — a hidden id
is absent from the list, `focus_amissus` derives).

**From the web, keep:** bubbling; "consumptus" as a return value.
**Do not import:** the capture phase, event-object mutation,
`stopImmediatePropagation`, focus-within / focus-visible (they exist
because focus was not state), React-era synthetic event pooling.

**Consequence for design order:** the COMMAND VOCABULARY (id, actio,
hit predicate, group bounds, role) is the event system's input as
well as the renderer's output — it is the interface both sides
depend on (Eskil: interface first). It is the first design section
of the raster editor.

---

## XV. DESIGN — raster editor, section 1 APPROVED (2026-09-04)

Working name for the sub-project: **pictor** (unsealed; Fran names).
A general raster editor — MacPaint/HyperCard in color — on the
low-pixel target. Raster and pixel-art are DIFFERENT affordance sets
(DECISUS); this is raster.

**§1 The two vocabularies — APPROVED, with Fran's revision.** React
and HTML fused BEHAVIORAL composition with DRAWING composition (the
accessibility tree is the separated behavioral tree, bolted on after
the fact with ARIA). Games never fused them: hitbox != sprite. Ludus
keeps two trees:

    islands -> componere(repo) -> arbor componentium   pure; LOGICAL
                                   (targeting/focus/capture walk this)
            -> pingere(arbor, repo, thema) -> mandata  pure; DRAWING
            -> rasterize -> pixel table

- **componens** (behavioral node): id . actio . partes (role) . fines
  . hit predicate (rect | polygonum | mask | proprius) . focusabilis
  . filii. No shapes.
- **mandata** (drawing): six primitives — `coetus` (nesting, clip,
  translate/integer-scale ONLY; carries an optional PROVENANCE ref
  to its componens for the inspector, which targeting never reads),
  `rectangulum`, `linea`, `polygonum`, `imago`, `textus`. Color is
  a thema token or a tagged literal, resolved by the rasterizer —
  the three 2D targets diverge here and nowhere else.
- **figurae** (drawing composites): a registry, chosen by role +
  thema, lowering to primitives by a pure fold to fixed point
  (depth-guarded). Anyone may register one — composition/extension.
  `elementa.h`'s five become the first five figurae AND the first
  five componens kinds: two registry entries each.
- **Load-bearing rule:** the componens OWNS bounds; `pingere`
  receives the componens and draws AT its bounds. Drawing is derived
  from logical geometry, never parallel — the trees cannot drift.
  Layout is therefore logical-side: fixed in v1; a mensurare/place
  pass slots there when the general 2D target pulls it (font metrics
  passed in to stay pure).
- Typed structs in an `xar`, piscina mark/reset per rebuild; STML is
  the SERIALIZATION (snapshot, inspection, quaestio, replay
  expectations) — tree for speed, text for truth, same rule as the
  islands. `canon` validates the serialized vocabulary.
- Snapshot altitudes: the LOGICAL tree is the primary probatio
  snapshot (stable across theme/target); each figura's lowering is
  tested once per thema in the registry's own probatio, never per
  widget.
- Lints sharpen: L2 -> `componere` pure + `pingere` pure; L9 gains a
  twin — the targeting strategy reads `arbor componentium` only,
  never `mandata`.
- Out of v1: curves (a `curva` figura lowering to polygonum — the
  vector target's pull, and the extension mechanism's first proof),
  gradients, rotation, alpha compositing in the command list (the
  canvas is one `imago`; layer compositing is inside the document
  model).

**§2 The state schema — APPROVED (2026-09-04).** The document IS its
stroke log: durabilis events project to a bitmap — "recipes, not
assets" applied to hand-made art, and exactly `volumen`'s shape.

| genus | holds | written |
|---|---|---|
| durabilis | canvas latitudo/altitudo . paletta . strata (list; ONE shipped, modeled from day one) . sigillum of current checkpoint . metadata | one event per completed stroke/op, into volumen ACTA |
| ephemerus | instrumentum . color_primus/secundus . magnitudo . stratum_activum . selectio (rect or floating region + offset) . zoom . focus | every discrete act |
| mobilis | ictus_pendens (in-progress stroke points) . pan . eyedropper preview . zoom mid-gesture | live preview; ONE durable write at release |

Pixels never live in the durabilis island (lab 0013 split): acta =
the event vocabulary (`<ictus instrumentum color magnitudo>` with
points, `<impletio>`, `<figura genus=...>`, `<insertio sigillum x y>`,
`<stratum>`, `<paletta>`, `<selectio_commissa>` — every event carries
full tool parameters so replay is deterministic); massae = bitmap
checkpoints every N acta by sigillum (bounded load and undo); plagulae
= the current bitmap, shown by one `imago` primitive. Undo/redo = pop
/ re-append acta, reproject from nearest checkpoint; granularity is
one stroke because the kinetic tier coalesced the drag. DECISUS on
both calls: (1) bitmap as PROJECTION of acta, not bitmap-as-truth —
a pictor document ships in the periodical as kilobytes of STML;
(2) paste = `<insertio sigillum>` with the region in massae — the one
non-event-derived byte source, which is what "bespoke as a recipe
returning bytes" was for.

**Refinement to §XII L4 (found while drafting §3):** the lab's
kinetic discipline is "pictura per eventum, scriptura IN QUIETE" —
PAINT per event, WRITE THE ISLAND in quiet; 0015: "drag preview live,
island INTACT, one durable write at rest." So the kinetic tier has an
IN-MEMORY home distinct from the island: a `motus` struct the
dispatcher owns, written per event through `mutare_motum` (cheap),
read by componere/pingere for live preview, FLUSHED to the ephemera
island only by the quiet seat (or superseded by the durable write at
release — a finished stroke never touches the ephemera island at
all). L4 therefore reads: kinetic ISLAND writes only from `<quies>`;
struct writes are free. Replay: full replay carries the kinetic
samples; the flush is deterministic given timestamps.

**§3 The first componens set and actiones — APPROVED (2026-09-04).**
MacPaint's anatomy with HyperCard's command palette instead of menus
(`registrum_commandi`; lab 0004/0011). Componentia: `prospectus`
(viewport; owns pan + integer zoom on its coetus), `tabula` (canvas;
partes=tabula; hit=PROPRIUS; default focus; its ACTIO IS CHOSEN AT
COMPONERE TIME from `ephemera.instrumentum` — named never computed,
the name a pure function of state, so handlers never branch on
tool), `instrumentarium` (tool palette; index of `optio`; vim-style
nav), `paletta` (color grid from durabilis.paletta; click primus,
shift-click secundus — `elementa_graticula_colorum` already),
`magnitudo_selector`, `tabella_mandatorum` (Cmd+K; focus pushed,
RETURNED on Enter and Escape), `dialogus` (veil consumes; children
campus_textus + bottone), `status_linea` (componens with no actio and
no focus — queryable, not interactive). Every optio/bottone/campus is
a componens kind AND a figura (two-registry rule).

The stroke end to end: depressus on tabula -> `penicillus.ictus` sets
captura, opens `motus.ictus_pendens` -> each captured motus appends
to the struct, pingere overlays live -> elevatus -> ONE `<ictus>` into
acta, struct cleared, capture released; ephemera island never touched.
Escape mid-stroke clears the struct, no acta (0014 revert).

Action registry v1 (~25 names; L10's list): instrumentum.eligere,
color_primus/secundus.ponere, magnitudo.ponere, penicillus.ictus,
impletio.punctum, color.capere, selectio.marquee/omnia/nulla/secare/
copiare, insertio, revocare, reficere, documentum.novum/aperire/
servare, stratum.novum/eligere/visibilitas, dialogus.confirmare/
abrogare, focus.proximum/praecedens, zoom.ponere.
**CONVENTION (DECISUS): one actio per INTENT, phase by event genus** —
`penicillus.ictus` receives depressus/motus/elevatus as one cohesive
handler; no incipere/ducere/finire triplets. Fewer names; a stroke's
writes across genera stay in one place where L3 can see them.

Keyboard by bubbling to root actiones: tool hotkeys p b f l r s i,
Cmd+Z / Shift+Cmd+Z, Cmd+K, Escape (innermost of: cancel stroke ->
deselect -> close dialog), Enter, Tab/Shift+Tab; palettes navigate
through `vim.h`. Figurae v1: figura_optionis, _indicis, _tabulae (one
imago + marquee + pending-stroke overlay + cursor), _bottonis,
_campi, _dialogi (veil + panel), _status. Tool icons: bespoke by
sigillum in capsula, or fons_6x8 glyphs to start.

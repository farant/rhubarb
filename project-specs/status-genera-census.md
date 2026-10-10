# State genera — census of live widgets (2026-10-10)

*Written in secunda while designing the agenda/calendar (see
project-specs/agenda-plan.md). Fran: "not just using the state model
but properly classifying (and having the right categories) for state
is part of the goal." Live widgets: pictor, scriba, vicus. This is a
census, not a decision.*

## I. Background (what the doctrine says today)

ludus-brainstorm §X (lab 0001-0032) + pictor-spec §2: three genera.

| genus | lab definition | gate | written |
|---|---|---|---|
| durabilis | "what a database would hold" | `mutare_durabile` | every discrete mutation |
| ephemerus | "lives with the projection, survives RESTORE" (cursor, pending query) | `mutare_ephemera` | every discrete user act |
| mobilis (kinetic) | continuous - scroll, drag | `mutare_motum` -> `Motus` struct | per event in memory; flushed to ephemera at quiet (300 ms) or superseded by the durable write at release |

Since then: document CONTENT moved to the volume (historia log) -
scriba canon: "textus ipse in volumine, non hic"; pictor pixels and
layers likewise.

Note: "survives RESTORE" in the lab means in-process rehydration
(lex rehydrationis: destroy the projection, rebuild from islands
alone), NOT surviving an app restart. The ephemera island is today
ALSO saved to disk, so the two meanings were merged.

## II. Fran's categories (2026-10-10)

1. Shared data / entities - shown by several widgets (documents).
2. Widget state persisted across app sessions.
3. Truly transient widget state (dropdown open, animation). Some of
   it is kinetic, some is not.

## III. Finding: kinetic is a write RHYTHM, not a lifetime

Two independent axes:

- **Lifetime/scope**: entity (shared) | widget, persisted | widget,
  session-only.
- **Rhythm**: discrete (one write per act) | kinetic (continuous,
  coalesced; in memory until rest).

| | discrete | kinetic |
|---|---|---|
| entity | an act in the log (stroke committed, layer added) | brushstroke in progress -> ONE act at release |
| widget, persisted | tool, colours, cursor, mode, selected layer, page | pan, zoom, scroll (settle, then saved) |
| widget, session-only | palette open, pending key chord, status message | drag in flight, line-tool rubber band, hover |

So "kinetic" covers part of category 3 (drag, rubber band) and part
of category 2 (pan/zoom settle and persist). Category 3 that is NOT
kinetic: open palette, pending chord, status message, date toggle.

Category-3 state must still live in an island: the lex rehydrationis
(component = pure projection of islands) applies to an open dropdown
as much as to a cursor. It is the DISK that it must not reach.

## IV. Census

Key: cat = Fran's category (1/2/3); K = kinetic; HOME = where it lives
today; ✗ = misfiled or off-model.

### pictor (ephemera canon + motus + PictorActiones)

| state | cat | K | home today | |
|---|---|---|---|---|
| instrumentum, color_primus/secundus, exemplar, magnitudo_* | 2 | | ephemera | ok |
| stratum_activum | 2 | | ephemera | ok (references a doc entity - clamped by hand) |
| zoom, pan_x, pan_y | 2 | K | motus -> ephemera at quiet | ok |
| palette (which palette is open) | 3 | | ephemera | ✗ persists |
| semen (spray preview seed) | 3 | | ephemera | ✗ persists |
| selectio_* | 2 or 3? | | ephemera (unused?) | ask |
| line pending point, layer drag (captura, ictus_pendens) | 3 | K | motus, never flushed | ok |
| tempora, semen (spray timing) | 3 | K | PictorActiones struct | ✗ off-island |
| durable: latitudo, altitudo, sigillum, paletta, titulus | 1? | | durable island, per pane | ✗ ambiguous (document facts copied per pane) |
| durable canon `stratum` elements | - | | declared, apparently unused since L2 | ✗ stale |

### scriba (ephemera canon)

| state | cat | K | home today | |
|---|---|---|---|---|
| cursor_linea/columna, modus, visualis_genus, selectio_*, pagina_positio | 2 | | ephemera | ok |
| clavis_praecedens (half a chord), fd_exspectans, fd_tempus | 3 | | ephemera | ✗ persists |
| nuntius (status message until next key) | 3 | | ephemera | ✗ persists |
| capsa, capsa_lineae (yank register) | 1? | | ephemera, per pane | ✗ yank in one pane, paste in another = shared |
| paginae_numerus | 1 | | ephemera | ✗ a fact about the book, copied per pane |

### vicus (ephemera canon + Vicus struct + layout index)

| state | cat | K | home today | |
|---|---|---|---|---|
| activa, prior, focus, focus_acervus | 2 | | ephemera | ok |
| tabs / panes / stacks layout | 2 | | own index file (`indicem_scribere`), not through the island gate | ✗ off-model |
| horologium_dies (clock/date toggle) | 3 | | Vicus struct field | ✗ off-island (S4b) |
| pila_centesimae (battery reading) | NEW (see V) | | Vicus struct field | ✗ |
| superficies_latitudo/altitudo (all three) | NEW (see V) | | ephemera, written by dispensator | ? |

## V. Categories the census suggests beyond Fran's three

1. **Observed / environment state**: battery level, current time,
   window size. Not owned by any widget and not an entity; a sample of
   the world through a port. The lab already treats time this way
   (pictor-spec §2.3: "time is data in the event"). Candidate: such
   samples arrive as events and are never persisted.
2. **App-wide non-document state**: the yank register, vicus layout,
   possibly the agenda pointer. Shared by widgets, but not a document
   with a log.
3. **Widget -> entity references** (stratum_activum, the agenda's
   selected item): category-2 state pointing into category 1; needs
   validation on rehydration and after undo.
4. **Entity facts copied per widget** (durable island, paginae_numerus):
   perhaps the durable island's real job is the BINDING "this pane
   shows that document", and everything else on it should be read from
   the document.

## VI. Open questions for Fran

- Is the arc "fix the model, then A2" (agenda-plan)? Order of work:
  vocabulary first (genera names, canon marking), then migrate pictor,
  scriba, vicus, then the calendar on the new model.
- Category 3: a separate island genus never saved to disk, or a canon
  marking (`transiens`) on attributes of the ephemera island?
- Naming: "ephemera" for the persisted widget tier is misleading.
- Which of section V are real categories?

## VII. Expressing it: extended canon vs widget manifest (2026-10-10)

What must be expressible. Per field: (1) type [canon has it];
(2) lifetime persisted/transient; (3) rhythm discrete/kinetic;
(4) owner action [domini.stml today]; (5) reference into the bound
document + fallback when the target vanishes; (6) DEFAULT value -
today buried in prose (`nota="absens = 1"`); (7) TEST-ONLY field -
buried in prose (`nota="probationes solae"`: fuga, numerus, puncta,
ictus_numerus); (8) framework-written (`scriptor="dispensator"`).
Per widget: (9) binding to document(s); (10) observation ports;
(11) registered actions.

**Shape A** - extend `<attributum>` inline (vita, rhythmus, scriptor,
absens). Compact, but canon.canon is shared by every dialect
(aedilis, norma...) and the canon's doctrine is "fundamentum, non
sapientiam": lifecycle semantics are not validation.

**Shape B (recommended)** - Fran's idea: a widget manifest STML
document with the canon as one section (canon infixus already exists:
`<radix><canon>...</canon>...</radix>`), plus `<ligamina>`,
`<portae>`, `<actiones>`, `<status>` (one `<campus attributum=...
vita= rhythmus= absens= scriptor= referentia= orbatus=>` per field).
The manifest's own canon (`organum.canon`, root name TBD) checks by
`<citatio>`: campus -> canon attribute, scriptor -> declared actio,
referentia -> declared ligamen. Loader/lint checks what the canon
cannot (completeness: every attribute has a campus; C registers
exactly the declared actions) - gradus III stays out of the canon.
Replaces durabilis.canon + ephemera.canon + domini.stml per app.

Consequence to decide: with `vita` per field, the durable/ephemera
island split no longer decides persistence - one island per widget,
"durable" reduced to the binding.

Next proposed step: write pictor's manifest in full by hand (no code)
to find missing vocabulary.

## VIII. Nesting: any widget mounts any widget (Fran, 2026-10-10)

Goal (Fran): "any component can mount any other as children... almost
like nested redux stores everywhere."

Measured against today:
- Principle already decided (insula-rami-plan I.1): one store owned by
  the host, a widget gets a BRANCH, reads/writes relative to it, never
  knows where it is mounted. That is Redux's one store + nested
  reducers, not many stores.
- BUT `InsulaRamus` is repo + (elementum, id) = a child OF THE ROOT:
  depth 1 only, though the plan said "node path". Calendar -> embedded
  scriba is depth 2 (vicus -> calendarium -> scriba): not expressible.
- Host singletons (rami AUDIENDA): one action registry, focus,
  dispatcher, Motus/gesture slot, surface - "refilled on switch". For
  nesting, a widget that mounts children must be able to play host to
  them (or the host must route by branch path at any depth).
- Already composes: componens trees (draw), canons per branch (judged
  on the branch node), the dispatcher handing an action the branch of
  the component it fired on, vicus `argumentum` (parent passes the
  child which entity to show = props).
- Cost parked: every write copies the whole store through text (T5:
  13.6 us at 2 tabs, 52.7 us at 11; terminal-planning park 010).
  Nesting grows the store.

Manifest consequence: a `<montationes>` section - child slots the
widget mounts (`<montatio nomen="pagina" genus="scriba"/>`), the
parent binding each child's `ligamen` (props); the child's manifest
judges its own subtree. Mapping: ligamen = props, status = local
state, actiones = handlers, montationes = children.

## IX. S0 findings - pictor's manifest by hand (2026-10-10)

Draft: project-specs/status-s0-pictor.stml (every field traced to
code). What strained the vocabulary:

1. **Embedded `<canon>` collides with canon infixus.** `canon` as the
   first child of a root is RESERVED: it is the document's OWN
   contract, so it cannot also be the island's schema. And no canon
   can say "judge this subtree by another dialect" (canon.canon). Draft
   instead: each `<campus>` carries `typus` (+ `<optio>` for electio),
   and the island canon is DERIVED from the campus list (canon_ex_nodo
   over a synthesized tree). Alternative: a general canon feature for
   foreign-dialect subtrees (XML-namespace analogue).
2. **One owner per field does not fit transient UI state.** `palette`
   is opened by palette.aperire and closed by 7 other actions (every
   pick, any stroke). Draft uses `scriptores="..."`; alternative: a
   declared dismissal behaviour (a popover closes on any other act),
   which would be generic across widgets.
3. **In-flight kinetic state is structured** (point lists, timings,
   capture) and lives in memory structs, not island attributes:
   `sedes="motus"`. Spray `tempora` and `semen` are off-island today
   (PictorActiones) and `semen` is duplicated in the ephemera island;
   they belong next to Motus.ictus_pendens (or in the gesture slot).
4. **Framework fields belong to a common base** (id, focus,
   focus_acervus, superficies_*), declared once: `<basis
   genus="organum-commune"/>`. superficies is an observation.
5. **Persisted pan is write-only**: motus flushes pan_x/pan_y to the
   island at quiet, pictor reads Motus.pan, nothing restores Motus
   from the island - pan never survives a restart. (zoom is read from
   the island, so it does.) A bug the manifest makes visible.
6. **Copied document facts**: durable latitudo/altitudo are copies of
   the bound document's size; read from the document instead.
7. Dead declarations: durable paletta, titulus, sigillum (owner
   declared, never written), `<stratum>` element; ephemera selectio_*,
   cursus, gradus.

Vocabulary used: organum genus / ligamina (ligamen a="parens") /
portae (porta modus="petitio|eventus") / actiones (actio systema) /
status campus (nomen typus vita=persistens|transiens|probatio
rhythmus=mobilis sedes=motus absens scriptor|scriptores referentia
orbatus) / basis / montationes.

**Decided (Fran, 2026-10-10) on finding 1:** the island canon is
GENERATED from the campus list - one definition, no two sources to
keep in sync.

**Correction on finding 5:** no widget pans today. Nothing outside
tests changes Motus.pan (or Motus.zoom); the pan/zoom plumbing is
scaffolding from pictor-spec (prospectus). The missing restore is
latent - it bites the day panning exists.

## X. Ownership principle: systems over aspects (Fran, 2026-10-10)

From comparing Redux/Elm (state declares which events affect it), the
web platform (`popover="auto"` light dismiss), React (controlled vs
uncontrolled, lifting state up), Solid (ownership by capability),
Marko (serialization derived by the compiler) and Muratori's "Big
OOPs" (Sketchpad: one shared structure + generic operations; the
35-year mistake = encapsulation boundaries around domain things
instead of around systems). Agreed lessons:

1. **One store is the Sketchpad move** (already made): widgets are
   views into the shared island tree, not owners of private state.
2. **Per-field owners are encapsulation in miniature**: keep `scriptor`
   as an invariant for VALUE fields (one writer), never as the
   organising principle. Many writers on one field = it is an aspect.
3. **Cross-cutting behaviour = a system over a declared aspect**
   (ECS for UI state). Aspects so far: persistence (vita), kinetic
   (rhythmus; motus flush), reference (referentia/orbatus;
   re-validation after acts/undo), popover (dismissal: Esc, outside
   click, other act, another popover opening), observation (portae),
   binding (ligamina). Candidates: focus, selection.

Consequences:
- `scriptores` lists dropped. palette = `aspectus="popover"
  scriptor="palette.aperire"`: opening owned, closing is the system's.
- Lifting state up is declared in `<montationes>`: a parent binds a
  child's field to its own (controlled) or leaves it (uncontrolled).
- Admission rule (PULL): an aspect enters the vocabulary when a second
  widget needs the same behaviour; until then it is widget code.

## XI. A hierarchy of states (Fran, 2026-10-10)

Fran: "the trick is to classify state... if you try to rigorously
apply [one general strategy] to all kinds of state (like react) you
end up with something awkward. but if you accept that there is a
hierarchy of states then you have the most general model, and then
different models proven / fitting well for real categories."

Evidence: React's ecosystem IS a list of specialised state models for
categories the general model (useState/props/effects) handled badly -
server cache (React Query/SWR), forms (react-hook-form, uncontrolled
on purpose), animation (refs/springs, bypassing render), URL (routers),
global (Redux/Zustand). The reverse failure: redux-form (general store
forced onto form state) was abandoned. Our own precedent: documents
got a specialised model (historia) instead of island attributes, and
undo/sharing/compatibility fell out.

- **General model** (everything satisfies it): a field in the one
  store, written through the gate, canon-checked, rehydratable,
  inspectable. State whose category is not understood STAYS here -
  a legitimate home, not a failure.
- **Specialised models per proven category**: document -> historia
  (proven); persisted value -> plain field (proven); kinetic -> motus
  (proven); observation -> ports (proven); transient -> not saved
  (to build); popover -> dismissal (candidate); reference ->
  validation (candidate); app-wide non-document -> unknown, general
  model for now.
- Open: strict hierarchy, or a base + orthogonal properties
  (lifetime, rhythm, relation) + whole models for some categories?
  zoom = persisted+kinetic, stratum_activum = persisted+reference
  suggest the latter. Let more widgets answer.

Risks and mitigations:
1. Invisible action at a distance - AOP (AspectJ) failed on woven
   behaviour; Redux sagas/middleware drew the same complaint. Aspects
   are NAMED on the field in the manifest; no pattern matching; few,
   named systems. The manifest is where the invisible becomes visible.
2. Aspect interactions grow ~n^2. Test: an aspect must compose with
   the others without special cases ("unless it is also X" = not one
   aspect).
3. Frozen wrong categories (Eskil: API first). Hand migrations before
   the generator; promotion from the general model only when a second
   consumer proves the category.
4. Over-rigour the other way: inventing a category so odd state has a
   home. The general model being respectable prevents it.
5. Cost: systems walk the store; store copy already parked (010);
   measure at S8.

**Positioning (Fran, 2026-10-10):** JS/browser frameworks cannot
realize the ideal state model because they build on layers they do not
own (the DOM keeps focus, selection, scroll, input values, IME state;
hydration exists because server and client are different runtimes).
We own every layer (fenestra's event pump, draw commands, the store,
the volume format, silva's lints), so we can learn from their
explorations and keep optimizing toward the ideal model, changing a
lower layer when the model needs it. Corollary: separate their
PRINCIPLES from their WORKAROUNDS - "keep UI state out of the store"
is a workaround for React's re-render cost, not a principle.

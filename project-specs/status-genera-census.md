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

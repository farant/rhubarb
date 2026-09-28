# Applicationes — state, commands, documents, and a declarative layer (brainstorm)

Date: 2026-09-28
Status: Brainstorm, round 1. Fran's decisions are marked DECISUS. Claude's
proposals are marked PROPOSITUM, and were agreed in conversation unless
marked OPEN. No spec yet. "Applicationes" is a working title for the
subject, not a name for anything. Fran names.

Born in a terminal-planning session (worktree `../rhubarb-secunda`,
planning folder `../terminal-planning/`). The question that prompted it:
**what does it take to close the loop, so writing a useful app (with
state, behavior, saved documents, and any of three surfaces) is as easy as
writing a script?** The session's raw record is
`../terminal-planning/worklog.md` (2026-09-28 entries); the paper drafts
are `../terminal-planning/exempla/todo-{declarativum,litterale}.thistle`
and `todo-comparatio.md`.

Related: ludus-brainstorm.md (§IV command list, §X–XVII state doctrine,
flows, briar's third shape), pictor-spec.md, briar-spec.md v1.10 (§9
deferrals: the `briar-c89` dialect, template fragments, ludus islands in
STML regions, the `status` region, assets by `via=`), stml-visio.md (§9.2
control plane, §10 strangler doctrine, §10.4–10.5 staleness and
vocabulary epistemics, §11 apps as projections), `../lapide-v2/docs/
briar-feedback/feature-requests/` 002 (runtime file scheme), 003 (pty),
008 (`scriba`), 010 (structured commands), 017 (repeated options).
Tabularium principium 01M3MCQXH14JCAR6A6N9KSAZEF (region "abstract ui
component library").

Influences: HyperCard (the stack is the document and the program), the
macOS document model (autosave in place, versions), SQLite's "application
file format" argument, re-frame (named events + named subscriptions),
Elm, nob.h, execline, PowerShell/nushell (records, not text), Ink and
OpenTUI (one app model, terminal as a target), Datomic (facts over time).

**Discipline note.** The PULL rule governs every row below: a module
graduates when a consumer needs it, never to complete a table. The
second rule, Fran's (§II.3), governs the ORDER: extract work into
composable modules first, so that a wrong top-level design still leaves
the house richer.

---

## I. What it is

Five things a useful app needs, and a house answer for each:

| need | answer | state of it |
|---|---|---|
| **state** | islands (STML trees, three genera) + canon | exists (`insula.h`, `canon.h`) |
| **behavior** | commands: named intents with typed inputs/outputs; effects through a pons | pieces in five places (§III); unjoined |
| **persistence** | documents: one sqlite file, event-sourced, self-describing | `volumen.h` exists; the binding to islands and the lineage do not |
| **surfaces** | one componens tree, rendered to a window, a terminal, or WebKit | window exists (ludus); terminal designed (terminal-planning 012/013); WebKit partly (vitrea + the lab's `componentia.js`) |
| **packaging** | briar: a thistle runs from its shebang and builds to an `.app` that owns a document type | briar + fasciculum exist; document types do not |

The spine (§IV) is that these share ONE vocabulary: a command's durable
effect is a standard verb; a standard verb is an event; in a document app,
the event log IS the document.

## II. DECISUS — what Fran decided (2026-09-28)

1. **C is always there** (principium 01M3MCQXH14J…). DSL layers grow in
   where they make something easier or more concise than the C, each with
   a clear way to work with C written elsewhere. Over time some apps may
   need little or no C, but C stays the escape hatch. Never a
   "declarative only" framework from the start.
2. **Build it in briar.** Widen the kinds of blocks a thistle has,
   including, possibly, TSX-style STML literals inside C, compiled by
   briar.
3. **Extract composable modules first** (this document's ordering rule).
   "Even if we get the design wrong for the final thing and have to make
   another run at it later, we're still ahead, because the library of
   reusable libraries has been enriched."
4. **A command is the unit for anything with a side effect.** A registry
   gives a command palette for free; logic becomes testable because
   effects run separately or batched. The same shape serves desktop apps
   AND shell-script-like programs (lapide 008 / 010).
5. **Persistence is the gap to close.** Entities event-sourced behind the
   scenes, with simple generic CRUD events; storage is a DOCUMENT file the
   app opens again later; the thistle declares the document type it
   handles (`.todos`), so its `.app` opens and saves those files; binary
   data is easy to store (probably sqlite via scrinium).
6. **Documents describe themselves.** Embed the whole thistle, or at least
   the schema that created the document, into the document, as a tool for
   schema evolution.
7. **Commands are categorized by entity kind:** what kinds they take as
   input, what they produce.
8. **User binaries are a primary use case:** a user pastes a screenshot
   into a todo's description. App assets are colocated with the thistle
   and bundled into the `.app`.
9. **The terminal is a render target for ludus apps** (and later for 3D),
   in the OpenTUI spirit. Tracked in terminal-planning 019/020 and
   012–015.

## III. What exists (measured 2026-09-28)

| piece | header | what it gives | consumers |
|---|---|---|---|
| document file | `volumen.h` | "documentum = plagula sqlite una, veritas eventilis intra": acta (append-only events), massae (SHA-256 content-addressed blobs), plagulae (rebuildable projection); "cp voluminis = cp documenti". Name NOT sealed | 25 files call `volumen_aperire` (grep, lib/apps/silex/briar) |
| sqlite | `scrinium.h` | wrapper + append-only migrations + ULID | volumen, tabularium |
| state | `insula.h` | STML tree, three genera, porta unica, restore/lie detection, `InsulaDominus` (owners), `InsulaActarius` (post-write hook: "pictor P3 hic acta appendit") | pictor, salutatio |
| schema | `canon.h` | closed vocabulary, cardinality, fines, citations | everywhere |
| actions | `actio.h` | named registry, resolved both ways (L10) | pictor |
| loop | `dispensator.h`, `derivare`, `destinatio`, `motus` | the ludus event loop | pictor |
| drawing | `componens.h`, `figura.h`, `mandatum.h`, `delineare_mandata.h` | logical tree → six primitives → pixels | pictor |
| window glue | `ludus_fenestra.h` | events in, frames out | pictor, salutatio |
| testing | `manus_ludus.h` | selector-driven, target-free | pictor probationes |
| selectors | `selectio.h` | CSS selectors over STML (id, attribute, combinators) | several |
| processes | `processus.h` | NULLA CONCHA (argv vectors), sync + incremental paths | many |
| arguments | `argumenta.h` | flags, options, positionals | tools |
| text diff | `differentia.h` | diff | tools |
| entity kinds | natura | `.genus` with `actiones`, `termini` (`terminus munus= ad= multiplex= necessaria=`), `machina_statuum` | censuses |
| clipboard images | `lib/clipboard_platform_macos.m` | reads TIFF/PNG from NSPasteboard into a bitmap | — |
| old command registry | `registrum_commandi.h` | bool fn + context coupled to `Pagina`; `$date` text tags | 1 file |
| old entity stack | `entitas.h`, `entitas_repositorium.h`, `persistentia.h` | typed properties incl. `TYPUS_BLOBUM` (auto-compressed), event sourcing to memoria/nuntium, "sqlite (futura)" | 1–2 files each |
| thistles | briar | regions, `methodus=` (tag = registration), `briar_tractator_exemplar` (signature check), fragments, `-app` (fasciculum) | lapide-v2, examples |

**The working prototype of the whole idea is the tabularium itself:**
entity genera, event-sourced truth (`gesta/annales/tabularium.jsonl`) with
a rebuildable sqlite projection, commands per genus exposed over MCP,
status machines. Read its record before designing the general version.

Gaps measured: fasciculum writes no `CFBundleDocumentTypes` /
`UTExportedTypeDeclarations`; fenestra has no `application:openURLs:` and
no open/save panels (only vitrea has an `NSOpenPanel`,
`lib/vitrea_macos.m:526`); fenestra has no drag-and-drop; `canon.h` has
no canon-to-canon diff; `Componens` has no link back into state, no
action argument, no layout roles.

## IV. PROPOSITUM — the spine: one vocabulary

```
        ┌──────────── command (named intent; termini typed by genus) ────────────┐
        │  pure logic in C (or a declarative region that compiles to C)          │
        │  effects through a PONS: real · recording · dry-run · replay · batched │
        └─────────┬───────────────────────────────────────────┬──────────────────┘
                  │ durable effect = STANDARD VERB             │ external effect
                  ▼                                            ▼ (process, file, net)
   island (STML, canon) ──actarius──▶ acta in the DOCUMENT      pons executes; not undoable
                  │                    (volumen: one sqlite file;
                  │                     massae hold user binaries,
                  │                     the canon, the thistle)
                  ▼
   componere → componens tree ──layout(metrics)──▶ Fines
                  ├─ pingere → Mandata ─┬─ delineare → window
                  │                     └─ cells     → terminal
                  └─ web components by partes       → WebKit
```

- **Standard verbs are the generic CRUD events.** `ponere` (set),
  `commutare` (toggle), `addere` (append, with id minting), `delere`,
  `vacare` (clear), each addressed by a node path. One verb call = one
  actum. The todo drafts found these independently as their keystone (C5
  in `todo-comparatio.md`): C89 has no closures, so without them every
  write is a handler plus a mutator callback plus a context struct.
- **The pons is how commands stay testable when effects interleave.** Pure
  "decide, then run the effects" fits UI actions. It breaks for scripts,
  whose decisions depend on effect results (run `pdfinfo`, then loop over
  the page count). Effects therefore go through a vtable, the house seam
  (tessera's pons, the planned pty pons). A **recording** pons turns a run
  into a plan; **dry-run** records without executing; **replay** feeds
  recorded results back (lapide 008's record/replay); **batched** queues
  UI effects until after dispatch. The control plane's "canon judges the
  plan before execution" (stml-visio §9.2) becomes a canon for effects
  that a recorded plan must pass.
- **Two effect classes.** Durable effects (standard verbs into the
  document) are undoable through the log. External effects (a process, a
  network call, a file outside the document) are not. A command declares
  which it has; undo stops at an external effect and says so.
- **Named reads are the twin of named actions** (re-frame's events and
  subscriptions). No expression language means derived values (counts,
  filters, "is this selected") are named pure functions over the islands,
  referenced by name. They're C by default, or a declarative region where
  that's shorter.

## V. PROPOSITUM — the module inventory

The heart of §II.3. Every row is a library that is **useful on its own**,
so each is worth building even if the top-level design (§IV) changes.
"Standalone value" names who benefits if the rest never happens.

| # | module (working name) | covers | depends | standalone value | state |
|---|---|---|---|---|---|
| M1 | **verba insulae** | the standard verbs as a C API over `insula`, addressed by node path; id minting for `addere` | insula, selectio | pictor and every ludus app: one-line writes instead of mutator + ctx | new, small; **keystone** |
| M2 | node paths | resolve a path (`durabilis/res[id=r3]`) to a node, and a node to its canonical path | selectio, stml | anything that addresses STML: islands, censuses, the tabularium | mostly `selectio`; the canonical path-of-node direction is new |
| M3 | **documentum** | islands ⇄ volumen: open = replay from nearest checkpoint; each standard-verb write = one actum; checkpoints; **writer lineage** (§VI.1) | volumen, insula, M1 | pictor's documents today; any event-sourced app | new; pictor_documentum is the precedent |
| M4 | volumen image source | an `ImagoFons` over massae (hash → `Imago`, decoded and cached) | volumen, imago_png | pictor; any app showing stored images | new, tiny |
| M5 | canon diff | two canons → the four quadrants (same / shifted / only-old / only-new) | canon | migrations (M3), vocabulary audits (stml-visio §10.5), census drift | new |
| M6 | **commands registry** | name, input/output schema, entity-genus termini, effect class; adapters: C call, CLI subcommand, palette entry, MCP tool | canon or natura, argumenta | CLI tools, MCP servers, palettes; supersedes `registrum_commandi` | new; §VI.2 |
| M7 | **effects pons** | `currere`, `capere`, file writes (atomic), temp dirs; real / recording / dry-run / replay implementations | processus, via | every thistle script (lapide 008); command tests | new; the core of `scriba` |
| M8 | argument schema | declarative options with typed defaults, `--help` from the schema, repeated options | argumenta | every CLI; M6's CLI adapter | extend `argumenta` (lapide 017, bugs/004) |
| M9 | structured commands | `find`, `wc`, `sha256sum`, `stat`… as C functions returning records, streamed; text → record parsers (`pdfinfo`) | M7 | lapide's plugins and jobs; any script | new; lapide 010's spike |
| M10 | layout pass | layout intents (column, row, fill) + spacing as **thema steps** → `Fines`, given target metrics | componens, thema | pictor (the reserved `mensurare` pass); every surface | new |
| M11 | componens growth | node-path link to state, action argument, layout roles, setters for every field | componens | everything above it | extend |
| M12 | terminal target | `Mandata` → cells, `ludus_tessera` | tessera, delineare_mandata, blitter | any ludus app in a tty | terminal-planning 012/013 |
| M13 | WebKit target | componens → web components by `partes`; `projectum://` runtime scheme for document blobs | vitrea, internuntius | any vitrea app (lapide 002) | new; `componentia.js` is the reference |
| M14 | document types | Info.plist document types in fasciculum; `application:openURLs:` → an event; open/save panels, in fenestra's platform seam | fasciculum, fenestra | every `.app` that owns a file type | new, bounded |
| M15 | drag-and-drop | files and images dropped on a window → an event | fenestra | every native app | new, later |
| M16 | briar region roles | the ludus shape: `<insulae>`, `<canon insula=>`, `<probatio>`, `<documentum>`, `<c! actio= / lectio= / componere>`; the fabrica generates the wiring | briar, M1–M3 | ends ~200 lines of wiring per app (`apps/pictor/pictor.c`) | new |
| M17 | briar tree literals | STML literals in C lowered to builder calls, line-preserving, table from canons | briar, canon | shorter C for any tree-building code | new; briar §9 `briar-c89` |
| M18 | rich text field | editable text with inline elements (an image inside a description) | componens, M10, text stack | every app with notes or descriptions | new, **hard**; the last one |

Rows M7–M9 are a separate track that needs nothing from ludus: the
script side of §II.4.

## VI. PROPOSITUM — details

### VI.1 Self-describing documents (§II.6)

- The canon text and the thistle source are stored as massae
  (content-addressed; the same version twice costs nothing).
- When an app opens a document with a different canon or thistle than the
  last writer, it appends a **writer** actum: canon hash, thistle hash,
  briar's closure key.
- So every event can be read under the schema it was **written** with.
  Migration becomes a canon diff (M5) with both declarations in hand:
  same and one-sided changes are automatic; **shifted** fields (same
  name, different shape) are loud and need a declared migration, written
  *before* the first incompatible change ships.
- The chain of writers is the document's lineage: stml-visio §10.5's
  "vocabulary stemmatics", with no trust in version stamps.
- Embedding the thistle means **a document can bring back the app that
  wrote it**: briar rebuilds from the embedded source. The document
  carries its own reader.
- Caveat: sharing a document shares the app's source. Document it; offer
  a canon-only mode if it matters.

### VI.2 Commands typed by entity kind (§II.4, §II.7)

- A command is (name, input termini, output termini, effect class). The
  termini reuse natura's vocabulary: `terminus munus= ad= multiplex=
  necessaria=`, where `ad` is an entity genus.
- What falls out:
  - the palette filters by the selection's genus (select a `res`, see the
    commands that take one)
  - CLI subcommands and `--help` from the input schema (M8)
  - MCP tool schemas (lapide 010)
  - typed pipelines (stml-visio §9.4's `genus=` discipline)
  - status transitions as commands (natura `machina_statuum`)
- A GUI app's commands work **headless on its documents**:
  `./todo.thistle -iussum res.addere lista.todos "legere psalmos"`. The
  palette, the CLI and MCP are three views of one registry.
- ludus `actio` becomes the UI binding: event → "invoke command X with
  these arguments". Logic moves into commands; actions stay thin.

### VI.3 Documents, binaries and the autosave model (§II.5, §II.8)

- Every standard-verb write is appended at once, so the model is
  **autosave in place**: no dirty flag, no "save changes?" dialog. Undo
  is the log. "Duplicate" is a file copy. A *new* untitled document is a
  temporary volumen until first named.
- Write granularity: typing in a field changes ephemeral state; the
  durable actum happens at commit (Enter, blur). Drags are kinetic
  (`motus`), and one durable write happens at release, as pictor does.
- User binaries are massae, referenced by hash from the islands. STML
  islands allow inline content, which a JSON-shaped store can't do
  cleanly:
  `<descriptio>fac hoc <imago sigillum="9f2c…"/> cras</descriptio>`
- Rendering is already designed around references: `Mandata`'s `imago`
  carries a name and an `ImagoFons` resolves it (pictor resolves hash →
  image). M4 serves the window and terminal; M13's `projectum://` serves
  WebKit.
- A deleted image's massa stays: undo and history need it. A "compact /
  clean copy" export is optional.
- App assets (icons, bundled images) are colocated with the thistle and
  packed into the `.app` by the capsula mechanism (briar §9 "multiple
  assets by `via=`").

### VI.4 The declarative layer, by the principle (§II.1–2)

From the paper drafts (`../terminal-planning/exempla/`):

- Every declarative construct **compiles** (briar's fabrica) to C you
  could have written. No hidden runtime interpreter.
- Every declarative region has a **C twin** of the same name and
  signature (`<lectio>` / `<c! lectio=>`, `<actio>` / `<c! actio=>`,
  `<visus>` / `<c! componere>`). They mix by name in both directions.
- **Removable:** a declarative region can be expanded and frozen into C
  (stml-visio §10.1).
- A construct earns its place when it is **shorter** *and* carries
  something C can't (canon judgment, target neutrality, serialization),
  measured on real thistles.
- Already winning as data in both drafts: `<insulae>` (initial state),
  `<canon>`, `<probatio>` (selector-driven; one test for all three
  surfaces), one-verb actions. Already winning as C: reads and views with
  conditions.
- The region tag is the ludus annotation (§XII's `<componens/>`,
  `<purus/>`, `<tractator/>`), so the ludus codices can judge thistle
  regions by role.

## VII. PROPOSITUM — the walking skeleton: one todo app, in stages

Each stage lands modules that are worth having even if the next stage
never happens.

| stage | the todo app gains | modules landed |
|---|---|---|
| S1 | a window, in plain C (no DSL) | M1, M2, M11 |
| S2 | persistence: `./todo.thistle lista.todos` | M3, M5 (writer lineage from day one) |
| S3 | a terminal surface | M10, M12 |
| S4 | commands: palette, CLI on documents | M6, M8 |
| S5 | `.app` that owns `.todos` | M14 |
| S6 | paste a screenshot into a description | M4 (+ clipboard, exists) |
| S7 | declarative regions where they win | M16 |
| S8 | tree literals | M17 |
| S9 | WebKit surface | M13 |
| later | drag-and-drop, rich text | M15, M18 |

In parallel, the script track: M7 + M8 + M9, proven by lapide 010's
spike (rewrite `paginae.thistle` on structured commands and compare line
counts with today's ~200).

## VIII. OPEN

- **Names**, all unsealed:
  - the command: `mandatum` is SEALED for ludus draw commands, and
    `imperium` is taken (`imperium.h`). Candidates: `iussum` (iubere),
    `praeceptum`.
  - `volumen` and `scrinium` (both marked NONDUM SIGILLATUM in their
    headers); `documentum` for M3.
  - node path: `via` in ludus-brainstorm (L6, "identity by VIA") and in
    the drafts, but `via.h` is FILE paths. One word for two things.
  - `lectio`, `visus`, `columna`, `ordo`, `spatium`, `argumentum`, and
    the verbs.
- **Two entity models.** The `entitas` graph (typed properties,
  relations, event-sourced, `TYPUS_BLOBUM`) or ludus's STML islands. The
  proposal is islands on volumen (pictor proved the path; canon, restore
  and replay all apply), learning from entitas's relations and typed
  parsing. ludus-brainstorm §VI.2 holds the same fork.
- **Is ephemeral state in the document?** Reopening a `.todos` could
  restore the filter and focus. Probably yes, as non-undoable state.
- Hole syntax for tree literals (`{…}` or house-shaped), `argumentum=`
  or one action per value, attribute-matching EXEMPLAR or element-shaped
  islands (`todo-comparatio.md`, questions 1–3).
- Several documents open in one app: one repository per document; what
  about commands that span documents?
- UTI naming for document types (`org.rhubarb.<app>.<ext>`?) and what
  happens on another Mac where the `.app` isn't installed.

## IX. AUDIENDA — not established

- volumen's write rate under an interactive app (pictor-spec §10: never
  measured).
- Replay time on open for a document with 10k+ acta, with and without
  checkpoints.
- sqlite blob behavior for many multi-MB images in one volumen (page
  size, vacuum, file growth).
- Whether `selectio` can express every path M2 needs (attribute equality
  is supported per its header; canonical path generation is not
  checked).
- Whether macOS delivers `application:openURLs:` to an unsigned,
  locally-built `.app` the way it does to a signed one.
- The state of `argumenta.h` against lapide 017's repeated options (not
  read).
- Whether `registrum_commandi`'s one consumer can move to M6 without
  loss (not read).
- Everything in the drafts is paper: no line of the todo app has been
  compiled.

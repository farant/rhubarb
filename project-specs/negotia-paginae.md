# negotia paginae — pages of a project in the ring

Status: DRAFT 2026-11-05 (builder check done, Fran's answers folded in),
brainstormed with Fran (secunda session 134c8417), after the first slice (`negotia.md`, plan T1-T6, all in
secunda). Latin names are proposals until each task's header text is
approved.

*The idea (Fran, 2026-11-05): a project's Cmd+H / Cmd+L ring becomes a
stack of heterogeneous pages - the built-in ones (feature list,
conversations) and pages added over time. The first added kinds: pictor
pages (Fran's art direction, mood boards) and design mock-ups (Claude's
proposals, drawn in vicus itself). Each page has an id we can both name,
its own notes, and stays with the project.*

## I. Intent

1. **A shared, named surface between Fran and Claude inside a project.**
   Claude proposes a pane design as a page; Fran steps to it with Cmd+L,
   writes notes on it, and says "look at page K7Q2XA". Fran collects art
   direction on a pictor page; Claude reads it as a PNG.
2. **Recent work is close.** The newest or most recently touched page is
   one Cmd+L past the built-in pages; old pages drift to the back by
   themselves.
3. **High-fidelity mock-ups that are almost the code.** A mock-up is drawn
   with the same layout system and drawing functions as real panes, so
   what Fran sees is what the pane would look like, and turning it into
   a pane is short.

Non-goals (v1): Fran adding pages from inside vicus (Claude adds them
through the CLI; the vicus-side gesture comes later); archiving or
deleting pages; an index view of all pages; interactive mock-ups
(clicks, hover); text in pictor.

Anti-goals: pages that leak into the common libraries (a project's
pictures in pictor's shared book); a mock-up format that drifts from
how panes really draw (then mock-ups lie); a second home for notes
(page notes are ordinary Cmd+T companion notebooks).

## II. Decisions (Fran, 2026-11-05)

1. Pages are kinds in the ring, beside the built-ins; the ring is the
   project's stack.
2. Every page has a stable **id**, shown on the page, used in
   conversation; Claude can look a page up by id.
3. Every page has its own **companion notebook** (Cmd+T); Claude reads it
   by id when Fran asks.
4. **Ring order:** built-in pages first (feature list, then
   conversations), then added pages **most recently active first**.
   Cmd+L from the last built-in reaches the newest; further = older.
5. **Pictor pages = mood boards / art direction** from Fran to Claude;
   exportable to PNG so Claude can look at them.
6. **Pictor pages are scoped to their project** like companion notebooks:
   they are not part of pictor's common book of pictures.
7. **Mock-ups are high-fidelity, data-driven,** close to the code that
   would render the real pane, **with an escape hatch into arbitrary
   drawing.** (Pictor has no text, so mock-ups are not pictor images.)
8. No archive in v1: old pages fall to the back of the stack. Later: an
   archive that takes a page out of the ring, plus an index view to
   drill into archived pages.
9. **Mood boards are made in vicus only.** vicus and the mobile app will
   share data some day, but each keeps its own capture/editing modality;
   no mobile editing of project pages now (pictor pages live in the vicus
   volume, no communio).
10. **Mock-up templates are STML macros** (fragments with declared
    parameters, called per sample record) - no new template language.

## II-a. Builder's calls (Claude, after the builder check; Fran may overrule)

1. **Writing notes on a page counts as activity** (most recently active,
   decision 4): a page's activity includes the last act of its companion
   notebook.
2. **Notes are per page, not per revision:** one notebook follows the
   design. The CLI's notes output interleaves the mock-up's revision
   times with the notes, so Claude sees what was written before and after
   each revision. Stepping through old revisions in vicus: later.
3. v1 mock-ups use fragments + calls only; the macros' iteration over a
   data block can come later without changing the format.

## III. What exists (cited)

- **Drawing commands have a text form.** Everything vicus draws is a
  display list of six primitives - coetus (group), rectangulum, linea,
  polygonum, imago, textus - with colours as theme slots or RGBA
  (`include/mandatum.h`), serialised both ways:
  `mandata_scribere_stml` / `mandata_legere_stml`.
- **Display list -> PNG headless:** `delineare_mandata` onto
  `tabula_pixelorum_creare_nuda`, `tabula_pixelorum_in_imaginem`,
  `imago_png_scribere` (the T3/T4 look checks did exactly this).
- **House drawing pieces:** `include/figura.h` (mensa, globulum,
  sagitta, segmentum, capsa, ordo electus / super, fascia, campus,
  textus audax); layout in character cells: `include/dispositio.h`
  (Clay-style; used by every list pane).
- **Pictor documents** live in the vicus volume, named by a path-like
  *spatium*: append-only acts (strokes, layers, pasted images; bitmap
  derived) - `include/pictor_documentum.h`. `pictor_documentum_aperire`
  + `pictor_documentum_proiectio` give the bitmap. The pictor pane opens
  a spatium by argument (`$pictor(x)`).
- **Pictor's shared lists:** `pictor_documenta_enumerare` /
  `pictor_documenta_recentia` (users: `lib/pictor_applicatio.c:321,616`,
  `lib/pictor_documentum.c:1254,2291`, `lib/vicus_applicatio.c:1645,1724`).
- **Companion notebooks** are scriba books named `comes/<locus>`; scriba's
  book list skips that prefix (`lib/vicus_applicatio.c:1107,1242`).
- **The ring** (negotia T5/T6): per tab, `vicus_anulum_ponere`; virtual
  genera via `vicus_aperitorem_addere`; set when a project is opened
  (`negotium_aperire_in_vico`): [("negotia", id), ("colloquium", s)...].
- **negotia records:** acts in the vicus volume, full snapshot per act,
  revisions for free, `negotia_versio` for change detection.

## IV. The bones

**Page** (`negotii-pagina`, full snapshot per act like features):
`{id (ULID), negotium, genus, titulus, corpus}`.
- `genus` = `pictor` | `designum` (mock-up). Fixed per page.
- `corpus`: for `designum` the mock-up document (below); for `pictor`
  empty (the picture lives in pictor's own acts).
- Every saved change = one act = one revision (as features).
- The **id** shown and spoken is the last 6 characters of the ULID (the
  CLI's existing suffix convention; full id or unique suffix accepted).

**Activity** (ring order): a page's last activity = the latest of its
last act, the last act of its companion notebook (II-a.1), and for a
pictor page the last act in its pictor document.

**Notes:** the page's companion location is `negotia/pagina/<id>` (full
id), so its notebook is `comes/negotia/pagina/<id>`.

**Pictor page document:** spatium `negotia/<page id>`; pictor's shared
lists skip the `negotia/` prefix (as scriba skips `comes/`).

## V. The mock-up format (designum)

An STML document, two layers, with **STML macros** for repetition
(decision 10; `include/stml_macros.h`, spec `stml-macros-spec.md`):
expansion happens at load time, so the renderer consumes the expanded
tree while the stored document keeps its template form; every reference
error is named (unknown fragment, unfilled or extra argument). Example:

```
<designum latitudo="600" altitudo="700">
  <#ordo titulus="@t" status="@s">
    <linea altitudo="2">
      <globulum status="&@s;"/>
      <textus>&@t;</textus>
    </linea>
  </#>
  <columna>
    <textus fons="titulus">negotia</textus>
    <<#ordo t="library" s="shipped">>
    <<#ordo t="vision" s="planned">>
    <mandata> ... raw primitives ... </mandata>
  </columna>
</designum>
```

(Exact element and attribute names are fixed in P2's header; this shows
the shape.)

1. **House pieces** - a layout tree in character cells (the dispositio
   model: column/row boxes, fixed or growing sizes, padding) whose leaves
   are the figura pieces and text: e.g. `<columna>`, `<linea>`,
   `<textus color="dim">`, `<globulum status="shipped">`, `<ordo
   electus="verum">`, `<sagitta directio="dextra">`, `<fascia>`,
   `<mensa>`. Rendered by the same functions real panes call.
2. **Escape hatch** - a `<mandata>` block holding the six primitives in
   the existing serialisation, positioned in its box.

Also: a declared surface size (default: the right pane), the theme
palette by slot names. Malformed input draws an error page naming the
problem, never a blank pane.

## VI. How it is seen and used

- **In vicus:** opening a project builds the ring
  [features, conversations..., pages by recent activity]. A mock-up page
  is a view of the negotia pane (argument `designum/<id>`); a pictor page
  is the ordinary pictor pane on spatium `negotia/<id>`. Both show the id.
  Cmd+T on either opens that page's notebook.
- **CLI (Claude):** add a page (genus, title, body file) -> prints id;
  revise a mock-up (new body); list a project's pages (id, kind, title,
  last activity); export a page to PNG (mock-up rendered headless; pictor
  page = its bitmap); print a page's notes (with the mock-up's revision
  times interleaved, II-a.2).

## VII. Phases (PROPOSED)

| | |
|---|---|
| **P1** | Page records in `lib/negotia.c` (create, revise, list, revisions, activity) + CLI verbs; ids; tests. |
| **P2** | Mock-up format: macro expansion, parser + renderer to a display list (house pieces + `<mandata>`), headless PNG; CLI export. |
| **P3** | Mock-up view in the negotia pane (`designum/<id>`), id shown, companion location. |
| **P4** | Pictor pages: spatium `negotia/<id>`, pictor's shared lists skip `negotia/`, PNG export, notes. |
| **P5** | Ring: pages appended after built-ins by recent activity; refresh when records change while the project is open. |

## VIII. AUDIENDA

- Whether the pictor pane can open a document whose spatium contains a
  `/` and is not in the shared book (not checked).
- How a companion notebook's text is read outside vicus (scriba book API
  from the CLI; not checked).
- Activity of a pictor page needs the last seq of its pictor acts
  (`pictor_documenta_recentia` computes it for the shared list; a
  per-spatium query was not checked).

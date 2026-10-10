# pictor strata — plan (layers)

*Written 2026-10-09 from Fran's answers, in secunda, after the pictor
tool arc (P0-P4, eraser, patterns, sizes, line tool) and the drawing
library (`$pictor-next/-prev/-new`). The question: pictor drawings are
one RGBA buffer; Fran wants layers - transparent sheets drawn on
separately, composited over the canvas background, with a panel to
select, add, delete, hide and reorder them.*

*Discipline note: phases land one at a time, each red-first with
plants, each committed with its gates. Nothing here is built ahead of
the phase that needs it (PULL rule).*

## I. Decisions (Fran, 2026-10-09)

1. **All layers start transparent**, including the first. The canvas
   background (theme `COLOR_BACKGROUND`, never stored) shows through
   wherever no visible layer has a pixel; lower layers show through
   undrawn pixels of upper ones.
2. **Tools draw on the current layer.** The eraser clears pixels of the
   current layer to transparent (today it paints the background colour
   into the single buffer).
3. **Hiding and showing a layer is history**: an entry in the
   drawing's log, saved, undoable, and the same in every pane that shows
   the drawing (drawings are shared objects - vicus S3e).
4. **Names are numbers**: "stratum 1", "stratum 2", ... - like scriba
   pages, the increasing id is the name. Renaming may come later.
5. **At most 16 layers for now**, expected to rise: one constant, and
   no stored format may assume it.
6. **Old drawings keep working**: a drawing made before layers opens as
   a single layer 1 and LOOKS IDENTICAL.
7. **UI**: a new square in the bottom bar shows the current layer's
   name; clicking it expands a panel - one row per layer (top layer
   first) with a visibility toggle and the name, the current row
   highlighted, a click selects; add and delete buttons; drag-and-drop
   to reorder.

## II. Where things stand (measured 2026-10-09)

- Pictor's projection is ONE fixed-size RGBA buffer
  (`PictorDocumentum.tabula`, `lib/pictor_documentum.c`), cleared to the
  theme background by `vacare_fundo` and painted by `ictum_applicare`.
- `historia` (`include/historia.h`) checkpoints the projection as one
  memory block every `intervallum` (64) live actions: the block is
  stored content-addressed (`volumen_massam_condere`) and keyed
  `checkpoint/<seq>` (`checkpoint_condere`, `lib/historia.c` ~276). The
  seal (`sigillum`) hashes that block; `historia_verificare` replays
  from nothing and compares seals.
- **Restore checks the size**: a checkpoint block whose length differs
  from the projection's is ignored and the log replays from the start
  (`proicere_ad`, `lib/historia.c` ~245). This is the compatibility
  hinge: a new checkpoint layout simply makes old checkpoints unusable,
  never wrong.
- The ephemeral canon already declares `stratum_activum` (numerus),
  unused (`apps/pictor/canones/ephemera.canon`).
- `flatura` (house deflate, `include/flatura.h`) exists: mostly
  transparent layers compress to almost nothing.
- Scriba is the other `historia` client (text grid); its checkpoints
  must not change.

## III. Phases

### L1 — historia: client-encoded checkpoints

`HistoriaProiectio` gains OPTIONAL hooks so a client can encode its own
checkpoint (and restore from it) instead of the raw fixed block. With
the hooks absent, behaviour is byte-for-byte today's (scriba). The seal
must stay cheap: today it hashes the raw block after every action;
compressing every layer per stroke to seal it would be too slow, so the
seal stays over the client's RAW canonical state and only the stored
checkpoint is compressed. Header text shown to Fran before coding.

Tests: historia with a toy client using the hooks (checkpoint written
encoded, restored on reopen, verificare green, a corrupt encoding
falls back to replay); scriba and pictor suites unchanged.

### L2 — pictor document with layers

- Strokes carry `stratum="<id>"`; absent = layer 1 (old logs unchanged
  in meaning).
- Layer operations are log entries in the drawing's own genus: add
  (new id = max + 1, placed above the current), delete, reorder,
  visibility.
- Projection = layer table + one RGBA buffer per existing layer
  (allocated when the layer appears; 0 alpha = transparent) + the
  COMPOSITE buffer the UI already reads (`pictor_documentum_proiectio`):
  background colour, then visible layers bottom to top.
- Eraser clears to alpha 0 on its layer.
- Checkpoint = encoded layer table + deflated layer buffers (L1 hooks).
  Old drawings: their raw-block checkpoints no longer match, so the
  first open replays the log once; layer 1 receives all strokes and the
  composite is pixel-identical to today's picture (asserted against the
  committed goldens). The stored reference seals change with the
  format - re-promoted, with the composite comparison as the evidence.
- 16-layer cap as one constant (`PICTOR_STRATA_MAXIMA`).

### L3 — layers UI

Bottom-bar square with the current layer's name; expanding panel: rows
top-first (visibility toggle, name, highlight), click selects
(ephemeral `stratum_activum`), add (+), delete (−, never the last).
Each control writes the matching log entry.

### L4 — reordering by drag and drop

Press a row, drag, release over a position; one reorder entry.

## IV. AUDIENDA (not yet verified)

- ~~Cost of the seal over N raw layers per stroke~~ MEASURED in L2
  (2026-10-09, test build, 468x440, 8 layers): ~29 ms per stroke
  commit, ~43 ms reopen from an encoded checkpoint. Acceptable (paid on
  mouse-up); follow-up: cache a seal per layer and re-hash only the
  layer an action touched (adds PictorStratum fields).
- Whether `volumen` massae deduplicate identical checkpoint encodings
  (they are content-addressed - expected yes, unmeasured here).
- Memory: every drawing a pane has shown stays cached (vicus S3e); with
  layers each costs layers × ~0.8 MB. Fine at today's scale; revisit if
  the library grows large.
- Previews (brush, line, spray) draw over the composite, i.e. above
  upper layers even when drawing below them. Accepted for now.

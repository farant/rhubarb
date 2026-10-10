
## 2026-10-09 - S3e-2: drawing library + swapping the drawing in a pane

For $pictor-next/-prev (Fran: option A - cycle inside the pane, skip
drawings open in other panes; the skip lives in step 3).

- `pictor_documenta_enumerare` lists spatia whose manifest plagula has
  ORIGO "pictor:documentum". Scriba writes "<spatium>/documentum" too,
  WITH latitudo/altitudo, so pictor_documentum_aperire would happily
  open a scriba manifest as a canvas - pictor_picturam_ponere only
  opens spatia the library lists (plant L2 needed a realistic scriba
  manifest to go red; "<documentum/>" failed to open on its own).
- PictorMontatio.documenta caches every drawing the pane has shown:
  each PictorDocumentum is ~1 MB of pixels + history in the arena, so
  re-opening on every swap would leak; the cache also keeps one doc
  object per spatium per pane. Root spatium "" is stored under "."
  (tabula_dispersa does not take an empty key; '.' never appears in a
  pane id - canon).
- The swap re-points montatio.doc, actiones_ctx.doc, figurae_ctx.doc
  and writes durable latitudo/altitudo (the composer sizes the canvas
  from those). PictorApplicatio.doc (standalone app copy) is NOT
  updated - only vicus panes swap.
- pictor_documentum_finis counts log entries, not strokes (+2 per
  stroke here); assert on numerus_vivorum.

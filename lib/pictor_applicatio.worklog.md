
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

## 2026-10-20 — semitae R4c: pictor on routes (Fran approved the text)

- New manifest field `pictura` (textus, transiens, ligatio impelle, no
  scriptor - derived like latitudo/altitudo). Written in two places
  only: the mount's initial attributes (` pictura="<id>"`, absent for
  the root pictor of the standalone app) and `dimensiones_mutator`
  (now takes a PicturaOstensa {doc, spatium}) inside
  pictor_picturam_ponere - the single switch point.
- `pictor_argumentum_ponere(m, arg, condere)`: URI ("pictor/flores")
  shows an EXISTING picture and never creates (routes only set
  state); existence is checked (pictura_exstat = memory or library)
  BEFORE semita_applicare so a refusal changes nothing. Alias keeps
  the S3e/S3f behaviour; mount passes condere=FALSUM (it never
  created), argument change passes VERUM ($pictor-new).
- The organum for the URI parse is loaded into a per-call scratch
  piscina and destroyed (no PictorMontatio field = no header struct
  change); argument changes are rare user commands.
- `pictor_semitam_secuta`: after a history step the document follows
  the `pictura` field.
- vicus `pictura_lateris` now reads the `pictura` field first: after
  Ctrl+O the pane's `argumentum` is stale, and $pictor-next would have
  cycled from the picture you left.
- Test note: without vicus's shared document memory a re-opened
  picture is a NEW PictorDocumentum pointer - compare dimensions, not
  pointers (probatio_pictor_semitae flores_ostensae).

## 2026-10-26 - relaunch reopens the last picture (pinacotheca T6, quarta)

Standalone pictor already persisted the `pictura` field (conservatio
writes persistent fields after every successful write) but
`pictor_montare` always mounted the root picture. `pictor_applicatio_
aedificare` now acts on the restored field after montare: a library
picture -> `pictor_picturam_ponere`; named but missing -> the most
recently edited one (`pictor_documenta_recentia`); empty -> root as
before. `app->doc` is assigned AFTER this (the audit trap: app->doc
stays the old picture after ponere). The spatium is copied before
ponere because ponere rewrites the field it was read from. Tests use a
real file in `credo_area(c)` (volumen_temporarium deletes on close).

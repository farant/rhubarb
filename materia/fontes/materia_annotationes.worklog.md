# materia_annotationes worklog

## 2026-09-25 — silva-migratio T11: decoration and the per-comment kernel

Born 2026-09-18 for crusta (excusatio arc) with one string of
decoration (`praefixum`). The excusatio spec §4 deferred block comments
"with a named trigger: a materia client whose comments are blocks";
silva became that client at T11.

- `MateriaDecoratio {aperitio, clausura, continuatio}` and
  `materia_annotationem_legere` (one comment → annotation or prose).
  `materia_annotationes_colligere(praefixum)` is now a wrapper over
  `materia_annotationes_decoratione_colligere(decoratio)`.
- ANCHOR = `<` followed by a LETTER (silva's rule). Measured over all
  250 house shell files: one comment changes class —
  `probationes/fixa/crusta/adversarius.sh:35`, `# </crusta-commentum>`,
  previously a malformed annotation that nothing reported (excusatio
  reports only tolera-shaped malformations). tools/diagnostica over all
  .sh: identical before and after.
- The anchor is judged on RAW bytes before any allocation, so an
  allocation failure keeps the annotation (unparsed), never prose.
- Parsing runs over the purged body WITH its leading whitespace and
  newlines (so error lines map from the comment's first line); `textus`
  is a view from the anchor, which excusatio's `_tolera_videtur`
  requires. For crusta that moves `columna_erroris` by one (the space
  after `#`); nothing reads it.
- Found writing the css test: with prefix-only decoration a block
  comment does NOT fail — the capture form `(>` swallows the trailing
  `*/` into the cause, so a css tolera would parse with a lying cause.
  The decorated collector fixes it; wiring css into tools/diagnostica is
  left for when css wants annotations (Fran: the comment model gets more
  flexible later — e.g. per-genus decorations for a language with both
  block and line comments, as C89 has; today silva picks per genus
  itself).
- Plants: newline dropped in purge → exact error line red; letter rule
  removed → both prose cases red.

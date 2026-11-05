# materia_nodus worklog

## 2026-11-02 — a node and its slots in one allocation

`materia_nodus_creare` made two arena allocations per node: the
24-byte header, then the `loci` array (24 bytes per slot). Now one
allocation holds both, and `loci` points just past the header
(`nodus + 1`). On the largest MusicXML score (Reger op. 74) that is
1.44M fewer allocations in the XML tree (7.84M -> 6.40M), with the
same bytes.

Why this is safe:
- Alignment: C pads a struct's size to its own alignment, and
  MateriaNodus holds pointers, so `nodus + 1` is pointer-aligned, which
  is all MateriaValor needs.
- Nothing in the repo re-points or reallocates `loci` (checked across
  lib, materia, silva, xml, oratio, css, md, html, toml, crusta, briar,
  tools). `loci` stays a pointer, so the API is unchanged.

silva's amalgam contains this file, so it was regenerated with the
change (`./silva/amalgamare.sh`).

All 9 computus goldens moved (briar, crusta, css, html, md, oratio,
silva, toml, xml): allocation columns only, all lower. oratio's
`commissa`/`otiosa` (bytes reserved / left idle by the arena) rose by
exactly 24 B on two documents while `usus` stayed identical: one bigger
allocation shifts where an arena block boundary falls.

Plant: allocate the header without room for the slots -> 9 of 14
materia suites FRACTA (aequalitas, annotationes, arbor, diagnostica,
excusatio, nodus, plena, scribere, sedes). As with materia_diagnostica,
the plant had to be run by hand with fabrica rebuilt around it (fabrica
links materia).

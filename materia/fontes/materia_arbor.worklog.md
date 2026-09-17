# materia_arbor worklog

## 2026-09-17 — sedes A1: the positioned view (materia-sedes-plan A1)

**What.** `MateriaArborConsilium.sedes_scribere` (last field) makes the
writer stamp `sedes="L:C-L:C"` and `octeti="B-B"` on every node element,
token element and trivium element; locus wrappers, `ante`/`post`, and
the envelope get nothing. The envelope says `visio="sedes"` (or
`"partialis sedes"` with `loci_admissi`), and the reader refuses it with
a cause naming the value (`"visio sedes: non arbor"`; the old
`"visio partialis: non arbor"` is byte-identical for the filter alone,
same attribute order `visio`, `omissi`).

**The range lives in `materia_nodus`, not here.** `materia_tractus_lexematis`
/ `_nodi` / `_conferre` sit beside `materia_sedes_tokeni`, because B's
derivation walker and the writer's refusal ranges need them without the
writer. Start = the effective position (through the origin hook); end =
start advanced over the value by the reader's own cursor rule (`\n` →
next line, column 1; any other byte → column + 1). A token whose hook
says `est_fons` FALSUM (md and oratio derive tokens today) is a
zero-width POINT: its value is not the bytes at that place.

**Node ranges are accumulated bottom-up in the one walk.** The scriptor
carries `tractus` + `tractus_inventus`; `_scribere_nodum_internum` saves
the parent's pair, resets, lets every token use (the transclusion path
too — the node owns the token even when its element is a reference)
merge into it, stamps its own element after the loci, then merges its
range into the saved parent pair and restores. O(n); calling
`materia_tractus_nodi` per node would be O(n·depth). Trivia never merge:
they hang off tokens, not loci, so a node's range never starts at the
comment above it. `materia_tractus_conferre` keeps the first fons it
sees (only C89 includes can mix fontes).

**Measured / noticed while building.**
- Every `MateriaArborConsilium` in the tree (sources, probationes,
  instruments, briar, the C89 shim) is filled by `materia_arbor_consilium_nudum`
  or a helper that calls it; the one struct copy (`cv = c`) copies such
  a consilium. The new field is therefore zero wherever nobody sets it.
- `sedes_colligere` / `paria` are collected but `sedes_valorum` is never
  filled (D2) — left alone; A2's gate pairs elements by document order.
- A test comment holding `/*` broke the build (`-Wcomment`) exactly as the
  memory warns; examen's "domesticum" for `strstr` on a chorda's data does
  not block, but the new gate uses `chorda_ut_cstr` and adds none.
- Gate `probatio_materia_sedes`: 63/63 at first compile; planted
  `finis + I` in `materia_tractus_lexematis` → red at `t.finis == 13`,
  reverted → green.

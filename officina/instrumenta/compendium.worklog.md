# compendium.worklog.md

## 2026-09-24 — born: the header summary, shared by legati and briar

Extracted from `legatus.c` (`_extenta_ex_semantica`,
`_declarationem_contrahere`, and the ordering loop of `_legati_caput`)
so briar's `-bibliotheca <x> -functiones` and legati's `caput` read
headers the SAME way. Lives in `officina/instrumenta/` next to
`praeparator.c` because legatus links instrumenta sources directly
(not the officina amalgam) and briar's runner can compile one more
file; silva itself stays frozen — this module only CALLS its public
API (`silva/amalgama/silva.h`).

- **Three functions.** `compendium_declarationes` (top-level
  declarations of the MAIN file, silva symbol order, with a pointer to
  the `SemanticaSymbolum` so legatus can still write its `signatura`),
  `compendium_ordo` (file order, enumerators deduped behind their
  type, missing corpus dropped), `compendium_contrahere` (one line).
- **Semantics kept exactly**, checked by capturing legati `caput` for
  10 files before and after: 768 lines, 174 changed, and every one of
  the 174 differs ONLY by the parenthesis-spacing fix. Two subtleties
  that nearly changed behaviour: the typedef re-pointing to the struct
  definition keeps the typedef's own comment unless the definition has
  one; and the primary pass sets the comment/corpus even when the other
  is absent.
- **The wart fixed:** contraction no longer writes a space after `(`
  or before `)` (`json_legere ( chorda input, …` → `json_legere
  (chorda input, …`). Done by deferring whitespace instead of writing
  it eagerly.
- **Test** `officina/probationes/probatio_officina_compendium.c`:
  a plain-C fixture (enum typedef, struct typedef, prototype with a
  comment, one without, extern variable, static definition, OPAQUE
  typedef defined later). Plants: dedupe off, the '(' rule off,
  re-pointing off — each red. **Lesson re-learned:** two plants first
  "passed" because they did not COMPILE (`-Wunused-but-set-variable`)
  and my loop counted failed assertions, not build failures. The
  re-pointing plant then compiled and stayed GREEN: the fixture's
  typedefs all defined their struct inline, so re-pointing changed
  nothing. Added `typedef struct Arbor Arbor;` + a later `struct Arbor
  {…};` and it went red on all three assertions.
- **legatus side:** `_extenta_ex_semantica` is now a thin conversion
  (legatus adds only `signatura`); `caput` converts cached extents to
  `CompendiumDeclaratio` and calls `compendium_ordo` +
  `compendium_contrahere`; `LegatiLinea` and the old contraction
  helpers are gone (−367 lines). Build: `compendium` joins the unit
  loops of legatus.sh and sonda.sh, a block in
  officina/compile_probationes.sh, and both generated source lists.
  (Recorded here, not in legatus.worklog.md: that file is Fran's to
  commit — commissio refuses it.)

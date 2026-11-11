# psalmodia_tabula worklog

## 2026-11-10 — R2: reading, canonical dump, totality

Plan: project-specs/psalmodia-plan-1.md R2. Header approved in R1.

**Measured.** The table is 53 794 bytes, 1 160 lines; one full read
(STML parse + walk) takes under 1 ms (`clock()` prints 0 ms in
`_tabulam_veram_probare`). No performance concern.

**Design decisions.**
- The reader collects every error instead of stopping at the first
  (contrast `instrumentarium_legere` in carmen.c): a slip list should
  be complete. Errors are `linea N: causa`, in walk order = file order.
- Arrays are sized by counting an element's children before filling
  (`_liberos_numerare`), not grown with xar as the plan sketched; only
  the error list grows (xar, copied into the piscina at the end).
  Simpler, and exact sizes.
- A `<tonus>` missing modus/clavis/tenor, or with an unknown modus,
  is reported and SKIPPED (its children are not walked: no clef, no
  meaning). Same for an `<exemplum>` missing pars/textus/notae or with
  an unknown pars. Clausulae and loci are never skipped; an unknown
  element inside them is reported and the rest read.
- Leaves (initium, flexa, praeparatio, accentus, nota) take their
  trimmed text; an element nested in a leaf is reported (otherwise
  `stml_textus_valor` would silently swallow its text).
- Booleans accept exactly `verum`/`falsum`; `casus` only
  `monosyllaba`; `positio` only `altera`; anything else is a vitium
  naming the allowed values.

**STML facts relied on.**
- `stml_legere` sets `elementum_radix` only on success: a file STML
  cannot parse (truncated, unclosed, empty) gives ONE vitium at STML's
  error line and no tones. STML is all-or-nothing; the "good tones
  survive" promise holds for semantic errors, not syntax errors.
- Attribute values are raw (entities decoded only in text) and
  interned in the piscina passed in — so the source buffer can be
  freed after reading, as the header promises.
- An attribute value may span lines (`notae="f  gh h\n   g ..."`):
  STML keeps the newline inside the value; tokens split on any ASCII
  whitespace.

**Plants (silva.planta, radix gate, filtrum psalmodia), both RED then
restored byte-identical (backup in build/plantae_psalmodia/):**
- (a) unknown-attribute check silenced → the `tonica` assertion in
  `_totalitas_probare` red. That assertion was ADDED for this plant:
  the plan's fixtures never exercised an unknown attribute.
- (b) tokens keep one trailing byte → `_tabulam_veram_probare` red
  (tone 1 intonation "f").

**Lint lesson.** `booleum`/`booleus`/`booleanum` are all unknown to
the lexicon (the house's `boolean` hits are tolerated English in
json/plist). Use `veritatis`/`veritatem`: `_veritatem_legere`,
`_campus_veritatis`. `silva/renominare.sh` refuses untracked files
(git is its undo), so renaming inside a brand-new file is a
whole-word replace.

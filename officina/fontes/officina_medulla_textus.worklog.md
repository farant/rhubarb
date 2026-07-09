# officina_medulla_textus worklog

## 2026-07-09 — Chunk C: scriptor + lector + amalgam birth (M1a)

- **Line-based strict lector**: canonical form is line-oriented, so
  the parser is too — pre-split into lines (comments/blanks dropped,
  ORIGINAL line numbers kept for error reporting), then per-line
  dispatch on first char ('@' label, '%' dest-instruction, keyword
  word, else no-dest instruction). No keyword/op collision: octeti/
  relocatio/modulus/functio/datum vs op names are disjoint, and
  `legere` (the op) always follows a '%dest ='.
- **Blocks pre-scanned per functio** (labels collected in file order
  before instructions parse) because ramus targets reference forward;
  registers are find-or-create at first sight — indices need NOT
  survive the round-trip, only NAMES print (worklog note from B
  resolved: the opaque-namespace answer).
- **medulla_emittere IS the parser's validator**: double terminators,
  unknown ops, bad registers all fail through the same fabrica
  discipline the builder enforces — the lector adds only syntax.
- i32-unsigned bite (again): dot-position sentinels `punctum_a = -I`
  declared i32. s32 for anything carrying -1 — the codebase's most
  reliable slip, now bitten in officina too.
- CREDO macro trap: CREDO_CHORDAE_AEQUALES takes C-STRINGS;
  chorda-vs-chorda is CREDO_CHORDA_AEQUALIS.
- **Amalgam birth finds** (manifest = officina/instrumenta/
  principalia/amalgamator.c, tessera exemplar):
  - Xar's typedef+body is ONE unit — CADENDA_TYPEDEF drops it whole
    (unlike Piscina's split tag declaration), so officina.h owns a
    TRANSPARENT layout-exact OfficinaXar copy (the TesseraChorda
    pattern, one size up).
  - MEDULLA_TYPUS_NIHIL must be TOKEN-identical in officina.h and the
    internal header — `(-1)` vs `(-I)` is a macro-redefinition error
    even though I expands to 1. Fontes now spells `(-1)` bare.
  - Excludenda harvested by iteration exactly as documented ("the
    compile clamat"): xar_creare pulls creare_cum_vexillis;
    xar pulls piscina_allocare_ordinatum (aligned grids);
    chorda excludes 44 of its 45 functions (only transcribere used).
  - NON_STATICA carries xar_numerus/obtinere/obtinere_s — consumers
    walk public Xar-field structs through officina_xar_* rows.
- Gates first-green after the fixes above: hospes 25/25 (golden text
  + byte-identical round-trip through the public surface), standalone
  severitas plena, nm-intersectio 0.

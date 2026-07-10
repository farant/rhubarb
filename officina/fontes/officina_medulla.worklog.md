# officina_medulla worklog

## 2026-07-09 — Chunk B: structs + builder ship (M1a)

- **Latina landmine, seventh firing: `magnitudo`.** Used it as the
  MedullaDatum size field and a prototype parameter — expands to
  `sizeof`, parse error. Renamed to `magnitudo_octetorum` (the
  TypusC89 precedent — semantica solved this the same way). The
  general rule held: EVERY latina #define is reserved; check before
  coining. (Also dodged proactively: no field named `registrum` —
  operand register indices live in `datum.index`; no field named
  `fluitans` — the float immediate is `immediatum_f`.)
- **Operand stays 16 bytes (sim C2 resolved):** sistere's diagnostic
  text is NOT a chorda in the operand (that would grow the
  most-copied struct to 24 bytes); causae are INTERNED at module
  level (Xar of chorda, dedup by content — the same diagnostic text
  repeats thousands of times corpus-wide) and the operand carries an
  index. New operand genus MEDULLA_OPERANDUM_CAUSA.
- **Three fixed operands + pool, not two:** ramus (cond, @verum,
  @falsum) and copia (finis, fons, magnitudo) both need exactly
  three — with a/b/c only vocare overflows to the per-functio
  operand stiva (extra_index/extra_numerus). The pool is only for
  call arguments, which keeps the common instruction self-contained.
- **All cross-references are indices** (blocci, registra, symbola,
  causae); names ride along for the dump (Chunk C) and indicium
  (M3). Register auto-names "tN" use N = register index — a user
  register literally named "tN" could collide in the textual form;
  the Chunk C parser should treat register names as an opaque
  namespace (name→index map), which dissolves the collision. Noted
  for C.
- **Conversion suffix order pinned in fields**: instructio.typus =
  FONS (first suffix), typus_secundus = FINIS (second) — matching
  `extendere.s32.s64` reading order. Non-conversion ops put their
  one type in `typus`, NIHIL in typus_secundus.
- Symbol/causa interning is linear scan — fine at per-TU scale;
  if fusor ever shows it hot, tabula_dispersa is the drop-in
  (measure first, per cultura).
- Runner born (saltuarius template minus tessera): first-run trap —
  chorda.c links against chorda_aedificator.c (chorda_iungere), so
  the dep list needs both. Silva amalgam compiled in from day one
  (demissio's substrate; cached object).
- Suite: 72 asserts, green after two fixes (both the magnitudo
  firing). $summa built via the fabrica matches vocabularium §VIII
  shape: IV blocci, III/II/III/I instructiones, terminator
  discipline verified positive and negative.

## 2026-07-10 — M2a: MedullaLineae

Distilled line table added medulla-side (structure + ponere/quaerere;
collector is demissio's — it needs the living front-end). Keys =
origo pointer BYTES via tabula_dispersa (DemissioIntroitus pattern);
values carry interned via-index + linea. Amalgam note: the public
MedullaLineae field for the internal table is `void*` in officina.h —
TabulaDispersa is an ANONYMOUS-struct typedef (no tag to
forward-reference; a forward `typedef struct X X;` COLLIDES with the
vendored anonymous typedef in the amalgam TU — the Xar landmine's
sibling, solved the opposite way: void* + implicit C89 conversion
instead of a layout copy, since medulla.c never touches members).

# silva_c89_semantica worklog

## 2026-07-08 — Chunk A ships (typus + forma + aestimator)

Notes that don't belong in the header comment:

- **long long accepted as extension, deliberately.** C89 has no long
  long, but the corpus REQUIRES it (latina.h s64 = signed long long,
  ~624 uses, -Wno-long-long repo-wide). PRIMITIVUM_LONGUS_LONGUS[_
  INSIGNATUM] exist; the multiset mapper takes longi==2. When
  standard-tagging lands (recensio #5), these should carry EXTENSIO.
- **long double = 8 on Apple arm64** (AAPCS64 Darwin: long double is
  double). Linux arm64 would be 16 (quad) — if silva ever runs
  elsewhere, the forma table is the port point. Haruspex will verify.
- **sizeof(expr) in constant expressions = named park (M0b).** Needs
  the expression's TYPE, which is M0b's expression typing. Corpus
  array sizes are literals post-expansion (Roman numeral macros), so
  zero corpus load. Diagnostic fires if hit.
- **Interning is a linear scan** (sem->derivati, structural compare
  at creation). Complete-first rule; per-file type counts are small.
  If percursus -semantica shows cost, switch to a hashed key. The
  CONTRACT (pointer equality = identity) is unaffected.
- **Flat-table tag redefinition (Chunk A limitation):** two same-name
  structs in different scopes both register in sem->tags; second
  definition creates a NEW type and re-registers (last wins). Correct
  per-scope resolution arrives with scopus (Chunk B). Legal C that
  hits this: rare in corpus (block-scope struct defs are the
  const-strip unions, all ANONYMOUS — untagged, never registered).
- **Enum tags share the tag namespace with struct/union tags** (one
  C89 tag namespace) — deliberately one sem->tags table.
- **\x and \NNN character escapes = FALSUM (park).** Corpus uses
  none in constant-expression positions.
- Probatio landmine that bit: hand-counted fixture lengths. Use
  strlen. (The trailing NUL parsed as OCTETUS_IGNOTUS -> ERROR node
  — which is silva behaving exactly as designed.)

## 2026-07-08 — Chunk B ships (scopus + ambulatio integra)

- **B bar AMENDED honestly**: INTENTIO said "diagnostics 0" over the
  corpus; the sweep gives 818 in 524 files — ALL of class "typus
  nominatus ignotus" (probe-verified on filum.c/piscina.c): plain
  `size_t n;` declarations commit as DECLARATIO with an
  oracle-unknown type name (only cast/sizeof positions fork into
  ambigui — census). This is PRECISELY the systema gap; Chunk C's
  bar inherits "semantica diagnostica -> ~0". Amendment named here
  and in the phase-log.
- Corpus sweep: 729 files, zero crashes, 234,427 symbola,
  semantica cost invisible (~108s total vs ~112s baseline run).
- Scope model: params scope (prof 1) + body corpus scope (prof 2)
  nested — redeclaring a param in the body block is legal-shadowing
  by this model (lint's call, not ours).
- Same-scope redeclaration: same genus = lenient overwrite
  (tentative definitions); different genus = diagnostic + overwrite.
- Labels namespace: not implemented (nothing consumes it; lint owns
  goto/label legality) — park named.
- Block-scope struct tag redefinition now CORRECT via scoped tag
  tables (Chunk A's flat-table limitation closed).

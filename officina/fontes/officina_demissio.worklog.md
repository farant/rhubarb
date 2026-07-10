# officina_demissio worklog

## 2026-07-09 — Chunk A: demissio nata (M1b)

- **The M0b contract pays exactly as designed**: demissio contains
  ZERO promotion/UAC logic — `conversus != NIHIL` → one call into
  the conversion-op family (_conversionem maps type pairs to
  extendere/amputare/fluitare/integrare/dilatare/artare; same-size
  sign changes are NO-OPS because types live on ops, not registers).
  mixtum's `a * 2 + b` dumps with the exact extendere.s32.s64 the
  annotations dictate.
- **Declarator→symbol binding via the lexemata map**: semantica's
  public index carries `lexema` (the name token) per symbol; one
  pointer-keyed table (token → symbol) built per TU binds every
  declarator through the PUBLIC surface. Use-sites go through
  symbolum_nodi. No new silva export needed — the M1a package was
  sufficient.
- **Residency**: pre-pass = ONE generic recursion over the public
  `nodus->loci[]` (SilvaNodus is transparent — no accessor walk
  needed), collecting locals in order + address-taken base objects
  (`&s.a[i].x` chain descends subscriptio/accessus-"."/parenthesis
  to the base identifier). Aggregates/arrays/captured → arca ops in
  the ENTRY block (LLVM alloca discipline); captured params respill
  (arca + scribere of the incoming register).
- **FIND (blessing the first goldens): byte-identity is not genus
  identity.** %.17g prints 100.0 as "100"; the lector reparses that
  as an INTEGER immediate. dump(parse(t)) stayed byte-identical —
  the wart was invisible to the round-trip bar and surfaced only by
  READING the golden. Fix in the scriptor: floats with no '.'/'e'
  get ".0" appended. Lesson: golden inspection is a real gate, not
  ceremony.
- **Nested case labels are FLAT in silva's tree**: `case 1: case 2:
  stmt` = sibling CASUS nodes (the first with empty sententiae) —
  the two-pass switch lowering (dispatch chain, then bodies in
  source order with fallthrough salire) handles it with no special
  case. Confirmed in fluxus.medulla.
- **Const-laundering at the forma boundary**: typatio rows return
  `const TypusC89*` but formam_computare mutates lazily (forma
  pigra) — one union-based _sine_constante at exactly that boundary;
  everything else stays const. Also: demissio_currere takes
  NON-const sem (INTENTIO said const; forma + aestimator mutate
  tables on query — emendatio recorded).
- **Literals ride as immediate operands** (_ut_operandum): folium
  integers/characters aestimated to immediates (float-context
  literals become immediatum_f at compile time), matching the
  vocabulary exemplum's `addere.s32 %i, 1` aesthetics. Everything
  else materializes to registers — assignment rhs deliberately
  materialized (simpler value-of-assignment semantics), v1.1 polish
  candidate.
- **Amalgam grew its documented external dependency**: officina.c
  now `#include "silva.h"` (prooemium); amalgamare.sh compiles with
  -Isilva/amalgama (the ONE include exception, commented) and hospes
  links BOTH amalgams (saltuarius dual-host pattern). Manifest
  gained tabula_dispersa + friatio (silva's excludenda lists as
  seed; harvest: chorda_comparare pulled back by tabula, continet/
  delere excluded). officina.h refers to silva types TAG-ONLY.
- Chunk A suite: fixtures scalaria.c + fluxus.c lower with ZERO
  sistere (asserted), all functiones terminated, dumps round-trip
  byte-identical through the lector, goldens pinned + inspected.
  Deferred genera emit named causae ("nondum vocatio (M1b B)" etc.)
  — the classification IS the B/C worklist.

## 2026-07-09 — Chunk B: vocationes + aggregata (M1b)

- The three "nondum (M1b B)" causae RETIRED: vocatio (direct via
  $symbolum when the callee identifier's symbol type is FUNCTIO,
  indirect via register otherwise — (*f)(x) collapses to the same
  register call as f(x) for free, since deref of a function type
  passes the address through); accessus (membra[].offset after
  formam_computare; "->" reads the pointer value, "." takes the
  base's address; offset-0 members skip the add); subscriptio
  (base + scaled index; commutative i[a] handled by swapping on
  which side is monstratorius).
- **Stiva contiguity trap dodged by design**: nested calls in
  arguments (rotunda: duplicare(facere(...))) would interleave the
  operand stiva if args were appended during evaluation — all arg
  operands are evaluated into a LOCAL buffer first, then appended
  in ONE operanda_addere. The golden pins the proof.
- **Aggregate convention (C7) end-to-end**: returns = explicit
  first param %reditus (i64 address), callee copia's into it;
  by-value args = caller allocates temp arca + copia, passes the
  address; by-value params = the incoming register IS the storage
  address (sedes flips to arca, no respill — the caller's copy is
  the callee's local). $duplicare/$copiat/$rotunda show all three.
- Variadic call args carry NO per-arg type tag in the stiva
  (operands are 64-bit slots) — arg-width convention for builtin
  bridges (printf %ld vs %d) lands with machinula M2; noted, not a
  medulla change.
- Polish notes (v1.1, not now): zero-offset subscript adds
  (movere.i64 0 + addere) could fold; postcrement old-value copy is
  dead when unused; aggregate arg from a fresh call-result temp
  double-copies (uniformity over cleverness for v1).
- aggregata.c lowers with ZERO sistere; golden pinned + blessed;
  suite 3/3; amalgam VERIFICATUM (hospes 28/28) with demissio
  changes folded.

## 2026-07-09 — Chunk C: data statica (M1b)

- All four M1a exports fire together for the first time:
  constans_aestimare (scalar images, case-style folding),
  chorda_decodere (string bytes — "salve\n" → 73616c76650a00),
  symbolum_nodi (&-chain resolution), plus formam offsets for member
  addends. The export package was priced exactly right.
- **Relocatio settled as ADDITIVE** (slot's prior bytes = addend):
  &acies_plena[1] emits addend 04 in the image + relocatio to the
  base symbol. One mechanism for plain/member/element addresses;
  vocabularium §VI updated. MedullaRelocatio needed NO new field.
- **decodere's SilvaPiscina resolved by encapsulation**: demissio
  owns a transient silva piscina (created/destroyed inside
  demissio_currere; decoded bytes are memcpy'd into datum images
  immediately, so lifetime is sound). No API churn.
- Float static images via a small demissio-side float evaluator
  (the public aestimator is integral-only — folium/-/+/parenthesis/
  conversio coverage suffices for corpus initializers); f32/f64 bit
  images through unions, LE-deterministic on this host.
- Local congeries: HYBRID — all-constant initializers become one
  anonymous $congeries_N image + single copia (the common case,
  beautiful dumps); mixed initializers become $zeri_N copia (C89
  zero-fill semantics for unlisted members) + per-element stores.
  char arrays from strings = padded $chorda_N + copia.
- Function-scope statics: $functio.titulus mangling per the sealed
  scheme, sibling-scope collisions suffixed; sedes carries the
  module symbol index and the identifier/locus paths route through
  locus $mangled.
- Tentative definitions: find-or-create datum; later initialized
  declaration of the same symbol writes into the existing image.
  Pure extern declarations skipped.
- File-scope failures cannot sistere (no functio context): _notare
  interns the causa at module level — fusor counts causae, so the
  honesty bar covers file scope too.
- statica.c (16 data incl. fn-pointer table, string globals, local
  statics) lowers with ZERO sistere; golden pinned + blessed byte
  by byte (42=2a, -7=f9ff.., 2.5=..0440, 1.5f=0000c03f all verified
  LE). Suite 3/3; amalgam VERIFICATUM hospes 28/28.

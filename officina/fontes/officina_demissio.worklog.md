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

## 2026-07-09 — Chunk D: fusor + corpus contact (M1b)

FIVE REAL FINDS on first corpus contact (the budgeted real-bugs day,
delivered again — the M0b lesson holds; every one found by fusor's
classification table + the -causa origo probe, the 20-minute
instrument reborn):
1. **ambiguus_canonica returns an INDEX** into interpretationes (locus
   species INDEX), not a node — _canonicus only handled the node case,
   so 442 retained-ambiguus nodes fell through as "generis ignoti
   (54)". Genus-numbered causae found it in one run.
2. **sizeof(local-expression) can't go through the public aestimator**
   post-analysis — scopes are closed, locals resolve to nothing.
   Demissio now reads the inner expression's UNdecayed type from the
   typationes (that's what naturalis is FOR) → _mensura_typi.
3. **Incomplete arrays completed by initializers** (x[] = "...", 
   x[] = {...}) — semantica's "initiatores non probantur" parca leaves
   acies incompleta; _mensura_completa computes from the initializer
   (string: decoded+1; congeries: element count × elemsize). Dissolved
   THREE causa rows incl. the "staticum ante declarationem" cascade;
   corpus data 3,054 → 8,653 in lib alone.
4. **static T* p = NULL fails the public aestimator** (NULL =
   ((void*)0); casts to non-integral types rejected) — 46 modules.
   Fix: strip conversio/parenthesis wrappers, aestimate the core.
5. **Float static tables need a binarium evaluator** (1.0/3.0) —
   extended the demissio-side float evaluator (+,-,*,/).
- **Corpus definition, principled**: fusor lowers .c ONLY (571 = all
  573 findable minus 2 over-tectum) — demissio's corpus is
  TRANSLATION UNITS; percursus parses .c+.h (750) — a parser's corpus
  is every parseable file. The delta is by design, recorded.
- Residuum fully attributed (per-file -v + -causa probes): POSIX
  faces (1160 accessus incompletae + locals), nested-chain park
  (unflipped (T)(x) forks: vocatio sine typo 205 + poison downstream
  ~870), stb_image vendored (imago.c forma rows), semantica-diagnosed
  identifiers (126), small tails (16 notae/sistere, all named).
- Apex memoriae 9.7 GB (demo_52 data-dense: dump text ~3x in memory
  during the round-trip) — survives; optimization on measurement only.

## 2026-07-10 — M2a: stem-mangling + distillatio linearum

**Stem-mangling gap closed** (found by conexio's first world-link,
see officina_conexio.worklog.md): new `_titulum_symboli` chokepoint —
file-scope statics (profunditas 0 + REPOSITIO_STATICA) get
`$<stirps>.<name>` per the SEALED vocabulary §II; `_titulum_dati`
always prefixes (anonymous data are inherently module-private).
Function-scope statics cascade automatically (their prefix is the
containing functio's — possibly already stem-mangled — title).
SIX sites route through the chokepoint (identifier value + locum
staticum + vocare callee + global locus + functio definition +
global datum) — definitions and references MUST share the path.
Goldens re-pinned + blessed: scalaria/fluxus/aggregata byte-identical
(no statics), statica pure renames ($statica_c.chorda_0 etc.;
extern $numerus correctly bare).

**demissio_lineas_colligere** (M2a deliverable): origo → (via, linea)
distilled WHILE the front-end lives — parsura->expansio is public
(fundus-medulla's "not reachable from nodus alone" resolved at the
parsura level); descent helper skips synthetic tokens
(byte_offset == -1). Result = MedullaLineae (medulla-side, no silva
types: origo pointers as OPAQUE KEYS — per-module maps, so
arena-reuse address collisions across TUs are impossible). Consumer:
M2b flight recorder; this IS proto-indicium's line table.

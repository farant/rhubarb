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

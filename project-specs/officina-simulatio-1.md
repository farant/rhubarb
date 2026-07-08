# Officina simulatio 1 — M0 semantica (typus + index)

Date: 2026-07-08. Technique: simulate-work (pretend implementation,
simulated 2h clock, real greps for grounding). Target: officina M0,
silva-side, per officina-spec-v2.md par VI.

Grounding greps (real): 358 untagged-typedef structs; 57 named-tag
typedefs; block-scope types dominated by the const-strip union idiom
(~14 files); 124 enums, values are Roman-numeral macros = literals
post-expansion; array sizes likewise literal post-expansion.

## Complications (four-part schema)

### 1. Hidden module: integer constant-expression evaluator
- discovered-while: typing declarator-aciei (array mensura is an
  expression node)
- consists-in: M0 needs a C89 integer-constant-expression evaluator
  (literals, unary, binary, ternary, casts, char literals, enum
  constants, sizeof); sizeof makes it MUTUALLY RECURSIVE with layout
- consequences: evaluator + forma share one context, layout lazy /
  on-demand; cycles impossible in valid C89
- handled-by: one shared context; corpus load trivial (sizes are
  literals post-expansion) but module must be complete

### 2. THE FIND: system headers are invisible to silva
- discovered-while: walking skeleton hit #include <stdio.h> in
  piscina.c
- consists-in: silva praebets repo headers only; system headers pass
  through -> trees contain NO libc declarations. printf = undeclared
  identifier (C89 implicit int extern function = correct semantics to
  implement); size_t/FILE = unknown type names = the bulk of the
  15,243 retained ambigui
- consequences: semantica cannot type libc-touching declarations;
  machinula cannot execute what it cannot size
- handled-by: NEW NAMED DELIVERABLE "praebenda systematis" — curated
  synthetic header set (the ~105-symbol audited extern surface +
  core types: size_t, FILE opaque, ...) injected via the EXISTING
  contextus lexica mechanism (latina.h precedent). No new machinery,
  new content. Doubles as machinula's platform definition file (same
  document = semantica's extern surface AND builtin surface). Payoff:
  ambigui 15,243 -> est. double digits after closure pass.

### 3. Oracle partition by type provenance
- discovered-while: emitting clang asserts for types defined in
  praebenda systematis (synthetic FILE)
- consists-in: systema types are invented opaques; clang's real
  <stdio.h> FILE differs; naive oracle "fails" on our own inventions
- consequences: oracle must partition: repo-defined types -> assert
  against clang truth; systema types -> assert only what machinula
  relies on (pointer-size opacity)
- handled-by: provenance mark on every type (fons_index threads
  through); oracle filters. ALSO yields M2 machinula rule: builtins
  never dereference systema-opaque types.

## Dissolved fears / cheap parks
- Anonymous-member oracle problem: DISSOLVED — C89 has no anonymous
  members; oracle verifies each named type independently, direct
  members only, nesting composes. Untagged inner member types pinned
  transitively (outer offsets + sizeof). Block-scope types: same code
  path as file-scope, coverage argument recorded; named park.
- K&R: cannot exist in corpus (-Wstrict-prototypes -Werror); grammar
  keeps parsing it; semantica types as unprototyped; free named park.
- Arena lifetime: semantica tables (own piscina) hold SilvaNodus*
  into parsura piscina -> lifetime nests inside parsura's. Fine for
  corpus bar (one piscina per file). REPL/incremental invalidation =
  M4 + incrementalitas doc, NOT M0. Recorded constraint.
- char / signed char / unsigned char = THREE distinct types; primitive
  multiset canonicalization table (14 valid combos).
- Qualifier-through-typedef (const IntPtr = int* const) — pinned
  fixture; quals ride the reference, wrap the TOP of aliased type.
- Enum constants enter the ORDINARY namespace (collision detection).
- Tag table with completion pointers; sizeof(incomplete)=diagnostic;
  layout must be lazy (second argument after complication 1).

## Structural proposal
Split the milestone:
- M0a: declarations + scopus (4 namespaces) + typus interning
  (pointer equality) + forma (lazy, arm64 LP64) + consteval + clang
  oracle (compile-time asserts, one generated TU, compile IS the
  verdict) + project index + ambiguity closure (resolutor datum =
  semantica tables; existing SilvaResolutor interface unchanged;
  12 block-scope typedefs close per decisiones 13). Demo moment:
  ambigui 15,243 -> ~0.
- M0b: expression typing + usual arithmetic conversions as EXPLICIT
  parallel-table annotations (nodus -> typus + conversio) = exactly
  what demissio materializes into medulla ops. Parallel-table
  architecture confirmed (oracle responsa precedent; trees immutable).

API draft: silva_c89_semantica_analysare(piscina, parsura,
praebenda) -> SilvaSemantica*; typus_declarationis(nodus),
typus_expressionis(nodus) [M0b], symbolum_invenire, forma_typi ->
{magnitudo, ordinatio}; oracle emitter as instrument. Semantica is
c89-specific by nature (no grammar-agnostic pretense).

## Interview questions for Fran
1. Praebenda systematis policy: hand-written curated header (latina.h
   precedent) vs generated from the audit list? Where does it live
   (silva/ vs officina/)? It IS officina's platform definition file.
2. Approve M0a/M0b split?
3. Expression-type storage: parallel table at analysis time (M0b) vs
   computed at demissio time?
4. Oracle coverage policy: block-scope types via coverage argument OK,
   or replicate-with-synthesized-tag for direct verification?
5. K&R free park OK?
6. Semantica lifetime: per-parse acceptable for M0 (REPL invalidation
   deferred to M4 + incrementalitas journal)?

## Prediction audit
Pre-simulation prediction (Claude): first wall = oracle anonymous
members OR type-representation-vs-arena-lifetime. Score: HALF. The
oracle complication existed but dissolved in minutes (C89 has no
anonymous members); the lifetime seam held but as a note. The real
wall was unnamed by both parties: the system-header void (praebenda
systematis).

# Officina M1 — medulla + demissio (planning spec)

Date: 2026-07-09
Status: sealed design (simulatio-3 → interview → this spec; all interview
answers = DECISUS). Method note: no v2 pass — simulatio-3's two grounding
agents (silva.h surface audit, corpus census) already did the codebase
exploration; their findings are §I of officina-simulatio-3.md.
Related: officina-brainstorm.md (architecture DECISUS), officina-spec-v2.md
(milestone bars, corpus waves), officina-simulatio-3.md (complications C1-C9),
officina-m1-interview.md (raw Q&A), officina/vocabularium-medullae.md
(the sealed vocabulary).

## I. DECISUS summary (interview 2026-07-09)

1. **Modulus-per-TU; conexio designed+built at M2**; internal-linkage naming
   sealed NOW (source-derived: `$<file-stem>.<name>` / `$<functio>.<name>`).
2. **Silva export package = M1a Chunk A** (silva-side chunk inside the
   officina milestone; pre-M1 precedent).
3. **Trap policy: sistere op** — diagnostic + source position, halts only if
   executed; M1b bar = "0 ruinae, N sistere, all classified". Enforcement
   layer's future runtime seat; NO type checking in demissio.
4. **M1 splits: M1a (exports + medulla core) / M1b (demissio)** — separate
   sealed bars, M0a/M0b precedent.
5. **Vocabulary**: house suffixes (.s/.i/.f), full sigils (%/$/@), classical
   bitwise set (et/vel/aut/complementum), named conversion family, sistere,
   fusor. Sealed in officina/vocabularium-medullae.md.
6. **Goldens**: officina/probationes/fixa/, .medulla extension, .c+.medulla
   pairs.
7. **Books**: officina/CLAUDE.md + phase-log.md born at M1a INTENTIO;
   milestone narrative lives officina-side; silva phase-log gets pointer
   entries when silva code changes.

## II. M1a — silva exports + medulla core

**Bar: byte-identical textual round-trip on golden fixtures
(dump(parse(t)) == t), suite green, VERIFICATUM (officina amalgam gates).**

**Chunk A — the silva export package** (silva-side; every row + hospes call;
silva phase-log pointer entry):
1. **Nexus symbolorum** (the big one): semantica RECORDS use-site→symbol
   bindings during analysis (typationes parallel-table pattern, pointer-keyed
   via creare_chorda). Public row (name proposal):
   `const SemanticaSymbolum* silva_c89_symbolum_nodi(const SilvaSemantica*, const SilvaNodus*);`
   — canonical-aware like the typatio rows; covers identifier uses; the
   declaration-site row already exists via vista/declarans.
2. **Constant evaluator row** (proposal):
   `int silva_c89_constans_aestimare(SilvaSemantica*, const SilvaNodus*, long long* valor);`
   — exposes the existing aestimator (case labels, static scalar folding).
3. **String decode surface** (proposal):
   `SilvaChorda silva_c89_chorda_decodere(SilvaPiscina*, const SilvaNodus*);`
   — escape-decoded bytes of a folium-chorda (adjacent-literal groups
   included); wraps _fugam_decodere, do NOT reimplement.
4. **Declare `silva_oraculum_responsa_vacare`** in silva.h (exists in
   amalgam with external linkage; one line + hospes call).

**Chunk B — medulla data structures + builder**: MedullaModulus / functio /
blocks / instructions with tagged operands (REGISTRUM | IMMEDIATUM |
SYMBOLUM | DATUM-ref; per-functio operand pool for vocare arity — sim C2);
in-memory origo (provenance) on every instruction (vision pin — sim C3);
arca frame-slot model; MedullaDatum {imago, relocationes} (sim C8);
terminator discipline enforced by builder. Unit suite.

**Chunk C — textual form**: dump + strict parser per vocabularium §VII
(dump defines canon); golden fixtures in probationes/fixa/ (hand-written
.medulla, canonical); round-trip harness; officina amalgam born here
(amalgama/officina.c + hand-written officina.h, shared silva_amalgama
mechanism, VERIFICATUM gates + hospes per standing rule).

## III. M1b — demissio

**Bar: every corpus TU lowers — 0 ruinae, N sistere all classified; fusor
sweep green; every corpus dump round-trips byte-identically; golden dumps
pinned for lowering fixtures.**

**Chunk A — driver + expressions + statements**: two-pass driver copied from
percursus (parse w/ contextus+lexicon → sem₁ → augere → responsa_vacare →
recanonicare → sem₂; demissio reads sem₂ ONLY); `_ut_valor`/`_ut_locus`
expression pair; conversus annotation → conversion op (ZERO UAC logic in
demissio); si/dum/fac/per/commutatio (compare-chain; jump tables parked)/
salta/frange/perge; &&/||/ternary naive block materialization.

**Chunk B — calls + aggregates + address-taken**: pre-pass marks
frame-resident symbols (address-of BASE-OBJECT chain — `&s.a[i].x` marks s;
aggregates + arrays always frame); vocare with pre-converted args; aggregate
return = explicit destination parameter (sim C7); member access via public
membra[].offset after formam_computare; copia for struct assignment.

**Chunk C — static data**: MedullaDatum emission for globals; aestimator
staticus (wraps constans_aestimare; address constants → relocatio pieces);
string literals → anonymous data (decoded bytes) + relocationes;
function-scope statics under `$<functio>.<name>`; sistere emission for
unlowerable constructs.

**Chunk D — fusor + close**: the corpus sweep instrument
(instrumenta/principalia/fusor.c + fusor.sh): lowers all TUs, counts
functiones/instructiones/sistere (classified like percursus diagnostics),
round-trips every dump; publica (officina.h rows exercised by hospes);
RELATIO + full phase-log audit (milestone rule).

## IV. Parks (named, with owners)

- Jump tables for commutatio → M2-if-profiled (compare-chain measured first).
- &&/|| branch-context optimization → M5-if-ever (naive materialization
  correct).
- Member name→offset lookup API → on-pull (manual membra[] scan suffices).
- Bitfield layout → already "parcata" in silva.h (est_campus flag); corpus
  has ZERO bitfields.
- Conexio (linking) design → M2 opening item; naming scheme pre-sealed here.
- systema_posix types face → NAMED M2-Wave-1 prerequisite (filum needs
  struct stat); sistere covers M1.
- Provenance textual attribute `[fons l:c]` → shape refinable in M1a C;
  indicium (M3) is the real consumer.
- est_implicitum = enforcement-layer evidence, NOT builtin routing (sim C6
  correction to the M0b addendum).

## V. Corpus facts the design leans on (census 2026-07-09)

564 .c / ~580K lines (amalgams excluded). knotapel = plain ENGLISH C (~25%
of corpus; grep-audits must be bilingual). goto 67/31 files; switch 347
(~10 cases avg); ZERO variadic bodies (calls only — no va prologue in v1
IR); FP = knotapel + tempus; u64 unused; s64 ≈ 401 uses (the one C89
extension the integer model owns); string-literal static tables enormous
(relocations are high-volume); fn-address initializers in 4 files (same
mechanism, rare tail); knotapel returns structs by value routinely.

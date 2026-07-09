# Officina simulatio-3 — M1 medulla + demissio (pretend-build)

Date: 2026-07-09
Status: simulation artifact (technique: pretend-implement, surface complications).
Grounding: two research agents (silva.h public-surface audit for demissio-as-consumer;
corpus census for op-vocabulary sizing) + M0b RELATIO + ADDENDUM ANTE COMPACTIONEM
(M1 engine hints) + officina-brainstorm §IV-V + spec-v2 §VI/§X/§XI.
Standing note from Fran: type CHECKING is the future enforcement layer — demissio
assumes well-typed input and never duplicates judgment; representation choices must
not foreclose the checker's later seat.

---

## I. Ground truth gathered before the clock started

**Surface audit (silva/amalgama/silva.h, verified):**
- AVAILABLE: 58-genus enum + 176 accessors (NIHIL-valor contract); loci walking;
  pater; ambiguus canonica; SemanticaSymbolum FULLY public incl. `repositio`
  storage bitmask (STATICA/EXTERNA/AUTOMATA/REGISTRI), `est_implicitum`, `valor`
  (enum consts), `declarans`; symbol iteration (`symbola_numerus`/`per_indicem`);
  TypusC89 full def, typedefs pre-resolved BY CONSTRUCTION (no typedef variant);
  `formam_computare` + public `membra[].offset`; function-type introspection
  (variadic/prototyped flags); M0b typatio rows with canonical-relative contract;
  initializer access through accessors; full two-pass driver sequence documented
  in percursus.c.
- MISSING (load-bearing, each now a named export task — §IV R1):
  (a) use-site identifier → symbol (semantica resolves internally but does not
      RECORD the binding; only file-scope by-name lookup is public);
  (b) `constans_aestimare` — no public row at all;
  (c) string-literal decoded bytes (`_fugam_decodere` interior — predicted by
      the M0b addendum, confirmed);
  (d) `silva_oraculum_responsa_vacare` used by the canonical two-pass but NOT
      declared in silva.h (one-line omission; blocks the driver under the
      house rule);
  (e) member name→offset lookup helper — MISSING as API but workable manually
      (public membra[] scan); nice-to-have only.

**Corpus census (564 .c / ~580K lines; amalgams excluded):**
- knotapel (139 files, ~25% of corpus) is plain ENGLISH C — latina.h in only 2
  files. No compiler impact (same tokens), but every grep-audit must be
  bilingual from now on.
- goto: 67 stmts / 31 files (Latin libs: TWO uses; the rest = knotapel
  `goto cleanup;` error-exits). switch: 347 (~10 cases avg; 3,467 case labels).
  do-while ~135. ternary ~2.4K lines. FP arithmetic = knotapel + lib/tempus.c
  essentially. `u64` = 0 uses. s64 (long long) ~401 — the one C89 extension the
  integer model owns.
- Variadic: ZERO definitions-with-bodies (re-confirmed by inspection of all 35
  `, ...)` hits). Variadic CALLS only → v1 IR needs no va prologue support at all.
- Static initializers: fn-address initializers exist in only 4 knotapel demo
  files (e.g. demo_58 `act_table[] = {{"Re>0", wrap_re_positive, 0}, ...}`) —
  BUT string-literal static tables are ENORMOUS (silva's own generated tables:
  thousands of rows each holding a string pointer; arbor2_glr_tabula alone has
  2,031 `= {` lines). Relocated static data is therefore a V1-MANDATORY,
  HIGH-VOLUME mechanism (strings), with fn-addresses a rare tail on the same
  mechanism.
- knotapel returns structs by value routinely (Cyc16 etc.) — aggregate-return
  convention is exercised, not a corner.

## II. The pretend-build timeline (~2h20m simulated)

- **T+0:00** officina/ skeleton per spec-v2 §X (CLAUDE.md, phase-log,
  compile_probationes.sh from the saltuarius pattern, amalgam wiring deferred
  until there is something to amalgamate). Uneventful; precedent strong.
- **T+0:05** FIRST WALL, before any struct: what is a MedullaModulus? Corpus
  programs are MULTI-TU (a suite = probatio_foo.c + lib/foo.c + deps linked).
  Sim decision: modulus = one TU's lowering; a separate CONEXIO step (M2)
  merges modules for machinula, resolving externals by name; internal-linkage
  (repositio STATICA at profunditas 0) symbols get qualified private names.
  → COMPLICATIO C1.
- **T+0:10** Instruction shape: fixed {op, typus, dest, a, b} fails vocare
  (N args) and ramus (2 targets). Sim decision: tagged operands
  (REGISTRUM | IMMEDIATUM | SYMBOLUM | DATUM) + per-functio operand pool for
  the variadic-arity instructions. → COMPLICATIO C2.
- **T+0:15** Pleasant find: the house typedef convention (i=unsigned,
  s=signed) IS the op-suffix scheme — `.s32`/`.i32`/`.f64`. No invention
  needed; the vocabulary inherits it.
- **T+0:20** Provenance: in-memory instruction always carries origo
  (SilvaNodus* / situs); textual form gets an OPTIONAL attribute — otherwise
  the byte-identical round-trip bar fights hand-written test files.
  → COMPLICATIO C3.
- **T+0:25** Textual canon decision: THE DUMP DEFINES CANONICAL FORM; parser
  is strict; bar = dump(parse(t)) == t. Hand-written fixtures are written
  canonically (a normalizing formatter can come later if it ever hurts).
- **T+0:30** Op vocabulary drafted from census (candidate list §V for the
  ceremony — including the vel/aut classical-logic gem for inclusive/exclusive
  or).
- **T+0:40** Builder API + first golden (hand-written summa.medulla → parse →
  dump → byte-identical). Grammar pinned: functio header, block labels,
  `%reg = op.typus a, b` lines.
- **T+0:45** Demissio driver: copy percursus's two-pass verbatim → immediately
  hits (d): `responsa_vacare` undeclared in silva.h. One-line export.
- **T+0:50** First real lowering (redde-only functio) — and the BIG WALL:
  lowering an identifier expression needs use-site → symbol, which has no
  public surface (a). Demissio MUST NOT reimplement scope resolution
  (semantica is the single home of C89 semantics — DECISUS 8). Sim decision:
  semantica records a binding table during analysis (nexus symbolorum — the
  typationes parallel-table pattern exactly) + one public row. Silva-side
  export, the largest of the package. → COMPLICATIO C4.
- **T+1:00** Expression core: `_ut_valor` / `_ut_locus` pair (rvalue/lvalue).
  The M0b contract pays instantly: `conversus != NIHIL` → emit convert op;
  demissio contains ZERO UAC/promotion logic. Compound assignment pre-chewed
  as the addendum promised.
- **T+1:05** Address-taken pre-pass (demissio-side): one walk over the body
  before lowering, marking symbols frame-resident (address-of BASE-OBJECT
  chain: `&s.a[i].x` forces s; aggregates and arrays always frame-resident).
  Side table pointer-keyed via the M0b creare_chorda pattern. → COMPLICATIO C5.
- **T+1:10** Control flow: si/dum/fac/per standard; `ramus` branches on
  register nonzero (no explicit compare-to-zero); &&/||/ternary lowered by
  naive value materialization through blocks (correct, unoptimized — parked
  permanently or until M5).
- **T+1:15** commutatio: compare-chain v1 (347 switches, ~10 cases avg —
  jump table PARKED, named, M2-if-profiled). Case labels need constant
  evaluation → hits (b): `constans_aestimare` not public. Export task. Array
  bounds need NOTHING (already baked into acies.numerus).
- **T+1:20** vocare: args pre-converted per annotations; zero variadic bodies
  → no va prologue anywhere in v1 IR; variadic CALLS carry full arg list +
  callee type. CORRECTION to an addendum hint: `est_implicitum` is NOT the
  builtin-routing signal — printf et al. are systema-DECLARED (prototyped,
  not implicit). Routing = "no definition at conexio time". est_implicitum
  stays a diagnostic-grade datum for the enforcement layer. → COMPLICATIO C6.
- **T+1:30** Aggregates: struct assignment/copy = `copia` op (size from
  layout); member access = base + public membra[].offset; struct-by-value
  RETURN = hidden destination parameter, EXPLICIT in medulla signatures
  (demissio is where semantics become visible — brainstorm §IV.3).
  → COMPLICATIO C7.
- **T+1:40** Static data: MedullaDatum {titulus, magnitudo, ordinatio, imago
  bytes, relocationes[{offset, symbolum}]}. Census verdict: relocations are
  v1-mandatory and HIGH-VOLUME (every string pointer in the generated tables
  = anonymous string datum + reloc); fn-address relocs are the rare tail on
  the same mechanism. String bytes must be DECODED → hits (c), the decode
  surface export. Initializer folding for scalar globals needs (b) again,
  wrapped in a demissio-side "aestimator staticus" that also handles ADDRESS
  constants (&global, function names, string literals) as reloc pieces
  rather than values. → COMPLICATIO C8.
- **T+1:50** Function-scope statics (hic_manens): module-level datum with a
  qualified name (functio.symbolum scheme) — feeds the C1 conexio naming
  design.
- **T+1:55** The POSIX-incomplete files (~31 plagulae, struct stat/sockaddr
  member access with no layout): demissio cannot compute offsets there. Sim
  decision: a TRAP op — the expression lowers to an instruction that carries
  the diagnostic and aborts with source position IF EXECUTED. Files still
  lower; the M1 bar stays honest ("monorepo lowers, N traps, all
  classified"). NOTE the pull-chain: M2 Wave 1 (filum) needs struct stat
  for real → systema_posix's types face becomes a NAMED M2-Wave-1
  prerequisite regardless. The trap op is also the natural runtime seat the
  enforcement layer can reuse later (checking stays OUT of demissio).
  → COMPLICATIO C9.
- **T+2:05** Harness: golden .medulla dumps for fixtures (committed);
  corpus sweep instrument (percursus analog) lowering every TU — bar:
  0 ruinae, traps counted + classified; round-trip harness re-parses every
  dump byte-identically (a free second bar over the whole corpus).
- **T+2:15** machinula risk peek only: tagged operands dispatch fine;
  register file = u64 array; conexio lands at M2 design. Stopped there.

## III. COMPLICATIONES (four-part schema)

**C1 — Module/linkage model needs a DECISUS at M1 design, not M2.**
Discovered-while: naming the top-level medulla container (T+0:05).
Consists-in: corpus programs are multi-TU; medulla must decide modulus = TU
+ later conexio step vs whole-program lowering. Internal-linkage symbols
(staticus at file scope, function-scope statics) need a private naming
scheme that survives merging.
Consequences: shapes MedullaModulus, symbol tables, and the textual form's
symbol syntax — cannot be retrofitted cheaply after golden dumps are pinned.
Handled-by: M1 INTENTIO decision (recommended: modulus-per-TU + conexio at
M2 + qualified-name scheme named now). Fran's call at interview.

**C2 — Instruction shape vs variable arity.**
Discovered-while: struct sketch (T+0:10). Consists-in: vocare (N args) and
ramus (2 targets) break fixed 2-operand instructions. Consequences: operand
representation is the IR's most-copied struct; wrong shape = whole-corpus
churn. Handled-by: tagged operands + per-functio operand pool (QBE/MIR
precedent); machinula dispatch unaffected.

**C3 — Provenance vs byte-identical round-trip.**
Discovered-while: adding origo to instructions (T+0:20). Consists-in:
in-memory provenance is a vision pin (never optional); textual form with
mandatory provenance makes hand-written fixtures miserable and the bar
brittle. Handled-by: in-memory always; textual = optional attribute;
round-trip bar applies to whatever the dump emits.

**C4 — Use-site→symbol binding table (silva export, the big one).**
Discovered-while: first identifier lowering (T+0:50). Consists-in: semantica
resolves every use internally but records nothing; public lookup is
file-scope-by-name only. Consequences: without it demissio would duplicate
scope resolution, violating DECISUS 8 and rotting independently.
Handled-by: semantica records nexus-symbolorum during analysis (typationes
parallel-table pattern) + public row + hospes call. Silva-side chunk.

**C5 — Address-taken pre-pass with base-object chains.**
Discovered-while: register-vs-frame residency (T+1:05). Consists-in: no
per-symbol usage summary exists; `&s.a[i].x` must mark s, not the member.
Handled-by: demissio-side pre-walk + pointer-keyed side table (M0b
creare_chorda pattern); aggregates/arrays always frame-resident.

**C6 — est_implicitum is NOT builtin routing (addendum correction).**
Discovered-while: vocare lowering (T+1:20). Consists-in: printf/malloc are
systema-DECLARED (prototyped), so est_implicitum=FALSUM for them; the M0b
addendum's "builtin-routing signal" hint is wrong. Consequences: routing =
definition-absent at conexio time (M2); est_implicitum remains
enforcement-layer evidence only. Handled-by: recorded here; conexio design
owns routing.

**C7 — Aggregate return convention, explicit.**
Discovered-while: knotapel by-value returns (census + T+1:30). Consists-in:
struct-returning functiones need a destination; hiding it makes the textual
form lie. Handled-by: explicit implicit-first destination parameter in
medulla signatures (officina-defined VM convention; native ABIs map at M5).

**C8 — Relocation model is v1-mandatory and string-dominated.**
Discovered-while: static data design (T+1:40) + census item 8. Consists-in:
every string pointer in static tables = anonymous datum + relocation;
fn-address initializers exist (4 files) on the same mechanism; string BYTES
must be escape-decoded (export (c)); scalar folding needs the evaluator
(export (b)) wrapped in an address-aware aestimator staticus.
Consequences: MedullaDatum carries {imago, relocationes} from day one; the
textual form needs datum + reloc syntax (ceremony items).
Handled-by: designed into medulla core; exports (b)+(c) land first.

**C9 — POSIX-incomplete files → trap op + the M2 pull-chain.**
Discovered-while: sweeping the ~31 diagnostic plagulae (T+1:55).
Consists-in: no layout for struct stat et al. → member offsets uncomputable;
"monorepo lowers" needs a policy. Consequences: without a policy the M1 bar
is either dishonest or blocked on systema_posix. Handled-by: TRAP op
carrying diagnostic + source position (aborts only if executed); M1 bar
restated "0 ruinae, N traps classified"; systema_posix types face NAMED as
M2-Wave-1 prerequisite (filum needs struct stat). Trap channel doubles as
the enforcement layer's future runtime seat.

## IV. Prediction audit + recommendations

Audit: queued predictions vocare-arity (C2), provenance-vs-roundtrip (C3),
lvalue dual-function — all CONFIRMED. Addendum hints: two-pass driver
CONFIRMED (+1 header line); decode surface CONFIRMED and WIDENED into the
relocation model (C8); congeries direct-correspondence CONFIRMED (elision
zero); est_implicitum routing CORRECTED (C6). UNPREDICTED finds: the
binding-table gap (C4 — the biggest), the missing public evaluator, the
module/linkage DECISUS (C1), knotapel-is-English, string-reloc dominance.

**R1 — M1 opens with a SILVA EXPORT PACKAGE chunk** (silva-side, before any
officina code): (a) nexus-symbolorum binding table + public row;
(b) constans_aestimare public row; (c) string decode surface;
(d) responsa_vacare declaration. Each with hospes calls per standing rule.
All four are evidence-named, none speculative.
**R2 — Naming ceremony scope**: ~35-40 base ops + the .s/.i/.f suffix scheme
(inherited from house typedefs), operand sigils, block-label syntax,
functio/datum/relocatio keywords, the trap op's name. Candidate list in §V;
Fran's DECISUS.
**R3 — Module model DECISUS at INTENTIO**: modulus-per-TU + conexio at M2 +
internal-linkage qualified-name scheme named now (C1).
**R4 — Textual canon**: dump defines canonical form; strict parser;
provenance optional attribute (C3).
**R5 — Chunking sketch**: A silva exports → B medulla core (structs, builder,
dump, parse, goldens) → C demissio expressions+statements → D aggregates+
static data+relocations → E corpus sweep + trap policy + RELATIO.
**R6 — Trap op** for unlowerable constructs; enforcement layer explicitly
gets the same channel later; checking stays out of demissio (Fran's note).
**R7 — Parks, named**: jump tables (M2-if-profiled); &&/|| naive
materialization (M5-if-ever); member-lookup helper API (on-pull);
bitfield layout (est_campus already flagged "parcata" in silva.h).

## V. Candidate op vocabulary (for the ceremony — NOT decided)

Arithmetic: addere subtrahere multiplicare dividere residuum negare.
Bitwise: et vel aut (inclusive/EXCLUSIVE or — classical logic distinction)
complementum (~), sinistrorsum dextrorsum (shifts; .s = arithmetic right).
Comparison (→ i32 0/1): aequalis inaequalis minor minor_aequalis maior
maior_aequalis (signedness via suffix).
Conversion: amputare (trunc), extendere (sign/zero via .s/.i suffix),
fluitare (int→float), integrare (float→int), dilatare/artare (f32↔f64).
Memory: legere scribere (typed load/store), locus (address of frame slot /
datum / functio — operand kind distinguishes), copia (aggregate copy).
Control: salire ramus redde vocare. Trap: candidates — ruina / vetitum /
interpellatio (ceremony pick).
Moves: movere (register copy); immediates are operands, not ops.
Suffix scheme: .i8 .i16 .i32 .i64 (unsigned — house convention) / .s8 .s16
.s32 .s64 (signed) / .f32 .f64.

## VI. What the interview should settle (open questions for Fran)

1. The ceremony itself: op names (§V), trap-op name, textual sigils
   (registers %? blocks? datum syntax?), and whether op suffixes use the
   house i/s convention (recommended).
2. C1: modulus-per-TU + conexio-at-M2 — approve? And the qualified-name
   scheme for internal linkage.
3. R1 export package: approve as M1 Chunk A (silva-side work inside an
   officina milestone — precedent: pre-M1 did the same).
4. Trap policy (C9/R6) and the restated M1 bar wording ("0 ruinae, traps
   classified").
5. Chunking R5 — one milestone or split (exports + medulla core vs demissio)?
6. Where do goldens live: officina/probationes/fixa/ with committed
   .medulla.golden files (proposed).

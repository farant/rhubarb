# officina — phase log

Append-only. INTENTIO opens a phase, RELATIO closes a chunk/milestone;
full-log audit at milestone boundaries. Chronology is the re-entry reader's
map — never insert, only append.

## NATIVITAS (2026-07-09)

Officina's books are born (interview Q14: milestone narrative lives here;
silva's phase-log gets pointer entries when silva code changes). Prior
history lives silva-side and in project-specs: M0 semantica CLOSED
2026-07-08 (M0a+M0b, silva/phase-log.md) — demissio's input contract
{typus naturalis, typus conversus} FROZEN, haruspex 4,801/0, auspex
3,753/0, coverage 100.00%. Design arc for M1: officina-simulatio-3.md
(pretend-build, complications C1-C9) → officina-m1-interview.md (15
DECISUS) → officina-m1-spec.md (the plan) → vocabularium-medullae.md
(the SEALED vocabulary).

## M1a — INTENTIO (2026-07-09): silva exports + medulla core

**Bar: byte-identical textual round-trip on golden fixtures
(dump(parse(t)) == t); suite green; officina amalgam VERIFICATUM.**

Sealed decisions this milestone leans on (spec §I): modulus-per-TU
(conexio at M2; internal-linkage naming pre-sealed: `$<file-stem>.<name>` /
`$<functio>.<name>`); vocabulary per vocabularium-medullae.md (house
suffixes .s/.i/.f, sigils %/$/@, et/vel/aut/complementum, named conversion
family, sistere, arca); dump defines canonical form, strict parser.

**Chunk A — silva export package** (silva-side; hospes call per row; silva
phase-log pointer entry on landing):
1. Nexus symbolorum: semantica records use-site→symbol during analysis
   (typationes parallel-table pattern); public row proposal
   `silva_c89_symbolum_nodi(sem, nodus)`, canonical-aware.
2. `silva_c89_constans_aestimare(sem, nodus, &valor)` — expose the
   existing aestimator.
3. `silva_c89_chorda_decodere(piscina, nodus)` — escape-decoded string
   bytes (wraps _fugam_decodere; do NOT reimplement).
4. Declare `silva_oraculum_responsa_vacare` in silva.h (one line; it
   already exists with external linkage).
Amalgamare after; suite + hospes green.

**Chunk B — medulla structs + builder**: modulus/functio/blocci/
instructiones with TAGGED operands (registrum | immediatum | symbolum |
datum-ref; per-functio operand pool for vocare arity — sim C2); origo on
every instruction (in-memory always — sim C3); arca frame-slot model;
MedullaDatum {imago, relocationes} (sim C8); builder enforces terminator
discipline. Unit suite (probatio_officina_medulla.c).

**Chunk C — textual form + amalgam birth**: dump + strict parser
(vocabularium §VII); hand-written canonical .medulla fixtures in
probationes/fixa/; round-trip harness = THE BAR; amalgama/officina.c +
hand-written officina.h born (shared silva_amalgama mechanism), hospes +
VERIFICATUM gates.

Then: M1a RELATIO + audit → M1b INTENTIO (demissio; bar: corpus lowers,
0 ruinae, sistere classified, fusor green, every dump round-trips).

Known risks carried from simulatio-3: C2 operand shape is the most-copied
struct (get it right in B before C pins goldens); C4 binding-table is the
largest export (semantica change, not a header line); goldens pin AFTER
the vocabulary — never before (it is already sealed, so safe).

### M1a CHUNK A — RELATIO (2026-07-09): exporta demissionis SHIPPED

All four export-package items landed silva-side (detail:
silva/fontes/silva_c89_semantica.worklog.md 2026-07-09):

1. **Nexus symbolorum** — `silva_c89_symbolum_nodi` + `nexus_numerus`.
   Bindings recorded at BOTH resolution sites (identifier expression +
   vocatio callee — implicit synthesized callees bind too:
   `symbolum_nodi(callee)->est_implicitum` works). Canonical-aware and
   canonical-RELATIVE like typatio.
2. **`silva_c89_constans_aestimare`** — existed in fontes with the exact
   proposed signature; the gap was the silva.h row only (cheaper than
   the sim priced).
3. **`silva_c89_chorda_decodere`** — decoded bytes into a caller piscina;
   fragments concatenated, embedded NUL safe, NO trailing NUL
   (acies.numerus = octeti+1 carries it; demissio zero-fills from the
   type). FINDING: the L-string park is PRE-PARSER (`L"ab"` does not
   parse) — decodere's L guard is defensive armor, not a live path.
4. **`silva_oraculum_responsa_vacare`** — declared in silva.h (existed
   in silva_commissio.h all along).

Gates: semantica suite 308/308 (+22 asserts: nexus identity through the
index, implicit-callee binding, decode with \x/\0/fragments,
invalid-escape FALSUM); silva 29/29; amalgam VERIFICATUM (hospes 28/28,
all five rows called with content asserts); percursus corpus sweep
UNCHANGED on every baseline (0 errores, coverage 100.00%, 399
diagnostica / 31 plagulis, 305 indecisa) — nexus recording is invisible
to every existing gate.

**NEXT: Chunk B — medulla structs + builder** (officina/fontes/
officina_medulla.{h,c}: modulus/functio/blocci/instructiones, tagged
operands + per-functio operand pool (sim C2 — the most-copied struct,
settle before goldens pin), origo on every instruction, arca slots,
MedullaDatum {imago, relocationes}, terminator discipline enforced by
the builder; probatio_officina_medulla.c; officina
compile_probationes.sh born).

### M1a CHUNK B — RELATIO (2026-07-09): medulla structurae + fabrica SHIPPED

First officina code. officina/fontes/officina_medulla.{h,c} +
probatio_officina_medulla.c (72 asserts, green) + compile_probationes.sh
born (saltuarius template; silva amalgam as object from day one).

Shape as INTENTIO'd, three refinements recorded in the worklog:
- **Operand held at 16 bytes** (sim C2): sistere causae are module-level
  INTERNED chordae (dedup — the same diagnostic repeats thousands of
  times corpus-wide); operand carries an index (new genus CAUSA).
- **Three fixed operands (a/b/c) + stiva**: ramus and copia need exactly
  three; only vocare's arguments overflow to the per-functio operand
  pool. The common instruction is self-contained.
- **Conversion suffix order pinned**: typus = FONS, typus_secundus =
  FINIS (matches `extendere.s32.s64` reading order).

Everything is index-referenced (blocci/registra/symbola/causae) with
names riding along for the dump. Terminator discipline lives in
medulla_emittere (positive + negative tested). MedullaDatum ships with
imago (zero-filled) + relocationes and bounds-checked writes (sim C8).
Register auto-names "tN"; the C parser should treat register names as
an opaque namespace (worklog note).

Latina landmine SEVENTH firing: `magnitudo` as field/param name
(→ magnitudo_octetorum, TypusC89 precedent).

**NEXT: Chunk C — textual form + amalgam birth**: dump + strict parser
per vocabularium §VII (dump defines canon; float-immediate canonical
form is C's one open decision — worklog note on exactness vs
readability); hand-written canonical .medulla fixtures in
probationes/fixa/; round-trip harness = THE M1a BAR
(dump(parse(t)) == t); amalgama/officina.c + officina.h born (shared
silva_amalgama mechanism) + hospes + VERIFICATUM gates; then M1a
RELATIO + audit.

### M1a CHUNK C — RELATIO (2026-07-09): forma textualis + amalgama nata SHIPPED

**THE M1a BAR IS GREEN: dump(parse(t)) == t byte-identical** — the
fabrica-built $summa dumps byte-identical to the COMMITTED fixture
(fixa/summa.medulla pins the scriptor); both canonical fixtures
round-trip byte-identical; the commented fixture holds the idempotence
bar; floats are bit-exact through the cycle (%.17g, DECISUS mid-chunk
with the comment-skip policy — vocabularium §VII updated to settled).

Shipped: officina_medulla_textus.{h,c} (scriptor canonicus + lector
strictus line-based; blocks pre-scanned per functio for forward ramus
targets; medulla_emittere doubles as the lector's validator — the
fabrica discipline catches double terminators and bad registers for
free); fixa/{summa,omnia,commenta}.medulla;
probatio_officina_textus.c (37 asserts: golden, round-trip,
idempotence, float bits, error lines). Suite 2/2 (109 asserts total).

**Amalgama nata**: amalgama/officina.{c,h} (hand-written vanilla
officina.h — ALL Medulla structs transparent, consumers build
instructions; OfficinaXar is a transparent layout-exact copy, the
TesseraChorda pattern one size up), manifest amalgamator.c (tessera
exemplar; chorda excludes 44/45 functions), hospes.c 25/25 (pollution
+ golden equivalence + round-trip through the PUBLIC surface, every
public row called), amalgamare.sh — **VERIFICATUM (standalone +
hospes + nm-intersectio 0)** on birth day. Finds in the textus
worklog: Xar typedef-unit vs Piscina split-tag cadenda behavior;
MEDULLA_TYPUS_NIHIL token-identity; the i32-unsigned slip's officina
debut.

## M1a — RELATIO MILLIARII (2026-07-09)

**M1a CLAUSUM. Silva exports + medulla core: the IR exists, builds,
prints, parses, and ships as a verified amalgam.** Bars, all met:
**byte-identical textual round-trip** (canonical fixtures + goldens;
idempotence for commented files); suite green (2 suites, 109 asserts;
silva side: semantica 308/308, silva 29/29); **VERIFICATUM** for BOTH
touched amalgams (silva.h grew 5 rows, hospes 28/28; officina.{c,h}
born, hospes 25/25, nm-intersectio 0).

Three chunks, one day: A silva export package (nexus symbolorum +
constans_aestimare row + chorda_decodere + responsa_vacare decl —
two of four were header-rows-only, cheaper than the sim priced);
B medulla structs + fabrica (tagged operands 16B with causa
interning, a/b/c + stiva, terminator discipline, MedullaDatum
{imago, relocationes}); C textual form + amalgam birth.

**AUDITUS (full officina phase-log re-read; young book, all entries
this milestone):** INTENTIO items all delivered; carried risks all
closed (C2 operand shape settled in B BEFORE goldens pinned in C;
C4 binding table landed as a semantica feature; goldens pinned after
the sealed vocabulary — zero op renames needed during
implementation, the ceremony held). Vocabulary refinements during C
were STRUCTURAL only (float form, comment policy — Fran DECISUS'd
mid-chunk; vocabularium §VII updated from proposed to settled).
Parks, named with owners: textual provenance attribute → M3
(indicium is the consumer; in-memory origo already carried);
partial-octeti tolerance = documented non-canonical convenience;
big-zero datum images materialized in full (optimization note, on
measurement); symbol/causa interning linear (fusor measures, then
tabula if hot).

Latina landmine counter: magnitudo fired in B (seventh); the
i32-unsigned slip made its officina debut in C.

**NEXT: M1b — demissio.** Bar: every corpus TU lowers, 0 ruinae,
N sistere all classified; fusor sweep green; every corpus dump
round-trips byte-identically; golden dumps pinned for lowering
fixtures. Chunks per officina-m1-spec.md §III: A driver + expressions
+ statements → B calls + aggregates + address-taken → C static data
→ D fusor + close. Consider the INTENTIO after Fran's commit.

## M1b — INTENTIO (2026-07-09): demissio

**Bar: every corpus TU lowers — 0 ruinae, N sistere ALL classified
(per-causa counts, the percursus-diagnostics discipline); fusor sweep
green; every corpus dump round-trips byte-identically through the
textus lector; golden .c→.medulla dumps pinned for lowering
fixtures.**

Design is ALREADY PAID: simulatio-3 pretend-built this pass
(complications C4-C9 all have shipped answers), the interview sealed
trap policy + module model + static naming, and every input demissio
reads is public and frozen ({typus, conversio}_expressionis,
symbolum_nodi, constans_aestimare, chorda_decodere, formam_computare,
symbolum->valor/repositio/est_implicitum, membra[].offset). No new
simulation; this INTENTIO is execution planning.

**Boundary decisions (recorded now):**
- officina_demissio.{h,c} consumes silva THROUGH amalgama/silva.h
  ONLY (house rule). officina.h refers to silva types TAG-ONLY
  (`struct SilvaParsura*`, `struct SilvaSemantica*` — both tags
  verified present in silva.h) so officina.h stays include-free;
  the amalgam officina.c #includes "silva.h" as its ONE documented
  external dependency (officina is downstream of silva BY
  ARCHITECTURE; amalgamare.sh compiles with -Isilva/amalgama and
  hospes links both amalgams — the saltuarius dual-host pattern).
- Entry (proposal): `MedullaModulus* demissio_currere(Piscina*,
  const SilvaParsura*, const SilvaSemantica*, OfficinaChorda
  titulus_moduli)` — takes the POST-FLIP parsura + sem₂ (the
  canonical-relative contract); the two-pass driver is the CALLER's
  (fusor copies percursus; probationes use a small local driver).
- Internal-linkage mangling implemented here per the sealed scheme:
  file statics `$<file-stem>.<name>` (stem from modulus titulus),
  function statics `$<functio>.<name>`.
- RESEQUENCING from spec §III (reason recorded): the address-taken
  pre-pass moves from Chunk B INTO Chunk A — scalar identifier
  lowering needs register-vs-arca residency on day one, the pre-pass
  is one small walk (sim design: base-object chain, `&s.a[i].x`
  marks s; aggregates/arrays always arca), and splitting residency
  across chunks would churn every golden at B.

**Chunk A — demissio nata: driver + expressiones + sententiae.**
officina_demissio.{h,c}; address-taken pre-pass (pointer-keyed side
table, M0b creare_chorda pattern); functio definitions → medulla
(params, register-resident scalars, arca for addressed/aggregates);
_ut_valor/_ut_locus expression pair — conversus annotation → the
conversion-op family, ZERO UAC logic; all operators (compound
assignment pre-chewed; shifts LEFT-promoted as annotated; &&/||/
ternary naive block materialization); statements si/dum/fac/per/
commutatio (compare-chain)/salta/frange/perge/redde/sententia;
sistere for anything unlowerable (NEVER crash — tree-sitter posture
inherited); golden fixtures fixa/*.c + .medulla pairs (scalar
arithmetic, control flow, goto/switch); manifest + officina.h rows
+ hospes when the entry lands; probatio_officina_demissio.c with a
local parse+analyse driver.

**Chunk B — vocationes + aggregata.** Direct/indirect/variadic calls
(args pre-converted; arity tolerated); builtin routing is NOT here
(conexio/M2 — symbols stay EXTERNUM); aggregate return = explicit
destination parameter (C7); member access via membra[].offset after
formam_computare; subscript scaling by element size; struct
assignment = copia; goldens grow (vocatio, structurae).

**Chunk C — data statica.** Globals → MedullaDatum (aestimator
staticus wrapping constans_aestimare; address constants → relocatio
pieces — strings HIGH-VOLUME, fn-addresses rare, same mechanism);
string literals → anonymous data via chorda_decodere (bytes; NUL
from acies.numerus zero-fill); congeries direct correspondence
(elision measured ZERO); union initializers first-member; tentative
definitions; function-scope statics mangled; goldens grow (tabulae,
chordae).

**Chunk D — fusor + clausura.** instrumenta/principalia/fusor.c +
fusor.sh: the percursus two-pass copied verbatim (parse w/ contextus
+ systema lexicon → sem₁ → augere → responsa_vacare → recanonicare →
sem₂ → demissio) over every corpus TU; report functiones/
instructiones/data/sistere per causa (classified, the honest bar);
round-trip every dump through the lector (byte-identical); timing +
apex memoriae (cost-curve history, cultura rule); publica complete +
hospes; M1b RELATIO + FULL phase-log audit (milestone rule) + M1
SEALED (spec-v2 §XI actuals; memory).

**Named risks:** the corpus meets a NEW consumer again — budget a
real-bugs day (the M0b lesson: three real bugs on first corpus
contact; classify by CAUSE before theorizing, the 20-minute
diagnostic-spike instrument); POSIX-incomplete plagulae (~31) are
the KNOWN sistere class — anything beyond them gets classified, not
papered over; arca-vs-register residency decisions are demissio's
first place to silently diverge from C semantics (goldens +
differential eyeballs on fixtures); dump volume on generated-table
TUs (arbor2_glr_tabula: MBs of octeti lines) — fusor round-trips in
memory, never writes corpus dumps to disk.

### M1b CHUNK A — RELATIO (2026-07-09): demissio nata SHIPPED

officina_demissio.{h,c} (~1,600 lines) — driver, expressions,
statements, residency — compiled CLEAN under the full flag wall on
the first try, and both lowering fixtures (scalaria.c: arithmetic/
conversions/pointers/increments; fluxus.c: switch/while/do/goto/
labels/break/continue) lower with **ZERO sistere** (asserted), all
functiones terminated, dumps byte-identical through the lector,
goldens pinned AND inspected. Suite 3/3.

Detail in officina_demissio.worklog.md; headlines:
- The frozen M0b contract pays exactly as designed — demissio has
  ZERO promotion logic, just annotation obedience; conversions dump
  as the named op family.
- Declarator binding needed NO new silva export: the public symbol
  index carries lexema tokens → one pointer-keyed map per TU.
- Pre-pass = one generic recursion over public nodus->loci; arca
  ops in the entry block; captured params respilled.
- **FIND: byte-identity is not genus identity** — %.17g dumped
  100.0 as "100", which reparses as an INTEGER immediate; the
  round-trip bar could not see it; golden INSPECTION caught it.
  Scriptor now appends ".0" to unmarked floats. Golden blessing is
  a real gate.
- Nested case labels are flat siblings in silva's tree — the
  two-pass switch lowering handles `case 1: case 2:` fallthrough
  with no special case.
- Amalgam: demissio folded in; officina.c gained its ONE documented
  external include (silva.h); hospes is now a dual-amalgam host
  (28/28: parse→semantica→demissio→golden text through the public
  surfaces of BOTH amalgams); manifest vendors tabula_dispersa +
  friatio. VERIFICATUM (standalone + hospes + nm-intersectio 0).
- Emendatio: demissio_currere takes NON-const sem (forma pigra +
  aestimator mutate tables on query; INTENTIO said const).

**NEXT: Chunk B — vocationes + aggregata**: vocatio (direct/
indirect/variadic; args pre-converted; symbols stay EXTERNUM —
routing is conexio's), accessus via membra[].offset, subscriptio
scaling, copia for struct assignment, aggregate params/returns
(explicit destination convention), the three "nondum (M1b B)"
causae retired; goldens grow.

### M1b CHUNK B — RELATIO (2026-07-09): vocationes + aggregata SHIPPED

The three "nondum (M1b B)" causae retired: vocatio (direct $symbolum /
indirect register — (*f)(x) collapses to f(x) for free), accessus
(membra[].offset; incomplete access keeps its classified sistere =
the POSIX causa), subscriptio (scaled, commutative). Aggregate
convention (C7) end-to-end and VISIBLE in the goldens: $duplicare is
`(i64 %reditus, i64 %p) -> vacuum` with `copia %reditus, %f, 8`;
by-value args copy into caller temps; by-value params flip sedes to
arca (the incoming register IS the address). Struct assignment and
aggregate initializers = copia. Nested-call stiva contiguity solved
by local-buffer-then-one-append (rotunda pins it).

aggregata.c (calls, variadic call, fn-ptr param, arrays, struct
by-value round trip) lowers with ZERO sistere; golden pinned +
blessed; suite 3/3; amalgam VERIFICATUM hospes 28/28. Worklog: the
variadic arg-width note (machinula M2 convention, not a medulla
change) + three v1.1 polish notes.

**NEXT: Chunk C — data statica**: globals → MedullaDatum (aestimator
staticus wrapping constans_aestimare; address constants → relocatio;
strings via chorda_decodere), congeries locals + statics, function-
scope statics ($functio.nomen mangling), tentative definitions;
"nondum (M1b C)" causae retired; goldens grow (tabulae, chordae).

### M1b CHUNK C — RELATIO (2026-07-09): data statica SHIPPED

Globals, tentative definitions, string literals, function-scope
statics ($functio.titulus per the sealed mangling), local congeries
(hybrid: constant → one $congeries_N image + copia; mixed → $zeri_N
+ element stores; char-array strings → padded $chorda_N + copia).
**Relocatio settled ADDITIVE** (prior slot bytes = addend —
&acies_plena[1] pins it in the golden; vocabularium §VI updated; no
new MedullaRelocatio field needed). decodere's SilvaPiscina need
resolved by a transient demissio-owned piscina (no API churn). Float
images via a small demissio-side evaluator (public aestimator is
integral-only). File-scope failures _notare (module-level causa —
the honesty bar covers file scope without a functio context).

statica.c lowers with ZERO sistere; golden blessed byte-by-byte
(LE images verified by hand). Suite 3/3; amalgam VERIFICATUM
(hospes 28/28); silva untouched green. All "nondum" causae now
RETIRED — demissio has no named holes left.

**NEXT: Chunk D — fusor + clausura**: the corpus sweep instrument
(percursus two-pass copied verbatim), sistere classification
per-causa over all TUs, round-trip every dump through the lector,
timing + apex memoriae, publica complete, M1b RELATIO + FULL
phase-log audit + M1 SEALED (spec-v2 §XI actuals; memory).

### M1b CHUNK D — RELATIO (2026-07-09): fusor + corpus SHIPPED

The sweep instrument lives (instrumenta/principalia/fusor.c +
fusor.sh — the percursus two-pass exscriptus through the PUBLIC
surface, with -v per-file attribution and -causa origo-line probes).
FIVE real finds on first corpus contact, all fixed same-day (worklog:
ambiguus-canonica-is-an-INDEX; sizeof-locals vs closed scopes;
incomplete arrays completed by initializers; NULL-initialized static
pointers vs the aestimator; float binarium initializers). The
classification table IS the instrument — genus-numbered causae +
origo probes turned each row into a cause in minutes.

## M1b — RELATIO MILLIARII (2026-07-09)

**M1b CLAUSUM. Demissio: arbor typata -> medulla, corpus totum.**
Bars, all met:
- **571/571 plagulae lowered, 0 ruinae** (571 = every findable .c
  minus 2 over-tectum; percursus's 750 counts .c+.h — a PARSER's
  corpus; demissio's corpus is TRANSLATION UNITS, delta by design).
- **11,289 functiones; 106,717 data; 1,153,305 instructiones.**
- **sistere 2,870 in 102 plagulis — 16 causae, ALL classified with
  owners**: POSIX faces (accessus structurae incompletae 1,160 +
  locals — systema_posix park, pulled by M2 Wave 1), nested-chain
  park (unflipped forks: vocatio sine typo 205 + downstream poison
  ~870 — the M0a park's demissio face), stb_image vendored (forma
  rows in imago.c), semantica-diagnosed identifiers (126), small
  named tails (16).
- **Circuli 571/571 byte-idem** — every corpus dump reparses and
  re-dumps identically through the lector.
- Goldens: 4 fixture pairs pinned + blessed (scalaria/fluxus/
  aggregata/statica).
- Tempus: arbor 99s | demissio 5s | circulus 207s. Apex 9.7 GB
  (demo_52, data-dense; dump ~3x in memory — on-measurement park).
- VERIFICATUM (hospes 28/28 dual-amalgam) after every change.

Four chunks, one day. Emendationes recorded: addr-taken resequenced
into A; sem non-const; est_implicitum routing (M1a). Eight real
finds across M1b (three in A-C, five in D) — the "new consumer meets
the corpus" law now has three data points (M0b: 3, M1b: 8).

**AUDITUS PLENUS (phase-log totum relectum):** all INTENTIO items
delivered or emended-with-reason; carried risks closed (C2 operand
shape settled before goldens; C4 binding table landed silva-side at
M1a; vocabulary held END TO END — zero op renames from ceremony to
corpus). Parks with owners: variadic arg-width convention → M2
builtin bridge; jump tables → M2-if-profiled; &&/|| naive
materialization → M5-if-ever; textual provenance attribute → M3
(indicium); float-evaluator scope + notae tails (3+5+4+1) → named,
-causa instrument makes each a 5-minute look; .h-not-lowered =
principled corpus definition; dump-memory 3x → on-measurement.

**M1 SEALED.** Medulla + demissio shipped: the IR exists, prints,
parses, round-trips byte-identically, and the WHOLE MONOREPO lowers
through the frozen M0b contract with every stop classified.
**NEXT: M2 — machinula + builtins** (corpus bar: credo suites pass
interpreted, Waves 0+1; piscina suite first — the memory-model
gate; conexio design opens the milestone: module merging, EXTERNUM
resolution, builtin routing = definition-absent, the additive
relocation loader, variadic arg-width convention).

### ADDENDUM ANTE COMPACTIONEM (2026-07-09) — M2 engine hints

Nothing in-flight (M1 SEALED, gates green); accumulated hints for
whoever designs M2 machinula, recorded before they evaporate:

- **M2 opens with CONEXIO design** (named in the M1b RELATIO), and
  the proven arc applies: consider simulatio-4 → interview →
  INTENTIO — machinula has REAL unknowns (region allocator layout,
  value representation, builtin bridge mechanics, module loading).
  Conexio's job list: merge moduli (symbol union by name; internal-
  linkage names already globally unique via the sealed $-prefixes);
  EXTERNUM resolution = definition-present → link, absent → builtin
  table, neither → honest failure; the ADDITIVE relocation loader
  (slot = symbol address + slot's prior 8 bytes as addend).
- **Named convention to DECISUS at M2 design**: register width
  semantics — registers are 64-bit words; define whether sub-64
  values sit sign-/zero-extended (affects legere/scribere/movere
  and the variadic builtin bridge: stiva operands are 64-bit slots,
  printf-family reads widths from format directives).
- **Corpus bar mechanics are pre-researched** (spec-v2 §III):
  exit-code only, CWD = repo root, piscina suite FIRST (the
  memory-model gate: pointer identity + 16-byte alignment +
  indeterminate malloc + union aliasing), Waves 0+1 = v1 (~80
  suites), Wave 0 builtin floor in §IV, qsort-as-builtin dissolves
  the only trampoline.
- **The 2,870 sistere sites are LOADED, not defused**: Wave 0/1
  suites should never EXECUTE one; if one fires at runtime, that is
  the honest signal (likely = the systema_posix pull, the NAMED
  Wave-1 prerequisite — filum needs struct stat).
- **sistere at runtime**: halt printing the causa + position;
  origo is in-memory only (textual provenance still parked → M3).
- M2's instrument analog: the corpus-bar runner (per-suite: lower
  all TUs → conexio → run principale → exit code) + differential
  spot-checks (same test native vs interpreted, byte-for-byte).
  Fusor remains the standing lowering gate.
- Re-entry: officina/CLAUDE.md → this phase-log tail (M1b RELATIO +
  AUDITUS + this addendum) → officina-spec-v2.md §III/§IV/§V/§VIII →
  vocabularium-medullae.md → demissio/medulla worklogs. NEVER
  implement from a compaction summary alone.
- **Diagnostic investments pre-mapped (2026-07-09, post-compact):**
  project-specs/officina-m2-diagnostica.md — six ranked investments
  (flight recorder w/ origo backtraces, deterministic mmap base,
  poison-fill guards, classification-table runner, dispatch counters,
  -vestigium trace) + the origo→(fons,linea) distillation item (=
  indicium's line table born early). Feeds simulatio-4 + M2 INTENTIO.

## M2 — INTENTIO (2026-07-09): machinula + conexio + builtins

**Bar (per chunk, sealing at M2d): M2a the world LINKS (all cached
lib moduli + test modulus, 0 load failures, trap stubs counted+named,
$main found, relocations patched, hand-written .medulla loads) →
M2b probatio_piscina GREEN interpreted (exit 0 + stdout
byte-identical to native — the memory-model gate) → M2c Wave 0 GREEN
(~65 suites, classification table honest) → M2d Waves 0+1 GREEN
(~80 suites) = M2 SEALED = officina v1 execution shipped.**

Design is ALREADY PAID: three grounding agents (loader surface, libc
reality, suite composition) ran BEFORE simulatio-4; the sim
pretend-built the pass (C1-C12 all have shipped answers); the
16-DECISUS interview sealed every fork (officina-m2-interview.md);
officina-m2-spec.md is the plan; officina-m2-diagnostica.md's six
instruments are ADOPTED as first-class deliverables. This INTENTIO is
execution planning.

**Boundary decisions (recorded now):**
- THREE new fontes modules + one instrument: officina_regio (owned
  region), officina_conexio (linker/loader), officina_machinula
  (interpreter + builtins), instrumenta/principalia/cursor.c +
  cursor.sh (corpus runner; SEPARATE from fusor, which stays the
  standing lowering gate). Vocabulary adds NOUNS ONLY (regio,
  descriptor, ansa, cursor, vestigium, custos) — the sealed op
  surface is untouched.
- SILVA BOUNDARY: regio/conexio/machinula never touch silva types.
  The origo→(fons,linea) distillation needs SilvaNodus descent + the
  SilvaExpansio, so it lives SILVA-FACING-SIDE as a demissio-adjacent
  helper producing a pure medulla-side line table (structure defined
  in officina_medulla.h, no silva types) — consumed by machinula's
  flight recorder without front-end coupling. Distillation runs while
  the front-end is alive; per-TU pipeline = parse → sem → demissio →
  distill → FREE front-end arenas (the 9.7GB-apex answer; moduli +
  line tables resident, ~100MB-scale).
- CONEXIO MODEL (sim C1-C6): global symbol table (tabula_dispersa +
  friatio, names interned length-aware into a conexio piscina) + one
  translation array per module; moduli IMMUTABLE (no instruction
  rewriting). LINK THE WORLD (no per-suite manifests exist or are
  computable — grounded). Lazy trap stubs for unresolved symbols
  (fault-if-CALLED, sistere's posture at the link seam;
  classification gains "builtin deest"). Function DESCRIPTORS: 16B
  region-resident {s64 tag; functio ref} per functio — descriptor
  address IS the fn-ptr value; tag = the M5 native seam. Extern-data
  cells: stdin/stdout/stderr as opaque ansae (0/1/2; fopen returns
  3+) + errno with bridge write-back (errno is on the WAVE-0 path —
  entitas).
- MACHINULA CONVENTIONS: width DECISUS = canonicalize on write (every
  op writes a full 64-bit word extended per op-suffix signedness; f64
  bit-pattern; reads take low bits — variadic bridge reads plain
  words). Switch dispatch (C89 forbids computed goto; ceiling until
  M5). Explicit stack check at vocare (breach = honest halt). exit =
  halt-flag unwind; main's redde = process status. Builtins: STATIC
  table, ALL calls through ONE chokepoint (the named v2 record/replay
  seam — nothing built); public registration row deferred to M4.
- REGIO: one large fixed-base mmap reservation (lazy commit), areas
  globals|stack|heap; region-malloc = free-list, bases >=16-aligned
  (piscina asserts real-pointer %16 == 0), malloc stays INDETERMINATE
  (poison-fill under debug flag makes it recognizable, never zero);
  in-region + alignment guards under the same flag. Deterministic
  base = diffable runs = time-travel brick one.
- DIAGNOSTICS FIRST-CLASS (diagnostica note): flight recorder ALWAYS
  ON (fixed ring: functio/bloccus/instr-index; halt report = position
  + registers + call stack + ring tail, file:line via line table, to
  stderr); dispatch counters (per-op/per-functio/builtin-frequency/
  memory high-water — the jump-table park's evidence); -vestigium
  full trace as escalation; cursor rows = praeteriit | exitus
  dissimilis | sistere(causa) | vitium VM | builtin deest(symbolum) |
  tempus, PLUS the standing stdout byte-diff column vs pinned native
  capture (pass-path proven deterministic; nondeterministic suites
  get named exclusions).

**Chunk M2a — regio + conexio + loader.** officina_regio
(reservation, areas, region-malloc + guards); officina_conexio
(global table + translations, placement per ordinatio, ADDITIVE
relocation patching, descriptors, trap stubs, extern-data cells,
line-table intake); distillation helper (silva-facing side);
lector-fed loading of hand-written .medulla; per-op-family fixture
suite BEGUN (~8-12 files — loading asserted here, execution at M2b);
probatio_officina_regio.c + probatio_officina_conexio.c; manifest +
officina.h rows + hospes per standing rule.

**Chunk M2b — machinula core + Wave-0 floor.** Dispatch loop (all 35
ops, width convention); frames + arca + stack check; flight recorder
+ halt report + counters; sistere runtime (halt w/ causa + position);
exit/redde plumbing; builtin chokepoint + floor (malloc free strlen
strcpy memcpy memset snprintf printf exit; %zu in the directive set;
ansa cells wired; errno write-back mechanism); .medulla fixture suite
COMPLETE + green (exit-value asserts); summa.medulla executes = the
walking skeleton. BAR: probatio_piscina GREEN interpreted.

**Chunk M2c — cursor + Wave-0 sweep.** cursor.c/cursor.sh
(lower-once-cache-all pipeline, world-link per suite, classification
+ timing + stdout-diff columns, native capture pinning); remaining
Wave-0 builtins AS THE TABLE PULLS THEM (strtol family + errno,
fabs/sin/cos/pow/fmod/ldexp, atoi/atof/strtod, ctype,
sprintf/fprintf); named exclusions recorded. BAR: Wave 0 GREEN.

**Chunk M2d — Wave 1 + systema_posix + clausura.** File builtins
(fopen family via ansa table, stat/mkdir/getcwd); the systema_posix
pull (struct stat layout — the NAMED prerequisite; the 1,160
POSIX-classified sistere sites are the honest signal); CWD = repo
root + results/ + /tmp write paths verified; Wave-1b/2/3 + GUI +
benchmark deferrals restated by name; M2 RELATIO + FULL phase-log
audit (milestone rule) + M2 SEALED (spec-v2 §XI actuals; memory).

**Parks updated at this INTENTIO:** variadic arg-width park
DISSOLVED (canonicalize-on-write answers it); qsort-as-builtin
re-parked to a knotapel wave (grounded: ZERO sites in Waves 0+1);
jump tables await M2c counter data; descriptor enrichment → M5/vindex
on real need; builtin record/replay → v2 (chokepoint is the door).

**Named risks:** the corpus meets a THIRD new consumer — the
corpus-contact law's data points are 3 (M0b) and 8 (M1b); budget the
days at M2c, classify by CAUSE before theorizing. The seams law says
surprises land at conexio↔medulla and machinula↔host-libc boundaries,
not in the designs above — the classification table + flight recorder
exist to make them cheap. snprintf bridge fidelity policed
continuously by the stdout-diff column. Indeterminate-malloc
divergence (a test accidentally depending on native garbage) = a
CORPUS bug, surfaced honestly, poison makes it recognizable.
arbor2_glr wall-clock: accept minutes, measure, no optimization
before M2c data.

### M2a — RELATIO (2026-07-10): regio + conexio + nexus mundi SHIPPED

**BAR GREEN: THE WORLD LINKS.** cursor v0 (link-only; grows into the
full runner at M2c): 113 lib TUs lowered once into a cached-moduli
piscina (trees destroyed per TU) + probatio_piscina.c → ONE conexio:
**0 load failures; 11,863 global symbols; 2,799 functiones nexae;
8,898 data placed + relocations patched; 164 decipulae COUNTED +
NAMED; $main found — descriptor 0x3000000c7c90, BYTE-IDENTICAL
ACROSS RUNS** (fixed-base determinism observed in the wild). Nexus
itself: 3 ms.

Shipped: officina_regio (fixed-base 64GB reservation, globalia|
stiva|acervus, LIFO-deterministic free-list malloc PINNED in
probatio, 16-aligned bases, poison custodia, validating liberare;
62 asserts) + officina_conexio (global table + per-module
translations, immutable moduli, ADDITIVE relocation patching
memcpy-safe, 16B descriptors signum-tagged, lazy decipulae, extern
cells stdin/stdout/stderr ansae + errno, duplicate = honest fractura
w/ named symbol; 105 asserts incl. 9 machinula fixtures loading) +
per-op-family .medulla fixtures (fixa/machinula/, 9 files, expected
exit values documented — M2b executes them) + demissio_lineas_
colligere + MedullaLineae (origo→(via,linea) distilled while the
front lives; proto-indicium) + cursor.c/cursor.sh. Amalgam: 5
capita/corpora, VERIFICATUM, **hospes 57/57** (was 28 — every new
row called). Suite 5/5. Fusor full-corpus gate GREEN after the
stem-mangling change: **576/576 plagulae** (corpus grew 571→576),
0 fractae, 0 ruinae, **circuli 576/576 byte-idem**, 1,160,432
instructiones; sistere 2,912 in 105 plagulis — same classified
causa rows, deltas consistent with corpus growth.

**THE CORPUS-CONTACT LAW: 4th data point — THREE real finds on
first world-link contact** (all at the conexio↔medulla seam, as the
INTENTIO predicted):
1. **Demissio stem-mangling gap** — $chorda_0 and file statics
   collided across modules (M1b's single-TU lowering couldn't see
   it). The SEALED vocabulary already prescribed $<stirps>.<name>;
   fixed at ONE chokepoint (_titulum_symboli: definitions AND
   references share the path). Goldens re-pinned + BLESSED (pure
   renames; extern names correctly bare).
2. **REAL CORPUS BUG** — include/coloratio.h + include/biblia.h had
   bare `enumeratio {...} Nomen;` (nomen MISSING): tentative-
   definition GLOBALS in every including TU, silently merged by
   native common-linking for months. FILE-FILUM class. Three
   typedefs repaired (names appear only in comments — intent
   unambiguous).
3. **Conexio duplicate-handling desync** — early-return corrupted
   moduli/translationes parity → downstream crash. Now collects ALL
   duplicates coherently (the collision CENSUS is the instrument);
   nectere refuses while fracturae > 0.

Emendationes (recorded, reasons in worklogs): cursor.c born at M2a
in link-only form (the world-link bar needed the driver; INTENTIO
listed it at M2c — it grows there); MedullaLineae's public table
field is void* (TabulaDispersa = anonymous-struct typedef, no tag —
the Xar landmine's sibling, solved by conversion not copy). Named
notes: decipulae contain ~70 BARE LOCAL NAMES (st/tv/stb internals)
= the POSIX/stb park's linker face → M2c classification row; only
2 cellae bound (errno may be reaching demissio via lexicon, not
symbol — CHECK at M2b errno write-back design); biblia_dr.c
over-tectum → biblia_* decipulae cascade (M2c: raise tectum for the
world). Latina 8th firing: externus.

**NEXT: M2b — machinula core + Wave-0 floor** (dispatch loop all 35
ops w/ canonicalize-on-write; frames + arca + stack check; flight
recorder ALWAYS ON w/ line-table backtraces + counters; sistere
runtime; exit/redde plumbing; builtin chokepoint + floor: malloc
free strlen strcpy memcpy memset snprintf printf exit + %zu; ansa
cells wired; errno write-back mechanism; the 9 fixtures EXECUTE with
asserted exit values; summa.medulla = walking skeleton). BAR:
**probatio_piscina GREEN interpreted** (exit 0 + stdout
byte-identical to native — the memory-model gate).

### M2b — RELATIO (2026-07-10): machinula core + pavimentum SHIPPED

**BAR GREEN: probatio_piscina INTERPRETATA — exitus 0 ET stdout
BYTE-IDEM nativo.** Porta modelli memoriae clausa: identitas
monstratorum trans reficere, ordinatio XVI per valores veros, reusus
arenae — omnia interpretata asserta. 46,521 instructiones, 2,103
vocationes, 871 aedificata, 3 ms. Fistula TOTA clausa: fons C89 →
silva → semantica → demissio → conexio → MACHINULA → mores
observabiles identici clang nativo.

Shipped: officina_machinula.{h,c} (~1700 lineae, compilatio prima
munda): dispensatio commutatione 35 operationum; CANONICUM-IN-
SCRIPTURA (DECISUS Q4) — arithmetica integralis in i64 insignato
(involutio definita), divisio honesta; tabulata in stiva regionis
(cursor per redde restauratus, arca probata, tectum 1M tabulatorum);
recordator volatus SEMPER ACTIVUS (anulus 4096 + relatio halitus:
positio + stiva vocationum + cauda anuli, via:linea per lineas
distillatas); numeratores (per-op/vocationes/aedificata/apex);
CANALIS UNUS aedificatorum (sedes v2) + ligatio in-situ
(decipulae→AEDIFICATUM, signum solum); pavimentum 14 aedificatorum
(malloc/free/realloc/str*/mem*/fabs/exit/printf-familia) + formator
plenus (%.*s, latitudo *, %zu/%lld); ansae stdout/stderr; cella
errno capta (scriptura-retro M2c). Fixturae 9 EXSECUTANTUR (omnes
→42); halitus honesti probati (sistere causa, decipula, exit 7,
recursio, functio absens). probatio_officina_machinula 63 asserts;
cursor exsecutionem crevit (diagnostica→stderr, stdout purus pro
diff). Amalgama: hospes 61/61 VERIFICATUM; suite 6/6.

Inventa primi contactus (fixturae): SYMBOLUM ut argumentum vocationis
(_valor_operandi sedem resolvit); vitium in argumentis vocationem
non sistebat (ruina hospitis — probatio currens addita); recursio
infinita 4GB stivae legitime implebat (tectum profunditatis).

**NEXT: M2c — cursor + Unda 0** (tabula classificationis + columna
stdout-diff + tempus per plagulam; aedificata residua Undae 0 dum
tabula ea trahit: strtol+errno, mathematica, ctype, atoi/strtod;
exclusiones nominatae; tectum biblia_dr). BAR: Unda 0 VIRIDIS (~65
suites). Lex contactus corporis: 5 puncta data (3/8/3/3) — dies
inventorum computa.

## M2c — INTENTIO (2026-07-10): cursor + Unda 0

**BAR: Unda 0 VIRIDIS (~65 suites purae) per tabulam
classificationis.**

Consilium (spec-v2 §III/§IV + relatio M2b):
- **cursor v2 = cursor multi-suite**: mundus demittitur SEMEL
  (moduli cachati, IMMUTABILES); tum PER SUITE: regio+conexio
  recentes (generare/destruere — basis fixa una), addere mundum +
  modulum probationis, nectere, machinula, currere $main. Columna
  per suite: PRAETERIIT(0) | EXITUS(n) | SISTERE(causa) |
  DECIPULA(symbolum) | VITIUM(nuntius) | tempus ms | instructiones.
  Summa in fine = tabula classificationis (fusor exemplar).
- **Selectio suitarum**: omnes probationes/probatio_*.c PRAETER
  exclusiones nominatas (Unda 1 filesystem: arbor2_file_roundtrip,
  arbor2_expandere, arbor, arbor_formator, arbor_index,
  entitas_repositorium, filum, iter_directoria, macho, generare,
  eventus_inspector; Unda 1b/2: uuid, tempestivum, actor*,
  tcp*, tls, reactor, http(vivum); GUI 11 + benchmark + biblia_dr
  consumers). Lista IN cursor.c (nominata, non tacita).
- **Aedificata Undae 0 residua DUM TABULA TRAHIT** (non ante):
  strtol/strtoul+errno-scriptura-retro, atoi/atof/strtod, abs,
  ctype (isalpha/isdigit/islower/isupper/isspace/iscntrl/tolower/
  toupper), mathematica (sin/cos/pow/fmod/ldexp), fflush, memmove,
  strchr/strcmp/strncmp/strstr/strncpy, qsort NON (knotapel solum).
- **Tectum biblia_dr**: -omnia modus (mundus completus).
- Columna stdout-diff: infra POST primam sweep viridem (captura
  nativa pinnata — instrumentum separatum); vectis M2c = exitus.
- Lex contactus: 5 puncta data — MURUM exspecta prima sweep; per
  causam classifica, -causa/origo probes, NUMQUAM theoriza ante
  tabulam.

### ADDENDUM IN CURSU (2026-07-10) — M2c prima sweep: status

IN FLIGHT (compactio imminens; codice INCOMMISSO post m2b commit):
- Aedificata 14→44 (str*/mem*/atoi/strtod/strtol+errno-retro/ctype/
  mathematica/fflush) — suite machinula VIRIDIS.
- cursor v2 = sweep multi-suite SCRIPTUS ET CURRIT: mundus cachatus
  semel (moduli_mundi Xar), per suite regio/conexio recentes + tabula
  classificationis (PRAETERIIT/EXITUS/SISTERE causa/DECIPULA/VITIUM/
  ms/instr) + exclusiones nominatae (~34) in cursor.c.
- PRIMA SWEEP: classificat recte (arbor2_glr → SISTERE "magnitudo
  inaestimabilis" @arbor2_glr_initializare_tabulas, 32k instr — sedes
  M1b onerata EXSECUTA, signum exspectatum!) tum SEGFAULT medio
  sweep, suite ignota (stderr buffered peribat — lectio per-suite
  superstes in scratchpad/unda0.txt).
- PROXIMUM: (1) suitem ruinae invenire (echo ante quamque / binaria
  quaesitio; suspecti: formator %s in regionem non probatus, aut
  vitium in aedificato novo, aut memoria trans suites — regio
  destruere inter suites sed piscina_ctx machinularum CRESCIT);
  (2) murum classificare per causam (fusor exemplar);
  (3) reficere per causam — "magnitudo inaestimabilis" = classis
  semantica-parcae, fortasse plures suites eadem causa.
- Re-entry: officina/CLAUDE.md → hic → M2c INTENTIO supra →
  officina-m2-spec.md → worklogs machinula/conexio. NUMQUAM ex
  compendio compactionis solo.

### ADDENDUM (2026-07-10, post-compactionem) — MURUS FRACTUS:
### latitudo suffixi auctoritas in lectura

Instrumenta nova (cursor v2.1, eadem die):
- **fork-per-suite**: mundus SEMEL demissus in parente (COW), pullus
  per suitem — ruina hospitis = versus RUINA unus, non mors sweep.
  Suspecti addendi superioris OMNES dissoluti gratis (memoria pulli
  cum pullo perit).
- **-custodia**: regio-probationes in omni accessu — 34 signum-11
  anonyma → 34 VITIUM honesta cum positione. NON standing (venenum
  allocationis contra paginas recentes nativas divergere posset);
  modus diagnosis.
- **fusor -textus <substr>**: medulla canonica unius TU ad stdout.
- **-mora <n> / TEMPUS**: alarm() in pullo, SIGALRM → versus TEMPUS
  (lapifex interpretatus 351s ante sistere suum ardebat; tectum 30s).
- Filtrum suitarum: suffixus .c requisitus (toml intrusum).

INVENTUM MURI (34/73 suites una causa): omnes ad EANDEM
instructionem cadebant — tabula_dispersa_inserere @si_finis_14+4.
Origo: `positus_primus_deletum != -1` (i32 domus = INSIGNATUS):
movere.i32 canonicum zephyro-extensum vs negare.s32 canonicum
signo-extensum; inaequalis.i32 verba PLENA comparabat ("registra
ut-sunt") → ramus falsus → reditus -1 → sloti+0xFFFFFFFF*40 →
extra regionem. REFINITIO DECISUS Q4: lectura ut-sunt sana est
SOLUM pro operationibus amputantibus; operationes latitudini
sensibiles (comparationes, dividere/residuum, dextrorsum, fluitare,
redde) operanda ad latitudinem suffixi canonicant (_valor_canonicus,
quinque sedes). Semantica instructionis per suffixum SE CONTINET —
demissio NON mutata (suffixus IPSE conversio implicita visibilis
est). Fixtura c8 in comparationes.medulla. Vide worklog machinulae.

Classes residuae (per -causa probas + tabulam):
- "vocatio sine typo functionis" (~10 suites: friatio, url, commandi,
  ...): CAST cum operando parenthesato `(typus)(expr)` ut VOCATIO
  parsatus, oraculum non vertit — **opus silva/oraculi**, classis
  maxima residua (etiam lapifex verisimiliter).
- "forma localis ignota" (fasti, sectio, symbola, macho_deps,
  entitas): sedes in plagulis propriis (fasti.c...), classis
  demissionis/semanticae.
- "magnitudo inaestimabilis" (arbor2_glr/scribere, compound, utf8):
  residuum classis sizeof.
- "accessus sine inscriptione basis" (calendarium, toml);
  "forma elementi ignota" (flatura); "monstrator nullus/humilis"
  (capsula, libri — VITIUM verum, investigandum).
- DECIPULAE nominatae: time, getcwd, persona_scribere, fopen
  (arbor_praeparator — filesystem, Unda 1).
- TEMPUS: suites lapifex (labor verus interpretatus, minuta; -mora
  600 pro cursu benedictionis).

TABULA POST REFECTIONEM (sweep secunda, eadem die):
**31 praeteritae | 0 exitus mali | 31 sisterae | 4 decipulae |
2 vitia | 0 ruinae | 5 tempora / 73 suites.** Viridis 16→31 una
refectione (lex causarum: una causa, quindecim suites). NOTA BENE:
**exitus mali ZEPHYRUM** — omnis suite quae ad finem currit, RECTE
currit. Murus residuus = sistere semantica-parcae (19 "vocatio sine
typo" = cast-ut-vocatio, opus oraculi silvae — vectis maxima:
verisimiliter etiam 5 tempora lapifex post eam cadunt), non vitia
machinulae. PROXIMUM: (1) oraculum cast-cum-parenthesi (silva),
(2) "forma localis ignota" (5), (3) capsula/libri monstrator nullus
(investigatio), (4) tunc columna stdout-diff.

### ADDENDUM (2026-07-10, serius) — SANATIO CATENARUM: 31 → 45/73

Vectis maxima TRACTA (opus silvae; detail:
silva/fontes/silva_c89_oraculum.worklog.md + silva phase-log):
classis "cast-ut-vocatio" = parca resolutoris M0a nominata
("catenae nidificatae"). Regula trivalens DUALIS (typus notus in
positione expressionis = lectio impossibilis) + fluxus victoris
solius per furcas interiores in situ resolutas. Portae silvae:
suite 29/29, percursus indecisa 305→9 / coopertura 100.00% /
fideles 759/760 (deferral pasta), **auspex 3,753/0**, amalgama
VERIFICATUM.

TABULA TERTIA: **45 praeteritae | 1 exitus malus | 16 sisterae |
4 decipulae | 2 vitia | 0 ruinae | 5 tempora / 73 suites.**
Fusor: sistere "vocatio sine typo" → 1 instructione toto corpore;
summa 2,912→2,659 in 105→68 plagulis; circuli 578/578.

Murus residuus (classes):
- "forma localis ignota" 9 (cursus, entitas, fasti, layout,
  macho_deps, sectio, servitium, sputnik_interpres, symbola) —
  nunc classis maxima; suites trans murum veterem PROGRESSAE.
- "magnitudo inaestimabilis" 4 (arbor2_glr/scribere, compound,
  utf8); "accessus sine inscriptione basis" 2 (calendarium, toml);
  "vocatio sine typo" 1 (arbor_quaestio — residuum verum).
- **nuntium EXITUS 1 — primus exitus malus**: suite ad finem
  currit, assertum fallit. Classis investigationis NOVA (semantica
  interpretata divergens? datum reale?). INVESTIGA PRIMUM — porta
  fiduciae machinulae.
- capsula/libri VITIUM monstrator nullus (investigatio);
  decipulae fopen/time/getcwd/persona_scribere (Unda 1 / parvae);
  lapifex 5 TEMPUS — cursus -mora 600 in fundo currens.

### ADDENDUM (2026-07-10, serius adhuc) — nuntium SANATUM: 46/73

nuntium EXITUS-1 investigatum PRIMUM (porta fiduciae) et SANATUM
eodem die: litera f32 ut argumentum vocationis (IMMEDIATUM_F sub
S64 → figura f64, dimidium falsum in parametro f32; 0.0f et
-123.456f casu praeteribant — negare.f32 per registrum!). Refectio:
argumenta per typum PARAMETRI aestimata ubi signatura nota (idem
principium diei: signatura auctoritas ad limitem — tertium
speculum absens: parametra intro, redde retro, nunc literae ad
sedem vocationis). Regressio in vocationes.medulla ($proba_f32).
Instrumenta: cursor **-sola <substr>** (suites congruentes solae,
stdout visibile + fflush ante _exit — _exit stdio non effundit!).
Vide worklog machinulae.

TABULA QUARTA: **46 praeteritae | 0 exitus mali | 16 sisterae |
4 decipulae | 2 vitia | 0 ruinae | 5 tempora / 73 suites.**
Hospes 61/61; suite 6/6.

INVENTUM lapifex: c89_decl TEMPUS etiam @600s — obice sublato
labor verus interpretatus ENORMIS est (native ~2-4s × 100-200
tarditas interpretis ≈ 10-20 min per suitem). NON vitium —
quaestio posturae undae (exclusio nominata? -mora ingens in cursu
benedictionis? celeritas machinulae = opus posterius M5/numquam
per mandatum). FRAN DECERNAT.

PROXIMUM M2c: (1) "forma localis ignota" classis (9 suites — sedes
per fusor -causa in plagulis propriis: fasti/sectio/symbola...);
(2) capsula/libri monstrator nullus; (3) "magnitudo inaestimabilis"
4; (4) decipulae parvae (time/getcwd — aedificata trivialia?);
(5) columna stdout-diff post murum sisterarum.

### ADDENDUM (2026-07-10, nocte) — TABULA RESIDUI COMPLETA
### (omnes 27 suites non-viridis classificatae per sedem)

Investigatio per relationes recordatoris (unda3) + fontes — quisque
halitus ad SEDEM UNAM reductus:

1. **POSIX transitivum (9)**: cursus/entitas/layout/servitium/
   sputnik_interpres → uuid_c._tempus_milliseconds (structura
   timeval!); macho_deps/sectio/symbola → filum_legere_totum;
   fasti → fasti_dies_hodie (timeval). "forma localis ignota" TOTA
   = parca POSIX per dependentias transitivas. NON vitium — opus
   systema_posix (M2d) aut reclassificatio undae. FRAN DECERNAT.
2. **magnitudo aciei completae (5-6)**: arbor2_glr/scribere/
   compound/utf8 ("magnitudo inaestimabilis" — idioma
   sizeof(acies)/sizeof(acies[0]) in aciebus [] completis) +
   capsula (EXITUS 1 via "initiator staticus inaestimabilis" —
   capsula_assets.mensura = magnitudo(acies completae) → 0 →
   aperire NIHIL). UNA radix: completio aciei per initiatorem non
   in typatione. DUAE viae: (A) sanatio radicis in SEMANTICA
   (typus symboli completur — C89 3.5.7; aestimator/typationes/
   demissio omnes sanantur; portae auspex/percursus/suite parata)
   aut (B) demissio-fallback tribus sedibus (_mensura_completa
   consulta). QUAESTIO STRATI — Fran consulendus.
3. **accessus membri in rvalore aggregato (2)**: calendarium/toml —
   `fasti_differentia(...).dies_totales` (membrum in fructu
   vocationis). Lacuna demissionis vera, refectio contenta
   (temporarium reditus iam allocatur — locus deest).
4. **intptr_t ignotum (1)**: arbor_quaestio — (intptr_t)(i+1),
   typus C99 extra systema ISO → supplementum systematis (M2d).
5. **tectum (1)**: libri — book_assets/capsula_libri.c 12.8MB
   (tertius locus comitum!) super tectum 4MB. Cum biblia_dr in
   ordine "gravia" nominando.
6. **decipulae (3)**: multipart→time, via→getcwd,
   arbor_praeparator→fopen. time = ISO (aedificatum parvum);
   getcwd/fopen = Unda 1.
7. lapifex TEMPUS (5) — parca nominata (velocitas mensurata).

INSTRUMENTUM: cursor v2.2 — COMITES probationum in mundum
(probationes/*.c non-probatio_: capsula_assets, persona_gen —
compile_tests.sh eas nominatim nectit, mundus noster nunc quoque).
Parca M2b "lectio decipulae-ut-datum tacita" IN AGRO CONFIRMATA
(capsula: octeti descriptoris ut CapsulaEmbed → monstrator 2).
Effectus: nuntium_rotunda VIRIDIS (persona_scribere soluta),
capsula VITIUM→EXITUS-1 (nunc classis 2 supra). **47/73.**

Maximum realisticum post refectiones 2+3: ~53-54/73; residuum =
POSIX/tectum/tempus/velocitas — materia M2d aut ordines nominati.

### ADDENDUM (2026-07-10, nocte serius) — COMPLETIO ACIEI: 52/73

Classis 2 (magnitudo aciei completae) SANATA via (A) — radix
semanticae (C89 3.5.7 ad litteram; vide silva phase-log +
silva_c89_semantica.worklog.md). Portae: suite 29/29, percursus
100.00%, **auspex 3,753/0**, amalgama VERIFICATUM; fusor sistere
2,659→2,604 in 68→53 plagulis (initiator staticus EVANUIT).

TABULA QUINTA: **52 praeteritae | 0 exitus mali | 12 sisterae |
3 decipulae | 1 vitium | 0 ruinae | 5 tempora / 73 suites.**
Quinque exspectatae verterunt (arbor2_glr **101M instructiones**
VIRIDIS!, arbor2_scribere 51M, compound 18M, utf8, capsula — TOC
+ inflatio gzip tota interpretata) + BONUS: flatura ("forma
elementi ignota" quoque forma aciei completae erat).

Residuum 21 — OMNE in ordine nominato: POSIX-transitivum 9 |
calendarium/toml 2 (membrum-in-rvalore, UNICA refectio demissionis
residua) | arbor_quaestio 1 (intptr_t) | libri 1 (tectum) |
decipulae 3 (fopen/time/getcwd) | lapifex 5 (TEMPUS parca).
PROXIMUM: membrum-in-rvalore (→~54) → decipula time (ISO, vilis)
→ tunc M2d materia (systema_posix trahit 10+).

### ADDENDUM (2026-07-10) — MEMBRUM IN RVALORE: 54/73

Ultima refectio demissionis vera CLAUSA. Classis "accessus sine
inscriptione basis" (`fasti_differentia(...).dies_totales`) sanata
in _ut_locum ramo `.`: locus basis deest ET typus aggregatus →
`inscriptio_basis = _ut_valor(basis)` — NON casus specialis sed
conventio domus visibilis facta (**valor aggregati EST inscriptio
eius**: _vocationem reddit arca_reditus, assignatio aggregati
"valor = inscriptio", argumenta valore COPIA ex _ut_valor).
Catenae f().a.b componuntur; evaluatio duplex nulla (_ut_locum in
vocatione invalidum SINE emissione reddit).

Regressio: aggregata.c membrum_fructus (offset +4 ADDERE + offset
0 via celeris + copia valore ex rvalore) — aureum re-pinnatum et
BENEDICTUM. Suite 69/69; amalgama VERIFICATUM (hospes 61/61).

Effectus corporis: fusor sistere 2,476 in 36 plagulis (2,604/53
antea) — SEPTENDECIM plagulae purgatae (forma per consumptores
fasti ubique, non 2 suites solae). Causa residua 9 OMNES in parca
stb_image (imago.c, lineae expansae) — causa in corpore vivo
EXSTINCTA. Circuli 578/578 byte-idem.

TABULA SEXTA: **54 praeteritae | 0 exitus mali | 10 sisterae |
3 decipulae | 1 vitium | 0 ruinae | 5 tempora / 73 suites.**
calendarium PRAETERIIT (124/124 credo, computus paschalis totus
interpretatus, 233,828 instr) + toml PRAETERIIT. Residuum 19 =
POSIX-transitivum 9 + intptr_t 1 + decipulae 3 (fopen/time/getcwd)
+ tectum libri 1 + lapifex 5 (parca).

PROXIMUM: decipula time (ISO, vilis → multipart) → tunc M2d
(systema_posix trahit 10+; getcwd/fopen ibi).

### ADDENDUM (2026-07-10) — AEDIFICATUM time: 55/73 + INVENTUM
### LIGATIONIS (lectio extra tabulam!)

Aedificatum #43 addendo INVENTUM: `#define AEDIFICATA_NUMERUS 44`
super tabulam 42 introituum — ansa ligationis in machinula_creare
DUAS AedificatumNota structuras extra tabulam legebat in OMNI
nexu mundi (UB tacitum, benignum solo fortuito: octeti sequentes
nomen decipulae numquam congruerunt). Sanatio: numerus nunc
STRUCTURALIS (magnitudo/magnitudo) — drift impossibilis. Lectio:
numerus manu iuxta tabulam manu = duo fontes veritatis.

time ipsum: _aed_time per formam strtol (scriptura-per-monstratorem
memcpy; time_t hospitis = verbum 8 octetorum; scriptura == fructus
in vocatione UNA = determinatum). Regressio: modulus tempus.medulla
in probatione machinulae (aequalitas scripturae + solum post-2020 +
ordo monotonus → 42). Suite 67/67; amalgama VERIFICATUM.

TABULA SEPTIMA: **55 praeteritae | 0 exitus mali | 10 sisterae |
2 decipulae | 1 vitium | 0 ruinae | 5 tempora / 73 suites.**
multipart PRAETERIIT (43,038 instr). RESIDUUM 18 — OMNE materia
M2d aut parca nominata: POSIX-transitivum 9 (forma localis ignota)
+ intptr_t 1 + fopen/getcwd 2 (Unda 1) + tectum libri 1 + lapifex
5 (parca). **M2c AD SOLUM NOMINATUM PERVENIT** — nihil restat quod
sine machinis M2d verti potest.

PROXIMUM: decisio sigilli M2c (columna stdout-diff = vectis
sequens per ordinem barrae) → M2d (systema_posix trahit 10+,
fopen/getcwd ibi).

### ADDENDUM (2026-07-10) — COLUMNA STDOUT VIVA (deliverable M2c
### ultimum): 50 eaedem | 0 DIFFERUNT | 5 exclusae

Vectis secundus permanens nunc stat. Mechanica:
- **captare.sh natus** (fixatio capturarum nativarum): suites undae
  ex `cursor -enumerare` (UNUS fons veritatis exclusionum — tabula
  in cursor.c sola), obiecta ex compile_tests.sh EXTRACTA (fons
  unus ibi quoque), quisque suitem BIS nativam currit — effusiones
  differentes = NONDETERMINISTA (non figitur), aequae = aureum in
  fixa/stdout_nativa/ (67 plagulae, 280K). generare NON curritur
  (paritas: cursor plagulas ut-sunt demittit).
- **cursor v2.3**: pullus effusionem in build/capturae/ scribit
  (freopen pro /dev/null); versus pulli sine novissima linea —
  parens post waitpid columnam appendit (idem | DIFFERT | sine
  aureo | exclusa (causa)); alarm(0) post cursum (SIGALRM medio
  versu vetitum); linea STDOUT sub TABULA; vectis: exitus != 0 si
  quaevis DIFFERT. Aedes deduplicatae: exclusiones_undae in gradum
  plagulae levatae (-enumerare + cursus eaedem).
- **Exclusiones nominatae 6** (causa recordata in cursor.c):
  arbor2_glr/arbor2_scribere/compound (monstratores DEBUG %p —
  ASLR), commandi (purgamentum pre-init consulto impressum),
  entitas (UUIDv7 tempus+fors), multipart (limes ex semine
  time(NULL)).

INVENTUM COLUMNAE PRIMO CURSU: multipart DIFFERT — cursus duplex
nativus in SECUNDO EODEM cadit (3ms), semen time(NULL) idem bis;
interpretatum secundo alio = limes alius. Fovea "secundi eiusdem"
in captare.sh nunc clausa (somnus 1s inter cursus). NON vitium
machinulae — machina fideliter limitem alium ex semine alio
computavit. Latina landmine 10ma: "nomen" ut parametrum (typedef!).

STATUS: 50/50 suites virides comparabiles OCTETIM IDENTICAE
nativae. TABULA immota: 55 | 0 | 10 | 2 | 1 | 0 | 5 / 73.
**OMNIA deliverables M2c completa** (cursor + sweep + builtins +
capturae fixae + exclusiones recordatae). Sigillum M2c = decisio
Franis. Post sigillum: M2d (systema_posix trahit 10+).

## 2026-07-10 — M2d INTENTIO (Unda 1 + systema_posix)

Vectis M2d (spec §III): **Undae 0+1 VIRIDES (~80 suites) = M2
SIGILLATUM = executio officinae v1 missa.** Chunks:

**A — systema_posix + certificatio formarum.** Plagula nova
silva/fontes/systema_posix.h (C89 vanilla, exemplar systema_c89.h):
typi (intptr_t, mode_t, off_t, dev_t...), formae VERAE hospitis
(structura timespec/timeval/stat — forma stat = praerequisitum
NOMINATUM; 64-bit-inode arm64), declarationes (stat/mkdir/getcwd/
gettimeofday/remove/rename), macra valoris/functionis per canalem
LEXICON (S_ISDIR = probatio prima macri functionis in canale!).
Mechanismus: CONCATENATIO textuum in parsuram UNAM apud oneratores
(cursor + fusor SOLI - silva-latus intactum, API immutata,
plagula ISO pura manet). PORTA NOVA: auspex_posix - clang formas
asserit (sizeof + offset omnium camporum contra capita vera; forma
falsa = corruptio memoriae tacita interpretata).
Effectus mensurandus: classis "forma localis ignota" (129) solvitur.

**B — ansae + aedificata plagularum.** Tabula ansarum in machinula
(FILE* = ansa opaca; 0/1/2 = flumina norma, DECISUS M2): fopen ->
ansa nova; fclose invalidat; fread/fwrite/fseek/ftell/fgetc/fgets/
ungetc/feof/ferror/fputc/fputs per ansam -> FILE* hospitis;
fprintf ansae verae (non 1/2) scribenda. + stat/mkdir/getcwd/
gettimeofday/remove/rename. Regressiones in probatione machinulae
(plagula temporaria sub officina/build/).

**C — Unda 1 in cursum.** Exclusiones trimmatae: filum,
entitas_repositorium, arbor_formator, arbor2_file_roundtrip, macho
(+ singillatim: arbor/arbor_index/arbor2_expandere/
eventus_inspector); iter_directoria + generare = Unda 1b NOMINE
(system+opendir); uuid/tempestivum = Unda 2 NOMINE. captare
recurritur (aurea nova), viae scripturae (results/, /tmp)
verificatae, dilationes 1b/2/3 nomine restitutae.

Lex suturarum vigilat: suturae machinula<->libc hospitis (ansae!)
et lexicon<->macra functionis = sedes exspectatae inventorum.

### ADDENDUM (2026-07-10) — M2d CHUNK A CLAUSUS: systema_posix

- silva/fontes/systema_posix.h NATA (C89 vanilla; intptr_t + typi
  POSIX Darwin; timespec/timeval/stat formis VERIS; stat/mkdir/
  getcwd/gettimeofday; S_IS*/S_I*USR macra). Insidiae mensuratae:
  st_mode/st_nlink 16-BIT, tv_usec 32-bit (suseconds_t = int!),
  stat 144 octeti (inodus 64-bit).
- **PORTA NOVA auspex_posix.sh VIRIDIS**: duae TU (capita vera |
  plagula nostra) eandem tabulam assertionum compilant - sizeof +
  offset omnium camporum + valores macrorum ≡. Forma falsa =
  corruptio tacita; porta ante machinulam stat.
- Oneratores cursor+fusor: textus ISO+POSIX CONCATENATUS in
  parsuram unam (silva-latus intactum - baselines immotae; API
  semantica immutata).
- Effectus: fusor sistere 2,476→2,426 in 36→30 plagulis; "forma
  localis ignota" in lib/ SOLUM in parcis nominatis restat
  (imago/stb 76, tcp_posix/Unda-3 10); "vocatio sine typo" (intptr_t)
  SOLUTA. Suites POSIX nunc DEMITTUNTUR et ad decipulas honestas
  perveniunt (fasti: DECIPULA gettimeofday 8ms) - Chunk B eas vertet.

### ADDENDUM (2026-07-10) — M2d CHUNK B CLAUSUS: 66/73, 0 sisterae

Tabula ansarum (FILE* = ansa opaca, 0/1/2 flumina; fopen locellos
3..64 scrutatur, fclose vacat) + XV aedificata nova (lista corpore
mensurata): fopen/fclose/fread/fwrite/fseek/ftell/fgets/fputc/
remove/rename/stat/mkdir/getcwd/gettimeofday/localtime → 58.
stat/gettimeofday DIRECTE per monstratores hospitis scribunt -
sanum QUIA auspex_posix formas ≡ certificat. Regressio
ansae.medulla (vita tota, st_size @96 per formam certificatam, 42).

**INVENTUM DEMISSIONIS (sputnik pecunia)**: PARENTHESIS in
_expressionem redde directum faciebat - conversionem ANNOTATAM
nodi ipsius (cauda) praetermittens. (s64)(a*b) et `s64 x = (a*b);`
ambo conversionem perdebant - fructus f64 crudus in parametrum
s64. Vectis rotundi-circuli caecus erat (forma non significatio -
lectio secunda). Sanatio 2 lineae; regressio scalaria
pecunia_exemplum (ambae formae); aureum re-pinnatum benedictum.
Detail: officina_demissio.worklog.md.

TABULA OCTAVA: **66 praeteritae | 0 exitus mali | 0 sisterae |
1 decipula | 1 vitium | 0 ruinae | 5 tempora / 73.**
STDOUT: 60 eaedem | 0 DIFFERUNT | 6 exclusae. Residuum 7 OMNE
nominatum: cursus (fenestra_tempus_* = Tier-2 .m, extra corpus
per DECISUM), libri (tectum 4MB), lapifex 5 (TEMPUS parca).
UNDA 0 AD TECTUM VERUM.

PROXIMUM (Chunk C): exclusiones Undae 1 trimmandae (filum,
entitas_repositorium, arbor_formator, arbor2_file_roundtrip,
macho + singillatim ceterae), captare aurea nova, viae scripturae
verificatae, dilationes 1b/2/3 restitutae nomine → vectis
M2: Undae 0+1 VIRIDES = M2 SIGILLATUM.

### ADDENDUM (2026-07-10) — M2d CHUNK C: UNDA 1 INTRAVIT — 74/80

Exclusiones trimmatae: IX suites filesystem in undam (filum,
entitas_repositorium, arbor_formator, arbor2_file_roundtrip,
macho, arbor, arbor_index, arbor2_expandere, eventus_inspector);
unda 73→80. Dilationes RESTITUTAE NOMINE in tabula ipsa cum
causis: Unda 1b (iter_directoria/generare — system+opendir),
Unda 2 (uuid/tempestivum/actor*), Unda 3 (rete), cursus (Tier-2
.m fenestra_tempus_*), libri (gravia 12.8MB > tectum, ordo cum
biblia_dr).

Auditio aedificatorum intrantium: UNUM solum deerat (unlink, 6
sedes) + clock (roundtrip) → 60 aedificata. captare recucurrit
(somnus 1s vim probavit: multipart nunc SE detexit!); aurea 73;
arbor2_file_roundtrip nondeterministicum (monstratores DEBUG
arbor2 eaedem causae) → exclusio 7ma.

**VIII ex IX suites intrantibus VIRIDES PRIMO CONTACTU** —
effusionibus octetim identicis. arbor2_file_roundtrip post clock:
PRAETERIIT 30.4M instr (plagulae verae per ansas scriptae lectae
deletae!). TABULA NONA: **74 praeteritae | 0 exitus mali |
0 sisterae | 0 decipulae | 0 vitia | 0 ruinae | 6 tempora / 80.**
STDOUT: 67 eaedem | 0 DIFFERUNT.

Residuum 6 = TEMPUS OMNE (arbor_formator lapifex-classis + 5
lapifex): labor verus interpretatus ultra 30s — NON vitia.
**CURSUS BENEDICTIONIS IN VOLATU (-mora 1800)** = vectis M2
ultimus: si VI virides → UNDAE 0+1 VIRIDES = M2 SIGILLANDUM
(caerimonia Franis).

### ADDENDUM (2026-07-10) — BENEDICTIO -O0: 75/80; CELER IN VOLATU

Cursus benedictionis -mora 1800 (-O0): **TABULA DECIMA: 75
praeteritae | 0 exitus mali | 0 sisterae | 0 decipulae | 0 vitia |
0 ruinae | 5 tempora / 80.** STDOUT: 68 eaedem | 0 DIFFERUNT |
7 exclusae. **arbor_formator BENEDICTUS: 673,181,933 instructiones
in 44s, effusio IDEM** (recordum novum, 6.6x prius). Lapifex V
etiam @1800s TEMPUS — labor verus >32 miliardis instructionum
quaeque; parca "cursor -celer" TRACTA (nominata ad hoc ipsum):
cursor.sh -celer = -O2+LTO in build/celer/ (3.6x mensuratum,
obiecta numquam mixta).

Instrumentum novum: cursor **-census** — numeri_op per suitem in
build/census/ (machinula_numerus_op + medulla_op_titulus publici
iam erant). Datum quaestionis dispatch parcatae: superinstructiones
ex mensura, non fide (spec: "decision falls out of timing data").

**CURSUS CELER IN VOLATU** (-celer -census -mora 3600, tabula
plena LXXX): si lapifex V virident → UNDAE 0+1 VIRIDES = vectis
M2 impletus → SIGILLUM (caerimonia Franis: relectio phase-log
plena + auditum). Cursus -O0 cotidianus manet.

Domus: officina/CLAUDE.md refectum (norma nova: milestone currens
= cauda phase-log, NUMQUAM hoc documentum; instrumenta omnia
documentata; decompositio celeritatis in worklog machinulae
indicata ante OMNE opus velocitatis).

### ADDENDUM (2026-07-10, nocte) — VECTIS M2 IMPLETUS: 80/80

Cursus benedictionis celer (-celer -census -mora 3600):

**TABULA UNDECIMA: 80 praeteritae | 0 exitus mali | 0 sisterae |
0 decipulae | 0 vitia | 0 ruinae | 0 tempora | 0 nexus fracti /
80 suites.**
**STDOUT: 73 eaedem | 0 DIFFERUNT | 0 sine aureo | 7 exclusae.**

Lapifex V benedicti: quaeque ~72.8 MILIARDA instructionum, ~21 min
apud -celer (~58M instr/s — praedictio 3.6x confirmata), effusio
quaeque IDEM. Summa cursus: **365.2 miliarda instructionum
interpretatarum, omnes suites virides, omnes effusiones
comparabiles octetim identicae.**

Census messis: build/census/ 80 plagulae (numeri_op per suitem).
Gustus (lapifex_c89_decl): movere 14.4G | ramus 13.5G | addere
9.0G | legere 6.7G | salire 5.2G — datum quaestionis dispatch
parcatae, nunc mensuratum.

**UNDAE 0+1 VIRIDES = VECTIS M2 IMPLETUS.** Dilationes nominatae
in tabula exclusionum cum causis (1b system+opendir | 2
tempus/entropia | 3 rete | Tier-2 .m | gravia). SIGILLUM =
caerimonia Franis: relectio phase-log plena + auditum (regula
limitis milestonis), tum M2 CLAUSUM et executio officinae v1
MISSA.

Post sigillum (consilium cum Frane statutum): instrumenta prima
(project-specs/instrumenta-simulatio-1.md — custos latinae,
nexus/emitte, interpretare; 4-5 dies, linea sectionis nominata) →
M3 (indicium + vindex).

### ADDENDUM (2026-07-10, ante compactionem) — STATUS: SIGILLUM PENDET

Vectis M2 IMPLETUS (vide TABULA UNDECIMA supra); libri omnes
scripti; nihil in volatu praeter ipsum SIGILLUM. Sessio sigilli
(post compactionem) faciat IN ORDINE:
1. RELECTIO PLENA huius phase-log (regula limitis milestonis —
   non ex compendio compactionis!).
2. AUDITUM: quaeque parca/dilatio adhuc nominata cum domino?
   quodque inventum in worklog suo? tabulae metricae rectae?
3. RELATIO M2 scripta (exemplar: M0a/M1 relationes supra) +
   sigillum = decisio Franis.
4. Tum: instrumenta prima (project-specs/instrumenta-simulatio-1
   + -2, DECISUS quinque sigillata: censor/emitte/origo/nexus/
   interpretare) → M3.
Non commissum: project-specs/instrumenta-simulatio-2.md.

## M2 — RELATIO MILLIARII (2026-07-10)

**M2 CLAUSUM. Machinula + conexio + aedificata: EXECUTIO OFFICINAE
v1.** Fistula tota vivit: fons C89 → silva → semantica → demissio →
conexio → machinula = mores nativi, octetim ubi comparabile.

Bars, all met:
- **TABULA UNDECIMA (cursus benedictionis -celer -census -mora
  3600): 80/80 praeteritae | 0 exitus mali | 0 sisterae |
  0 decipulae | 0 vitia | 0 ruinae | 0 tempora | 0 nexus fracti.**
- **Vectis secundus permanens: 73/73 effusiones comparabiles
  OCTETIM IDENTICAE capturis nativis fixis; 7 exclusiones nominatae
  cum causis recordatis** (3+1 monstratores DEBUG/ASLR, purgamentum
  pre-init, UUIDv7, semen time(NULL)).
- Porta modelli memoriae (M2b): probatio_piscina interpretata
  exitus 0 + stdout byte-idem nativo.
- Benedictio: lapifex V quaeque ~72.8 MILIARDA instructionum
  (~21 min @ ~58M instr/s; -celer 3.6x confirmatum); arbor_formator
  673M; summa cursus **365.2 miliarda instructionum
  interpretatarum**.
- Portae stantes die sigilli RECURSAE: suite 6/6; hospes 61/61
  VERIFICATUM; fusor circuli 578/578 byte-idem, sistere 2,426 in
  30 plagulis — OMNES in parcis nominatis.

Quattuor chunks, UNO DIE (2026-07-10; INTENTIO pridie): M2a regio +
conexio + nexus mundi (11,863 symbola, 3 ms) → M2b machinula
(dispensatio 35 op, canonicum-in-scriptura, recordator volatus
SEMPER ACTIVUS, pavimentum 14 aedificatorum) → M2c cursor + Unda 0
(aedificata 44, murus latitudinis fractus, sanatio catenarum silvae,
completio aciei semanticae, membrum-in-rvalore, columna stdout) →
M2d systema_posix + ansae + Unda 1 (aedificata 60, auspex_posix
porta certificationis, VIII/IX intrantes virides primo contactu).

**Lex contactus corporis — puncta data nunc 3/8/3/3 + ~10 (M2c+d)**:
murus latitudinis (34 suites, instructio UNA); cast-ut-vocatio
(oraculum silvae, 19 suites; indecisa 305→9 toto fronte); completio
aciei C89 3.5.7 (semantica, 6 suites); membrum-in-rvalore (17
plagulae); litera f32 per typum parametri; AEDIFICATA_NUMERUS extra
tabulam legens; PARENTHESIS conversionem devorans; vitia corporis
VERA (coloratio.h/biblia.h enumerationes sine nomine); multipart
secundum-idem; capsula decipula-ut-datum in agro. TRIA inventa
refectiones silvae/semanticae erant — frons tota melior facta.
Tabularium caecitatum vectium: 2 (fluitans "100"; PARENTHESIS
byte-idem-sed-falsum) — benedictio aureorum + vectes morum, non
formae.

**AUDITUS PLENUS (phase-log totum relectum + systema plagularum):**
- INTENTIO omnia praestita aut emendata-cum-ratione (cursor natus
  M2a link-only; campus tabulae MedullaLineae void*; arithmetica
  undae 73+9−2=80 — cursus/libri ex ordinibus in exclusiones
  nominatas moti; captare somnus-1; -celer varians).
- Tabulae metricae contra systema plagularum VERIFICATAE: cursor
  -enumerare = 80; census = 80 plagulae; aurea 75 = 73 comparabilia
  + 2 fixa stantia (cursus, libri — pinnata dum in unda erant,
  retenta pro tractionibus suis nominatis).
- Quodque inventum in worklog suo (machinula 7 introitus, demissio
  7, conexio 1 — capita inspecta).
- Actuales contra spec-v2 §XI: vectis dixit "~80 suites, dilationes
  nominatae" → ACTUALE exacte 80, omnes virides, dilationes IN
  TABULA cum causis, PLUS vectis stdout stans 73/73.

**Parcae cum dominis (restitutae ad sigillum):** Unda 1b
(system+opendir) | Unda 2 (tempus/entropia) | Unda 3 (rete) |
cursus Tier-2 .m | libri gravia (cum biblia_dr) — omnes IN tabula
exclusionum cursoris cum causis. Velocitas: -celer benedictioni
sufficit; acies instructionum planae = apertura M3 (~1.3-2x super
celer); dispensatio/superinstructiones = EX DATIS census (messis
in build/census/, 80 plagulae); ultra → M5. Lectio decipulae-ut-
datum tacita: in agro confirmata, -custodia = instrumentum
diagnosis (NON stans — venenum allocationis), ditatio → M5/vindex.
Registratio aedificatorum publica → M4; record/replay aedificatorum
→ v2 (canalis unus = ianua); qsort-ut-aedificatum → unda knotapel;
provenientia textualis → M3 (indicium); parca stb_image vendita
(imago.c, sistere 76) + tcp_posix (10, cum Unda 3); mktime
periculum nominatum + auditum-purum; aurea stantia cursus/libri
notata.

**M2 SIGILLATUM (Fran, 2026-07-10) = EXECUTIO OFFICINAE v1 MISSA.**
NEXT: **instrumenta prima** (censor/emitte/origo/nexus/interpretare;
DECISUS quinque sigillata; specs = instrumenta-simulatio-1+2.md;
~4-4.5 dies, linea sectionis censor+emitte+origo) → **M3 indicium +
vindex** (apertura: acies instructionum planae; ad clausuram:
auditum praedictionis usus instrumentorum, sim-2 §5).

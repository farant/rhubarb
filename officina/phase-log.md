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

## INSTRUMENTA PRIMA — INTENTIO (2026-07-10)

Five dogfood CLIs, design ALREADY PAID (instrumenta-simulatio-1.md
C1-C5 + A1-A2; -2.md episodes 1-3; DECISUS quinque sigillata in
sim-2). CLI-before-MCP stands. Homes: silva/instrumenta/principalia/
(censor, emitte, origo, nexus) + officina/instrumenta/principalia/
(interpretare). Shell wrappers per percursus.sh precedent. Books:
worklog entry per tool; this phase-log; sim docs get the RELATIO.

Order + bars:
1. **censor** (~1d) — latina lint. Check = origo EXPANSIO +
   nomen_macro ∈ latina-set, in NAME-BINDING slots ONLY (slot
   inventory derived MECHANICALLY from the generated accessor table
   — hand list forbidden, the AEDIFICATA lesson); per-TU
   latina.h-inclusion awareness (knotapel English-C can't fire).
   Wired into officina/silva compile-script gates DAY ONE;
   full-corpus on-demand until FP rate measured on one run. BAR:
   the ~10 historical firings × `git show` pre-fix blob = the credo
   corpus — censor finds ALL of them at their recorded lines.
2. **emitte** (~0.5d) — single-TU byte-exact definition dump,
   `[via:linea-linea]` header. BAR: byte-diff vs source extent on
   10 symbols incl. probationes files.
3. **origo** (~0.5d) — expansion story of the token at
   via:linea:columna (invocatio → nomen_macro → corpus per layer);
   shares emitte's loader skeleton. Promoted by sim-2 (M3
   macro-stepping asks it constantly). BAR: known macro sites
   report full chains.
4. **nexus** (~1d) — nexus_percursus sweep → build/nexus.tsv
   (DISPOSABLE, never committed, mtime-staleness warning; explicit
   pre-acta scaffolding) + nexus CLI (grep the tsv, group-by-file
   w/ counts, definition site marked, WHOLE repo). BAR: spot-check
   vs grep on 10 symbols.
5. **interpretare** (~1-1.5d) — wrapper-module generator over
   cursor guts (inline-module pattern from probatio machinula);
   -plagulae manual scoping; scalars + string literals v0
   (aggregates = named deferral); halt report prints free on
   non-BENE. BAR: the sputnik pecunia repro + →42 fixtures smoke.

Cut line if sprawl: censor + emitte + origo ship first. The
falsifiable usage prediction (sim-2 §5: emitte >> nexus >
interpretare > origo in counts; origo highest leverage-per-call)
gets audited at M3 close.

### ADDENDUM (2026-07-10) — CENSOR NATUS ET IN PORTIS (tool 1/5)

**Bar MET same day**: silva/instrumenta/principalia/censor.c +
silva/censor.sh. Corpus sweep: **762 plagulae / 528 cum latina /
0 flagrationes — FP rate 0.00 primo contactu** (the two sealed
exceptions principale+FILUM exactly sufficient). Gates wired DAY
ONE per DECISUS: porta 6 CENSURA in BOTH amalgamare.sh (silva:
regression probe 13/13 + ~77-file sweep ~12s; officina: ~25-file
sweep ~4s); both VERIFICATUM "+ censura" green.

Mechanics as simulated, both no-hand-list pledges kept: slot
inventory RUNTIME-derived from SILVA_C89_REGISTRUM ("tok_titulus"
slots; tok_valor/tok_destinatio consciously excluded); latina set
from the expander's macro journal (fons basename = latina.h), no
word list in the tool; BFS worklist walker (no depth cap); origo
chain walked through all layers.

**FINDING (sim assumption killed)**: git has NO pre-fix landmine
blobs — every historical firing was fixed within its session
before commit (tessera has 2 commits total). Validation corpus is
SYNTHETIC: probationes/fixa/censoris/flagrationes.c (classis A, 13
blessed rows incl. position-discrimination proof) +
fragores.c.fragor (classis B — suffix keeps percursus/-corpus
away from deliberate parse errors). Ledger audit: only exire (#4)
was ever SILENT; the other ~9 firings were loud-but-cryptic →
classis B rows list DISTINCT latina macros per ERROR node
('integer', 'nomen', 'redde' → the culprit jumps out), cap VIII
with explicit ", ..." marker.

Detail: silva/instrumenta/principalia/censor.worklog.md. Named
deferrals there (standing corpus sweep = Fran's promotion call now
that FP=0 is measured; B-lane ranking; basename collisions).
**NEXT: emitte** (single-TU byte-exact definition dump,
[via:linea-linea] header; ~0.5d).

### ADDENDUM (2026-07-10) — EMITTE NATUS, VECTIS 10/10 (tool 2/5)

silva/instrumenta/principalia/emitte.c + silva/emitte.sh.
**BAR MET: 10/10 symbola octetim identica extractioni sed** —
fontes officinae/silvae/lib, capita (Xar/MedullaLineae), staticum
gradus plagulae, DUAE probationes, unum caput knotapel anglicum.
Mechanica communis censori (loci tok_titulus e registro; corpora
functionum in ambulatione congruentiae praetermissa — usus = opus
nexus).

Duo inventa vera, AMBO per compositionem vectis ipsius capta:
(1) valor lexematis = textus EXPANSUS — "principale" in arbore
"main" est; congruentia nunc valor AUT valor radicis originis
(scriptum ET expansum inveniunt); (2) tag-slots in USIBUS
congruebant (quaerens Xar etiam XarIterator emittebat) — tags
ligant solum cum membra definiunt (_tag_definit, registro-genericum).
Detail + asperitates notatae: emitte.worklog.md.

**DEBRIEF INSTRUMENTORUM (praxis nova Franis, exemplar primum —
dies censor+emitte):** *adhibita*: censor 6× (fixturae, corpus,
portae), emitte 12× (vectis + usus proprius in structuris silvae
legendis — iam in die natali suo pro grep+Read adhibitum!), fusor
0×, cursor 0× (dies sine tactu officinae). *fructus*: vectis
emitte duo lacunas veras cepit (radix-valor, tag-refs); sweep
censoris FP=0 mensuravit. *asperitates* (in worklogs ad momentum
notatae): ordo blocorum emitte = ordo inventionis non linearum;
praeparatio capitum ~1s per invocationem (pretium initii frigidi,
notum, acta/M4 respondet). *desiderata*: nulla nova — nexus iam
in ordine (blast-radius quaesitum bis hodie manu factum est).
**NEXT: origo** (CLI fabulae expansionis; ~0.5d, sceletum emitte
communicat).

### ADDENDUM (2026-07-10) — ORIGO NATUS, VECTIS IMPLETUS (tool 3/5)
### + DESIDERATUM PRIMUM: bibliotheca similitudinis

silva/instrumenta/principalia/origo.c + silva/origo.sh. Simplius
emitte: NULLA ambulatio arboris — fluxus expansus + catenae
originis insertae SUNT fabula. `origo <plagula> <linea>[:<col>]`;
quaestio per lineam solam CONSULTO (relationes halitus machinulae
via:linea dant, columna absente). **VECTIS IMPLETUS: sedes
macrorum notae catenas plenas referunt** — `'if' <- si`;
`'NULL' <- NIHIL <- CREDO_NON_NIHIL` (profunditas 2!); brachium
chordae `'"p"' <- CREDO_NON_NIHIL (chorda #)`; `'34' <- __LINE__
(chorda #)` (canalis M0b visibilis); argumenta ad positionem SUAM
scriptam attribuuntur. Asperitates in worklog (capita gregum
iterata in lineis macro-functionum; solum plagula princeps v0).

**DESIDERATUM PRIMUM PHASIS (Fran, conversatione post-debrief):**
bibliotheca parva similitudinis nebulosae (fzf-genus: congruentia
subsequentiae + punctatio limitum verborum, ~150 lineae C89 super
chordas + suite credo). Signum demandae IAM HODIE accensum (vectis
emitte: symbolum male coniectum in scrutinium.h → circuitus grep).
Consumptores visibiles tres: emitte via "non inventum" (nomina
ligaminum IAM ambulata — "fortasse: ..."), nexus -similis (12k
symbola, vis bruta sufficit), saltuarius type-jump (v0.1 backlog).
CONSILIUM SIGILLATUM cum Frane: origo primum (factum), tum
bibliotheca similitudinis ANTE nexum (emitte "fortasse" + nexus
-similis simul nascuntur). TITULUS = caerimonia Franis (candidati:
similitudo / rete / chorda_similitudo). "Fixing the little
frictions adds up" — Fran.
**NEXT: bibliotheca similitudinis (titulus Franis) → nexus.**

### ADDENDUM (2026-07-10) — SIMILITUDO NATA (titulus Franis) +
### emitte "fortasse:" VIVUM

lib/similitudo.{c,h} + probatio_similitudo (20 asserta, viridis
primo cursu; in SOURCE_FILES compile_tests.sh). Congruentia
subsequentiae avara (limes +16 / continuum +8 / casus +1 / hiatus
−3/−1), functiones purae super chordas, sine allocatione; DP
alineationis optimae = dilatio nominata (API immutata maneret).

emitte "fortasse:" VIVUM eodem die: nomina ligaminum in ambulatione
congruentiae gratis collecta → similitudo_optima → V suggestiones
distinctae in stderr. **Fractura natalis replicata et sanata:**
`scr_signatura_computare` → "fortasse: scr_signatura_ex,
scr_signatura_libera, scr_signatura_aequalis".

**INVENTUM (hora prima cibi canini): directio subsequentiae in
casu did-you-mean INVERTITUR** — nomen totum male coniectum
subsequentia nullius est. Sanatio = politica CONSUMPTORIS (emitte
quaestionem a dextra decurtat donec candidati appareant, solum
III), bibliotheca pura manet; regula duorum ante promotionem in
API. Detail: lib/similitudo.worklog.md.

Consecutio: probatio_similitudo undam cursoris sponte intravit
(80→81) — bibliotheca nova INTERPRETATA in machinula die natali
suo curritur (captare aurea nova; verificatio -sola infra).
**NEXT: nexus** (sweep totius repositorii → build/nexus.tsv
disponibile + CLI grep; -similis gratis nunc).

### ADDENDUM (2026-07-10) — DEBRIEF SESSIONIS (rogatu Franis):
### duo data pro auditu M3

1. **Caveat validationis**: usus instrumentorum novorum hodie
   maxima parte SE PROBANTES erant (vectes, fixturae, replicationes)
   — non organici. Numeri organici veri incipient cum nexus +
   interpretare exsistant et M3 aperiatur; auditum praedictionis
   sim-2 §5 hoc discrimen servet (validatio ≠ demanda).
2. **Datum nexus (ex grep-numeratione sessionis)**: dimidium fere
   grep-quaestionum mearum erat "in QUA plagula est X?" — quaestio
   quam emitte respondere NON potest (plagulam poscit!) et cuius
   responsum nexus est. Confirmat: nexus = instrumentum desideratum
   maximum residuum; group-by-file + sedes definitionis signata
   (pins sim-2) recte sculpta sunt.

### ADDENDUM (2026-07-10) — UNCI VIVI (canalis push) + silva-lsp
### FUTURUM NOMINATUM

Ex conversatione adoptionis cum Frane ("infundibulum
venditationis": omnis sessio recens = usor novus; regulae
memorandae = frictio quae conversionem perdit; visio-doc §XIV
novum). Duo unci in .claude/settings.json (+ .claude/hooks/):

- **censor-custos.sh** (PostToolUse, Write|Edit): censor in
  plagulam .c/.h modo scriptam (~0.9s; .md etc 17ms praetermissa);
  SILENS cum pura (FP=0.00 rem sustinet), flagrationes solae
  additionalContext iniciunt (exemplar clangd — informatio, non
  obex). Exclusa: censoris/, scratchpad/, build/.
- **generata-custos.sh** (PreToolUse, Write|Edit): editiones
  manuales plagularum GENERATARUM VETAT — mechanicum per signum
  capitis (GENERATUM + NE MANU EDITES / NOLI MANU MUTARE), nulla
  lista viarum; plagulae generatae futurae se sponte protegunt.
  Regeneratio libera (generatores per Bash scribunt).

Probati V casibus (nega/permitte/silens/flagra/praetermitte) per
JSON simulatum; VIVI a sessione proxima (unci ad initium sessionis
capiuntur). **silva-lsp = futurum nominatum** (visio-doc §XIV):
servus LSP silvae quattuor dilationes unit (daemon/cold-start,
vehiculum laneae censoriae, tractio incrementalitatis, superficies
editorum); post phasim + M3. NEXT: nexus (cum sculptura per datum
debrief confirmata).

### ADDENDUM (2026-07-10, nocte) — NEXUS NATUS, VECTIS X/X (4/5)

nexus_percursus.c (sceletum percursus duo-passus + corpus fusionis:
sedes symbolorum per indicem + usus per ambulationem
FOLIUM_IDENTIFICATOR super symbolum_nodi; sem2 post recanonicare —
tabula ligaminum canonicae-RELATIVA) → build/nexus.tsv
(DISPONIBILE per DECISUM, 45MB, 640,032 ordines, 769 plagulae,
133s) → nexus.c CLI (TSV purum sine silva, ~0.15s: greges per
plagulam, sedes primum, -omnia localia, -similis, fortasse).
Simplificatio M1a persoluta: vocati impliciti in folio vocati
ligant — brachium VOCATIO non necessarium.

**VECTIS X/X**: fasti_differentia (radius displosionis membri:
15 usus/2 plagulae statim); similitudo_optima (nexus SE IPSUM
indexavit — nexus.c horarum aetatis inter consumptores!); malloc
(quaestio auditus aedificatorum M2: 134 usus/29 plagulae);
chorda_aequalis_literis 332 semantici vs grep 414 crudi (delta =
commenta — praecisio semantica); MedullaLineae sedes typedef;
localia cum genere+profunditate; -similis; fortasse; CAUTIO
vetustatis. Inventa: viae capitum sub DUOBUS nominibus (basename
praebitum vs via plena) → ordines basename praetermissi, dedup
176k→3.4k. **DILATIO NOMINATA: usus typorum absunt** (typus-
nominatus extra tabulam ligaminum M1a; tractio = exportum
silva-side cum saltuarius type-jump rogat). Detail:
nexus.worklog.md. **NEXT: interpretare** (ultimum, ~1-1.5d) →
RELATIO phasis + debrief.

## INSTRUMENTA PRIMA — RELATIO PHASIS (2026-07-10, nocte)

**PHASIS CLAUSA. Quinque instrumenta + bibliotheca + duo unci +
praxis debrief — UNO DIE (aestimatio erat 4-4.5).** Omnes vectes
impleti; linea sectionis numquam necessaria; promotio origonis
(sim-2) validata.

Vectes, omnes impleti:
- **censor**: FP 0.00 / 762 plagulae primo contactu; porta 6 in
  AMBOBUS amalgamare.sh; corpus regressionis syntheticum (XIII
  ordines benedicti + fragores classis B).
- **emitte**: 10/10 symbola octetim identica extractioni sed;
  "fortasse" suggestiones (similitudo).
- **origo**: catenae plenae (profunditas 2, brachia omnia, canalis
  __FILE__/__LINE__ visibilis).
- **similitudo** (desideratum #1, titulus Franis): 28 asserta
  nativa ET interpretata die natali (unda cursoris 80→81).
- **nexus**: 10/10 probationes punctuales (640k ordines, 133s
  sweep, CLI ~0.15s; malloc 134 usus/29 plagulis).
- **interpretare**: sex responsa nota recta (b32/s32/f32; unda_sinus
  1.41421!); recusatio aggregatorum honesta; relatio halitus
  gratis; ~2s cum -plagulae (venatio sputnik erat ~10 min).
- **unci** (.claude/): censor-custos push ~0.9s silens-cum-purus +
  generata-custos DENY (signum-ductus); vivi a sessione proxima.

**Inventa vera phasi (lex contactus etiam pro instrumentis):**
git sine blobs prae-sanationum (corpus validationis syntheticum);
solum exire umquam SILENS fuit (classes A/B); valor lexematis =
textus EXPANSUS (radix quaerenda); tag-slots in usibus; inversio
directionis subsequentiae (did-you-mean); viae capitum sub duobus
nominibus; usus typorum extra tabulam ligaminum (dilatio nominata);
dilatare non extendere pro f32→f64 + LECTOR CLEMENS in paribus
suffixorum (duritia futura vilis); conventio %reditus in signatura
visibilis (recusatio aggregatorum).

**DEBRIEF PHASIS (praxis nova, exemplar plenum):**
- *adhibita*: emitte ~20× (12 vectis + 8 organici), nexus ~15×
  (10 vectis + 5 organici — instrumenta quaestiones FRATRIS SUI
  responderunt: sedes unda_sinus, signaturae fasti), censor 7×,
  origo 4×, similitudo (per emitte/nexus), interpretare 12×;
  grep/Read basis adhuc gravis in constructione ipsa.
- *fructus*: vectes ceperunt lacunas veras (radix-valor, tag-refs,
  dilatare); nexus quaestionem "in qua plagula" delevit; explorator
  superficialis censoris consilium melioravit (acta macrorum).
- *asperitates* (in worklogs ad momentum): initium frigidum ~1-2s
  per invocationem (acta/M4 respondet; silva-lsp futurum);
  fortasse nexus localia filtrata suggerit (confusio -omnia);
  lector clemens; ordo blocorum emitte.
- *desiderata*: similitudo NATA ET SOLUTA intra phasim (circulus
  desiderati ~2 horae); nulla alia nova.
- Auditus praedictionis sim-2 §5 ad clausuram M3 sigillatus manet
  (cave: numeri huius phasis maxima parte VALIDATIO, non demanda).

**AUDITUS**: INTENTIO omnia praestita; parcae nominatae cum
dominis (usus typorum → exportum silva cum type-jump; hex-dump
aggregatorum → in tractione; duritia lectoris → vilis futura;
sweep corporis censoris standing → decisio Franis, FP=0 mensurata;
silva-lsp → post M3). Sim docs (instrumenta-simulatio-1/2)
RELATIONE hac clausi — praedictiones formae C1-C5 omnes tenuerunt,
duo suppositi mortui (git-blobs; "afternoon" lint).

**NEXT: M3 — indicium + vindex.** Apertura: acies instructionum
planae (~1.3-2x super celer, datum census paratum). On-ramp
probatus: exploratores fundantes → simulatio → interrogatio →
spec. Instrumenta parata: origo pro gressu macro-conscio, emitte/
nexus pro constructione, interpretare pro verificatione morum,
census pro dispatch. Sigillum phasis = commissio Franis.

### ADDENDUM (2026-07-10, post RELATIONEM) — STRATUM PRAESENTATIONIS

Auditus praesentationis (rogatu Franis: quid agens recens videt?)
lacunam MEAM invenit: instrumenta nova in NEUTRO CLAUDE.md erant,
et tabula symptomatum (promissa in conversatione §XIV) numquam
scripta. SANATUM: (1) tabula "dolor → mandatum" in MEMORY.md
(superficies SEMPER onerata); (2) gradus 6 "Warm the bench" in
AMBOBUS protocollis re-introitus (silva + officina CLAUDE.md) —
idea Franis: singula instrumenta semel currere post compactionem,
mandata parata cum scopis notis-bonis; experientia in-contextu >
catalogus lectus; (3) sectio bench in Build/test utriusque.
NB: CAUTIO vetustatis nexus in calefactione ipsa docet -renovare
— pars rituali, non vitium.

## M3 — INDICIUM + VINDEX: ON-RAMP APERTUM (2026-07-11)

### GRADUS 1/4 — EXPLORATORES FUNDANTES (facti)

Quattuor exploratores paralleli (machinula / origo-catena / RDI /
consumptores vindicis); synthesis integra in
**project-specs/officina-m3-fundamenta.md** (SEDES RE-INTROITUS pro
M3 — lege ante simulationem/spec). Quinque facta portantia:

1. Provenientia MEMORIA-SOLA est (origo = SilvaNodus*; forma textus
   NIHIL scribit, lector NIHIL restituit) — indicium coquendum est
   tempore compilationis, numquam ex .medulla reficiendum.
2. Tabula linearum hodierna sedem SCRIPTURAE (definitio macronis)
   registrat, non sedem EXPANSIONIS — halt in credo.h monstrat, non
   in linea probationis. Ambae sedes in catena originis recuperabiles
   (silva_token_radix + profunditas + nomen_macro per gradum) — sed
   solum dum parsura vivit.
3. Fistulae linearum EXSISTUNT sed numquam datum verum portaverunt:
   interpretare arborem destruit sine colligere (interpretare.c:179);
   machinula_lineas_praebere semel tantum vocatur, cum NULL
   (hospes.c:444). Primus filus M3 = collige-ante-destrue + praebe.
4. Pausa VM vilis est: index instructionis in Tabulatum (structura,
   non locale) — machinula_gradus() = extractio corporis ansae;
   limites vocationum gratis recte gradiuntur.
5. Coordinata plana instructionum NON exsistit (scopi ramorum =
   indices bloccorum; 3× xar_obtinere per fetch; rami ~26% executionis
   bis solvunt). Congelatio plana in machinula_creare (passus 4us,
   IR IMMUTABILE manet) coordinatam creat quam ET velocitas ET
   breakpoints poscunt.

Instincta RDI adoptata (directorium sectionum per genus-indicem,
chorda-in-disco, tabulae linearum binae ordinatae, sedes macronum =
introitus consecutivi eodem indice — exemplar inline-site eorum) et
complexitas praetermissa (bytecode locorum, ingestus PDB/DWARF,
compressio) in fundamenta §V. Decem quaestiones apertae pro
interrogatione in §VI (unitas coctionis per-TU vs per-mundum; quis
scribit quando; mensa variabilium v0; typi; profunditas aciei
macronum; TITULUS = caerimonia Franis; sedes vindicis; UX gradus;
ordo aperturae; hex-dump aggregatorum).

NEXT: gradus 2/4 = SIMULATIO (contra codicem veram, more sim-3/4);
tum interrogatio; tum spec. Correctiones parvae notatae (fundamenta
§VII): commentum machinula.h:14 "registra" mendax; anulus 64KB non
128KB; census in officina/build/census/.

### GRADUS 2/4 — SIMULATIO ⑤ (facta, 2026-07-11)

**project-specs/officina-simulatio-5.md** — C1-C13 schemate quadripartito.
NULLA reversio; apertura (congelatio plana) CONFIRMATA et promota:
structura quae breakpoints possibilitat (C11 — patching in exemplari
congelato LICITUM, quia exemplar machinulae proprium est, IR intactum),
non solum velocitas. Inventa maiora: C5 pactum ambulationis canonicae
(indices demissione cocti == indices congelatione computati — probatio
figenda, non suppositio); C7 inscriptio per-functio voff globalem RDI
NECAT (VM functio numquam obliviscitur — UnitVMap/ScopeVMap/renumeratio
omnia supervacua; coctio per-modulum fit possibilis); C9 extenta
variabilium in v0 DISSOLVUNTUR (demissio omnia localia in introitu
collocat — "omnia registra tabulati" VERITAS est, non approximatio).
Duae quaestiones §VI dissolutae (acies macronum sine fine; extenta),
duae novae natae (Q11 sententia-op pausae = caerimonia; Q12 pretium
gradus() sub -O0 = mensurandum). Ordo chunkorum: ① congelatio →
② lineae+distillator → ③ modulus indicii → ④ pausa/gradus/breakpoint →
⑤ probatio-vectis sine TUI → ⑥ TUI (exemplar saltuarii).
NEXT: gradus 3/4 = INTERROGATIO Franis (quaestiones in simulatio-5
"Effect on the interview" — Q1/Q2/Q4/Q6/Q7/Q8/Q11/Q12 vivae).

### GRADUS 3/4 + 4/4 — INTERROGATIO + SPEC (facti, 2026-07-11)

Interrogatio: XII quaestiones, tres circuli —
**project-specs/officina-m3-interrogatio.md** (transcriptum crudum
ante spec, more domus). DECISUS maiora: **TITULUS SIGILLATUS =
indicium** (.indicium; magica capitis "INDICIUM"); coctio per-MUNDUM
v0 (per-modulum = futurum nominatum); productores = vindex solum
(coque-scribe-RECLUDE); typi v0 = chorda typi + tag medullae; sedes
vindicis = instrumenta/principalia; superficies v0 = nucleus + fini
+ tabula memoriae + tabula anuli + puncta per-functionem; sententia
pausae = MACHINULA_OP_PAUSA machinula-privata (medulla.h +
vocabularium INTACTA); UX macronum = monstratio-sola (acies in linea
positionis, gressus per lineam RADICIS); **lineae in memoria =
SEMPER-VIVAE in omnibus consumptoribus** (relationes halitus ubique
[via:linea] radicis lucrantur); sigillum M3 = probatio-vectis SINE
capite (formalis) + TUI cotidie-agibilis; sigillum congelationis =
paritas + mensura-et-nota (sine limine); lingua imperiorum = claves
+ linea ':' latina.

Spec: **project-specs/officina-m3-spec.md** — sex chunki, quisque
sigillabilis: ① congelatio plana (+ PACTUM AMBULATIONIS CANONICAE
primum — probatio figens indices demissionis == congelationis) →
② lineae ubique (distillator radicis + filum numquam-exercitum) →
③ indicium (sectiones, X-macro schema, scriptor/lector, circuitus
probatione) → ④ pausa/gradus/puncta (patching exemplaris congelati;
mensura gradus sub -O0) → ⑤ VECTIS: probatio sine capite per
plagulam SOLAM → ⑥ TUI (exemplar saltuarii; pingere per snapshot,
numquam Machinula* viva). Dilationes nominatae cum dominis in §IX.

**ON-RAMP COMPLETUS (4/4 uno die).** NEXT: recensio Franis + commissio
(sigillum on-rampi), tum chunk ① congelatio plana.

### CHUNK ① — CONGELATIO PLANA: INTENTIO (2026-07-11)

Sigillum on-rampi commissum (Fran). Aperitur chunk ① per spec §III:
FunctioPlana (instructiones contiguae + blocci_initia + operanda
plana) ut passus 4us machinulae_creare POST ligationem decipularum;
planae[] parallela tabulae functionum conexionis (index quem VOCARE
iam solvit); Tabulatum.bloccus DELETUR (flat index unicus; bloccus
derivatur quaestione binaria ad tempus imprimendi solum); anulus fert
plana + indicem planum; cache tabulatum_summum (monstratores Xar
stabiles). PACTUM AMBULATIONIS CANONICAE in medulla.h; probatio
trans-phasica figens (indices demissionis == congelationis) DIFFERTUR
ad chunk ③ ubi latus indicii exsistit — congelatio sola nihil habet
quocum comparet; auto-constantia (blocci_initia stricte crescentia)
probatur hic. INVENTUM praevium: medulla_functio_terminata numquam
vocatur (lector NON validat) → blocci non-terminati servant
honestatem hodiernam per CUSTODEM machinula-privatum
(MACHINULA_OP_FLUXUS_CUSTOS = MEDULLA_OP_NUMERUS, sedes privatae
post NUMERUS in acie numeratorum) appensum solis bloccis
non-terminatis — corpus (semper terminatum) manet 1:1 cum ambulatione
canonica. Vectis: paritas sweep (stdout octetim) + suites virides +
mensurae notatae (vim ANTE: 550ms/9,075,028 instr in -sola; sweep
ANTE in cursu). NULLA superficies publica nova (hospes intactus).

### CHUNK ① — CONGELATIO PLANA: RELATIO (2026-07-11)

**CLAUSUS, primo transitu purus sub muro vexillorum. VECTIS IMPLETUS
ET SUPERATUS: vim 550 → 171 ms (3.2x; aestimatio fundata erat
1.3-2x) @ 9,075,028 instructionibus identicis; paritas TOTA (69
eaedem / 0 DIFFERUNT); arbor_formator e sinu TEMPUS EVASIT (>30s →
15.7s, 673M instr, idem) — sweep 75→76 praeteritae, tempora 6→5,
murus 4:24 → 3:46.** Aestimatio superata quia TRES strata xar simul
mortua: fetch instructionis (3 vocationes → 0) + re-fetch tabulati
summi (cache, monstratores Xar stabiles) + stiva operandorum VOCARE.
Pretium congelationis invisibile (suites parvae NET celeriores: xml
20→7ms). Amalgama VERIFICATUM (hospes 61/61; xar_copiare_ad_tabulam
ex EXCLUDENDA deletum — porta standalone clamavit sicut documentum).

Inventa (detail: officina_machinula.worklog.md 2026-07-11):
custos fluxus pro bloccis non-terminatis (lector numquam validat —
inventum INTENTIONIS); scopi SALIRE/RAMUS nunc explicite probati
(honestas quae gratis veniebat fit explicita); numerus
instructionum sputnik_interpres inter cursus movetur (−2,740, via
horologii — stdout manet vectis, non numerus). Probatio figens
trans-phasica → chunk ③ (latus indicii deest adhuc).

DEBRIEF INSTRUMENTORUM: *adhibita* — nexus 1× + emitte 1× + origo
1× (calefactio ad scopum verum: machinula_currere), grep/Read
gravia in constructione, diagnostica clangd VIVA per uncos
(progressum editionum meum vestigavit — sedes 'bloccus' reliquae
enumeratae post quamque seriem); censor per uncum SILENS (purus).
*fructus* — calefactio emitte dedit lectionem ansae dispensationis
ANTE laborem; clangd-push = lista laborum viva. *asperitates* —
nullae novae. *desiderata* — nulla nova.

**NEXT: commissio Franis (sigillum chunk ①) → chunk ② lineae
ubique** (distillator radicis silva_token_radix in _lineam_colligere
+ filum collige-ante-destrue in interpretare/cursor +
machinula_lineas_praebere semper). Praemium visibile: relationes
halitus cum [via:linea] radicis UBIQUE.

### CHUNK ② — LINEAE UBIQUE: INTENTIO (2026-07-11)

Per spec §IV. Tria: (1) DISTILLATOR RADICIS — _lineam_colligere
(officina_demissio.c) ambulat silva_token_radix (iam in amalgama,
silva.h:254) ad sedem INVOCATIONIS pro sede scripturae; halt in
brachio CREDO monstrabit lineam plagulae probationis, non credo.h.
(2) FILUM COLLIGE-ANTE-DESTRUE — _plagulam_demittere in AMBOBUS
(interpretare.c + cursor.c, exemplaria separata — refactor oneris
communis = quaestio chunk ⑤) parametrum exitus MedullaLineae**
accipit, collectio ante silva_piscina_destruere; Xar lineae_mundi
parallelus moduli_mundi; suita = index post mundum. (3) PRAEBERE
SEMPER (DECISUS 9) — post machinula_creare, quisque modulus suas
lineas accipit. Vectis: relatio halitus VITII coacti monstrat
[via:linea] RADICIS per cursor ET interpretare (probatio effimera,
delenda post demonstrationem); paritas sweep immutata; suite +
amalgama VERIFICATUM. Pretium collectionis (semel per TU, ~ms)
notandum in mensuris sweep.

### CHUNK ② — LINEAE UBIQUE: RELATIO (2026-07-11)

**CLAUSUS. VECTIS IMPLETUS AMBOBUS CONSUMPTORIBUS:** probatio
effimera (lectio insana in CORPORE macronis) rettulit
[probatio_m3_lineae_scratch.c:23] — lineam INVOCATIONIS, non :11
definitionis (distillator radicis probatus); cauda anuli
credo_aperire per lib/credo.c:73-76 lineatim ambulavit; interpretare
(piscina_allocare(42,16)) stivam positionatam plenam dedit
(piscina.c:141 ← :348 ← main nudum honeste — involucrum textuale).
**Primum datum verum per machinula_lineas_praebere UMQUAM** — filum
numquam-exercitum nunc in omni cursu cursoris/interpretare vivit
(DECISUS 9). Paritas: 76 praeteritae / 69 eaedem / 0 DIFFERUNT / 5
tempora — figura post-congelationem exacta. **Pretium semper-vivum:
+0.7s muri sweep TOTI** (115 moduli mundi + 81 suitae; 3:45.93 →
3:46.64) — intra strepitum. Suite 6/6; amalgama VERIFICATUM.
Probatio effimera DELETA. Detail: officina_demissio.worklog.md +
interpretare.worklog.md (gemini consulto — refactor oneratoris
communis = quaestio nominata chunk ⑤).

DEBRIEF INSTRUMENTORUM: *adhibita* — grep/sed lectio gravis
(regiones ligationis); diagnostica clangd per uncos ITERUM viva
(unum verum: signatura credo_aperire in probatione effimera capta
ante compilationem!); censor silens. *fructus* — clangd in plagulis
probationum (viae inclusionis verae) vitium ante muri vexillorum
cepit. *asperitates/desiderata* — nullae novae.

**NEXT: commissio Franis (sigillum chunk ②) → chunk ③ INDICIUM**
(modulus officina_indicium.{c,h}: scriptor demissione + lector
plana; sectiones per X-macro; acies expansionis PLENA in tabulam
prorsam; tabula retro per lineas RADICIS; VARIABILIA; probatio
circuitus + PROBATIO FIGENS trans-phasica pacti ambulationis).

### CHUNK ③ — INDICIUM: INTENTIO (2026-07-11)

Sigilla ①+② commissa (Fran). Recognitio pre-constructionis probavit:
NIHIL silva-latus mutandum — unio SilvaOrigo TOTA publica in
amalgamate (silva.h:192-215: expansio.invocatio/pasta.sinister/
stringificatio.primus + nomen_macro per brachium — ambulatio aciei
plenae officina-latere possibilis); SemanticaSymbolum.typus +
TypusC89 plene ambulabilis (redditor typi parvus officina-latere
scribendus — typi RESOLUTI monstrantur: "unsigned int" non "i32",
honestum, notandum); SilvaNodus.pater publicus (probatio ancestriae
pro variabilibus). DECISA constructionis: (a) emissio ordine
CONEXIONIS — scriptor colligit per-TU clavibus monstratorum
functionum (exemplar clavium MedullaLineae), scribere post nexum
iungit → functio_idx indicii == index functionis conexionis ==
index planae (nulla translatio ad tempus cursus); (b) INDEX_CURSUS
OMISSUS v0 — omnis lista variabilis unum solum consumptorem habet,
cursus in sectionibus typatis inline vivunt (piscina communis =
dilatio cum duo consumptores); (c) mensa variabilium tempore
COLLECTIONIS (registra nominata × symbola semanticae, probatio
ancestriae per pater; umbrae x_37 sine typo = lacuna honesta
interrogatione benedicta; registratio tempore demissionis = ascensus
nominatus); (d) probatio figens = numerus instructionum per functio
(indicium ex ambulatione collectionis == plana machinulae ex
ambulatione congelationis; ordo iam vinctus per paritatem sweep —
scopi ramorum falsi = strages corporis) per ordinem publicum novum
machinula_numerus_instructionum_planarum (+ hospes). Vectis (spec
§V): probatio circuitus — plagula scripta, reclusa, quaesita
(prorsum acervata cum macro/retro/variabilia/asserta magnitudinum
X-macro) + determinismus (bis scripta, memcmp aequalis). Sectiones:
CHORDAE_DATA/OFFSETA (chorda-in-disco), VIAE, MODULI, FUNCTIONES,
LINEAE_INDICES (+introitus claudens per functio), LINEAE_RES
{via,linea,profunditas,nomen_macro}, RETRO_LINEAE/INTERVALLA/SITUS,
VARIABILIA {titulus,registrum,typus_medulla,typus_scriptus}. Magica
"INDICIUM" 8 octeti, versio dure reiecta, endian hospitis, sectiones
8-ordinatae, indices u32 ubique.

### CHUNK ③ — INDICIUM: RELATIO (2026-07-11)

**CLAUSUS. Probatio circuitus 51/51** (acies macronum acervata:
linea 15 radix + linea 4 GEMINARE profunditate 1; retro; variabilia
typata "int"/"unsigned int"/"char*" cum tags medullae; determinismus
bis-scriptus memcmp; magica corrupta reiecta; asserta magnitudinum
numerica) **+ PROBATIO FIGENS pacti ambulationis VIVA BIS** (probatio
+ hospes "indicium pactum ambulationis" — ambulatio collectionis ==
congelatio machinulae per ordinem publicum novum
machinula_numerus_instructionum_planarum). Suite 7/7; hospes 61→73
fideles (XII ordines novi); amalgama VERIFICATUM; paritas sweep
EXACTA (76/69/0 DIFFERUNT/5 tempora).

**INVENTUM CAPITALE: MedullaFunctio.origo NUMQUAM assignatum** —
locellus provenientiae designatus, vacuus ab M1; omnes VI fracturae
primi cursus (congruentiae variabilium semanticae omnes) UNAM
causam habuerunt (probatio ancestriae ad scopum NIHIL). Sanatio
una linea in _functionem; _lineam_colligere quoque meliorescit.
Cursus primus: 45/51 — fistula tota (acies/retro/determinismus/
pactum) recta primo ictu.

Firmationes latinae II novae: magnitudo + registrum ut CAMPI
structurarum in .h (clangd-push + censor suspecta ANTE compilationem
ceperunt; campi identificatores quoque sunt) + `nomen` classicum in
probatione. Lectiones amalgamatoris II novae: statica duplicata
trans TU concatenatum (_ind_ praefixum IV); **campus 'chorda'
VETITUS in fontibus amalgamatis** (renominatio typorum per signa
campum rescripsit — spatium nominum camporum = spatium signorum).
Deltae consilii (in INTENTIONE notatae): INDEX_CURSUS omissus v0;
X-macro → exemplar domus (superficies manu speculatae + asserta
numerica). Detail: officina_indicium.worklog.md (natus).

DEBRIEF INSTRUMENTORUM: *adhibita* — recognitio per grep/sed gravis
(silva.h superficies, exempla probationum); emitte 0/nexus 0 hoc
chunk (quaestiones erant "quid superficies publica continet" — grep
in capite amalgamatis directior); diagnostica clangd + censor
suspecta = trio firmationum latinarum statim captarum; PORTAE
amalgamatoris = co-fabricator (III captiones distinctae trans
chunks: excludenda ①, statica duplicata ③, campus chorda ③).
*fructus* — porta standalone erroris cuiusque genus recte nominavit.
*asperitates* — nullae novae in subsellio. *desiderata* — selecta
(parcata) iterum desiderata semel ("quae functiones officina.h
declarat" = quaestio structurae).

**NEXT: commissio Franis (sigillum chunk ③) → chunk ④
pausa/gradus/puncta** (MACHINULA_PAUSA halitus; machinula_gradus +
machinula_pergere extractione corporis ansae; puncta per patching
exemplaris congelati MACHINULA_OP_PAUSA = NUMERUS+1; mensura
oneris gradus sub -O0 = Q12).

### CHUNK ④ — PAUSA/GRADUS/PUNCTA: INTENTIO (2026-07-11)

Superficies VM pro vindice (spec §VI): (1) genus halitus novum
MACHINULA_PAUSA — resumabile, sine relatione, positio intacta
(pausa ANTE exsecutionem instructionis). (2) Extractio corporis
ansae: machinula_gradus (instructio UNA, b32 = pergendum) +
machinula_aperire (status + tabulatum primum, ex capite currere) +
machinula_pergere (usque ad halitum/pausam; resumptio ex pausa =
restitue-grade-repone punctum sub cursore); machinula_currere =
involucrum compatibile EXACTUM (aperire + pergere + relatio +
exitus). (3) Puncta per PATCHING exemplaris congelati (sim C11;
DECISUS Q11 machinula-privatum): MACHINULA_OP_PAUSA =
MEDULLA_OP_NUMERUS+1 (sedes numeratoris iam reservata ①);
FunctioPlana.instructiones fit non-constans (exemplar machinulae
proprium — mutabilitas est punctum); punctum_ponere/tollere per
(index functionis conexionis, index planus). (4) Inspectio minima
pro vecte ⑤: tabulata_numerus + positionem_inspicere (index
functionis derivatur t->plana - m->planae, SINE campo novo) +
registrum_legere. VIII ordines publici novi → hospes + officina.h.
Vectis: probatio nova (fixtura .medulla textualis — puncta per
indices, lineae non necessariae): pergere sine punctis == currere;
punctum tactum → PAUSA positione recta; registrum lectum in pausa;
pergere iterum → BENE 42 idem; tollere; gradus-singuli numerati.
MENSURA Q12: vim solo ante (171ms) / post — pretium vocationis
gradus sub -O0; postura: structura vincit, fallback nominatum =
ansa interna pergere.

### CHUNK ④ — PAUSA/GRADUS/PUNCTA: RELATIO (2026-07-11)

**CLAUSUS. Probatio 66/66 PRIMO CURSU** (fundamentum currere 42;
gradus singuli numerati == numerus instructionum cursus == 42 idem;
punctum medio-main → PAUSA indice plano exacto ANTE exsecutionem,
%t1 == 21 lectum in pausa, resumptio → 42 idem; punctum instructione
0 → nihil exsecutum; punctum in VOCATO → profunditas 2, %a == 20 in
tabulato vocati, imum = main; punctum per iter DESCRIPTORIS (vocare
%f); tollere + tollere iteratum; fines insani reiecti). Extractio
corporis ansae primo transitu pura (clausura blocci indentationem
servavit honeste). Suite 8/8; hospes 73→77 (aperire/gradus/positio/
puncta+resumptio); amalgama VERIFICATUM (excludenda clamavit iterum:
xar_removere_cum_ultimo — firmatio secunda portae); paritas sweep
EXACTA (76/69/0/5).

**Q12 MENSURATUM (incognitum nominatum chunki):** vim 171 → 190 ms
(+11% — vocatio non-inlineata per instructionem sub -O0; NET contra
fundamentum prae-M3 adhuc 2.9x: 550→190); murus sweep +1.6% solum
(3:46.64 → 3:50.20 — dispensatio pars minor temporis sweep).
ACCEPTUM per posturam interrogationis: corpus UNUM sine
duplicatione; aedificationes amalgamatis/-celer gradus in pergere
inlineant (TU eadem) → cursus benedictionis nihil solvunt; fallback
manet nominatum (ansa interna pergere) si labor gressuum umquam
dolet. IX ordines publici novi (aperire/gradus/pergere/punctum
ponere+tollere/halitus_codex/tabulata_numerus/positionem_inspicere/
registrum_legere) — index functionis derivatur t->plana - m->planae
SINE campo novo. Detail: officina_machinula.worklog.md.

**NEXT: commissio Franis (sigillum chunk ④) → chunk ⑤ VECTIS
FORMALIS M3** (probatio sine capite: mundus probatio_piscina per
viam oneris interpretare, indicium coctum-scriptum-RECLUSUM, punctum
per lineam RETRO, pergere → pausa, inspectio per NOMEN FONTIS e
VARIABILIA, gradus-super-lineam cum custode profunditatis, fini,
perge → BENE stdout intacto — OMNIA per plagulam solam).

### CHUNK ⑤ — VECTIS FORMALIS M3: INTENTIO (2026-07-11)

Sigillum ④ commissum. probatio_officina_vindex.c = SIGILLUM FORMALE
M3 (spec §VII; DECISUS 10): onerator proprius (exscriptum
interpretare — TRES gemini nunc, refactor oneratoris communis
PARCATUM nominatim, tractio viva), mundus scopo dato {piscina,
chorda, chorda_aedificator, credo} + probationes/probatio_piscina.c;
collectio indicii IN oneratore (disciplina C12: coque in via
oneris); scribe → RECLUDE → ab eo puncto LECTOR SOLUS. Phases:
(1) punctum per RETRO ("probationes/probatio_piscina.c", linea 37 —
linea CREDO!) → pergere → PAUSA situ exacto; ACIES MACRONUM in situ
pausae ≥2 (radix 37 prof. 0 + framea CREDO_AEQUALIS_I32 → credo.h)
= "macro-aware" litteris vectis; variabile "usus" per NOMEN FONTIS
e VARIABILIA → registrum_legere == 0; pactum ambulationis pro main.
(2) gradus-super-lineam (radix mutatur ∧ profunditas ≤ introitus) →
linea 39 (piscina_destruere). (3) punctum trans-modulum
(lib/piscina.c:407 in piscina_summa_usus) → pergere → PAUSA
profunditate 2 (vocatio lineae 54), imum = main; FINI (gradus dum
profunditas < 2) → summum iterum main. (4) tollere ambo → pergere →
BENE codex 0 (probatio_piscina interpretata perfecta stdout
fluente), summa instructionum > XL milia (testis "probationis
verae"). Effusio interior fluit in stdout probationis (exemplar
cursor -sola) — vectis octetim maneat cursoris officium.

### CHUNK ⑤ — VECTIS FORMALIS M3: RELATIO (2026-07-11)

**CLAUSUS — VECTIS M3 IMPLETUS. probatio_officina_vindex 69/69,
quattuor phases omnes; probatio_piscina intra eam INTERPRETATA
perfecta (60/60 OMNIA PRAETERIERUNT, effusio fluens, codex 0,
>40k instructiones) — punctum/gradus/inspectio PER PLAGULAM
INDICII SOLAM.** Linea CREDO 37 per RETRO → PAUSA situ exacto cum
ACIE MACRONUM (radix :37 + framea CREDO_AEQUALIS_I32 — clausula
"macro-aware" vectis litteris demonstrata in probatione vera);
"usus" per NOMEN FONTIS → 0; gradus-super-lineam → :39; punctum
per-functionem TRANS-MODULUM + FINI; pactum ambulationis iterum.
Suite 9/9; hospes 77/77; amalgama VERIFICATUM; paritas sweep
EXACTA (76/69/0/5).

**VECTIS DUO INVENTA VERA CEPIT (ambo sanata eodem die):**
(1) machinula_gradus ex VM pausata FALSUM reddidit — chorea
resumptionis in pergere sola vivebat, sed GRESSUS ex pausa panis
debuggeris est; sedes resumptionis IN GRADUM mota (pergere =
`dum (gradus)` nudum — sedes una, consumptores ambo). (2) nullus
accessor publicus IndiciumLinea.via → chorda solvebat (index in
VIAE, non in tabulam chordarum — impressio debug 'unsigned long'
pro via monstravit!); indicium_via_chorda natus (fontes + vanilla
+ ordo hospitis). Probatio chunki ③ viam tabulae prorsae numquam
solvit — VECTIS consumptor primus verus campi fuit. Vectes
inveniunt quod validatio non invenit (lex e phasi instrumentorum
iterum confirmata).

Onerator = geminus TERTIUS (interpretare/cursor/vectis) —
**tractio refactoris oneratoris communis VIVA (tres exemplaria);
nominata pro chunk ⑥** (TUI eundem oneratorem poscit — extrahe
TUNC, consumptore quarto vero in mensa). DEBRIEF: *adhibita* —
diagnostica clangd (trio in probatione ipsa capta ante murum);
grep/sed pro recognitione; instrumenta subsellii 0 hoc chunk
(quaestiones = superficies propriae recentes). *desiderata* —
nulla nova. Detail: officina_indicium.worklog.md.

**M3 STATUS POST ⑤: VECTIS FORMALIS SIGILLATUS (DECISUS 10 pars
prima). Restat chunk ⑥ = vindex TUI (cotidie-agibilis, polituram
non obstante) → tum RELATIO MILLIARII M3 plena + auditus
praedictionis sim-2 §5.**

**NEXT: commissio Franis (sigillum chunk ⑤) → chunk ⑥ VINDEX TUI**
(officina/instrumenta/principalia/vindex.c + vindex.sh; exemplar
saltuarii: pingere per snapshot numquam Machinula* viva; tabulae
fons/registra/stiva/memoria/anulus/puncta; claves + linea ':'
latina; probationes cellularum sine capite per tessera_pons_
memoriae; refactor oneratoris communis HIC).

### CHUNK ⑥ — VINDEX TUI: INTENTIO (2026-07-11)

Sigillum ⑤ commissum. VINDEX = app super amalgamata TRIA (officina
+ silva + tessera; exemplar aedificationis saltuarii — plagulae
fontium amalgamatorum CONSULTO absentes, limes amalgamatis = API),
sedes = instrumenta officinae (DECISUS 5). Partes:
(1) **officina/instrumenta/vindex_onerator.{c,h}** — EXTRACTIO
oneratoris (consumptor 4us): systema+capita+demittere(lineae+
indicium)+nectere super superficies PUBLICAS (officina.h+silva.h).
NOTA ARCHITECTURAE: familiae oneratorum DUAE sunt — fontes-latus
(cursor/interpretare/vectis, typi domus) et app-latus (vindex nunc,
REPL M4 mox); extractio app-latus catenam geminorum novam praevenit;
gemini fontes-lateris migrant tactu proximo (dilatio nominata).
(2) **officina/instrumenta/vindex_visum.{c,h}** — CONSPECTUS
(snapshot purum: lineae fontis, positio+acies macronum, tabulata,
variabilia, anulus, puncta, memoria, modus/imperium) + pingere
super tessera.h — NUMQUAM Machinula*/lector (sutura saltuarii;
probationes cellularum sine capite). (3) **vindex.c** (principalia)
— onerator → machinula → indicium coque-scribe-RECLUDE (positio
omnis per LECTOREM, numquam per lineas memoriae — forma-primum
visibiliter); ansa eventuum; auxilia gressuum = ansae vectis ⑤
(radix-mutatur ∧ custos profunditatis). Claves: g gradus / s super
/ i intra / f fini / p perge / v-a-u-m tabula dextra inferior /
q exi / ':' linea imperii latina (:punctum via:linea aut functio,
:dele, :memoria hex, :curre). (4) **officina/vindex.sh** + modus
**-imago <via>:<linea>**: pons memoriae, punctum, pergere, UNA
effigies picta → stdout ut textus (fumus sine terminale — pars
verificationis meae; sessio viva = pars Franis). (5) Ordo publicus
NOVUS machinula_anulum_inspicere (tabula anuli; + vanilla + hospes).
(6) compile_probationes.sh dilatatur (amalgama tesserae + obiectum
visus) pro probatio_officina_vindex_visum (cellulae fixturae).
Vectis ⑥ (DECISUS 10): probationes cellularum virides + fumus
-imago + SESSIO VERA FRANIS (cotidie-agibilis; politura non
obstat). Post ⑥: RELATIO MILLIARII M3 plena + auditus sim-2 §5.

### CHUNK ⑥ — VINDEX TUI: RELATIO (latus meum, 2026-07-11)

**CONSTRUCTUM ET FUMATUM. Probatio cellularum 24/24 PRIMO CURSU;
fumus -imago DUPLEX vivus:** (1) probatio_piscina.c:37 — TUI
plena picta: signa */> linea 37, STIVA, VARIABILIA cum valoribus
VIVIS tabulati pausati per plagulam ("usus unsigned long 0";
"piscina struct Piscina*" cum monstratore regionis vero; p_1..p_N
umbrae honeste sine typo — lacuna benedicta, visibiliter strepitosa:
filtrum = candidatus v0.1, arbitrium Franis); linea positionis cum
acie macronum. (2) piscina_summa_usus (punctum per-functionem):
pausa TRANS-MODULUM, tabula fontis sponte ad lib/piscina.c mutata,
stiva bi-tabulata posita. Suite 10/10; hospes 78/78 (ordo novus
machinula_anulum_inspicere — anulus erat privatus, tabula anuli
consumptor externus primus); amalgama VERIFICATUM; paritas sweep
EXACTA (76/69/0/5).

Partes: vindex_onerator.{c,h} (EXTRACTIO oneratoris app-lateris —
familiae DUAE notatae: fontes-latus exemplaria tenet, migratio
tactu proximo; REPL M4 = consumptor app-lateris secundus),
vindex_visum.{c,h} (conspectus purus + pictura — sutura saltuarii
honorata, probationes cellularum id probant), vindex.c (positio
OMNIS per LECTOREM — numquam lineae memoriae: forma-primum
visibiliter), vindex.sh (amalgamata TRIA; officina.c cum
-Isilva/amalgama — dependentia externa unica documentata),
compile_probationes.sh dilatatum (tessera + visum obiecta stantia).
Inventum notatum: acies positionis "…:37 per CREDO <- …:37" (via
eadem bis) cum instructio pausata argumentum macronis aestimat —
rectum, non vitium (corpus macronis credo.h monstrat); in worklog
ne mirum videatur. Detail: vindex.worklog.md (natus).

**RESTAT (vectis ⑥ pars Franis): SESSIO VIVA cotidie-agibilis**
(`./officina/vindex.sh probationes/probatio_piscina.c -plagulae
piscina,chorda,credo,friatio` in terminale vero; politura non
obstat — asperitates in worklog notandae). Post sessionem +
commissionem: RELATIO MILLIARII M3 plena + auditus praedictionis
sim-2 §5 (numeri organici nunc exsistunt).

### CHUNK ⑥ — SESSIO VIVA FRANIS: VECTIS IMPLETUS (2026-07-11)

**Fran vindicem VIVUM egit — duo inventa vera, ambo eodem die
sanata, tum "working perfectly":** (1) linea positionis iterata
(framea argumenti macronis eadem via:linea — compressio in strato
picturae, forma intacta); (2) **PHANTASMATA VOLUMINIS** (cum
imagine photographica!): printf mundi interpretati in TERMINALE
IPSUM scribebat — volumen physicum quod diff tesserae nescit;
lineae duplicatae accumulatae quoque gressu super lineam
imprimentem. Sanatio: captura fd 1+2 circum exsecutionem VM
(_vm_capere/_vm_solvere, dup2 → officina/build/vindex.stdout;
stderr quoque — relationes halitus eodem via; -imago consulto
sine captura). Tessera ipsa INNOCENS probata (purgare + diff
recte spatia emittunt — solae scripturae externae in tty
phantasmant). Vectis cotidie-agibilis: probationes cellularum
virides + fumus -imago + sessio vera + inventa-sanata-iterum-acta.
**CHUNK ⑥ CLAUSUS. Sex ex sex.**

**NEXT: commissio Franis (sigillum ⑥) → RELATIO MILLIARII M3**
(relectio phase-log TOTA + auditus regulae milliarii; auditus
praedictionis sim-2 §5 cum numeris organicis; parcae omnes cum
dominis; sigillum M3 = commissio + RELATIO).

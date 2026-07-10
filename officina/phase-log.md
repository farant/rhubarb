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

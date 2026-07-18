# Silva fluxus gradus 1 — initialisatio dataflow (spec v1)

Date: 2026-07-17. Sources: calibration battery (17 specimens, 4
rounds, Apple clang 16.0.0, scratchpad calibratio-fluxus-1/),
simulatio against shipped FLUXUS-0, interview
(silva-fluxus-1-interview.md). Park with banked ledgers:
01KXSETFAPHYG42S1K5QQC722R. Predecessor spec:
silva-fluxus-0-spec.md.

Deliverable: three new codices judging uninitialized reads of
automatic locals — **71 LECTIO_ININITIATA** (oracle-paired
-Wuninitialized, SUSPECTUM), **72 ININITIATA_QUANDOCUMQUE**
(oracle-paired -Wsometimes-uninitialized, SUSPECTUM), **73
ININITIATA_FORSITAN** (dormant, census-only — clang's off-by-default
-Wconditional-uninitialized class) — plus the CFG expression-edge
surgery and the retained dataflow tables that the codex-62 re-arm
(decretum 01KXRBS0VQ) and solarium queries consume later.

## §1 The oracle model (calibrated, not assumed)

Clang runs ONE analysis with three diagnostic classes. The class
boundary is what claim the compiler can make:

1. **Definite** (`-Wuninitialized`, "variable 'x' is uninitialized
   when used here"): no init reaches the use on ANY path to it. The
   use itself may be conditionally reached (a read in one ternary arm
   of a never-initialized variable is DEFINITE). Anchor = use line.
2. **Sometimes** (`-Wsometimes-uninitialized`, "is used uninitialized
   whenever X"): ONE branch edge deterministically leads to the
   uninit use. The "whenever" is LITERAL — no exceptions. Anchor =
   condition line; use site is a note. Pinned message forms: 'if'
   condition is false / is true; '&&' condition is false; '||'
   condition is true; 'while' / 'for' loop exits because its
   condition is false (only when a break makes the claim exact — a
   pure back-edge loop is SILENT); switch default is taken (only
   when a default LABEL exists).
3. **Residual** (`-Wconditional-uninitialized`, "may be
   uninitialized") — OFF by default, outside -Wall: multi-branch
   conjunctions (correlated guards INCLUDED — clang computes no
   correlation; the silence falls out of attribution being
   impossible), zero-trip loops, first-iteration reads, unlabeled
   switch fall-past. Arming: -Wuninitialized is the master switch.

Consequence: a corpus clean under -Wall -Werror is a free negative
oracle for classes 1+2 EXACTLY — the parity scope. Class 3 is not
covered by corpus cleanliness (codex 73 stays dormant).

## §2 Calibration pin table (binding on the predicate)

- **Address-taken = a flow-positioned def EVENT**, not a variable
  property: a read BEFORE `&x` fires; `&x` under folded `si (0)` does
  not count (block unreachable); `&x` in a live conditional branch =
  sometimes; `&x` to a **const T\* parameter does NOT suppress**
  (qualifier-aware — v1 implements the call-site param lookup;
  unknown callee / variadic / K&R = assume non-const).
- **Aggregates**: member/element READS are never events (invisible
  both directions); member/element STORES = **may-def of the whole
  variable** (derived from the sigillum corpus datum: element-wise
  filled struct classed residual, not definite); whole-value uses
  (pass/return/assign whole struct) are uses. Builder-pattern
  structs stay silent under parity.
- statics: silent (zero-init) — excluded from tracking. Params:
  initialized at entry. Globals: excluded. volatile: tracked
  normally (clang fires). register: normal.
- `(void)x` is NOT a use. `magnitudo(x)` subtree skipped
  (unevaluated) — pin with a new specimen in-arc. `int x = x` =
  own message form, same codex 71. `*p = 1` reads p.
- Evaluation-order rules: assignment walks RHS before LHS-def;
  `+=`/`++` emit use-then-def; array subscript reads the index, not
  the array; `&` operand is not a read; declarators left-to-right
  (`int a = 1, b = a;` orders correctly).
- ERROR/AMBIGUUS opaque leaves = **def-all** (could hide anything;
  the abstention flavor, per-function).
- Alienity funnel seat inherited: DOMESTICUM judgments never judge
  alien code. TU degradation: reuse the 69/70 venom/abstention
  pattern where types are unknown.

## §3 Chunk 0 — CFG surgery (expression edges)

Split-at-short-circuit, ternary included (interview Q5/Q6): blocks
remain runs-of-flow; ONLY statements containing `&&` / `||` / `?:`
decompose. The block ends mid-statement with VERUS/FALSUS edges
(origo = the operator node — CONIUNCTIO / DISIUNCTIO /
CONDITIONALE); continuation resumes in a successor block whose
sententiae hold subexpression granules. Unaffected statements are
byte-identical to stage 0. No shadow micro-CFG; the edges live in
THE graph (solarium sees them).

Blast-radius bars (all must pass before chunk A):
- vectis TENET (exit 0) on the full pinned fixture suite;
- census meters byte-stable: 63 = 0 (pure wall), 64 = 1, 65 = 1,
  66/67 unchanged, plus all non-fluxus meters;
- probationes green (root + silva + officina), amalgams VERIFICATUM;
- an explicit audit of every consumer that iterates
  `FluxusBlocus.sententiae` (they now see granules for split
  statements) — v2 spec names them file:line.

## §4 Chunk A — eventa (extraction + retained tables)

New sidecar module `silva_c89_fluxus_datorum.{h,c}`;
`FluxusFunctionis` gains ONE field: `FluxusDatorum* datorum` (NIHIL
until built). Built at the stage-1 seat in semantica's
`_definitionem_ambulare`, always-on (Q8).

Contents (codex-est-basis-datorum — every field a future query
column):
- per-block ordered event stream: `FluxusEventum { symbolum, genus
  (USUS | DEFINITIO | DEFINITIO_LOCI), nodus, ordo }`;
- the tracked-variable set: automatic non-static locals of the
  function, each with a dense index (bitset position); binding via
  the `nexus` table (use-site → SemanticaSymbolum*, M1a) — shadowing
  solved by symbol identity;
- per-block may/must bitsets (chunk B fills them).

Block event sources (TWO, keyed on edge genus — conditions live on
edges, not in sententiae): (1) leaf statements in order; (2) the
terminal condition expression of the block's conditional out-edges
(VERUS/FALSUS share an origo; switch head expression = pater-climb
from a CASUS edge's origo).

## §5 Chunk B — punctum fixum (may + must)

The park undersold this: intersection alone cannot separate definite
from sometimes. TWO bitsets per block — may-init (union meet) and
must-init (intersection meet) — one shared transfer (any DEF sets
both; may-def sets may only). Predecessor index built in one pass
(margines are outgoing-only). Worklist from introitus; never-visited
predecessors are skipped in meets (this is what makes folded
`si (0)` branches invisible — s04d parity is structural). Per-use
states are not stored: one post-fixpoint re-walk per block with a
running state judges uses as they stream past.

## §6 Chunk C — iudicium

- **71 LECTIO_ININITIATA** (definite): use U of x with x ∉ may. Anchor
  at use. Self-init gets its own causa string.
- **72 ININITIATA_QUANDOCUMQUE** (sometimes): x ∈ may, ∉ must, AND a
  blamable edge exists. **The two-condition rule** (verified
  analytically against all 17 specimens): edge E = (P → S) is
  blamable for U iff
  (a) x ∉ may_out(P) — must-uninit crossing E — and
  (b) U is inevitable from S: every path from S reaches U before any
  def of x and before exitus.
  Implementation: per candidate use (rare — violations-as-data cost
  model), backward mark of def-free blocks reaching U, then a boolean
  fixed point for "all out-edges stay inside." Attribution vocabulary
  from E.genus + E.origo genus (draft, Fran revises): si-FALSUS
  "quandocumque conditio 'si' falsa est"; si-VERUS "… vera est";
  dum/per-FALSUS "quandocumque ansa exit quia conditio falsa est";
  CONIUNCTIO-FALSUS "quandocumque '&&' falsum est";
  DISIUNCTIO-VERUS "quandocumque '||' verum est"; ORDINARIUS with
  origo genus ORDINARIUS "quandocumque ordinarius sumitur". The
  labeled-default distinction is already in the graph: implicit
  non-match edges carry origo = commutatio and are NOT blamable.
  Anchor at condition line, socius = use site (Q10).
- **73 ININITIATA_FORSITAN** (dormant): maybe-uses with no blamable
  edge. Census-visible, never fires (`_forsitan_vigil` FALSUM);
  re-arm condition = adjudication appetite with full-corpus census
  data in hand. lib/ baseline: 3 sites (chorda c_praecedans, flatura
  distantia, sigillum fructus — all false-positive-shaped).
- 71 and 72 TOLERA-bilis (Q11).

## §7 Bars

- All 17 calibration specimens graduate to fixa/examinis with
  EXSPECTA-PROXIMA pins; silent behaviors become *.probus fixtures
  (the exemptions are pins too). Add the 2 missing specimens:
  sizeof(x), member-write-then-whole-value-use.
- Vectis legs: normal + inverse + oracle-leak; the oracle-leak leg is
  the parity meter (clang's exact flags: -Wall covers both classes).
- Corpus first contact: expect PURE WALL for 71/72 (classes 1+2 are
  exactly what house cleanliness guarantees). Any firing site is
  either a predicate bug or a real bug clang missed — both are
  harvest.
- Census: -census 71/72/73 wired; 73's count is the standing
  dormant meter.
- Perf: RELATIO reports measured push latency delta + piscina delta
  over the corpus (always-on policy, Q8); capsula_libri
  INFRA-under-load is the canary.

## §8 Named deferrals (paths back)

- Unknown-callee const-ness: assumed non-const (silent-leaning);
  revisit if a real bug class appears.
- setjmp/longjmp: out of corpus, unhandled, named here.
- Full clang-style CFG linearization (blocks hold expression
  elements everywhere): not needed for parity; the split-at-
  short-circuit granularity is the v1 contract. Path back exists if
  solarium's expression-level queries ever want it.
- Codex 62 re-arm: NOT in this arc — but the datorum tables are its
  named substrate; re-arm becomes its own pull once def-use chains
  exist here.

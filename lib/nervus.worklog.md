# nervus worklog

## 2026-10-16 — T1: the core (project-specs/nervus-plan.md)

**Built.** `include/nervus.h`, `lib/nervus.c`: networks as straight-line
programs (AFFINE / RELU with Magnus coefficients and a bias, the
`addendum`), single assignment (instruction k writes register
introitus + k, sources must be earlier), outputs naming registers;
exact evaluation with an optional activation pattern (bit k = ReLU k
in instruction order, ON iff argument > 0, packed 8 per byte); shape
(instructions, ReLUs, total terms, depth, ReLU depth, width); full
validation (`nervus_validus`) at the entry of every reader.

**Decisions made while building.**
- Builders check only the network's HEADER (counts, pointers, pool)
  in O(1). Full validation per append would make building quadratic.
  `nervus_validus` is the O(terms) check, and evaluation / shape call
  it.
- Width = the most registers LIVE at once, sampled before the first
  instruction and after each one; live = defined and still read by a
  later instruction or named as an output. There is no standard width
  for a straight-line program; this is the honest one for this
  representation (an input nobody reads is never live).
- `NervusFigura.termini` is i64: 2^24 registers times 2^16 terms
  overflows i32.
- Evaluation allocates values and scratch in the CALLER's pool (no
  hidden pools in the hot path); loops use piscina_notare /
  piscina_reficere around each call (tested: usage stable over 1000
  calls). The shape function uses a private pool.
- latina.h hazards hit in the first header draft: `constans` (const)
  and `registrum` (register) as identifiers. Now `addendum` and
  `registrum_novum`.

**Oracles.**
- In the suite (8 sections, 153 checks): hand networks (AND/OR/XOR on
  {0,1}^2 from one ReLU, |x|, max(x, y), constant network, outputs
  that are inputs, 2^200 values); a SECOND evaluator in the test
  (memoized recursion from the outputs, checked s64, not Magnus, not
  instruction order) on 1000 random networks: 0 overflows, 0
  discrepancies, values and patterns; positive homogeneity of
  bias-free networks at t = 2^200 (theorem: f(t x) = t f(x), same
  pattern); hand-computed shapes.
- Outside the gates (oracle 1 of the plan): a Python evaluator from
  the header's definition against a scratch C driver, 3000 random
  networks (inputs to 2^200, coefficients and biases to 2^70, up to
  40 instructions): 29,723 pattern bits, outputs up to 489 bits,
  0 mismatches. Not committed: a generated fixture would need the
  GENERATUM / fabrica generator machinery for one-off evidence; the
  T4 reviewer brings its own oracle.

**Plants (silva.planta, 12, all red).** Tie counted as active; a stale
pattern bit never cleared; validus letting a source equal its own
register; the builder letting a source equal the new register; the
bias ignored; ReLUs not counted; outputs not live at the end; ReLU
depth ignoring the ReLU; outputs accepting a negative register; the
three limits off by one (terms, registers, creare). The three limit
plants SURVIVED the first suite (it only tested one past each limit);
boundary tests were added (exactly 2^16 terms accepted and summed;
creare(2^24) valid but full; from 2^24 - 1 inputs one instruction,
then refused). The creare plant first went red by SEGFAULT (an
uninitialized network pointer after a refused create); the test now
initializes it, and the plant goes red by assertion.

**Gates run by hand.** `./compile_tests.sh nervus` 153/153; venenum
(ASan + UBSan + pool poison) sana, nervus 153; examen ACCIPE on all
three files; formator CONFORMIS; Latin lint NOVA 0 after glossary
entries `relu` (technical term) and `bitus` (form of `bit`).

## 2026-10-16 — T2: patterns, the affine map, composition

**Built.** Pattern helpers (`nervus_forma_ponere`, `_activae`,
`nervus_formae_aequales`, `nervus_forma_clavis`: the packed bytes with
padding bits masked, for tabula_dispersa); the FORCED evaluation
(`nervus_aestimare_coacte`: ReLU k follows bit k whatever the sign,
which makes the network affine); the exact affine map of a pattern
(`nervus_forma_affinis`: x -> M x + c, M a Matrix over
ANULUS_INTEGRORUM, by propagating affine forms register by register in
a private pool; only M and c are transcribed into the caller's pool);
composition (`nervus_componere`: b after a, b's inputs mapped to a's
output registers, pattern of the composite = a's bits then b's).

**Decisions.**
- Plain and forced evaluation share one internal `_aestimare` (a
  `scripta` pattern to write, a `coacta` pattern to obey). The forced
  path is still independent of the affine-form propagation, which is
  what oracle 2 needs.
- The affine map refuses when registers x (inputs + 1) > 2^27: the
  forms are dense, indices are i32, and memory would run out first.
- nervus now depends on matrix (and its closure: anulus, polynomium,
  fractio, congruentia). The venenum suite entry and the plant gate's
  source list had to follow; the first T2 plant run "broke the build"
  on every plant because the gate's list was stale - the gate, not
  the plants.
- A buffer overflow caught before compiling: composition first wrote
  b's remapped outputs into the 2^16-term source buffer before
  checking b's output count. Outputs now get their own buffer.

**Oracles (12 sections, 212 checks).** Pattern keys counted through
tabula_dispersa against hand counts (|x| over -5..5: 3 patterns; 17
staggered ReLUs over -2..20: 18, keys 3 bytes); hand-dirtied padding
bits ignored by keys and equality. Forced |x| under all four patterns
(0, x, -x, 0). Oracle 2: on 500 random networks (half bias-free) with
random patterns, M x + c = forced evaluation on 5 random x each; on
the realized pattern, M x + c = ordinary evaluation; bias-free gives
c = 0; hand maps for |x| and the gates (AND = x + y - 1, XOR =
-x - y + 2 under the pattern "on"). Oracle 4: on 500 random triples,
compose(a, b)(x) = b(a(x)), composite pattern = a's then b's, counts
add, and (a.b).d = a.(b.d) on outputs.

**Plants (silva.planta).** T2: 10, all red (forced path following the
sign; NIHIL pattern accepted; inactive ReLU not zeroed in the map;
bias dropped; input unit vectors shifted; key padding not masked;
equality and count skipping the last bit; composition with b's sources
not remapped, or b's inputs mapped to a's inputs). The 12 T1 plants
re-run against the refactored evaluator: all red.

**Gates by hand.** nervus 212/212; venenum sana (nervus 212); examen
ACCIPE; formator CONFORMIS; Latin lint NOVA 0 (glossary:
`associativus`, Latina media).

## 2026-10-16 — T3: plexus emits into nervus

`plexus_nervus` (lib/plexus.c, see lib/plexus.worklog.md) is nervus's
first real producer. plexus.h now includes nervus.h, so every plexus
consumer (D128's live build included) links nervus; D128 still builds
and passes 24/24 quick. venenum's plexus closure gained lib/nervus.c.
The width of a braid letter under nervus's definition is 2n - 3 (end)
and 2n + 2 (middle): affine temporaries count as live registers.

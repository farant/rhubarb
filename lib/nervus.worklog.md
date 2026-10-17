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

## 2026-10-16 — T4: independent review and fixes

**Review** (recensor-nervus, own oracles; record in the scratchpad
recensio_nervus/findings_summary.md): H 0, M 3, L 5, N 5. Python
evaluator from the header on 26,000 random networks (3000 under ASan +
UBSan; coefficients to 2^80, biases to 2^90, inputs to 2^200, 0..7
outputs incl. repeats and inputs), its own affine map (forced
evaluation on basis vectors), brute-force width, keys, composition:
0 mismatches. plexus_nervus against a Python Dynnikov action from
Thiffeault eq. 8-15 on 14,000 words (n = 3..9, 2^220): 0 mismatches;
braid relations on the C networks themselves, 44k checks, an oracle
that kills all 38 single-sign mutants of its own rules.

**What it found: test gaps, not wrong results.**
- M1: the copy of M and c out of the private pool was untested beyond
  s64 (mutants dropping `magnus_transcribe` passed all 212, even under
  ASan, since small Magnus have no heap limbs). Now tested with 2^100
  coefficients and a 2^90 bias, reading M and c after REUSING the freed
  memory (256 KiB of 0xA5 from a fresh pool) - the plain build catches
  the mutant, not only ASan.
- M2: key masking untested at ReLU counts that are multiples of 8 (a
  mutant masking full bytes zeroes the last byte: different patterns,
  same key - N6 would undercount). Now 8 and 16 ReLUs, patterns
  differing only in bit 7 / 15.
- M3: promised refusals untested (pattern equality across sizes and
  forced evaluation with a foreign pattern - both out-of-bounds reads
  if the check went; forma_creare on an invalid network). Now tested;
  plus the 2^27 guard (2^24 - 1 inputs) and composition at exactly
  2^24 registers (accepted, valid) and 2^24 + 1 (refused).
- L1/L2: header wording for width (sampled BEFORE the first
  instruction too) and depth (1 + max over sources, 0 if none). L3:
  the plexus section has 18 checks. L4: plexus_nervus now counts
  registers exactly (4 per end letter, 11 per middle) instead of 11
  for all. L5: the dense forms in the affine map use
  piscina_conari_allocare, so a failed huge allocation returns FALSUM
  instead of aborting.

**API changes made before any consumer exists.**
- N2: one argument order for both evaluators:
  nervus_aestimare_coacte(rete, introitus, exitus, forma, piscina).
- N1: `NervusForma.nullae` counts the ReLUs whose argument was EXACTLY
  0 in the last evaluation. Counting patterns at lattice points is not
  counting linear regions: a point on a wall gets the pattern of a
  lower-dimensional cell (|x|: 3 patterns, 2 regions), and bias-free
  networks (braids) have every wall through the origin. N6 can count
  only points with nullae = 0, or both. Keys and equality ignore it.
- N3 (partly): composition sizes its source buffer to b's largest
  instruction instead of a fixed 256 KiB. N5: header says outputs may
  share limbs with inputs or biases.

**Deferred to their consumers.** A validated-once handle / unchecked
evaluation (N2's search, once measured); joining per-pass patterns of
w^m into one key (N6's plan); lifting M to t I - M over polynomials
(plexus v2, exact dilatation).

**Plants (8, red):** the reviewer's mutants (M and c not transcribed,
the full-byte mask, equality without the size check, a foreign pattern
accepted, forma_creare weakened to a NIHIL check) and two for the tie
count. Not planted, by reason: raising the 2^27 guard (the fallible
allocation now also refuses, so the test still sees FALSUM - the guard
protects i32 indexing between 2^27 and 2^32 elements, which needs a
multi-GB allocation to reach) and the composition limit (redundant with
the builder's refusal, as the reviewer noted). L4's exact count is not
tested on the accepting side: that needs a ~4M-letter network.

## 2026-10-16 — T5.0a: exact planar regions (D129 task 0)

**Built.** `nervus_regiones_plani(rete, piscina, &planum)`: the activation
regions of a bias-free network with 2 inputs, exactly. Types
`NervusRadius` (primitive integer direction), `NervusSector` (walls
`initium` / `finis`, an interior direction `medius`, the pattern, the
exitus x 2 matrix, the index of its linear piece `frustum`),
`NervusPlanum` (`numerus` regions, `regiones`, `frusta` pieces).

**Algorithm.** Four quarter arcs between the axes. Per instruction, each
arc carries the linear form (x, y coefficients) of every register; at a
ReLU an arc whose argument g changes sign STRICTLY between its ends u, v
splits at the ray |g(v)| u + |g(u)| v (reduced to primitive). Arcs stay
< pi, so u + v is always interior and the endpoint signs decide the
interior sign. At the end, walls = arc boundaries where the pattern
changes; adjacent arcs with the same pattern (only possible across an
axis that is not a wall) merge. Cost O(arcs x registers) Magnus; the
arc buffers are double-buffered with separate capacities (a first draft
kept one capacity across the swap, which would have overrun the smaller
buffer - caught on reading, before running).

**Facts the API relies on (stated in the header).**
- A pattern's region is CONVEX (each ReLU condition is a half-plane
  under the earlier bits), so one pattern = one sector, angle <= pi,
  and the sectors' patterns are pairwise distinct. Tested.
- Exactly one wall is impossible (the pattern would come back around),
  so walls are 0 or >= 2; no walls = one sector, the whole plane, with
  initium = finis = (1, 0).
- `forma.nullae` of a sector = ReLUs whose argument is IDENTICALLY 0 on
  it (bit 0 by the > 0 convention). nervus_aestimare at any interior
  point returns the same nullae - tested on every sector and on every
  lattice point inside one.
- Pieces: consecutive sectors with equal matrices, cyclically (the run
  through (1, 0) joins piece 0).

**Finding for D129 (not a bug): identically-zero arguments are common.**
345 of 400 random bias-free networks have a sector where some ReLU's
argument is identically 0 (typically a ReLU reading a dead register).
Every point of such a sector has nullae > 0, so the plan's lattice
criterion "interior = nullae == 0" (Part B / C) never counts that
sector. The lower bound stays a lower bound, but it can miss whole
regions, not just thin ones. Whether braid networks have such sectors
is a D129 measurement (Part B can compare with the exact sweep on
B_3); for B_4 the fix would be "interior = no argument vanishes that is
not identically zero under the point's own pattern", which needs the
argument forms under a pattern (a possible nervus helper, for T5b).

**Oracles (section "plana", 81 checks; suite 334).**
- Hand networks: relu(x) (half-planes, walls on the axis), max(x, y)
  (axes merged away), |x| + |y| (4 quadrants, 4 pieces),
  relu(x) - relu(-x) (2 regions, 1 piece), no ReLU (1 sector, 0 x 2
  matrix), relu(x - x) (nullae 1), relu(relu(x) - y) (4 regions, 3
  pieces: the wall (0, 1) changes the pattern, not the map),
  relu(4x - 6y) (ray (6, 4) reduced to (3, 2)), 2^100 coefficients
  (rays and M read after the private pool is freed and its memory
  rewritten with 0xA5).
- A second algorithm in the test, a FIXPOINT: candidate rays; on each
  arc evaluate (nervus_aestimare) at u + v, propagate the ReLU argument
  forms under that pattern in the test's own code, add every zero ray
  of an argument that changes sign strictly inside the arc; repeat
  until nothing is added (then no argument changes sign inside any arc
  under its midpoint pattern, so the pattern is constant on it). It
  shares nothing with the sweep. On 400 random networks: walls,
  patterns, nullae equal; plus M = nervus_forma_affinis of the pattern,
  primitive rays, strict ccw order, angle <= pi, distinct patterns,
  pieces = cyclic matrix changes.
- Lattice: on the shell max(|x|, |y|) = 12 of 100 networks, a point
  inside a sector evaluates to its pattern and nullae, a point on a
  wall gives M p for both neighbours, no point in two sectors.
- Refinement: a^m for m = 1..5 by composition on 40 networks: regions
  never decrease, every sector of a^(m+1) lies in one sector of a^m
  whose pattern is its prefix.

**Plants (silva.planta, 16, all red).** Split ray with the coefficients
swapped; ray not reduced to primitive; an identically-zero argument
counted active; split sides swapped; nullae counted when only one end
is zero; walls compared with the NEXT arc; an inactive form not zeroed;
a bias accepted; three inputs accepted; the wrap-around piece not
merged; the wrap merge applied to a single piece; medius = the end of
the sector's first raw arc (interior only when the sector spans several
arcs); M and a ray coordinate not transcribed out of the private pool
(caught only by the 2^100 test after the 0xA5 rewrite); splitting also
at a zero end. "Second arc misses the new form" goes red by SEGFAULT:
the mutant itself reads an uninitialized Magnus inside the library (not
a test reading a refused result), so it stays as is. A first version of
the medius plant did not compile (-Werror: unused variable) and was
replaced by the sharper one above.

**Gates by hand.** nervus 334/334; examen ACCIPE; formator CONFORMIS;
Latin lint NOVA 0 - `sector` is real Latin AND English, so instead of a
glossary entry the member is `regiones` (the type stays NervusSector;
comments say sectores), and two-letter locals (gx, gu, vx, ...) were
spelled out. silva's renominare refuses dirty files (git is its undo),
so the renames were done with exact word-boundary replacements scoped
to each function body.

## 2026-10-16 — T5.0 review (recensor-plani): nervus side

No defects in `nervus_regiones_plani`: the reviewer's own exact oracles
(enumeration of all 2^R patterns with an exact nonempty-cone test, and
a separate fixpoint) agreed field by field on ~2,500 networks (2^100
coefficients, deep compositions, 45 real B_3 braid networks, 400 under
ASan + UBSan); cost on the full B_3 census to length 8 at m = 1..12:
at most 100 regions, under 10 ms per sweep. Header fixes: L1 - only
PRIVATE memory failure gives FALSUM (caller-pool allocation is fatal,
as in every pool); N2 - `medius` is some interior direction, not a
canonical one (it depends on the arc subdivision), so consumers must
not pin it. Main design finding (M1, for D129, not nervus): regions
belong to the WORD, pieces to the function (three geodesic words of
Delta sigma_1: 12, 12 and 6 regions at m = 4, the same pieces); the
D129 plan now reports pieces as the headline and regions as min/max
over all geodesic words.

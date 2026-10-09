# Demo 118: Exact Quaternions — Findings

Demos 66-97 did their quaternion geometry in doubles: generator products,
dedup of group elements and axes by epsilon, and "Voronoi" activations
decided by `>` on rounded dot products. This demo redoes the foundations
exactly with two new house libraries, `extensio` (Q(sqrt 2), Q(sqrt 5)
with exact signs) and `quaternio` (quaternions over any house ring). It
then recounts the one capacity result whose cells can be decided exactly
with them: D66 Part H.

Run: `./bin/aedilis knotapel/demo_118_exact_quaternions/main.c && bash
build/aedilis/main/struere.sh && ./build/aedilis/main/main` (0.7 s, 41
checks). Independent oracle for Part C: `python3 -I oracle.py` (about 1
min; own exact a + b sqrt 2 arithmetic, no house code, a different search
for "possible").

## Headline

**D66's "35/2024 antipodal triples compute XOR6 with the 24-cell Voronoi
activation" is an artifact of how exact ties were broken. The exact answer
is a clean characterization:**

- **possible** (passes under SOME tie resolution) <=> the three classes
  are **mutually orthogonal**: 96 sets.
- **robust** (passes under EVERY tie resolution) <=> the three classes lie
  in **one coset of the quaternion group** Q8 = {+-1, +-i, +-j, +-k}: 24
  sets.
  - Q8 is normal in 2O, and its 6 cosets are 6 orthonormal frames of 4
    classes each.
  - The catalog has 24 orthonormal frames in all; the other 18 contain no
    robust triple.
  - The 24 robust sets form ONE orbit under the group's symmetries.
- **lowest catalog index wins**: 35 sets. That is exactly D66's float
  result, set by set (D66's actual winner list was extracted and is
  checked in the demo).

| tie rule | sets computing XOR6 |
|---|---|
| robust: every tie resolution | **24** (one Q8 coset each) |
| lowest index (= D66's floats, set by set) | 35 |
| possible: some tie resolution | **96** (mutually orthogonal) |

**Mechanism.** Every one of the 2,024 sets has an exactly tied sum (tie
sizes only 2 or 4), but there are two kinds:

- **1,928 sets are not mutually orthogonal.** For some pair, the sum
  c_i +- c_j is exactly equidistant from its own two members:
  (c_i +- c_j) . c_i = 1 +- g and (c_i +- c_j) . c_j = g +- 1, with equal
  absolute values. Such a set fails under EVERY tie rule:
  - c_i alone (odd) lands on cell i, and c_j alone on cell j;
  - the even pair sum must land on one of them.
  This tie decides nothing.
- **The 96 orthogonal sets** have no such pair tie. Their ties are 4-way:
  e.g. in [0,1,9], c0 + c9 ties cells {4,6,8,17}. These ties are where the
  rule matters, and they decide between 24, 35 and 96.

**Symmetry.**
- Left and right multiplication by the 48 group elements gives 96 maps,
  which are 47 distinct permutations of the catalog (D66 also printed 47).
  Each keeps every dot product, so a verdict about the geometry must be
  invariant under all of them.
- The robust and possible verdicts are invariant under all 96 maps.
- The lowest-index rule breaks every one of the 92 non-trivial maps; the 4
  multiplications by +-1 fix every class.
- So 35 is not a property of the weights.

**D66's 35 winners, re-read:**
- 24 are robust, i.e. the whole Q8-coset orbit, including D66's winner #1
  [0,1,18].
- 11 depend on the tie rule: [0,1,23], [2,9,19], [2,9,21], [2,18,22],
  [4,5,23], [6,12,20], [7,11,17], [8,14,20], [9,15,16], [10,13,17],
  [17,20,23]. They include D66's printed winner #3.
- D66 Part I's "16 symmetry orbits" are classes of matching labelings,
  not orbits of the group action. It splits the single true orbit of 24
  into 12 + 3 + 4 + 4 + 1; its single-member class [4,5,15] is robust and
  lies in the same orbit as [0,1,18]. The 11 tie-dependent winners are
  the other 11 of its 12 single-member classes.

**What survives:** the 24-cell Voronoi activation does compute XOR6 with
antipodal pairs, exactly for weight triples inside one Q8 coset. The
comparison "beats the 6x6 geographic grid (3 winners)" is not settled
here: the geographic cells use atan2 sector boundaries, which were not
recounted.

## Part A — D66's generators over Q(sqrt 2) give 2O

- sigma_1 -> (1+i)/sqrt 2, sigma_2 -> (1-k)/sqrt 2.
  - Braid relation exact; s1 s2 s1 = (i - k)/sqrt 2.
  - Closure: exactly 48 elements.
- Element orders: 1^1 2^1 3^8 4^18 6^8 8^12, so the largest order is 8.
  - The finite subgroups of SU(2) are cyclic, binary dihedral, 2T, 2O
    and 2I. An order-48 cyclic group has an element of order 48, and the
    order-48 binary dihedral group has one of order 24.
  - So the group IS the binary octahedral group 2O, confirming note 34's
    correction ("binary octahedral, NOT the 24-cell").
- D66's word enumeration, replayed through the exact multiplication table:
  - words on 2-3 strands, length <= 8, in D66's digit order;
  - it reaches all 48 elements;
  - its catalog is the 24 classes mod sign, in D66's order and with D66's
    signs (the float and exact catalogs agree entry by entry to 4.9e-16);
  - the types are 4 axis + 12 edge + 8 body: the 24 F4 roots mod sign.

## Part B — rotation axes of 2O

Exact dedup with `quaternio_eadem_axis` (cross product = 0) gives exactly
**13** axes, the 13 directions used from D66 on. The fold of an axis is
(non-scalar elements on it + 2)/2:
- 3 four-fold axes (cube faces),
- 4 three-fold axes (vertices),
- 6 two-fold axes (edges).

## Part D — D94's binary icosahedral group over Q(sqrt 5)

- Generators s = (1+i+j+k)/2 and t = (phi + phi^-1 i + j)/2.
- Closure: exactly 120 elements.
- Orders: 1^1 2^1 3^20 4^30 5^24 6^20 10^24, largest 10. By the same
  classification the group is 2I.
- 60 classes mod sign and **31** axes (6 five-fold + 10 three-fold + 15
  two-fold), matching D94's printed 60 entries and 31 directions. D94's
  float dedup was right here.

## Method

- **Exactness.**
  - Every comparison is exact: |d| against |d'| is compared as d^2
    against d'^2 with `algebraicus_compara`. A refusal would abort the run
    (none occurs).
  - The zero sum is detected exactly: all 24 dots vanish, and the catalog
    spans R^4.
  - Doubles appear only in the float replica and in the catalog
    cross-check, never in a verdict.
- **Reduction.**
  - With antipodal weights +-c_a, +-c_b, +-c_c, a mask's sum is
    x_a c_a + x_b c_b + x_c c_c with x in {-1, 0, 1}.
  - Its parity is the number of nonzero x: an x = 0 pair contributes 0 or
    2 bits.
  - So there are 27 x-PATTERNS, each with one parity.
  - As vectors there are 19-27 distinct sums per set. In 32 sets a
    relation such as c_c = c_a + c_b makes two patterns give the same
    vector with opposite parities (64 nonzero patterns sum to 0). A tie
    rule is a function of the point, so such a set fails outright; all 32
    are non-orthogonal and fail anyway (recensio L1).
- **"Possible"** asks whether some labeling of the cells by parity gives
  every sum a tied cell with its own parity, with one cell per vector.
  - main.c answers it by forced labels plus exhaustive search over the
    free cells (never more than 20; "undecided" = 0).
  - oracle.py answers it by backtracking over each sum's tie choice.
- **Oracles.**
  - oracle.py is independent exact Python (its own Q(sqrt 2), closure and
    search; it shares the word replay and the Gram table with main.c). It
    reproduces 35/24/96, the 32 clash sets, and the characterization with
    0 violations.
  - The recensio (D118) confirmed everything again with a vector-level
    oracle (no Gram table) and with the real D66 binary, instrumented.
  - main.c's checks "= oracle.py's counts" are constants recorded from
    those runs, not a live call.
- **D66's actual winners:** D66's main.c, with one fprintf added in a
  scratch copy, is frozen as a list in main.c and compared set by set with
  the replica and with the exact first-index rule.
- **Plants:** 17 in total.
  - Red (15):
    - ties never detected
    - axes by eadem_rotatio
    - fold formula
    - robust check sees only the first tie
    - "possible" always true
    - zero cell never
    - Gram sign
    - replay with the inverse generator
    - element order off by one
    - wrong parity
    - right-multiplication maps replaced by left ones — this one first
      survived; the count of 47 distinct permutations kills it
    - float replica without the sign identification
    - robust characterized by orthogonality instead of the Q8 coset
  - Equivalent (2):
    - own-pair tie counted on one member: for a pair sum, both members'
      |dots| are equal, so one is in the tie set iff both are;
    - float replica without normalization: scaling every |dot| by
      1/|sum| does not move the argmax, and D66's winners are unchanged.
  - The recensio's survivors are equivalent too:
    - the non-trivial zero path decides nothing (its sets are
      non-orthogonal);
    - the forced-label conflict is re-rejected by the exhaustive search.

## Not covered (next)

- **Demo 119:** D94's capacity tables (XOR/AND/MAJ at N = 3-8). These use
  angle-sector x direction cells with k = 12 sectors, so the boundaries
  are cos(m pi/12) and need sqrt 3 and sqrt 6 beside sqrt 5. The generic
  extensio constructor can build that field.
- **The true braid images at zeta_6 and zeta_10** (note 41's corrected
  generators) need sin(pi/l) and the braid-relation axis angle in the
  field.
- **D66 Part E/G geographic grids** (atan2 cells) and the full
  non-antipodal search (D66: 0/475,020), not recounted.

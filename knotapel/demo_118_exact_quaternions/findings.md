# Demo 118: Exact Quaternions — Findings

Demos 66-97 did their quaternion geometry in doubles: generator products,
dedup of group elements and axes by epsilon, and "Voronoi" activations
decided by `>` on rounded dot products. This demo redoes the foundations
exactly with two new house libraries, `extensio` (Q(sqrt 2), Q(sqrt 5)
with exact signs) and `quaternio` (quaternions over any house ring). It
then recounts the one capacity result whose cells can be decided exactly
with them: D66 Part H.

Run: `./bin/aedilis knotapel/demo_118_exact_quaternions/main.c && bash
build/aedilis/main/struere.sh && ./build/aedilis/main/main` (0.6 s, 33
checks). Independent oracle for Part C: `python3 -I oracle.py` (42 s; own
exact a + b sqrt 2 arithmetic, no house code, a different search for
"possible").

## Headline

**D66's "35/2024 antipodal triples compute XOR6 with the 24-cell Voronoi
activation" is a tie-breaking artifact.** In EVERY one of the 2,024 weight
sets, some subset sum is exactly equidistant from two or more cells (up to
4). That is structural, not rounding: (c_a + c_b) . c_a = 1 + c_a . c_b =
(c_a + c_b) . c_b, so a pair's sum ties its two members unless a third
cell is nearer. The verdict therefore depends on how ties are broken:

| tie rule | sets computing XOR6 |
|---|---|
| passes under EVERY tie resolution (robust) | **24** |
| lowest catalog index wins (= D66's floats, set by set) | 35 |
| passes under SOME tie resolution (possible) | **96** |

- D66's float pipeline (replicated, reproducing its printed 35) agrees
  with "lowest index wins" on every single set. Its rounded dot products
  happened to break each exact tie toward the earlier catalog entry.
- **11 of D66's 35 winners are not robust**: [0,1,23], [2,9,19],
  [2,9,21], [2,18,22], [4,5,23], [6,12,20], [7,11,17], [8,14,20],
  [9,15,16], [10,13,17], [17,20,23]. They include its printed winner #3.
  These are 11 of the 12 single-member "symmetry orbits" in D66 Part I.
- **The decisive check is symmetry.** Left and right multiplication by
  the 48 group elements gives 96 maps, which are 47 distinct
  permutations of the catalog (D66 also printed 47). Each keeps every dot
  product, so a verdict about the geometry must be invariant under all of
  them.
  - The robust and possible verdicts are invariant under all 96.
  - The lowest-index rule breaks every one of the 92 non-trivial maps;
    the 4 multiplications by +-1 fix every class.
  - So 35 is not a property of the weights; 24 (robust) and 96 (possible)
    are.
- **What survives:** the qualitative claim that the 24-cell Voronoi
  activation computes XOR6 with antipodal pairs (24 robust sets, among
  them D66's winner #1 [0,1,18] and its 12-member orbit). The comparison
  "beats the 6x6 geographic grid (3 winners)" is not settled here: the
  geographic cells use atan2 sector boundaries, which were not recounted.

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
  - So the 64 masks are 27 sums, each with one parity.
  - Dots come from the exact Gram table.
- **"Possible"** asks whether some labeling of the cells by parity gives
  every sum a tied cell with its own parity. main.c answers it by forced
  labels plus exhaustive search over the free cells (never more than 20;
  "undecided" = 0). oracle.py answers it by backtracking over each sum's
  tie choice.
- **Plants (12, all red):**
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
  - right-multiplication maps replaced by left ones — this one survived
    until the distinct-permutation count (47) was added
  - float replica without the sign identification

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

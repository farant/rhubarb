# reticulum: plan (house tier)

Approved by Fran (2026-10-13): the slice-1 API sketch and five
decisions:
- vectors are ROWS (`reticulum_relationes` transposes the kernel basis
  that `matrix_reticulum_nuclei` returns as columns);
- slice 1 takes linearly independent bases only; dependent generating
  sets (MLLL) come later, and slice 1 refuses them with FALSUM;
- the quadratic-form (Gram) variant is in slice 1, on the same core;
- default delta = 99/100, any exact delta in (1/4, 1] allowed;
- enumeration keeps TRUE counts separate from stored vectors, with a
  node budget and an explicit `completa` flag.

## Purpose

Exact integer lattices: LLL reduction, short-vector enumeration and
integer relations among exactly given vectors. This is #20 on the math
library map (`project-specs/mathematica-bibliothecae-visio.md`),
started from its exact half. The first consumer is knotapel's audit P1
(KC 47-48 in `knotapel/claims.md`): the relation profile of a catalog,
its exact coincidences and successive minima as a measure of
genericity. Later consumers: polynomial factorization over Z, minimal
polynomials for `extensio` beyond quadratic fields, and quadratic forms
from knot theory (Goeritz and linking forms).

## Governing rules (Fran, 2026-10-13)

1. **No approximate result may pass as exact.**
   - Slice 1 has no floating point anywhere: integral LLL (de Weger;
     Cohen, Algorithm 2.6.7) keeps Gram-Schmidt data as integers d_i
     and lambda_ij; enumeration uses exact rationals.
   - Every bounded computation says whether it completed. A truncated
     enumeration returns `completa = FALSUM`, and its count is a lower
     bound, named as such in the header. Demos assert `completa`
     before printing a count as a result.
   - Slice 2 (float discovery, below) returns CANDIDATES in a distinct
     type. A candidate becomes a result only through an exact check in
     slice-1 arithmetic. The types keep "found" and "proved" apart.
2. **A general-purpose library, strengthened over time.**
   - Results carry certificates where cheap: `u` with reducta =
     u * basis and det u = +-1, checkable without this library.
   - Outputs are canonical (enumeration order: norm, then
     lexicographic; sign: first nonzero coordinate positive), so they
     compare vector for vector with any oracle.
   - Tests check lattice-theory invariants (reducedness, unimodularity,
     same lattice by equal Hermite forms, LLL's bound on the first
     vector), not only pinned outputs: LLL output is not unique across
     algorithms.
   - Nothing knotapel-specific lives in the library.

## Slice 1 API

As approved; see `include/reticulum.h` once T1 lands. Functions:
`reticulum_reduce`, `reticulum_formam_reduce`, `reticulum_est_reductum`,
`reticulum_orthogonaliza`, `reticulum_enumera`,
`reticulum_formam_enumera`, `reticulum_minimum`, `reticulum_relationes`,
`reticulum_apex_officinarum`; types `ReticulumStatistica`,
`ReticulumQuaestio`, `ReticulumEnumeratio`.

## Scale

Exact LLL is polynomial but heavy in bignum operations. The intended
range is lattices of rank up to a few hundred: catalogs of 2T, 2O, 2I
(relation lattices of rank 16-52) and single weight sets. A 4,096-point
catalog (rank about 4,000) is out of reach and out of scope.

## Tasks

Each task follows the house cycle:
1. implement;
2. tests red, then green;
3. planted faults (`silva.planta`);
4. worklog (`lib/reticulum.worklog.md`);
5. commit behind the gates;
6. stop for Fran's go-ahead.

- **T1. Exact Gram-Schmidt and the reducedness judge.** Header with the
  whole slice-1 API (later functions refuse until implemented: FALSUM,
  documented). `reticulum_orthogonaliza` (mu over Q, |b*_i|^2 as
  Fractio) and `reticulum_est_reductum`. These are the oracle for
  everything after, so they come first.
  Credo: hand-computed 2x2 and 3x3 cases; a basis that is size-reduced
  but fails Lovasz at delta = 99/100 and passes at 3/4; dependent rows
  refused.
  *(2026-10-13: landed with two form-variant additions,
  `reticulum_formam_orthogonaliza` and `reticulum_formam_est_reducta`,
  which T2's tests need. 1,591 checks against an in-test textbook
  Gram-Schmidt and Bareiss; 12 plants red, one only after a reduced
  dimension >= 3 regime was added to the tests. See the worklog.)*
- **T2. Integral LLL.** `reticulum_reduce` and `reticulum_formam_reduce`
  on one integral core, with the `u` certificate and statistics.
  Credo: output judged by `est_reductum`; u unimodular; same lattice
  (equal Hermite forms); |b_1|^2 <= alpha^(n-1) lambda_1^2 with
  alpha = 4/(4 delta - 1), checked against enumeration in T3; knapsack and subset-sum lattices; the form variant
  agrees with the basis variant on G = B B^T.
  *(2026-10-13: landed. Agrees with a Python reference on outputs and
  swap/reduction counts; 60 random bases judged by invariants (u * b =
  reducta, |det u| = 1, equal Hermite forms, est_reductum); knapsack
  recovered in 216 swaps; form variant gives the same u. Scratch-arena
  compaction: 8.9 MB -> 1.1 MB peak at n = 20. 12 plants red; one
  looped forever, so every LLL section now runs first in a child under
  a 30 s bound. The bound on |b_1|^2 waits for T3's enumeration.)*
- **T3. Enumeration.** Fincke-Pohst on the reduced basis:
  `reticulum_enumera`, `reticulum_formam_enumera`, `reticulum_minimum`;
  canonical order, true counts, storage limit (first in canonical
  order), node budget.
  Credo: equal to brute-force box enumeration in small dimensions,
  vector for vector; Z^n and A_n / D_n / E_8 root counts (E_8: 240
  roots, i.e. 120 up to sign); budget hit -> completa FALSUM.
  *(2026-10-13: landed. Integer Fincke-Pohst on the LLL-reduced form,
  no fractions: the remaining budget is scaled by a path-independent
  product of Gram determinants, and the leaf norm comes from an exact
  division that doubles as a consistency check. 180 random cases equal
  an independent brute force vector for vector (54 with norm ties);
  A_n, D_n, E6-E8 root counts and E8 at norms 4 and 6 (1,200 and
  4,560); identical output for B and U B; the minimum, and T2's
  deferred bound on |b_1|^2. 15 plants red, the node-budget boundary
  only after an exact-size tree was added. See the worklog.)*
- **T4. Integer relations.** `reticulum_relationes`.
  Credo: relations among exactly embedded algebraic numbers (e.g. the
  minimal polynomial of 1 + sqrt2 recovered from the vector of its
  powers in Q(sqrt2) coordinates); relations among the 24 points of 2O
  include every coincident pair sum.
  *(2026-10-13: landed, with Q accepted as well as Z: rows are
  multiplied by their denominators' lcm, so T6 can pass 2O's exact
  coordinates. Judged by relation, rank, reducedness and saturation
  (Hermite membership) against a brute force and all 216 signed
  pair-sum coincidences of 2O (rank-16 relation lattice). 7 plants
  red, delta-ignored only after the composition with
  reticulum_reduce was asserted.)*
- **T5. Review** (independent reviewer with its own Python oracle),
  then fixes.
  *(2026-10-13: ~45,000 differential cases against a from-scratch
  Fraction oracle, no wrong results. Fixed: an i32 overflow at n =
  65536, tall bases refused at once, Gram-Schmidt rewound per entry
  (424 MB -> 2.8 MB at n = 80), the apex as a SUM of arena peaks (Fran)
  reset at every entry, a `nodi` field (Fran), one wrong assertion, and
  new tests for the regimes the review found unexercised. 8 plants red.
  See the worklog.)*
- **T6. First consumer:** a knotapel demo of relation profiles of 2T,
  2O and 2I (feeds audits P1 and P2), with its own plan.

## Gates

`radix`, `silva` (new lib/*.c), `generata`, `venenum` (scratch arenas);
`oratio` if the glossary is touched (`orthogonaliza` may need an
entry).

## Later slices

- **Slice 2: float discovery.** A big fixed-point / dyadic front end
  (the 2026-10-05 review: exact LLL on high-precision approximations
  held as scaled integers, not PSLQ in doubles). Output is a candidate
  type, verified exactly before use (rule 1).
- **MLLL:** dependent generating sets, returning the relations among
  generators.
- **Polynomial factorization over Z** (van Hoeij / LLL) as its own
  library on this one.
- **Speed:** measured first (agenda A3); faster LLL variants only where
  a real workload needs them.
- **Prepared lattice** (review T5, finding 11): reduce once, then
  enumerate at several radii (successive minima) without re-running
  LLL and Gram-Schmidt; new functions, `reticulum_enumera` unchanged.
- **`reticulum_formam_minimum`** with a node budget and `completa`
  (review T5, finding 12; Fran: later). Today: reduce, then enumerate
  with `limes_servandi` 1.

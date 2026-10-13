# reticulum worklog

## 2026-10-13 - T1: header, exact Gram-Schmidt, the reducedness judge

Plan: `project-specs/reticulum-plan.md` (Fran approved the API the same
day). T1 lands the whole slice-1 header. The functions of T2-T4 return
FALSUM, and a test checks that they leave their outputs untouched.

**One core for basis and form.** Integral Gram-Schmidt (de Weger;
Cohen, Algorithm 2.6.7) needs only inner products, so `Fons` hands out
either row dot products (basis) or matrix entries (Gram form), and
everything after is shared. Data: d_0 = 1, d_k = Gram determinant of
the first k vectors; integri_kj = d_(j+1) mu_kj. Every division is exact
by theory; a nonzero remainder returns FALSUM (it would be a bug). Note
the 0-based indexing against Cohen's 1-based: the recurrence is
u <- (d_(i+1) u - integri_ki integri_ji) / d_i.

**d_k > 0 for all k is the single admissibility test.** For a basis,
d_k = 0 means dependent rows (a Gram matrix is semidefinite); for a
form, it is Sylvester's criterion for positive definiteness. Dependent
rows, zero rows, more rows than columns, [1, 2; 2, 1], [0, 0; 0, 1] and
[-1, 0; 0, 1] are all refused by the same line.

**The judge in integers.** Size-reduced: 2 |integri_kj| <= d_(j+1).
Lovasz, multiplied through by d_k d_(k-1) > 0 and with delta = p/q:
q (d_(k+1) d_(k-1) + integri_k,k-1^2) >= p d_k^2. No fractions.

**Tests: an independent route, not the same code twice.** The suite
carries its own textbook Gram-Schmidt in Fractio vectors
(b*_i = b_i - sum mu_ij b*_j) and a rational judge, and compares with
the library on 60 random bases (dimension 1-6, small, mid-size and
10^25-scale entries). The product of the norms is checked against
det(B B^T) by Bareiss (matrix.h), a third route. Hand cases (checked in
Python Fractions first): the common LLL example [1,1,1; -1,0,2; 3,5,6]
(mu 1/3, 14/3, 13/14; norms 3, 14/3, 9/14) and its reduced basis
(mu = 1/2 exactly: the size-reduction boundary); [4,0; 2,3], which
passes at delta 3/4 and fails at 99/100; [1,1; 0,1], where Lovasz holds
with EQUALITY at delta = 1/2.

**Plant that survived first: Lovasz with mu_k,0 instead of mu_k,k-1.**
11 of 12 plants were red at once. This one survived because the two
coincide for two vectors, and random bases of three or more vectors are
almost never reduced, so the comparison with the textbook judge never
saw a reduced basis where they differ (21 reduced bases, all small).
Fixes:
- a hand case where mu_20 = 0 and mu_21 = 1/2: [1,0,0,0; 0,6,0,0;
  0,3,5,2] passes (29 >= 26.64) but fails the planted judge
  (29 < 35.64);
- near-orthogonal bases in the comparison (entries +-2 plus a growing
  diagonal 30 + 10 i), and an assertion that at least two reduced
  bases of dimension >= 3 were judged. My first try used a random,
  non-increasing diagonal, which fails Lovasz almost always (0 such
  bases); a debug print showed it. Now 18.
Lesson: a judge compared on random inputs only sees the regime the
generator produces. Count the cases in the regime that matters and
assert the count.

**Not a plant target.** `_divide_exacte`'s remainder check cannot fire
on correct code (the divisions are exact by theory); it guards against
a future bug in the recurrence, and plant "recursio d_i" shows it does
turn such a bug into FALSUM.

**Memory.** One scratch arena per public call, destroyed at the end;
only coefficients and norms are transcribed into the caller's arena.
`reticulum_apex_officinarum` records the scratch peak. The inner
products for a basis allocate in the scratch arena without rewinding
(O(n^2 m) small values); fine at T1 sizes, revisit with T2's
measurements. Added to the `venenum` gate (1,591 checks under poison
and ASan/UBSan, clean).

**Glossary.** `orthogonalizo` (verb, imperative `orthogonaliza`) and
`orthogonalis` added as Neo-Latin entries.

## 2026-10-13 - T2: integral LLL (basis and form on one core)

**Algorithm.** Cohen, Algorithm 2.6.7 (de Weger's integral LLL), run on
the current Gram matrix G and the transformation u, never on the basis
itself. `Reductio` holds G (n x n), u (n x n), d_0..d_n and the
integers lambda_kj, all as Magnus. RED(k, l): q = floor((2 lambda + d) /
(2 d)) with d = d_(l+1), skipped when 2 |lambda| <= d (so mu = 1/2
exactly is NOT reduced, the same boundary the T1 judge uses). SWAP:
B = (d_(k-1) d_(k+1) + lambda^2) / d_k, then Cohen's updates of the
lambdas left of k-1 (plain exchange) and below k (the exact-division
pair). The Lovasz test is the T1 inequality, in integers. After the
loop, the basis variant returns reducta = u * basis (matrix_multiplica:
an independent product, not the maintained G), the form variant returns
the maintained G as `forma_reducta`. The two variants share everything
except the source of the inner products (`Fons`), so the form variant on
B B^T returns the same u as the basis variant on B (tested).

**Checked against Python first.** `scratchpad/reticulum/lll.py`
(Fractions, textbook LLL in the same order of operations) agrees with the
C on the outputs AND the swap and reduction counts. The common example
[1,1,1; -1,0,2; 3,5,6] at delta 3/4 gives [0,1,0; 1,0,1; -1,0,2] in 2
swaps and 3 reductions; the knapsack lattice (summa partium) needs 216
swaps and recovers the hidden subset.

**What the tests check.** Pinned outputs only for the hand cases. For
60 random bases (dimension 1-6, three entry scales, delta 3/4, 99/100
and 1) the checks are lattice invariants that hold for ANY correct LLL:
u * basis = reducta; |det u| = 1 (Bareiss); the same lattice (equal
Hermite forms, matrix.h); reducta judged reduced by the T1 judge. The
form variant: same u as the basis variant, E8 Cartan matrix (reduced
form has 2 on the diagonal), A4.

**Memory: compaction.** Without care, every Magnus temporary of every
step stays in the scratch arena: n = 20 with 10^25 entries peaked at
8.9 MB. `_reductio_curre` now transcribes the live state (G, u, d,
lambdas) into a fresh arena when usage passes a limit (1 MB at first;
when the live data alone fill more than half of it, the limit becomes
twice the live size), so the peak follows the size of the live numbers.
Measured: 1.1 MB peak, and about 10x faster (less to touch). The test
bounds the peak at 3 MB, and the plant "compactio nulla" is red on that
bound alone.

**Crash only in the house build (exit 139).** Compaction copies d_0..d_n,
but `_reductio_para` had only set d_0..d_k for the k reached so far; the
rest were uninitialized pointers that magnus_transcribe followed; only
the house build crashed on them. Fix: initialize every d to 0
in `_reductio_para`. Lesson: a struct the compactor copies wholesale must
be fully initialized at birth, not lazily.

**Unsigned loop index.** `for (l = k - 2; l >= 0; l--)` with i32 l
(UNSIGNED in this house) never ends; it is s32 now.

**Plants: 12, all red.** Rounding (floor instead of nearest), the
size-reduction boundary (< instead of <=), the sign in the new G_kk, u
not reduced, lambdas left of k-1 not swapped, d_k not updated on a swap,
the RED loop over l < k-1 skipped, compaction dropping u, no compaction,
reducta = basis, swaps not counted, form variant returning u instead of
G. Two plants first errored, not survived: a multi-line anchor with a
one-line replacement, which planta refuses (it would lose the layout);
re-run with replacements of the same line count.

**A plant that hung.** "d_k not updated" made LLL loop forever. That is
red in substance, but silva.planta has no timeout and waited for two
hours until I killed the test binary by hand. Fix in the suite, not in
the tool: each LLL section (hand, random, knapsack, form, memory) runs
through `_sub_termino`, which runs it first in a forked child under
CREDO_NON_PENDET (30 s; all LLL sections together take ~0.25 s) and
then again in the parent, which counts, ONLY if the child passed. My
first version (one "non pendet" section running everything in a child,
the normal sections after it) still hung: the child was killed and the
failure recorded, and then the parent's own sections looped. The
"only if the child passed" step is the whole point; probatio_matrix
does the same around its normal forms. Now the plant is red in about
3.5 minutes, most of it compiling plus 30 s per section.
Lesson: any loop whose termination depends on the algorithm being
right (LLL's potential argument) needs a test-side time bound, and a
bound that records a failure and then runs the loop anyway is not a
bound.

## 2026-10-13 - T3: enumeration (integer Fincke-Pohst), the minimum

**Algorithm, without fractions.** Reduce the form with T2's LLL
(delta 99/100), then recompute the integral Gram-Schmidt of the reduced
form with T1's `_orthogonaliza` (rather than reusing LLL's internal
lambdas: one less invariant to trust). With B_k = d_(k+1)/d_k and
mu_jk = lambda_jk / d_(k+1), each term of
Q(x) = sum_k B_k (x_k + sum_(j>k) mu_jk x_j)^2 is t_k^2 / (d_k d_(k+1))
with t_k = d_(k+1) x_k + s_k, s_k = sum_(j>k) lambda_jk x_j, all
integers. Scaling the remaining budget by F_k = prod_(i>=k) d_i d_(i+1)
gives an integer recurrence: N_n = R, N_k = N_(k+1) d_k d_(k+1) -
t_k^2 F_(k+1), and x_k is admissible iff N_k >= 0. At a leaf,
Q(x) = R - N_0 / F_0, and that division must be exact (a free internal
consistency check: a remainder means a bug, and returns FALSUM). The
denominators F_k do not depend on the path, so the numbers stay a fixed
size per level. Checked first in Python (`scratchpad/reticulum/enum.py`)
against brute force on 300 random forms and the E8 theta series.

**Admissible x_k.** The admissible set is an interval around
-s_k / d_(k+1); if the nearest integer is not admissible, none is. So
the search goes up from the rounded center, then down from center - 1,
each until the first failure; no square roots at all.

**Counting modulo sign.** While every higher coordinate is zero, x_k
is restricted to >= 0, and the all-zero leaf is skipped. Each pair
{v, -v} is visited exactly once, in the coordinates of the reduced
basis. The output (ambient vector for a basis, coefficients in the
GIVEN form's coordinates for a form) is a linear bijection of x, so the
pairing carries over; the stored representative is then flipped to its
canonical sign (first nonzero coordinate positive).

**Budget and completeness.** A node is a feasible partial assignment.
The budget is checked after feasibility and before counting the node,
so a tree of exactly `limes_nodorum` nodes completes. A truncated run
says so (`completa = FALSUM`); its count is a lower bound and its
stored vectors are the canonically first AMONG THOSE FOUND, which the
header now says. Node counts depend on the reduced basis (an
implementation detail), so there is no node-count field in the API and
the tests only check monotonicity and completion.

**Memory.** Each level marks the arena and rewinds after each x_k; x_k
itself is allocated BEFORE the mark (after a rewind, x + 1 is allocated
at the old mark, below the next one), so the values deeper levels read
stay alive. Measured on Z^6 scaled by 10^10 in 60 columns (norms
~10^21, so every node allocates): counting 11,896 vectors peaks at
~35 KB; without the rewind it is ~24 MB. Stored vectors live in their
own arena, in a sorted array (binary search, then shift), capacity
doubling up to the limit. An evicted entry's vector slot is reused for
the newcomer. Only the digit arrays of values beyond s64 are left
behind. I first wrote a T2-style compaction for this arena, but
measured
insertions are only ~2.5-3x the limit (Python, Z^2 and Z^3; the early
norm test rejects most leaves), so compaction never fired in any test
and a plant disabling it could not go red. Removed; recycling instead,
measured: 50 stored, ~78 KB with it, ~286 KB without.

**Tests (7,519 checks with T1-T2).**
- An independent brute force: every y in a box, with the exact box
  bound y_i^2 <= R (G^-1)_ii = R det(G without i) / det G (Cramer,
  Bareiss), no Gram-Schmidt, no reduction. 180 random cases (forms and
  ambient bases, dimension 1-4) compared vector for vector in canonical
  order; the regime is counted and asserted: 54 cases with equal norms
  (where the lexicographic tiebreak matters), 45 with >= 5 vectors.
- Root systems from Cartan matrices: A_1..A_8 n(n+1)/2, D_4..D_8
  n(n-1), E6 36, E7 63, E8 120; E8 at R = 4 and 6: 1,200 and 4,560
  (theta series 240, 2160, 6720, halved).
- Lattice, not basis: enumeration of B and of U B (U a product of 40
  random row operations) gives identical vectors, norms and counts.
  My first generator drew a new multiplier per COLUMN, which is not a
  row operation and not unimodular: the test failed on correct code.
- Limits: storage 0, 1, 3, 121 = prefixes of the full list; budget 1 is
  incomplete; doubling budgets give non-decreasing lower bounds that
  end at the true count.
- The minimum against brute force (59 cases) and T2's deferred check:
  (4p - q)^(n-1) |b_1|^2 <= (4q)^(n-1) lambda_1^2 for delta = p/q in
  {3/4, 99/100, 1}, 60 bases.
- Refusals with outputs untouched; memory bounds as measured above.
- All T3 sections run under `_sub_termino` (child first, 30 s).

**Plants: 15, all red, one only after a new test.** Center sign; the
budget recurrence with F_k for F_(k+1); F without d_(k+1) (the exact
leaf division turns it into FALSUM); sign restriction removed; the
"all higher zero" flag propagated wrongly; zero vector counted;
completa always true; canonical sign inverted; lexicographic order
reversed; output without u; minimum radius one too small; no per-level
rewind (memory bound); evicted slot not reused (memory bound);
negative radius accepted. The survivor: budget `>` instead of `==`
(one node too many). Nothing pinned the exact boundary: budget 1 was
incomplete either way and the doubling loop only checked monotonicity.
Fix: the lattice [1] at R = 9, whose tree is exactly x = 0, 1, 2, 3 (4
nodes, the zero leaf not counted): budget 4 completes with 3 vectors,
budget 3 does not and counts 2. This pins the documented node
definition too.

**Glossary.** `cartan` (Elie Cartan, Cartan matrices in the tests).

## 2026-10-13 - T4: integer relations

**Thin by design.** `reticulum_relationes` = `matrix_reticulum_nuclei`
(Hermite with certificate, matrix.h v2: a basis of the FULL kernel
lattice, so every integer relation is an integer combination, not only
every rational one) -> transpose to rows -> T2's LLL at the requested
delta. An empty kernel returns 0 x n (n = columns of a), not 0 x 0.

**Scope addition: Q as well as Z.** Each row of a rational matrix is
multiplied by the lcm of its denominators (the kernel is unchanged), so
callers can pass exact coordinates with halves (2O's points, T6)
without scaling by hand. The header's refusal list says "Z (relationes:
Z or Q)". Every other ring is refused.

**Tests: no relation oracle needed.** Four judgments:
1. every row is a relation (a r^T = 0);
2. n - rank(a) rows (Bareiss);
3. reduced at the requested delta (T1's judge);
4. SATURATED: every relation found independently lies in the output
   lattice, by Hermite membership (Hermite of r stacked with v equals
   Hermite of r plus a zero row).
Independent relations: a brute force over [-2, 2]^n on 40 random
matrices (3,126 relations), and the 2O catalog. The 24 points up to
sign are embedded exactly in (Z + Z sqrt2)^4 -> Z^8, scaled by 2.
Every signed pair sum that coincides with another, or with its
negative, gives a relation: 216 in all, every one in the rank-16
output lattice. Hand cases:
- the minimal polynomial of sqrt2 + sqrt3 (t^4 - 10 t^2 + 1) from its
  powers in the basis (1, sqrt2, sqrt3, sqrt6);
- the powers 0..4 of 1 + sqrt2, whose shortest relation is
  (0, 0, 1, 2, -1) = -t^2 (t^2 - 2t - 1), norm 6 (Python brute force),
  found with T3's `reticulum_minimum` on the output;
- Q inputs; no rows (Z^n entire); full rank; no columns.

**My test bug, again an ordering artifact.** I pinned "172 equal pair
sums" from Python, and C counted 166 (+50 opposite against Python's
44). Both have the same point set, but C orders the 2T block
differently. A signed pair (a < b, a's sign +) represents p_a + s p_b,
so reordering turns some p + q into q + p and some p - q into
-(q - p): the equal/opposite split depends on the order, the total
(216) does not. Now the total is asserted. Lesson: before pinning a
count from an oracle, ask whether it is invariant under the choices
(order, sign representatives) that the two implementations make
differently.

**Plants: 7, all red, one only after a new test.** Kernel from
`matrix_nucleus` (rational, not saturated: the sqrt2 + sqrt3 relation
came back as a multiple); no LLL; Q refused; lcm of the first column
only; kernel not transposed; empty kernel with 0 columns; delta
ignored. The survivor was delta ignored: small random kernels reduced
at 99/100 are almost always reduced at 1 too, so "reduced at delta"
could not tell. Fix:
- the composition is now asserted: the output equals
  `reticulum_reduce(kernel^T, delta)`;
- the regime is counted: the delta 3/4 and delta 1 outputs differ for
  3 of the 40 random matrices, and the test asserts > 0 (the plant
  makes them always equal).

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

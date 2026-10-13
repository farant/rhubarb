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

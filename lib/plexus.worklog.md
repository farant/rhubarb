# plexus worklog

## 2026-10-15 — T1: words and the Artin action

Plan: `project-specs/plexus-plan.md` (approved by Fran 2026-10-15).

**Names.** The plan's sketch called the free-group action
`plexus_artin`. The Latin lint does not know `artin` or `burau`
(proper names are not words), so identifiers avoid them: the action is
`plexus_imago_libera` (the image of a free generator). Proper names
stay in comments. T3's Burau functions need the same treatment.

**Convention.** phi_p = phi_{a_1} o ... o phi_{a_m}, so phi_{pq} =
phi_p o phi_q. The library computes it left to right by keeping the
images W_k of all generators and updating two of them per letter:
- sigma_i:    W_i' = W_i W_{i+1} W_i^-1,  W_{i+1}' = W_i
- sigma_i^-1: W_i' = W_{i+1},  W_{i+1}' = W_{i+1}^-1 W_i W_{i+1}

The test's oracle applies the same automorphism the other way, by
substituting letters from the right (phi_{a_m} first). 1,203 images
agree. With this convention Delta^2 acts as conjugation by
x_1 x_2 ... x_n (checked on B_3 for Delta^2 and Delta^4).

**Equality.** `plexus_aequales` reduces r = p q^-1 and tests two exact
necessary invariants first: the exponent sum (a homomorphism to Z) and
the permutation. Only then does it compute phi_r, which is faithful
(Artin), so the answer is exact either way.

**Growth.** Free-word length under (sigma_1 sigma_2^-1)^m is 3, 7, 17,
43, 111, 289, ... (observed: a_{m+1} = 3 a_m - a_{m-1} - 1), so the
ratio tends to (3 + sqrt 5)/2. That is the braid's dilatation
(Thiffeault 2010, Sect. III B): word growth under a pseudo-Anosov
automorphism has the same rate. The test checks the ratio within ~0.5%
exactly in integers (|a_{m+1}^2 - 3 a_{m+1} a_m + a_m^2| < a_m^2/100,
m = 8..12). m = 20 exceeds a limit of 10^8 letters: the Artin action
is exact but exponential, so it is not the tool for long braids. That
is the Dynnikov action's job (T2).

**Memory.** Each letter allocates new words and leaves the old ones as
garbage, so a long word whose images stay small (e.g. (s1 s1^-1)^50000)
used to grow the pool without bound. The image pool is compacted (live
words copied to a fresh pool, the old one destroyed) whenever its use
exceeds 4x the live words + 64 KB. `plexus_aequales` uses the caller's
pool as scratch with notare/reficere, and leaves it as it found it
(tested).

**Plants (silva.planta, 15).** 14 red. One survivor, an equivalent
mutant: `plexus_ex_chorda` accepting a letter equal to `fila` changes
nothing, because `plexus_creare` refuses the letter anyway. Two first
versions of tests were too weak, both found by plants:
- compaction was never triggered, so a compaction that copied the wrong
  image survived. Added the "contractio" section: a long trivial word
  forces compaction, and (s1 s2)^900 = (Delta^2)^300 must give exactly
  u^300 x_k u^-300 (built by the test's own reducer);
- the running-total limit was masked by the per-product limit. Added a
  test with the limit exactly one below the true total.

My own first guesses that the tests caught: (sigma_1 sigma_2^-1)^20
fits in 10^8 letters (it does not: about 2 x 10^8); the commutator
s1 s2 s1^-1 s2^-1 grows linearly (it is pseudo-Anosov, exponential);
the images of Delta^2m all have length 2*3m + 1 (x_3's is shorter
after free reduction).

venenum: probatio_plexus clean under ASan/UBSan (plexus added to the
math-tier suites).

## 2026-10-15 — T2: the Dynnikov action on loops

**Source, checked twice.** Thiffeault 2010 (arXiv:0906.3647) prints the
update rules as equations (8)-(15) and again as Matlab in appendix A.2
(`loopsigma`, `loopinter`). I compared the two line by line before
coding: they agree, including the sigma_1 / sigma_{n-1} boundary cases
and all four inverse rules. The library follows the equations; the test
re-types the Matlab listing literally in s64 (`_regula_fontis`,
`_regula_sectionum`) as its oracle, on 2,000 random (vector, word)
pairs and on L(u).

**Conventions.** The source's sigma_i is a CLOCKWISE exchange (Birman's
is counterclockwise), and a word acts on a loop from left to right.
Neither matters for equality: mirroring (sigma_i <-> sigma_i^-1) is an
automorphism and reversal an anti-automorphism of B_n, and both
preserve equality. A wrong rule breaks the braid relations, which the
"ansae relationes" section checks on random vectors (half of them
scaled by 2^70).

**The loop E_k** around punctures k, k+1: crossing numbers nu_k = 2,
all others 0, so a = 0, b_k = 1, b_{k-1} = -1. L(E_k) = 2 for every
n = 3..8. The paper's Fig. 8 vector (n = 5) gives L = 12.

**Which disk: settled exhaustively.** Every braid word up to length 6
on B_3 (5,461 words), 5 on B_4 (9,331), 4 on B_5 (4,681), grouped by
the Artin images and by the images of E_1 .. E_{n+1} on D_{n+2}
(central punctures): the partitions are identical (577, 1,579 and
1,143 classes, no disagreement in either direction). On D_n itself the
loops see fewer classes (378 for B_3 <= 6), and Delta^2 fixes every
E_k on D_n but not on D_{n+2}, as expected: on D_n the full twist is a
boundary twist and closed curves cannot see it. `plexus_aequales_ansis`
therefore works on D_{n+2}.

A plant showed that E_{n+1} is redundant within the tested range: the
family E_1 .. E_n gives the same partitions. The library keeps the full
family (one extra loop, cheap); I have no proof that the smaller family
is faithful in general.

**Growth.** L(u) under (s1 s2^-1)^m grows with ratio -> (3 + sqrt 5)/2
(checked exactly in integers for m = 10..20; beyond m ~ 22 the check's
squares overflow s64, which UBSan caught in the first version of the
test). The paper's 4-strand braid s3^-1 s2^-1 s3^-1 s2 s1 grows with
ratio 2.2966 = e^0.83144 within 0.1% (m = 20..30), matching the
paper's entropy 0.83144. (s1 s2)^3 = Delta^2 leaves L = 2 on D_3.
(s1 s2^-1)^3000 gives a 4,166-bit L (predicted 3000 log2 2.618 =
4,165.2) and its inverse returns E_1 exactly.

**Memory.** `ansa_agere` computes in its own pool and transcribes the
result into the caller's (tested: < 4 KB left behind for a 400-letter
word with 280-bit results). Long words with big coordinates compact the
pool every 256 letters once it exceeds 1 MB. The first compaction test
(1,000 periods, n = 3) never reached the threshold, which a plant
showed; 3,000 periods does.

**Plants (16).** 15 red, 1 equivalent mutant within the tested range
(the E_{n+1} omission above). Survivors of the first run, all fixed by
stronger tests: L without the max in b_0 (only hand vectors tested L:
now the loopinter oracle on random vectors), compaction (threshold not
reached), `ansae_aequales` ignoring b (no test compared loops that
differ only in b).

## 2026-10-15 — Review of T1 and the fixes

An independent reviewer (recensor-plexus-t1) reviewed eda26fc7 with
two Python oracles written from the definitions: the Artin action,
built as an explicit right fold, and the unreduced Burau representation
over Z[t, t^-1] (faithful on B_3). It compared 345,888 images and
1,827,901,184 equality verdicts, including every pair of words on B_3
up to length 7 and on B_4 up to length 6: 0 disagreements. The limit
contract matched exactly on 800 words, and the caller's pool was never
moved. Findings: H 0, M 3, L 8, N 6, all acted on (summary in the
session scratchpad, recensio_plexus_t1/findings_summary.md).

- **M1 (cliff):** compaction compared pool use against 16 bytes per
  live letter and left out the fixed cost of about 24 bytes per strand.
  Above ~8,192 strands every letter compacted (12 ms per letter at
  2^20). It now compacts when the pool has grown 4x since setup or the
  last compaction (`basis`), which is amortized O(1) per allocated
  byte. Test: 20,000 strands with 1,000 letters, 0 compactions (was
  1,000). The same rule now drives the loop pool.
- **M2 (test gap):** almost every "unequal" test was decided by the
  exponent-sum or permutation prefilter, so comparing only phi(x_1)
  survived 882 checks and the exhaustive B_3 run. The witness is
  s2^2 s3^-2 in B_4. Added: that witness, s1^2 s3^-2 in B_5, random p
  against q.c for pure c with exponent sum 0 (n = 4..6, both equality
  methods), and a pool test whose r really runs the action.
- **M3 (test gap):** plexus_creare's own upper letter bound was never
  tested, because the parser checked first. Now tested directly. Since
  L3, plexus_ex_chorda no longer goes through plexus_creare, so its own
  check is tested too (the T1 "equivalent mutant" is no longer
  equivalent).
- **L1:** the limit boundary was tested on one side only. The test now
  computes the minimal limit L* from the header's own definition
  (fila; each product length before reduction; the image total after
  each letter) and checks VERUM at L* and FALSUM at L* - 1, on 200
  words.
- **L2:** when r reduces to the empty word, the answer is "equal" with
  no limit.
- **L3:** plexus_ex_chorda builds the braid on its parse buffer, with
  nothing else left in the caller's pool.
- **L4:** the action's own pools use piscina_conari_allocare, so
  running out of memory there returns FALSUM. The caller's pool and
  Magnus still exit, as everywhere.
- **L5:** both inclusive bounds of PLEXUS_FILA_MAXIMA are tested.
- **L6:** `plexus_contractiones()`, a process-global counter, lets tests
  assert that compaction happens and that it is rare.
- **L7:** header wording: the permutation law, and what the parser
  accepts.
- **L8:** `plexus_validus`. Every action refuses a hand-built braid
  with an invalid letter, and `plexus_permutatio` now returns b32.

From the API notes: `plexus_imagines_liberae` returns all n images (N3
needs them); the header states the action directions (F_n: left,
phi_{pq} = phi_p o phi_q; loops: right, u.(pq) = (u.p).q), and both are
tested; there is a warning about unsigned reverse loops; and
plexus_aequales is named as the reference, with plexus_aequales_ansis
as the faster method.

Plants: the reviewer's 11 surviving mutants, re-planted against the new
suite, are all red.

## 2026-10-15 — T3: reduced Burau, Alexander, the Burau(-1) bound

**Names.** Fran approved adding the proper names to the glossary.
`burau` and `artin` follow the house form for mathematicians
(hopf, galois, cauchy, ...): `ignotum-permissum` with a nota "nomen
proprium: ...". The lint accepts them in identifiers, and oratio's
Latin lexicon is left alone. T1's `plexus_imago_libera` keeps its
descriptive name.

**Convention.** These are the transposes of the common textbook
matrices: sigma_1 -> [[-t, 0], [1, 1]], sigma_{n-1} -> [[1, t],
[0, -t]], middle [[1, t, 0], [0, -t, 0], [0, 1, 1]]. The braid
relation was checked by hand for B_3 before coding: ABA = BAB =
[[0, -t^2], [-t, 0]]. Transposition changes neither determinants nor
characteristic polynomials, so the Alexander formula and the spectral
data are the same. Multiplying on the right by a generator changes one
column only, so each letter costs O(n) polynomial operations, not
O(n^3).

**Oracles, all green:**
- the hand matrices of every generator and its inverse (n = 2, 3, 4);
- the relations, B(pq) = B(p) B(q) on random words, and det B(p) =
  (-t)^(exponent sum), an independent theorem;
- the exhaustive partition: Burau groups B_3 words up to length 6
  exactly as Artin does (577 classes, faithful). B_4 up to length 5
  also gives 1,579 = 1,579, so Burau is not seen to be unfaithful at
  that length (it is known to be unfaithful only for n >= 5);
- B(-1), computed directly in integers, equals B(t) evaluated at t =
  -1 on random words;
- **Alexander against the knot table**, a fully independent route: the
  table's polynomials come from `laqueus` on KnotInfo PD codes. All
  eight closures match: 3_1 (s1^3), 4_1 ((s1 s2^-1)^2), 5_1 (s1^5),
  and five words I gave from memory, 5_2, 6_1, 6_2, 6_3 and 7_1. A
  match on a specific polynomial confirms the word as well as the code.
  Also checked: Hopf link 1 - t, split link 0, unknot 1, and
  invariance under conjugation and stabilization (Markov) on 100
  random braids;
- the radix: s1 s2^-1 gives (3 + sqrt 5)/2 exactly, (s1 s2^-1)^2 the
  root of t^2 - 7t + 1, and s1 s2 gives 1. In B_3, B(-1)(Delta^2) = -I,
  so s1 s2^-1 Delta^2 has negative eigenvalues, and the chi(x) chi(-x)
  construction still returns (3 + sqrt 5)/2. On 18 random
  pseudo-Anosov B_3 braids the radix matches the loop growth rate
  L_40/L_39 within 1%.

**Limitation.** For the paper's 4-strand braid (dilatation ~2.2966),
the largest REAL eigenvalue of B(-1) is 1: the spectral radius must
come from complex eigenvalues. `plexus_burau_radix_maxima` is honest
about this. It is max(1, max |real eigenvalue|), which is still a lower
bound, but there it is uninformative. An exact complex spectral radius
(modulus of algebraic numbers) would be its own slice, if a demo needs
it.

**Memory.** Both Burau computations compact their pools: 14
compactions for B(w w^-1) with w = (s1 s2^-1)^300, whose polynomials
reach degree ~600 and come back exactly to I.

**Plants (13).** 12 red. One equivalent mutant in practice: dropping
the floor of 1. Among 20,000 random words on B_4..B_6, none had all
real eigenvalues below 1 in absolute value. That is consistent with
real eigenvalues coming in pairs (lambda, 1/lambda), because B(-1)
preserves a bilinear form (recalled, Squier), though that is not proved
here.

## 2026-10-15 — T4: entropy by exact growth of L(u)

`plexus_incrementum(p, u, periodi)` returns the exact sequence
L(u . p^m) for m = 0 .. periodi, in the caller's pool. Each step runs
in the other of two scratch pools, and the previous pool is reset, so
memory follows the live coordinates. For 200 periods of s1 s2^-1 (a
279-bit L), it leaves +8.5 KB in the caller's pool.

**Measured before writing the tests** (scratch program, not committed):
- reducible: s1 on E_2 gives L = 2, 4, 6, ..., exactly 2 + 2m (linear,
  a twist);
- periodic: (s1 s2)^3 = Delta^2 keeps L = 2;
- across 25 random braids on B_3..B_5, the Burau(-1) radix never
  exceeds the growth max_k (L^(k)_40 / L^(k)_30)^(1/10) over the loops
  E_k, and agrees with it to 4 digits whenever it exceeds 1. Several
  braids have radix 1 but grow at 2.2966 or 2.618, the complex
  eigenvalue (or Burau kernel) case already seen in T3. Loop growth is
  the primary entropy tool; the Burau bound is a check.

**Oracle 6 in the suite:**
- the sequence equals manual iteration;
- the ratio approaches (3 + sqrt 5)/2 (exactly, in integers) and the
  paper's 2.2966 (within 0.1%);
- linear L = 2 + 2m for the twist, and a constant 2 for the periodic
  braid;
- on 40 random braids on B_3..B_5 (16 with radix > 1), radix^10 <=
  (101/100) max_k L^(k)_40 / L^(k)_30, with an exact fraction check
  (radix narrowed to 10^-6, lower endpoint to the 10th power): 0
  failures.

A single loop gives only a LOWER bound on the growth, since a reducing
loop does not grow. The test therefore takes the maximum over E_1 ..
E_{n-1}, and the header says so. The exact dilatation (Hall and
Yurttas train-track matrices) stays deferred, as Fran decided.

**Plants (4):** all red: the loop not advancing, the wrong scratch pool
reset, an extra application of p, and the last value dropped.

## 2026-10-15 — T5: the full review and the fixes

An independent reviewer (recensor-plexus-t5) reviewed b0636f72 with
oracles written from the papers and definitions. Every comparison
showed 0 disagreements:
- the Dynnikov rules typed from Thiffeault's equations: 25,000 pairs,
  values up to 2^200;
- loop equality against its own Artin oracle: exhaustive partitions on
  B_3 up to length 8, B_4 up to 6 and B_5 up to 5, plus 234,450 library
  verdicts, half of them near misses;
- Burau, confirmed to be exactly the transpose of the textbook
  convention on reversed words: 1,500 words;
- Alexander against Fox calculus, a route that does not use Burau: 400
  knot closures and several links. The table also matches for 8_19,
  9_1, 10_124 and 8_20;
- the radix against Sturm sequences: 2,110 braids;
- incrementum: 600 cases. The paper's entropies were reproduced to 6-12
  digits.

Findings: H 0, M 3, L 6, N 5 (summary in the session scratchpad,
recensio_plexus_t5/findings_summary.md). What changed:
- **M1:** plexus_aequales_ansis now has the same structure as
  plexus_aequales: an empty r answers at once, the exponent sum and
  permutation are checked first, and each loop runs under its own pool
  mark. Memory is O(n), down from O(n^2) (about 860 MB at n = 4000).
  Test: the caller pool's peak stays under 8 MB at n = 4000, with
  r = s_3997^2 s_3999^-2. The first version of this test used
  s1^2 s3^-2, which moves E_1, so equality stopped after one loop and
  the O(n^2) mutant survived. That was caught by a plant.
- **M2:** the loop pool is inspected after every letter, with threshold
  4·basis + max(1 MB, 4·numerus·sizeof(Magnus)), so the O(n)
  transcription is amortized. At D_65536 with (s1 s2^-1)^15000 there
  are 34 compactions and a 6.3 MB peak, against 776 MB before.
- **M3:** invalid braids are tested on all four Burau functions, and
  their outputs are checked to be untouched. Removing validus from
  alexander or the radix is an equivalent mutant: both call the Burau
  function first, which refuses.
- **L1:** the Burau scratch matrix lives in its own pool and moves at
  each compaction. Only the result is left in the caller's pool: at
  n = 200, exactly 199^2 x 24 bytes.
- **L2:** `ansa_valida`, checked at the entry of every loop function
  (sectiones returns 0, ad_chordam returns "").
- **L3:** the undocumented refusals are now in the header.
- **L4:** tests for (good p, bad q) in both equality functions, the
  upper bound of ansa_ex_coordinatis, the periodi guard, and
  incrementum's strand check at periodi = 0. One gap remains, by cost:
  accepting exactly PLEXUS_FILA_BURAU (4,096) would need about 400 MB
  for the matrix.
- **L5:** `plexus_apex_proprius()` / `plexus_apicem_purgare()`, a
  high-water mark of plexus's own pools, recorded whenever one of them
  is destroyed or reset (all of them go through two helpers).
  incrementum over 1,000 periods peaks at ~1.4 KB, and a pool that is
  never reset would reach ~250 KB.
- **L6:** header wording. B_3 faithfulness is cited, with agreement
  checked exhaustively up to length 6; the Kolev bound is cited and
  compared.
- **N1:** smaller loop families (without E_1, or on D_{n+1}) also
  reproduce Artin's partition in the reviewer's exhaustive runs. The
  reviewer recalled a proof sketch (the stabilizer of the chain is the
  center, and B_n meets <Delta^2_{n+1}> trivially). It is not verified
  here, so the full family stays and the header states only the
  observation.

Plants: the reviewer's surviving mutants plus the reversals of the
fixes, 13 in all. 11 are red; 2 are equivalent (the validus checks in
alexander and radix are redundant behind Burau's own check).

## 2026-10-16 — T3b: an exact Burau certificate at roots of unity

This was prompted by the D128 review (M3). On B_4, 1,008 braids up to
length 6 have measured exponential loop growth, yet Burau(-1) is blind
to them: its characteristic polynomial for s1 s3^-1 s2 is (x-1)^3. The
reviewer found numerically that Burau at t = e^{2 pi i/3} has spectral
radius >= 1.957 on all of them. Kolev's bound (radius of B(zeta) <=
dilatation for |zeta| = 1; restated in Band and Boyland 2007) then
certifies positive entropy. Fran approved doing it exactly in plexus.

`plexus_burau_radius_maior(p, ordo, &maior)`, ordo 2..12:
- B(t) is reduced into Z[zeta_ordo] with `cyclotomia`.
- Restriction of scalars gives an integer matrix R of size
  (n-1) phi(ordo), whose eigenvalues are those of B(zeta^k) for every
  Galois conjugate. Kolev holds at each one, so the certificate is
  "some primitive root of that order".
- chi_R is monic with constant term +-1 (det B = (-t)^e is a unit).
- Kronecker (1857): a monic integer polynomial whose roots all lie in
  the closed unit disk is a product of cyclotomic polynomials. So after
  every Phi_k with phi(k) <= deg is divided out (k <= 2 D^2 + 2, since
  phi(k) >= sqrt(k/2)), a remainder of degree > 0 means some root has
  modulus > 1.
- The dimension limit is D = (n-1) phi(ordo) <= 22, so that every
  needed Phi_k lies within cyclotomia's range (k <= 1000).

Tests:
- Zero-entropy braids give FALSUM at orders 2..6 (the contrapositive of
  Kolev): delta, epsilon, Delta^2, the twists s1 and s1^5, and s1 s3.
- s1 s3^-1 s2 is FALSUM at order 2 and VERUM at order 3, and so is
  Thiffeault's braid at order 3.
- On 150 random braids on B_3/B_4: order 2 equals
  `plexus_burau_radix_maxima` > 1 (on B_4 because p(1) = 0 always,
  the review's lemma), and the certificate always implies exponential
  loop growth. 8 of the 150 are certified only at order 3.

A mistake of mine the test caught: I listed s2^2 s1^-2 as zero entropy.
It is twists about intersecting curves with opposite signs (Penner),
which is pseudo-Anosov. Its loops grow to ~10^23 in 30 periods, and it
is certified at orders 2 and 5. It is now a positive example.

5 plants, all red: a single division in place of repeated division,
the realification without zeta^j, the diagonal x misplaced, a
Kronecker range of k <= 2, and maior always VERUM.

## 2026-10-16 — plexus_nervus (nervus-plan T3)

`plexus_nervus(p)` emits the Dynnikov action of a word as a nervus
network: 2n - 4 inputs and outputs (a_1..a_{n-2}, b_1..b_{n-2}), no
biases. Each letter follows Thiffeault eq. 8-15 with f- = f - relu(f),
the same programs D128's Part A typed (D128's code is frozen and
untouched; the emission here is separate and is checked against
`ansa_agere`, which is a third, direct max/min transcription). The two
middle cases (8)-(9) and (12)-(13) share one emission with a sign
`sg` (+1 / -1) and a switch `pos` (the c-term in b_p' and b_q' exists
only for positive letters); checked term by term against D128's two
programs before the first run. Locals mirror `ansa_agere`'s names
(a_prior, a_sequens, ...); comments keep the paper's a_p, b_q.

Instruction helper: `_nervus_instructio` takes five (source,
coefficient) pairs and returns the new register, or -1 once `bene` is
FALSUM. The first draft chained `bene && call(..., &t1)` and examen
rightly flagged reads of t1 when an earlier step had failed (harmless,
but an uninitialized read); returning the register removed them.

Tests (section "nervus", 18 checks; first written "17", corrected by the T4 review): 3000 random words, n = 3..8,
length 0..20, every fourth vector scaled by 2^200: network = ansa_agere,
0 discrepancies. Per letter, n = 3..8: end letters 2 ReLU / 4
instructions, middle 5 / 11; depth 4, ReLU depth 2; inputs and outputs
2n - 4. Empty word = identity network. nervus(p) o nervus(q) =
nervus(pq) on 200 random pairs. Refusals (fila 2, invalid letter,
NIHIL pool / output).

Width, measured: end letters 2n - 3, middle letters 2n + 2 (nervus's
definition: live registers, so the affine temporaries d and the
partial sum count). D128 stated "hidden width at most 2n + 1 (state
plus a middle letter's 5 ReLUs)" - a different count (ReLUs only, as
if the affine nodes were folded into the next layer), not a
contradiction; the test asserts nervus's exact values.

**D128's Fibonacci line was a print bug.** (s1 s2^-1)^20 on E_1 is
(63,245,986; -39,088,169) = (F_39; -F_38), and L = 331,160,282 =
2 F_41. D128 printed "(63245986; 331160282)" because `main.c:654`
calls `mstr()` twice in one printf and `mstr` returns ONE static
buffer, so the second coordinate showed L. Its exact check compared
the right values. Correction notes in D128's findings.md and plan.md;
the test asserts the true pair and L. Lesson: a helper returning a
static buffer must never appear twice in one printf - the value
printed is not the value checked.

8 plants, all red: the sign of b_q+ in d, the c/d switch always on,
sigma_1's a' = +b + (b')+, the sign of (b')- for sigma_{n-1}^-1, the
edge write-back swapped, the sign of d in w, the sign of w+ in a_q',
the sign of b_p+ in the partial sum.

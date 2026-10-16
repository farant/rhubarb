# Demo 128: Braids as exact ReLU networks; a growth census; an audit of D106's classification (plan)

Status: approved by Fran (2026-10-15), with all four proposals in "Open
for Fran": the scope B_3 up to 8 and B_4 up to 6; the D106 audit
included; the layer interpreter in the demo; Part D descriptive only.
Task T6 of
`project-specs/plexus-plan.md`. Proposal N1 in
`knotapel/demo-proposals.md` (ledger …AKY6Z7). First demo built on
`plexus`.

## Question

Three questions, in order:

1. **N1: is a braid word an exact, untrained ReLU network?** Each Artin
   generator acts on Dynnikov coordinates by a piecewise-linear update
   built from max(0, x) and min(0, x) (Thiffeault 2010, eq. 8-15).
   Written as an explicit program of affine operations and ReLUs, a
   braid word of length m on n strands is a network of depth O(m) and
   width 2n - 4, in which each layer touches at most 4 coordinates.
   The demo writes that program, evaluates it, and checks it against
   `ansa_agere` exactly.
2. **What does exact loop growth say about braids, by word length?**
   For every distinct braid up to a given length: is it periodic
   (decided exactly), of positive entropy (certified, or measured), or
   neither (zero entropy, not periodic)?
3. **Does D106's classification survive?** D106 sorted 4-strand braids
   into "pseudo-Anosov" and "periodic" using the characteristic
   polynomial p of reduced Burau at t = -1. Its rule: p(1) != 0 means
   pseudo-Anosov; otherwise |1 - tr| > 2 means pseudo-Anosov, else
   periodic. Its "pA fraction by depth" table (0% at depth 1 to 77.2%
   at depth 8) and its "Fibonacci" claim rest on that rule. KC 31 does
   not cover them: KC 31 audits only D106's sign hash.

**Not in scope:** computing Boolean functions with the braid network.
That is N2 (ledger …4291V2), which gets its own plan once D128 has
shown what the network is and how it behaves.

## Why the D106 rule is in doubt (before any run)

- **Burau(-1) is only a lower bound.** Its spectral radius does not
  exceed the dilatation (Kolev 1989; Band and Boyland 2007, cited in
  D106 itself). A braid can have positive entropy while every
  eigenvalue of Burau(-1) has modulus 1. plexus T3 and T4 found such
  braids: s3^-1 s2^-1 s3^-1 s2 s1 (Thiffeault's example, entropy
  0.83144) has largest real eigenvalue 1. Such braids are "periodic"
  under D106's second branch only if |1 - tr| <= 2; the first branch
  catches any with p(1) != 0.
- **"p(1) != 0 means pseudo-Anosov" is false as stated.** A periodic
  braid such as delta = s1 s2 s3 (delta^4 = Delta^2) has Burau(-1)
  eigenvalues that are roots of unity other than 1 (to be checked
  exactly), so p(1) != 0, and the rule calls it pseudo-Anosov.
- **Neither branch separates pseudo-Anosov from reducible.** A reducible
  braid with a pseudo-Anosov piece has positive entropy without being
  pseudo-Anosov. This demo does not separate those two either (see
  "Classes" below), and says so.

## Classes (exact where possible, measured where not)

For each distinct braid beta in B_n:

- **Periodic: exact.** Every periodic braid is conjugate to a power of
  delta = s1 ... s_{n-1} (delta^n = Delta^2) or of
  epsilon = s1 delta (epsilon^(n-1) = Delta^2) (Brouwer, Kerekjarto,
  Eilenberg; recalled, to cite properly in the findings). So beta is
  periodic iff beta^n or beta^(n-1) equals a power of Delta^2. The
  exponent sum fixes which power: beta^k = Delta^(2j) needs
  k e(beta) = j n (n - 1). That gives an exact test with
  `plexus_aequales_ansis`, cross-checked against `plexus_aequales`
  (Artin) on a sample.
- **Positive entropy, certified:** Burau(-1) radix > 1
  (`plexus_burau_radix_maxima`, exact; the lower bound is cited, as in
  plexus).
- **Positive entropy, measured:** not certified, but the loop growth is
  clearly exponential. The measured rate is
  lambda_hat = max_k (L^(k)_60 / L^(k)_40)^(1/20) over the loops E_k,
  all exact integers. The threshold is stated in the output, and the
  class is marked "measured".
- **Zero entropy, not periodic:** not periodic, and growth at most
  polynomial (measured, threshold stated). These are reducible braids
  with only periodic pieces, e.g. a twist (L = 2 + 2m).

Pseudo-Anosov and reducible-with-a-pseudo-Anosov-piece both fall under
"positive entropy". Telling them apart needs reduction curves or train
tracks, which are not in plexus (deferred).

## Parts

**A. The layer program (N1).**
- Each generator's update, written as a straight-line program over
  integer registers with three instructions:
  - affine (sum of registers times small integer coefficients);
  - relu (max(0, x));
  - copy.

  min(0, x) is written as x - relu(x).
- An interpreter in the demo runs the program on Magnus registers.
- Check: interpreter equals `ansa_agere` on random words and vectors,
  n = 3..8, including large values (exact). This is the demo's anchor.
- Report, per generator type (sigma_1, sigma_{n-1}, middle sigma_i, and
  the three inverses): ReLU count, affine operation count, depth, and
  registers touched. Then the shape of a word's network: depth = sum
  over letters, width 2n - 4, sparsity.

**B. Growth census.**
- Every braid word up to length l on B_3 (l = 8) and B_4 (l = 6),
  deduplicated exactly (key: loop images on D_{n+2}, plexus T2).
- Classify each distinct braid as above. Report by word length:
  distinct braids, periodic (exact), positive entropy (certified or
  measured), and zero-entropy non-periodic.
- For each braid also report the rate lambda_hat, the Burau bound, and
  their agreement where the bound is above 1. The bound must never
  exceed the rate, which is asserted.

**C. Audit of D106.** On the same B_4 braids:
- apply D106's rule exactly (characteristic polynomial of Burau(-1));
- cross-tabulate it against the classes of Part B;
- list every braid where the rule disagrees, in each direction, with
  the shortest examples.

Then check D106's specific claims:
- depth 1 is all periodic;
- the depth-2 "pseudo-Anosov" braids all have characteristic polynomial
  x^3 - 4x^2 + 4x - 1 and dilatation phi^2, i.e. entropy per crossing
  log phi;
- the pseudo-Anosov fraction rises with length.

D106's catalog was a dual breadth-first search with dedup on
(TL, Burau) matrices and a 32,768 cap, so its counts are not
reproducible here without its TL representation. The audit therefore
judges the RULE on an exhaustive word set, and the claims that do not
depend on the catalog.

**D. Network statistics (descriptive).** Along each braid's iteration,
record which ReLUs are active (the linear region) for each letter. For
positive-entropy braids, report whether the pattern per period becomes
periodic within the run. That is the ReLU-network view of Hall and
Yurttas's stabilization. It is reported only; no exact dilatation is
derived from it (see Out of scope).

## Checks (assert characterizations, not counts)

- Part A interpreter = `ansa_agere` on every tested input.
- Every word's class key equals its equality class: a sample of pairs
  checked against `plexus_aequales` (Artin).
- Periodic braids: the exact power test succeeds, the loop lengths
  stay bounded, and the Burau radix is 1 (consistency in both
  directions).
- Burau radix <= measured rate for every braid.
- delta = s1 s2 s3 and epsilon = s1 s1 s2 s3 are classified periodic.
  s3^-1 s2^-1 s3^-1 s2 s1 has positive entropy, with rate 2.2966
  (within 0.1%).
- Plants for each check (silva.planta), quick mode for the plants.

## Registered before running

- **P1:** the layer program equals `ansa_agere` exactly on every input.
- **P2:** D106's rule disagrees with the exact classes in BOTH
  directions on B_4 up to length 6: some periodic braid is called
  "pseudo-Anosov", and some positive-entropy braid is called
  "periodic". If either direction has zero cases, P2 fails for that
  direction and the findings say so.
- **P3:** the fraction of distinct braids with positive entropy rises
  with word length on B_3 and B_4.
- **P4:** the length-2 positive-entropy braids on B_4 have dilatation
  exactly phi^2: certified by Burau, since on these braids the bound
  should be tight.

How the outcome reaches claims.md:
- If P2 holds in either direction, D106's findings 3-5 get a
  [not-reproduced] or [falls] node with the exact counts, and the
  "pA fraction" is restated over the classes defined here.
- KC 31.2 (pseudo-Anosov braids do not compute better) is not touched:
  its classes would change, but re-running D106's computing comparison
  is outside this demo. A note says so.

## Size and time (estimate)

- B_3 up to length 8 has 87,381 words, and B_4 up to length 6 has
  55,987. Distinct braids: 2,589 and 5,121 (from the plexus T5
  review).
- Per distinct braid: growth over 60 periods on n - 1 loops, the
  periodic power test, and the Burau radix. About 1-5 minutes total;
  no workers needed.
- Quick mode `D128_CELER=1` (B_3 up to 5, B_4 up to 4) for the plants.

## Tasks (one per turn, Fran's go-ahead each)

1. **T6a:** Part A (layer program, interpreter, anchor, operation
   counts), with checks and plants.
2. **T6b:** Parts B and C (census, D106 audit), with checks and plants.
3. **T6c:** Part D, then findings.md, claims.md (new nodes for D106's
   classification), the ledger, and the freeze with archive.sh.
4. **T6d:** an independent review with its own oracle, then fixes.

## Out of scope

- The exact dilatation from the stabilized pattern (Hall-Yurttas
  train-track matrices). This is the deferred plexus T4 piece. It is
  general code, so it belongs in plexus, not in a demo; Part D only
  reports whether stabilization happens.
- Telling pseudo-Anosov from reducible.
- Computing with the network (N2).
- Re-running D106's Boolean-function comparison.

## Open for Fran

1. The census scope: B_3 up to 8 and B_4 up to 6 (proposed), or larger
   (B_5 up to 5 is 37,449 words, 4,743 classes).
2. Including the D106 audit (proposed: yes; it is the part with
   consequences for claims.md).
3. The layer-program interpreter lives in the demo (proposed), as
   scaffolding for counting. A general "logic to exact layers"
   compiler is N4's design question, for later.
4. Part D as descriptive only (proposed), with the exact dilatation
   left to a future plexus slice.

## Decisions made during T6a

- **Layer form.** Two instructions, AFFINE and RELU (the ReLU of an
  affine form, like handwired's gated ReLU), with f- written as
  f - relu(f). Each letter's program reads the old coordinates and
  writes new registers; the outputs are the next layer's neurons, so
  the final copy is not counted as an operation.
- **What a generator costs, as measured.** Every generator is depth 4
  with two ReLU stages. sigma_1, sigma_{n-1} and their inverses: 2 ReLU,
  2 affine, 2 coordinates touched. A middle sigma_i or its inverse:
  5 ReLU, 6 affine (one a_q' sum is split in two to keep at most five
  terms per instruction), 4 coordinates touched.
- **Checks.** Every program for n = 3..8 touches <= 4 coordinates with
  ReLU depth 2. sigma then sigma^-1 through the networks is the
  identity, a group relation independent of plexus. The anchor:
  interpreter = `ansa_agere` on 3,000 random pairs and 1,000 more with
  coordinates up to 1000 * 2^200, 0 disagreements (P1). (s1 s2^-1)^20
  through the network equals plexus's iteration.
- **Observed, not claimed:** (s1 s2^-1)^20 on E_1 gives
  (63,245,986; 331,160,282) = (F_39; 2 F_41), Fibonacci numbers, in
  line with the dilatation phi^2. To be looked at in the findings, not
  asserted.
- **Plants (silva.planta, 8):** all red. They cover a sign in c, a sign
  in w, the inverse's b_p, sigma_1's a_1, sigma_{n-1}'s b, an
  interpreter that ignores the ReLU, swapped outputs, and a
  miscounted ReLU depth.

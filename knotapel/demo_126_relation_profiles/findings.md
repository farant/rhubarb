# Demo 126: Relation Profiles of 2T, 2O, 2I — Findings

A neuron sees only sums of its weight points (claims.md KC 47.1.2). Two
input patterns with the same sum are indistinguishable: no activation
can separate them. (A boundary tie, by contrast, depends on the
activation.) The difference of two such patterns is an integer relation
among the points, with coefficients that depend on the input encoding:

| encoding | demos | coefficients of a coincidence |
|---|---|---|
| signed, S = Σ ±q_i | D94, D119–D123 | {−1, 0, 1}; every such relation joins two patterns |
| antipodal 0/1 (inputs +q_i and −q_i per weight) | D66/D118, D79–D84/D124, D125 | {−2 … 2} |

The encodings were identified by the review; the first version claimed
{−1, 0, 1} "exactly", which holds only for the signed encoding.

All integer relations of a point set S = (p_1 .. p_N) form a lattice,

    L(S) = { x in Z^N : sum x_i p_i = 0 },

and its rank, its shells by norm and its shortest vectors are the set's
**relation profile**.

**Parity** [proved, one line]. Σ x_i ≡ Σ x_i² (mod 2), so a relation's
coefficient sum is odd exactly when its norm is odd. In both encodings,
two patterns of opposite parity differ by a relation with an odd
coefficient sum. So only relations of **odd norm** can matter for XOR,
the function of every audited capacity demo. The demo checks
"coefficient sum odd ⇔ norm odd" on every stored vector.

This demo measures the profile exactly, with the house library
`reticulum` (integral LLL and integer Fincke-Pohst; no floating point),
for the group catalogs and for knot-free point sets. It is that
library's first consumer. It measures **no capacity and no activation**.
Plan: `plan.md` (approved 2026-10-13). Revised after an independent
review the same day (see "Review" below).

## Result

- **Every catalog has relations of norm 3**, p_a ± p_b ± p_c = 0: 16
  in 2T, 32 in 2O, 200 in 2I. These are odd, so they join patterns of
  opposite parity. No height-matched knot-free set has more than 1.
- **XOR-relevant (odd-norm) coincidences:**

  | | relations of odd norm | radius |
  |---|---:|---:|
  | 2T | 608 (320 with entries in {−1, 0, 1}) | 10 |
  | 2O | 2,720 | 8 |
  | 2I | 16,592 | 5 |

  Random rational unit points of the SAME small height have none at
  odd height (proved, Part F) and about 1,000 at height 6 (N = 24,
  against 2O's 2,720).
- **The even coincidences are not distinctive.** Random rational unit
  points of small height have thousands of relations: about 4,500
  with entries in {−1, 0, 1} at height 6 (N = 24), against 2O's 9,472.
  2I's 1,800 four-term and 169,100 six-term coincidences are even and
  can never matter for parity.
- **The high-height nulls of Part D have no relation at all.** Their
  coordinate denominators are about 10^30 and more. That reflects
  their height, not the absence of a group; Part F matches the height.
- **2O adds nothing beyond 2T** [proved, Part E]. Its points split into
  2T and σ1·2T in separate coordinate blocks, so its profile is two
  copies of 2T's (θ_2O = θ_2T²), checked shell by shell.
- **A null with a catalog's whole relation lattice:**
  - it is a Q-linear image of the catalog [proved];
  - on the unit sphere it must be congruent to 2T (rank 10 of 10)
    [proved];
  - it need not be congruent to 2O: a unit set with exactly 2O's
    lattice and different geometry exists (Part E);
  - for 2I this is open.

## Catalogs (Part A)

Built exactly, as in D118, then taken modulo sign: one representative
per {g, −g}, with its first nonzero part positive.
- **2O:** D66's generators σ1 → (1+i)/√2 and σ2 → (1−k)/√2 over Q(√2)
  close to 48 elements. The largest element order is 8, which
  classifies the group.
- **2I:** D94's generators over Q(√5) close to 120 elements, largest
  order 10.
- **2T:** the 24 elements of 2O with rational coordinates. They are
  closed under multiplication, so 2T is a subgroup of 2O.
- **Points:** 12, 24 and 60. Every element has norm exactly 1.

A coordinate a + b√d becomes the pair (a, b), so a point is a vector in
Q^8. Since 1 and √d are independent over Q, the relation lattice of
these coordinates is exactly the relation lattice of the points.

## Profiles (Part B)

Relations are counted modulo sign (x and −x are the same coincidence).
Every shell is complete. The columns are all relations, those with
entries in {−1, 0, 1} (the signed encoding) and those in {−2 … 2} (the
antipodal encoding). Odd norms are marked *.

| norm | 2T all | 2T ±1 | 2O all | 2O ±1 | 2I all (= ±1) |
|---|---:|---:|---:|---:|---:|
| 3* | 16 | 16 | 32 | 32 | 200 |
| 4 | 36 | 36 | 72 | 72 | 1,800 |
| 5* | 48 | 48 | 96 | 96 | 16,392 |
| 6 | 96 | 96 | 704 | 704 | (169,100)† |
| 7* | 144 | 144 | 2,592 | 2,592 | — |
| 8 | 252 | 156 | 6,168 | 5,976 | — |
| 9* | 400 | 112 | — | — | — |
| 10 | 288 | 0 | — | — | — |

| | 2T | 2O | 2I |
|---|---:|---:|---:|
| points N | 12 | 24 | 60 |
| ambient rank over Q | 4 | 8 | 8 |
| relation rank N − ambient | 8 | 16 | 52 |
| radius run | 10 | 8 | 5 |
| nodes | 2,740 | 38,514 | 1,938,019 |

- Every relation here has entries in {−2 … 2}; the demo checks this.
  An entry ±3 would need 3p_a to equal a sum of the other points.
  Below norm 12 that leaves at most two others, and |3p_a| = 3 exceeds
  the most two unit points can reach (2).
- The non-±1 relations all have one entry ±2: 2T 96, 288 and 288 at
  norms 8, 9 and 10, and 2O 192 at norm 8. They matter only in the
  antipodal encoding. Those at norm 9 are odd: they join patterns of
  opposite parity there.
- † 2I at norm 6 is not part of the run. The number comes from both
  oracles (author's and reviewer's) and from one library run in scratch
  (R = 6, 15.9M nodes, 289 MB). It is even, so it does not matter for
  XOR. The review found 3,300 relations with a ±2 entry in 2I at norm 8
  (reviewer's oracle; beyond this run's radius).

**Where the relations come from** [proved, one line]. Left
multiplication by a group element g maps each point to plus or minus
another point, and maps relations to relations:
Σ x_i p_i = 0 implies Σ x_i (g p_i) = 0. So one relation spreads over
its orbit. Example, checked by hand: ω = (1+i+j+k)/2 has order 6 and
ω + ω̄ = 1, so 1 − ω − ω̄ = 0 is a norm-3 relation in 2T.

**Agreement with T4.** 2O's 72 four-term relations are the 216
signed pair-sum coincidences of reticulum's T4 test. A relation
p_a ± p_b ± p_c ± p_d = 0 splits into two pairs in exactly 3 ways, and
72 × 3 = 216. Among unit vectors, pair-sum coincidences cannot share a
point.

## Invariance (Part C)

For r = (1 + 2i + 2j)/3, a rational unit quaternion, the sets r S r^-1
and r S give the identical profile for all three catalogs, in every
column. So does S with the representative of every third point flipped.
- Relations are invariant under any linear map applied to all points.
- A sign flip is an isometry of Z^N.

## High-height nulls (Part D)

Knot-free point sets, exact, matched in count N, five fixed seeds per
kind and catalog. Points are distinct modulo sign: repeats are redrawn,
though none occurred.

- **(a) r g:** r a random rational unit quaternion, g a random element
  of the catalog's own group. Same field and same ambient rank as the
  catalog (checked: 4, 8, 8), so the same relation rank. No group
  structure (an independent r per point).
- **(b) rational:** random rational unit quaternions by inverse
  stereographic projection of (n_1/m_1, n_2/m_2, n_3/m_3), with
  |n| ≤ 9 and 1 ≤ m ≤ 9. Ambient rank 4 (checked), so relation rank
  N − 4, which differs from 2O's and 2I's.

Both kinds have **high height**: coordinate denominators of 10^30 and
more (measured by the reviewer). They do NOT match the catalogs' height
(denominators 2 and 4); Part F does.

| catalog (radius) | null | relation rank | relations within radius | LLL \|b_1\|^2 (an upper bound on λ_1) |
|---|---|---:|---:|---|
| 2T (10) | a | 8 | 0 | 2.8 × 10^8 – 2.7 × 10^11 |
| 2T (10) | b | 8 | 0 | 5.2 × 10^7 – 1.2 × 10^10 |
| 2O (8) | a | 16 | 0 | 9.9 × 10^6 – 8.3 × 10^8 |
| 2O (8) | b | 20 | 0 | 2.2 × 10^4 – 3.0 × 10^7 |
| 2I (5) | a | 52 | 0 | 1.0 × 10^6 – 2.9 × 10^6 |
| 2I (5) | b | 56 | 0 | 6.7 × 10^3 – 1.5 × 10^5 |

- Every null shell is complete and empty, so every nonzero relation
  has |x|² > R, exactly.
- `oracle_nulls.py`, written by the reviewer, replays all 960 points
  draw for draw and confirms "no relation of norm ≤ R" without any
  lattice. It counts all of Z^12 for 2T, and the only possible types
  for 2O and 2I.

**A development finding.** At heights ≤ 5 one rational null contained
the same point twice, a "relation" of norm 2 made by the sampler. Small
heights make short relations likely; Part F measures this on purpose.

## Height-matched nulls (Part F)

All rational unit quaternions of height H, v/H with v ∈ Z^4 and
|v| = H, taken modulo sign. Random subsets of N points (three seeds; the
same sphere order and LCG draws as `oracle.py -F`). None is closed
under multiplication. Ambient rank 4.

**At H = 2 and H = 4 these points ARE 2T** (checked): r_4(4) = r_4(16) =
24. At the catalogs' own height, height and group structure cannot be
separated.

| H | N (radius) | ±1 relations | norm 3 | odd-norm relations |
|---|---|---:|---:|---:|
| 3 | 12 (10) | 20–44 | 0 | 0 |
| 5 | 12 (10) | 2–3 | 0 | 0 |
| 5 | 24 (8) | 1,662–1,882 | 0 | 0 |
| 6 | 24 (8) | 4,427–4,561 | 1 | 1,004–1,056 |
| 9 | 24 (8) | 169–195 | 0 | 0 |
| 25 | 24 (8) | 3–5 | 0 | 0 |
| 5 | 60 (5) | 318–416 | 0 | 0 |
| 25 | 60 (5) | 0–1 | 0 | 0 |

For comparison: 2T (N = 12, to 10) has 608 ±1 relations, 320 of them
odd; 2O (N = 24, to 8) 9,472 and 2,720; 2I (N = 60, to 5) 18,392 and
16,592.

**Odd height** [proved, one line]. If |v|² = H² is odd, then v's
coordinate sum is odd, since Σ v_k ≡ Σ v_k² (mod 2). A relation
Σ x_i v_i = 0 then gives Σ x_i ≡ 0 (mod 2). So at odd height no
coincidence joins patterns of opposite parity. The demo checks this
(0 odd-norm relations at H = 3, 5, 9, 25).

**Reading** [interpretation]:
- Small height alone produces many coincidences. "Coincident sums
  belong to the group catalogs" (the first version's claim) falls.
- What separates the catalogs from every height-matched set measured:
  - many norm-3 relations (16, 32, 200 against ≤ 1);
  - more coincidences overall;
  - many odd ones, which matter for XOR (none at odd height, about
    1,000 at H = 6 against 2O's 2,720).

## The structure of 2O (Part E)

- **Blocks.** 2O modulo sign is 2T ∪ σ1·2T. The 2T points have zero √2
  parts, and the σ1·2T points have zero rational parts, so the
  coordinate matrix is block-diagonal and L(2O) = L(2T) ⊕ L(σ1·2T).
  σ1·2T is a Q-linear image of 2T, so it has 2T's lattice, and 2O's
  shells are 2T's squared. Checked in every column to norm 8: 2O adds
  no relation beyond two copies of 2T [proved]. This also checks the
  192 relations with an ±2 entry, which no oracle covered in the first
  version.
- **A whole-lattice null that is not congruent** (the review's
  counter-example). S′ = 2T ∪ r·(σ1·2T), with r = (1+2i+2j)/3:
  - unit points with exactly 2O's kernel (ranks 8, 8, and 8 stacked)
    and 2O's profile;
  - 11 distinct inner products against 2O's 5;
  - not closed under multiplication.
- **When must a whole-lattice unit null be congruent?** If p′_i = A p_i
  and every p′_i is a unit, A preserves both the rational part and the
  √d part of the norm on every p_i. If the quadratic monomials of the
  points have full rank on the coordinates they span, both quadratic
  forms are pinned. Then A preserves every inner product, so S′ is
  congruent to S. The evaluation ranks:
  - 2T: 10 of 10, congruent [proved];
  - 2O: 20 of 36, not pinned (S′ above);
  - 2I: 35 of 36, open. The missing form is the √5 part of the norm,
    which vanishes on unit points anyway; the reviewer's reading,
    not settled here.
- **Consequence for P1.** A null with a catalog's whole relation
  lattice is a Q-linear image of it.
  - Non-unit images A·S (A ∈ GL(Q)) keep every coincidence and change
    every metric. That may be exactly the null that separates
    coincidences from geometry.
  - On the unit sphere such a null is congruent to the catalog for
    2T, not necessarily for 2O, and open for 2I.
  - The first version's "a knot-free null can only match part of the
    profile" falls.

## Interpretation [interpretation]

- KC 48.2 predicted that group structure lowers capacity "through ties
  and coincident sums". The odd-norm coincidences are the only ones
  that can affect XOR, and the catalogs have far more of them than
  height-matched sets. That is consistent with the prediction, and it
  is not a capacity result: that needs activations (P1).
- A coincidence matters only if the target function needs to separate
  the two patterns it joins. For XOR that is exactly the odd ones.
  For other functions it is open.

## Oracles

Three, sharing no code with the house libraries (Python Fractions):
- `oracle.py` (the author's):
  - its own Q(√d) and quaternion arithmetic, and its own closure from
    the same generators;
  - ranks by Gaussian elimination;
  - signed coincidences by direct meet-in-the-middle over signed
    subset sums;
  - 2T's complete shells to norm 10, from halves of 6 with entries in
    [−3, 3];
  - `-F`: Part F replayed, with the same sphere order and LCG.
- `oracle_nulls.py` + `oracle_common.py` (written by the reviewer,
  adopted): Part D replayed draw for draw, with "no relation within
  the radius" counted by type, without a lattice.

The demo pins the oracles' numbers as checks. A live rerun against
changed libraries is therefore a regression check of `reticulum`.

## Checks (40)

- **Part A (6):** fields, closures and classification, unit norms, 2T
  as a subgroup, the counts modulo sign.
- **Part B (11):**
  - the oracle pins;
  - 2O's norm-8 total;
  - the ranks, and relation rank = N − ambient rank;
  - basis rows re-checked as relations in quaternion arithmetic;
  - every shell complete and classified;
  - the parity lemma on every stored vector;
  - no entry beyond ±2.
- **Part C (10):** the rotor is a unit, and 9 identical profiles.
- **Part D (1):** 30 nulls with distinct unit points, ambient rank as
  labeled, relation rank N − ambient, relation rows and complete
  shells. The relation count is printed as an OBSERVATION.
- **Part E (7):** 2O = 2T ∪ σ1·2T; the block form; θ_2O = θ_2T²; the
  counter-example's kernel and profile; its non-congruence; the three
  quadric ranks (two checks).
- **Part F (5):** heights 2 and 4 are 2T (two checks); 24 sets complete
  with ranks and not closed; the oracle pins; the odd-height lemma.

Every enumeration has a node budget of 4 × 10^7 and a storage limit of
200,000, so a wrong configuration fails a check instead of running for
ever. A failed Part A stops the run.

## Plants

Each plant is one exact replacement in a scratch copy of `main.c`,
built with clang against the house libraries and run in quick mode
(`D126_CELER=1`). The tree is never touched.

| plant | caught by |
|---|---|
| coordinates without the √d coefficient | 10 checks: oracle pins, ranks (fails, no longer hangs: the node budget) |
| ±1 classification ignores −1 | oracle pins (6 checks) |
| both representatives of {g, −g} kept | the Part A count, then stop. It first timed out, so the demo now stops after a failed Part A |
| relation re-check ignores the coefficients | quaternion re-check of the basis rows (2 checks) |
| 2T accepts every element of 2O | the subgroup check (got 48), then stop |
| null (a) without g (all rational) | null ambient rank asserted. **Survived the first version** (review F7) |
| θ² without the factor 2 | θ_2O = θ_2T² |
| quadric rank without the squares | 2T's rank 10 (2 checks) |
| height subsets drawn with bias | Part F oracle pins |
| odd counter on even norms | the odd-height lemma (7,313 counted) |
| counter-example without the rotation | non-congruence (5 inner products, closed) |

Untested: the redraw of repeated null points (no repeat occurs at
these heights and seeds).

## Review (2026-10-13)

An independent reviewer (its own Fraction oracle, written before
reading oracle.py) found no wrong number, and reproduced every one,
including 169,100. What changed:
- **F1 (overstatement).** "A whole-lattice null is the same
  configuration, so a knot-free null can only match part" fell to a
  counter-example for 2O. The proof (S′ = A S) is right but never uses
  unit norm. Now Part E: the counter-example, and the rigidity ranks
  (2T congruent, 2O not, 2I open).
- **F2 (overstatement).** The nulls differed from the catalogs mainly
  in height, so "coincident sums belong to the group catalogs" fell.
  Now Part F. While adding it, the odd-height lemma came out (no
  XOR-relevant coincidence at odd height).
- **F3 (premise).** "Exactly when" held only for the signed encoding.
  Now the encodings are named and the {−2 … 2} column is reported.
- **F4 (strengthening).** Only odd-coefficient-sum relations matter
  for XOR. Simplified here to odd norm, by Σx ≡ Σx² (mod 2), and
  checked on every vector.
- **F5 (grade).** The null claim was graded [exact] with no null
  oracle. The reviewer wrote one; it is now part of the demo.
- **F6 (provenance).** 169,100 is not compared by the run; it is now
  marked.
- **F7 (plant survived).** The (a) nulls' ambient rank was printed,
  not asserted. Now it is, with relation rank = N − ambient everywhere.
- **F8 (wording).** "Matched in ambient rank" was said of all nulls;
  the (b) nulls are not.
- **F9 (wording).** "(or ±3)" corrected: no entry beyond ±2 is
  possible below norm 12, now checked. "Up to norm 7 every relation is
  ±1" was confirmed for 2I by the reviewer.
- **F10 (wording).** "λ_1 > R" now reads "every nonzero relation has
  |x|² > R". The LLL range is labeled as an upper bound.
- **F11 (positive).** θ_2O = θ_2T². Now proved and checked in Part E.

## Not covered

- The KC 48.1 nulls (same axes with random angles; same angles with
  random axes), the non-unit linear images A·S, and every activation,
  capacity or tie-resolution question: that is P1.
- 2I beyond norm 5 in the run. 2O beyond norm 8. 2T beyond norm 10.
  2I's norm-9 shell (odd, with ±2 entries).
- Whether 2I's whole-lattice unit nulls must be congruent.
- Successive minima beyond the shortest norm. Relations among subsets,
  such as the Q8 cosets of KC 49 or particular weight sets.

## Run

```
./bin/aedilis knotapel/demo_126_relation_profiles/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
```

About 2.5 minutes, 40/40. `D126_CELER=1` runs 2I to norm 4 (the plants used
it). Oracles: `python3 -I oracle.py -F` (about 70 s),
`python3 -I oracle_nulls.py` (about 85 s). Frozen in `demo-snapshot.c`
via `knotapel/archive.sh`.

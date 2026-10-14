# Demo 126: Relation Profiles of 2T, 2O, 2I — Findings

A neuron sees only sums of its weight points (claims.md KC 47.1.2). Two
input patterns give the same sum exactly when their difference is an
integer relation among the points with coefficients in {-1, 0, 1}.
Such a coincidence makes the two patterns indistinguishable: no
activation can separate them. (A boundary tie, by contrast, depends on
the activation.) All integer relations of a point set
S = (p_1 .. p_N) form a lattice,

    L(S) = { x in Z^N : sum x_i p_i = 0 },

and its rank, its shells by norm and its shortest vectors are the set's
**relation profile**. This demo measures it exactly for the group
catalogs and for knot-free point sets, with the house library
`reticulum` (integral LLL and integer Fincke-Pohst; no floating point).
It is that library's first consumer.

It measures **no capacity and no activation**. It isolates the one
constraint on a catalog's sums that does not depend on the activation:
coincident sums. A matched null must reproduce it, alongside the tie
structure of KC 48.1.1. Plan: `plan.md` (approved 2026-10-13).

## Result

- **Every catalog has many short exact relations.** The shortest have
  norm 3, of the form p_a ± p_b ± p_c = 0: 16 in 2T, 32 in 2O, 200 in
  2I. The shells then grow fast: 2I has 1,800 four-term, 16,392
  five-term and 169,100 six-term signed coincidences.
- **No knot-free null has any.** 30 null sets, matched in count and in
  ambient rank, have no relation at all within the catalog's radius.
  Their LLL first vectors run from about 7 × 10^3 to 3 × 10^11 in
  norm.
- **So coincident sums belong to the group catalogs.** No activation
  can separate the input patterns they join, and generic point sets
  lack them entirely at these lengths.
- **What P1 can match.** A null that reproduces a catalog's entire
  relation lattice is a linear image of the catalog (proved below). A
  knot-free null can therefore match only part of the profile, such as
  its shortest shells.

## Catalogs (Part A)

Built exactly, as in D118, then taken modulo sign: one representative
per {g, -g}, with its first nonzero part positive.
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
Every shell is complete, and every stored vector was classified.

| | 2T | 2O | 2I |
|---|---:|---:|---:|
| points N | 12 | 24 | 60 |
| ambient rank over Q | 4 | 8 | 8 |
| relation rank N − ambient | 8 | 16 | 52 |
| shortest relation norm | 3 | 3 | 3 |
| norm 3 | 16 | 32 | 200 |
| norm 4 | 36 | 72 | 1,800 |
| norm 5 | 48 | 96 | 16,392 |
| norm 6 | 96 | 704 | (169,100)* |
| norm 7 | 144 | 2,592 | — |
| norm 8 | 252 (156 ±1) | 6,168 (5,976 ±1) | — |
| norm 9 | 400 (112 ±1) | — | — |
| norm 10 | 288 (0 ±1) | — | — |
| radius run | 10 | 8 | 5 |
| nodes | 2,740 | 38,514 | 1,938,019 |

- Up to norm 7, every relation has entries in {-1, 0, 1}. From norm 8
  on, relations with an entry ±2 (or ±3) appear.
- \* 2I at norm 6 comes from the oracle and from a one-off library run
  in scratch (R = 6: 15.9M nodes, 289 MB, 264 s; 169,100 matches). It
  is left out of the demo's run for cost.

**Where the relations come from** [proved, one line]. Left
multiplication by a group element g maps each point to plus or minus
another point, and maps relations to relations:
sum x_i p_i = 0 implies sum x_i (g p_i) = 0. So one relation spreads
over its orbit. Example, checked by hand: ω = (1+i+j+k)/2 has order 6
and ω + ω̄ = 1, so 1 − ω − ω̄ = 0 is a norm-3 relation in 2T.

**Agreement with T4.** 2O's 72 four-term relations are the 216
signed pair-sum coincidences of reticulum's T4 test
(`_relationes_octaedri`). A relation p_a ± p_b ± p_c ± p_d = 0 splits
into two pairs in exactly 3 ways, and 72 × 3 = 216.

## Invariance (Part C)

For r = (1 + 2i + 2j)/3, a rational unit quaternion, the sets r S r^-1
and r S give the identical profile for all three catalogs, shell by
shell. So does S with the representative of every third point flipped.
- Relations are invariant under any linear map applied to all points.
- A sign flip is an isometry of Z^N.

The profile is therefore a property of the configuration, not of the
coordinates or the representatives chosen. This also checks the
pipeline: conjugation changes every coordinate, yet the profile does
not move.

## Nulls (Part D)

Knot-free point sets, exact, matched in count N, five fixed seeds per
kind and catalog. Points are distinct modulo sign: repeats are redrawn,
though none occurred at these heights.

- **(a) r g:** r a random rational unit quaternion, g a random element
  of the catalog's own group. The set has the same field, the same
  ambient rank and so the same relation rank as the catalog, but no
  group structure (an independent r for every point).
- **(b) rational:** random rational unit quaternions by inverse
  stereographic projection of (n_1/m_1, n_2/m_2, n_3/m_3), with
  |n| ≤ 9 and 1 ≤ m ≤ 9. Ambient Q^4, so their relation rank is N − 4
  and differs from the catalog's, as labeled.

| catalog (radius) | null | relation rank | relations within radius | LLL \|b_1\|^2 (range) |
|---|---|---:|---:|---|
| 2T (10) | a | 8 | 0 | 2.8 × 10^8 – 2.7 × 10^11 |
| 2T (10) | b | 8 | 0 | 5.2 × 10^7 – 1.2 × 10^10 |
| 2O (8) | a | 16 | 0 | 9.9 × 10^6 – 8.3 × 10^8 |
| 2O (8) | b | 20 | 0 | 2.2 × 10^4 – 3.0 × 10^7 |
| 2I (5) | a | 52 | 0 | 1.0 × 10^6 – 2.9 × 10^6 |
| 2I (5) | b | 56 | 0 | 6.7 × 10^3 – 1.5 × 10^5 |

- Every null shell is complete and empty, so λ_1 > R exactly.
- The LLL column is an **upper bound** on λ_1 (the first reduced
  vector), not λ_1 itself.
- Even the (b) nulls for 2I, 60 rational points in Q^4 (relation rank
  56), have no relation of norm ≤ 5. Their first reduced vectors have
  norm 6.7 × 10^3 or more.

**A development finding.** At heights ≤ 5 one rational null contained
the same point twice, a "relation" of norm 2 made by the sampler, not
by any structure. Repeats are now redrawn. Small heights make
accidental short relations likelier; the null's heights are part of
its definition.

## What a matched null can match [proved]

If S′ has the same relation lattice as S, then p′_i = A p_i for a
single Q-linear map A.
- **Proof:** the coordinate matrices P (8 × N) and P′ (8′ × N) have
  the same kernel, hence the same row space over Q, hence P′ = A P.
- **Consequence for P1:** a null that reproduces a catalog's ENTIRE
  coincidence structure is a linear image of the catalog: the same
  configuration seen through a linear map, not a knot-free
  alternative.
- A meaningful null therefore matches the profile only partially, for
  example its shortest shells (norm 3, or norms 3–4), and must say
  which part it matches. Random sampling matches none of it (Part D).

## Interpretation [interpretation]

- KC 48.2 predicted that group structure lowers capacity "through ties
  and coincident sums". This demo confirms that the coincident sums
  exist, counts them exactly, and finds them absent in knot-free sets.
  It says nothing about their effect on capacity: that needs
  activations, which is P1's job.
- A coincidence among the weight points only matters if a function
  needs to tell the two input patterns apart. Which coincidences
  matter for which Boolean function is open.

## Oracle

`oracle.py` (Python Fractions, about 30 s) shares no code with the
house libraries.
- It does its own arithmetic in Q(√d), its own quaternion product and
  its own closure from the same generators.
- It computes ranks by Gaussian elimination.
- It counts signed coincidences by direct meet-in-the-middle over
  signed subset sums, with no lattice and no LLL, deduplicated modulo
  overall sign: support 3–10 for 2T, 3–8 for 2O, 3–6 for 2I.
- For 2T it also counts every relation with any integer entries up to
  norm 10, by splitting the 12 points into halves of 6 with entries in
  [−3, 3]. Norm ≤ 10 forces |x_i| ≤ 3.

Agreement on every number:
- the ranks 8, 16, 52;
- every ±1 shell of all three catalogs;
- 2T's complete shells, ±2 and ±3 entries included;
- 2I's norm-6 shell from the scratch run.

The demo pins the oracle's numbers as checks, so a live rerun against
changed libraries is a regression check of `reticulum`.

**Not oracle-checked:** 2O's relations with an entry ±2 at norm 8
(192). They are pinned as a library-only regression.

## Checks (25)

- **Part A (6):** fields, closures and classification, unit norms, 2T
  as a subgroup, the counts modulo sign.
- **Part B (8):** the oracle pins (2T all shells, ±1 shells of 2T, 2O
  and 2I), 2O's norm-8 total, the ranks, every basis row re-checked as
  a relation in quaternion arithmetic (a second route), every shell
  complete and classified.
- **Part C (10):** the rotor is a unit, and 9 identical profiles.
- **Part D (1):** 30 nulls with distinct unit points, relation rows and
  complete shells. Their relation count is printed as an OBSERVATION,
  not asserted.

Every enumeration has a node budget of 4 × 10^7, about 20 times what
2I to norm 5 needs, and a storage limit of 200,000. A wrong
configuration therefore fails a check (INCOMPLETE) instead of running
for ever. The first plant below found this: before the budget it
timed out. A failed Part A stops the run.

## Plants

Each plant is one exact replacement in a scratch copy of `main.c`,
built with clang against the house libraries and run in quick mode
(`D126_CELER=1`). The tree is never touched.

| plant | caught by |
|---|---|
| coordinates without the √d coefficient | 10 checks: oracle pins, ranks (fails, no longer hangs: the node budget) |
| ±1 classification ignores −1 | oracle pins (6 checks) |
| both representatives of {g, −g} kept | the Part A count, then stop. It first timed out: later parts ground through doubled catalogs, so the demo now stops after a failed Part A |
| relation re-check ignores the coefficients | quaternion re-check of the basis rows (2 checks) |
| 2T accepts every element of 2O | the subgroup check (got 48), then stop |

Untested: the redraw of repeated null points. No repeat occurred at
these heights and seeds, so a plant removing it is equivalent here (see
Part D).

## Not covered

- The KC 48.1 nulls (same axes with random angles; same angles with
  random axes) and every activation, capacity or tie-resolution
  question: that is P1.
- 2I beyond norm 5 in the run (norm 6 by the oracle and once in
  scratch). 2O beyond norm 8. 2T beyond norm 10.
- Successive minima beyond λ_1. Relations among subsets, such as the
  Q8 cosets of KC 49 or particular weight sets.
- Whether the shells would be the same for other embeddings of the
  same group. They are, by the linear-image argument, for any
  Q-linear change of coordinates.

## Run

```
./bin/aedilis knotapel/demo_126_relation_profiles/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
```

76 s, 25/25. `D126_CELER=1` runs 2I to norm 4 (the plants used it).
Frozen in `demo-snapshot.c` via `knotapel/archive.sh`.

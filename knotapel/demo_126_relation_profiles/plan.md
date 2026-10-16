# Demo 126: Relation profiles of 2T, 2O, 2I (plan)

Status: DRAFT for Fran's approval (2026-10-13). First consumer of
`reticulum` (project-specs/reticulum-plan.md, T6).

## Question

A neuron sees only sums of its weight points (KC 47.1.2). Two input
patterns give the same sum exactly when their difference is an integer
relation among the catalog points with coefficients in {-1, 0, 1}.
Such a coincidence makes the two patterns indistinguishable under ANY
activation (unlike a boundary tie, which depends on it): a constraint a
matched null for P1 must reproduce whatever the activation (KC 48.1.1).
(Wording corrected 2026-10-13 from "exact ties".)

All integer relations of a point set S = (p_1 .. p_N) form a lattice,
L(S) = {x in Z^N : sum x_i p_i = 0}. Its rank, its shortest vectors
and its shells by norm are the set's **relation profile**.

1. How much short linear structure do 2T, 2O and 2I carry?
2. How does it compare with knot-free point sets in the same ambient
   space?

**Not in scope:** capacity, activations, or any claim about DKC itself.
This demo measures one ingredient (coincident sums) exactly and says
what a null would have to match. P1 then builds and judges those nulls.

## Catalogs (exact)

| catalog | built from | field | ambient |
|---|---|---|---|
| 2O | D66's generators, closed under multiplication, as in D118 Part A | Q(sqrt2) | Q^8 |
| 2T | the rational-coordinate elements of 2O, checked to be a subgroup of order 24 | Q | Q^4 |
| 2I | D94's generators, as in D118 Part D | Q(sqrt5) | Q^8 |

- Points are taken modulo sign: one representative per pair {g, -g},
  as the catalogs' 24-cell and 600-cell pictures use them.
  - That gives 12 points for 2T, 24 for 2O and 60 for 2I.
  - The full groups add only the trivial relations g + (-g) = 0.
- Each coordinate a + b sqrt d becomes (a, b). The relation lattice
  over Q of these coordinates equals the relation lattice of the
  points, since 1 and sqrt d are independent over Q.
- `reticulum_relationes` takes the coordinates directly (Q accepted).

## Parts

**A. Catalogs.** Build and check: group orders 24/48/120, unit norm
exactly, closure, the mod-sign counts, the 2T-in-2O subgroup.

**B. Relation profiles.** For each catalog:
- the rank of L(S), which should be N - 4 for 2T, 24 - 8 for 2O and
  60 - 8 for 2I (8, 16, 52);
- the reduced basis from `reticulum_relationes`, every row re-checked
  as a relation in K^4 itself (quaternio arithmetic, a second route);
- the shells: relations by norm up to a radius R, counted modulo sign,
  with `completa` asserted;
  - R is chosen per catalog by measurement, probably 6-8;
- signed coincidences: the shell vectors with entries in {-1, 0, 1},
  by support size;
- the shortest relation norm and the number of shortest relations.

**C. Invariance (pipeline check).** For a rational unit quaternion r:
- r S r^-1 and r S must give the identical profile, because relations
  are invariant under any linear map applied to every point;
- re-signing points (another representative of {g, -g}) must too,
  because a sign flip is an isometry of Z^N.

The profile is then a property of the configuration, not of the
coordinates chosen.

**D. Baseline nulls (knot-free, exact, fixed seeds).** For each
catalog, N points matched in count:
- **(a) generic over the same field K:** products r g, with r a random
  rational unit quaternion and g a random catalog element.
  - They are unit, in K^4, and in the same ambient Q-dimension, so
    their relation lattice has the same rank as the catalog's.
- **(b) generic rational:** random rational unit quaternions (inverse
  stereographic projection).
  - Their ambient is Q^4, a different rank, reported as such.
- Expected: no short relations, so a much larger shortest norm.
- Several seeds, reported as a range, never as one sample read as
  typical.

**E. Oracle (independent, Python, Fractions).**
- Arithmetic in K by hand.
- Coincidences found by direct search: hash every signed subset sum
  of up to 4 points (5 for 2T), count the collisions. No lattice and
  no LLL.
- Ranks by Gaussian elimination.
- It must agree with B on every signed-coincidence count up to that
  support, and on the ranks.

## Rules applied (Fran's)

- Everything exact. Every shell count carries `completa`. A truncated
  count is printed as a lower bound and named so.
- Interpretation is kept apart from results in findings.md (graded as
  in claims.md).
- The null constructions are labeled with what they match (count,
  field, ambient rank) and what they do not (angles, axes, group
  structure). Building the (b)/(c)-style nulls of KC 48.1 is P1's job.

## Deliverables

- `knotapel/demo_126_relation_profiles/`:
  - `main.c` on the house libraries (reticulum, quaternio, extensio);
  - `oracle.py`;
  - `findings.md`;
  - `demo-snapshot.c` via `knotapel/archive.sh`.
- claims.md: new children under 48 (append-only addresses), graded.
- Ledger P1 (…E72XG3): a note with what a matched null must reproduce.
- Then an independent review, as for D118-D125.

# Demo 74: Computational Invariant of Braids

## Summary

The DKC eigenvector-Voronoi activation gives a braid invariant (the Voronoi cell)
that is **incomparable** with the Kauffman bracket. Neither subsumes the other.
The cell separates 97.8% of bracket collisions at the braid level, while the
bracket separates 53.8% of cell collisions. Their combination (119 classes)
is strictly finer than either alone (100 bracket classes, 14 cell classes).

**Test count: 20/20 pass (Parts A-I)**

---

## 1. Bracket Collision Eigenvector Analysis (Part A)

At the braid level (2,000 correspondence records, 1,124,250 pairs analyzed):

| Category | Count | Rate |
|----------|-------|------|
| Same bracket, same quaternion | 28,500 | — |
| Same bracket, different quaternion | 58,078 | — |
| ...of which, different cell | 56,825 | **97.8%** |
| ...of which, same cell | 1,253 | 2.2% |

**97.8% of bracket collisions are separated by the Voronoi cell.** The cell
almost perfectly resolves bracket collisions — braids that the bracket cannot
tell apart are almost always distinguishable by their eigenvector direction.

The 2.2% unseparated collisions correspond to quaternion pairs that share the
same eigenvector direction but have different eigenvalue angles (the cell
discards angle information).

## 2. Reverse Direction: Same Cell, Different Bracket? (Part B)

At the quaternion catalog level (24 quaternions, C(24,2)=276 pairs):

| Category | Count |
|----------|-------|
| Quaternion pairs sharing a cell | 13 |
| Same bracket | 6 |
| Different bracket | 7 |

**The cell does NOT subsume the bracket.** Seven pairs of quaternions share a
Voronoi cell but have different bracket values (via different braids). This
means the cell is strictly coarser than the bracket in some cases.

Per-cell bracket diversity (using qcat_bracket, which is a lower bound):

| Cell | Quaternions | Distinct brackets |
|------|-------------|-------------------|
| 0 | 3 | 3 |
| 1 | 3 | 1 |
| 2 | 2 | 2 |
| 10 | 3 | 2 |
| others | 1-2 | 1 |

## 3. Combined Invariant Power (Part C)

Braid-level equivalence class counting:

| Invariant | Classes | Level |
|-----------|---------|-------|
| Voronoi cell | 14 | Coarsest |
| Quaternion | 24 | Intermediate |
| Bracket | 100 | Intermediate |
| **(Bracket, Cell)** | **119** | **Finest** |

The combined (bracket, cell) invariant has 119 classes — **19% more than
bracket alone** and 8.5x more than cell alone. This proves the two invariants
capture genuinely different information.

### Bracket values split across cells

| Cells per bracket | Bracket values |
|-------------------|---------------|
| 1 cell | 15 |
| 2 cells | 2 |
| 3 cells | 12 |
| 4 cells | 10 |
| 6 cells | 4 |

28 of 100 bracket values (28%) map to braids in multiple cells. Max distinct
brackets per cell: 19 (the cell is very coarse relative to the bracket).

### Why the combined invariant is finer

The bracket is a trace operation (loses eigenvector direction). The cell is a
direction projection (loses eigenvalue angle AND bracket normalization). These
are orthogonal information losses. Combining them recovers most of what
each discards.

## 4. Computational Significance (Part D)

For each of the 36 XOR6 winning triples, the cell labeling assigns a binary
output (0 or 1) to each cell. A bracket collision is "XOR6-separated" if the
two braids land in cells with different labels.

| Winner pattern | Separation | Count |
|----------------|-----------|-------|
| [*,{4,5,9},{19,21,23}] | 5/24 | 27 winners |
| [*,18,22] | 0/24 | 3 winners |
| [{4,5,9},15,16] | 9/24 | 3 winners |
| [17,{19,21,23},20] | 8/24 | 3 winners |

**33 of 36 winners (92%) separate at least some bracket collisions via XOR6.**
The 3 exceptions are all triples containing quaternions 18 and 22 — these use
a labeling where the colliding cells happen to share the same label.

Total XOR6-separated collisions across all winners: 186.

This means the DKC computation actively exploits the information that the
bracket discards. The eigenvector direction isn't just theoretically different
from the bracket — it's computationally different.

## 5. Full Invariant Hierarchy (Part E)

### Quaternion-to-cell factoring

| Cell | Quaternions | Eigenvalue angles |
|------|-------------|-------------------|
| 0 | q0, q1, q2 | 45°, 90° |
| 1 | q4, q5, q9 | 45°, 90° |
| 2 | q6, q14 | 60° |
| 3 | q7, q10 | 60° |
| 4 | q8, q12 | 60° |
| 5 | q11, q13 | 60° |
| 10 | q19, q21, q23 | 45°, 90° |
| 6-9, 11-13 | 1 each | single angle |

The factoring: **Quaternion (24) → Direction (13) = Cell (13)**.
Direction and cell are identical (the Voronoi partition IS the direction catalog).
Information lost: eigenvalue angle only (24/13 ≈ 1.85:1 compression).

Three cells (0, 1, 10) each contain 3 quaternions with 2 distinct eigenvalue
angles (45° and 90°). Four cells (2-5) each contain 2 quaternions with the same
angle (60°). These are the axis-type (3 quats, 2 angles) and body-type
(2 quats, 1 angle) cells from the 24-cell geometry.

### The bracket doesn't factor through quaternion

Different braids with the same quaternion can have different brackets (Demo 66:
8,424 such pairs). This means:

```
Braid → Quaternion → Cell       (well-defined at each step)
Braid → Bracket                  (well-defined)
Quaternion → Bracket             (NOT well-defined: many-to-many)
```

The bracket and cell are genuinely independent projections of the braid.

## 6. Synthesis (Part F)

| Metric | Bracket collisions | Cell collisions |
|--------|-------------------|-----------------|
| Total (qcat level) | 30 | 13 |
| Separated by other | 24 (80%) | 7 (54%) |
| Not separated | 6 | 6 |

**The cell and bracket are INCOMPARABLE braid invariants.** Neither refines
the other. The cell captures eigenvector direction (geometric/topological),
while the bracket captures trace + normalization (algebraic). Together
they form a strictly finer invariant (119 classes vs 100 or 14).

### Why this matters for DKC

The DKC computation works because:
1. The eigenvector direction (cell) captures information the bracket discards
2. The Voronoi partition creates a binary labeling of these directions
3. This labeling separates XOR6 parity — something the bracket alone cannot do
4. The bracket's 100 algebraic classes cannot be organized into a parity-
   separating binary labeling (no such threshold exists for bracket values)
5. The cell's 14 geometric classes CAN be organized this way (36 winning triples)

DKC doesn't just "use the bracket differently" — it accesses a fundamentally
different projection of the braid group representation.

### The 6 unseparated collisions

At the quaternion catalog level, 6 bracket collision pairs share the same
cell AND the same bracket (at qcat level). These are pairs of quaternions
with the same eigenvector direction but different eigenvalue angles, where
the stored bracket values happen to coincide. They correspond to the body-type
cells (2-5) where both quaternions have 60° angles but are different rotations.

## 7. Computational Type Vectors (Part G)

Each of the 36 winning triples assigns a binary label (0 = even parity, 1 = odd
parity) to each Voronoi cell. The 36-bit **type vector** of a cell is its label
under all 36 triples. This captures how the cell is *used computationally*.

| Type | Cells | Description | Quats |
|------|-------|-------------|-------|
| 0 | {0} | Axis (σ₁) | 3 |
| 1 | {1} | Axis (σ₂) | 3 |
| 2 | {2,3,4,5} | Body (60° cells) | 8 |
| 3 | {6,7,8,9,11,12} | Edge cells | 6 |
| 4 | {10} | Axis (third) | 3 |
| 5 | {13} | Identity | 1 |

**Only 6 distinct type vectors among 14 cells.** The computational labeling
merges cells aggressively:

- The 4 body-type cells (2,3,4,5) are **computationally interchangeable** —
  they receive identical parity labels under all 36 winning triples.
- All 6 edge cells (6,7,8,9,11,12) are also interchangeable.
- Only the 3 axis cells (0,1,10) and identity (13) are distinguishable.

The DKC computation uses only 6 computational "roles." The 13-direction
Voronoi partition is geometrically finer than computationally necessary.

The octahedral symmetry group acts transitively within each geometric class —
body diagonals are interchangeable, edge midpoints are interchangeable. DKC
sees geometry at the level of **symmetry orbits**, not individual directions.

### Hierarchy at the braid level

```
Type (6) < Cell (14) < Quaternion (24) < Bracket (100) < (Bracket,Cell) (119)
```

The type vector is the COARSEST invariant, not the finest. It captures
computational role, while the bracket captures algebraic structure. These
are complementary: the bracket tells you the algebra, the type tells you
the computation.

## 8. Concrete Bracket Collision Examples (Part H)

Specific braid pairs with the same bracket but different Voronoi cells:

| # | Braid A | Braid B | Bracket | Cells |
|---|---------|---------|---------|-------|
| 1 | σ₁⁸ (2-str) | σ₂'⁴·σ₁² (3-str) | (0,0,8,0) | 13 vs 0 |
| 2 | σ₁⁻⁸ (2-str) | σ₂²·σ₁⁴ (3-str) | (0,0,-8,0) | 13 vs 1 |
| 3 | σ₁⁻⁸ (2-str) | σ₁·σ₂²·σ₁³ (3-str) | (0,0,-8,0) | 13 vs 10 |

**Pattern**: The identity quaternion (q3, cell 13) is the main collision partner.
The 2-strand braid σ₁⁸ has period 8 (returns to identity) but its bracket
value (0,0,±8,0) also appears for shorter 3-strand braids that land in
non-identity cells. The bracket "sees" the algebraic value but is blind
to the geometric destination.

## 9. Refinement Chain (Part I)

Grouping all braids by bracket, then subgrouping by type, then by quaternion:

| Level | Groups | Description |
|-------|--------|-------------|
| Bracket | 43 | Distinct bracket values (of 100 in catalog) |
| → Type | 61 | Subgroups by computational type |
| → → Quaternion | 168 | Subgroups by quaternion identity |

- **Max types per bracket**: 4 (bracket (0,0,-8,0) splits into 4 types)
- **Max quats per type within bracket**: 8
- Each level genuinely refines the previous: 43 → 61 → 168

Bracket classes with multiple types include the high-symmetry values like
(4,0,0,0) with 112 braids across 3 types and 3 quaternions.

## 10. The [*,18,22] Exception Pattern

Three of 36 winning triples — [0,18,22], [1,18,22], [2,18,22] — separate
**zero** bracket collisions. These are the only triples with q18 and q22,
which are edge-type quaternions (type 3). In these triples, the colliding
cells happen to share the same parity label, making the bracket collision
invisible to the DKC computation.

This connects to Demo 70's music-space orthogonality (r = -0.06): the
computational structure and the geometric structure are nearly independent.
The [*,18,22] triples occupy a "computational dead zone" where the
eigenvector labeling fails to distinguish what the bracket cannot.

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 66 | Quaternionic DKC | 24 quaternions, 100 brackets, 24-cell |
| 67 | Coordinate Zoo | 13 directions, 36 XOR6 solutions |
| 70 | Music Space | r = -0.06 (geometric ⊥ computational) |
| 71 | Spectral DKC | Bandwidth l=6; 2l+1=13=directions |
| 72 | Spherical Design | NOT a t-design; algebra > geometry |
| 73 | Automaton | NOT an automaton (82.8% universal); additive |
| **74** | **Computational Invariant** | **Incomparable invariants; 6 types; 119 combined; 97.8% separation** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo74
./demo74
```

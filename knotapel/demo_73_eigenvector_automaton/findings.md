# Demo 73: Eigenvector Automaton — Can DKC Be a Finite Automaton?

## Summary

The DKC computation is NOT a finite automaton on the Voronoi cells — but it's
close (~83% deterministic). Two formulations tested: multiplicative (braid
product) and additive (weight sum). The multiplicative formulation cannot
compute XOR6 at all. The additive formulation computes XOR6 but the Voronoi
cell loses ~17% of the state information needed for deterministic transitions.

**DKC is fundamentally additive (vector sum), not multiplicative (matrix
product).** The multiplicative braid product at ζ_8 cannot distinguish parity.

**Test count: 7/7 pass (Parts A–G)**

---

## 1. Generator Eigenvectors (Part A)

The 4 braid generators at ζ_8:

| Generator | Quaternion | Axis | Cell | Angle |
|-----------|-----------|------|------|-------|
| σ_1 | (+.707,+.707, 0, 0) | (+1, 0, 0) | 0 | 45° |
| σ_1⁻¹ | (+.707,−.707, 0, 0) | (−1, 0, 0) | 0 | 45° |
| σ_2 | (+.707, 0, 0,−.707) | (0, 0,−1) | 1 | 45° |
| σ_2⁻¹ | (+.707, 0, 0,+.707) | (0, 0,+1) | 1 | 45° |

Key observation: σ and σ⁻¹ share the same Voronoi cell (antipodal axes
are identified). Only **2 distinct generator cells** out of 13 directions.
This means the cell cannot distinguish between a generator and its inverse
at any single step — already a sign that cell-level tracking loses information.

## 2. Multiplicative Formulation: Does NOT Compute XOR6 (Part B)

6-step braid product with alternating σ_1/σ_2:

| Final Cell | Even parity | Odd parity | Pure? |
|-----------|------------|-----------|-------|
| 0 | 8 | 8 | NO |
| 1 | 8 | 8 | NO |
| 10 | 8 | 8 | NO |
| 13 (identity) | 8 | 8 | NO |

Every final cell gets exactly 8 even + 8 odd parity inputs — perfectly
balanced, maximally non-computing. The braid product treats parity as
invisible.

**Why**: The multiplicative product M = σ₁^{±1}·σ₂^{±1}·...·σ₂^{±1} is
a group element in SU(2). Flipping ALL signs (all bits) gives the conjugate
braid, which maps M → M⁻¹ (same eigenvector, negated angle). Since the
Voronoi cell depends only on the eigenvector direction (not the angle),
M and M⁻¹ share the same cell. But flipping all 6 bits changes the parity.
So the cell cannot distinguish parity — it's structurally impossible.

**This is the most important finding: DKC is fundamentally ADDITIVE.**

## 3. Multiplicative Determinism: 81.2% (Part C)

| Step | Triples | Determ | Non-det | Pct |
|------|---------|--------|---------|------|
| 0 | 2 | 2 | 0 | 100% |
| 1 | 2 | 0 | 2 | 0% |
| 2 | 8 | 8 | 0 | 100% |
| 3 | 6 | 4 | 2 | 67% |
| 4 | 8 | 8 | 0 | 100% |
| 5 | 6 | 4 | 2 | 67% |
| **ALL** | **32** | **26** | **6** | **81%** |

Pattern: even steps (σ_1) are 100% deterministic; odd steps (σ_2) split.
This is because σ_1 and σ_1⁻¹ produce different quaternions but the same
cell (0), and subsequent σ_2 multiplication diverges them.

The specific splitting cells: 0 (step 1), 10 (step 3), 1 (step 5).
All splits are binary (2-way).

## 4. Additive DKC: Computes XOR6, 82.8% Deterministic (Parts D–E)

First winning triple: catalog[0, 4, 19]

Weights map to only 3 distinct cells:
- w[0], w[1] = ±q_a → cell 0
- w[2], w[3] = ±q_b → cell 1
- w[4], w[5] = ±q_c → cell 10

### XOR6 verification

All 14 final cell states are parity-pure (confirmed XOR6).

### Trajectory structure

| Step | Distinct cells |
|------|---------------|
| 0 | 2 |
| 1 | 2 |
| 2 | 5 |
| 3 | 5 |
| 4 | 14 |
| 5 | 14 |

The state space expands from 2 (step 0: zero or w[0]) to 14 (step 4:
all cells used including identity). Steps pair up because w[2k] and
w[2k+1] = -w[2k] share the same cell, so adding either creates similar
branching.

### Determinism test

| Step | Triples | Determ | Non-det | Pct |
|------|---------|--------|---------|------|
| 0 | 2 | 2 | 0 | 100% |
| 1 | 4 | 4 | 0 | 100% |
| 2 | 4 | 3 | 1 | 75% |
| 3 | 10 | 9 | 1 | 90% |
| 4 | 10 | 6 | 4 | 60% |
| 5 | 28 | 24 | 4 | 86% |
| **ALL** | **58** | **48** | **10** | **83%** |

**Every non-deterministic transition is bit=1** (adding a weight). Bit=0
transitions are trivially deterministic (the sum doesn't change, so the
cell doesn't change). The splits occur when two different partial sums
happen to land in the same Voronoi cell, then diverge when the same
weight is added.

Cell 0 is the main culprit: it splits at steps 2, 3, 4, and 5.
Cell 1 splits at steps 4 and 5.

### Cell collisions

1,296 instances where two inputs have the same cell at some step but
different partial sum histories. These collisions are the source of all
non-determinism.

### Sufficient statistic test

The cell at step k does NOT predict the final XOR6 output for any
step k < 5 (the final step). This means the cell is not just failing
as an automaton state — it's also insufficient as a predictor of the
computation's outcome. Only at the final step (when the full sum is
computed) does the cell determine the output.

## 5. Why ~83% and Not 100%?

The Voronoi cell captures the DIRECTION of the partial sum's eigenvector
but discards two pieces of information:

1. **Eigenvalue angle**: Two quaternions in the same cell can have
   different rotation angles (θ). When a weight is added, the angle
   affects the result.

2. **Magnitude**: The partial sum is not unit-norm. Two sums with the
   same direction but different magnitudes will behave differently
   under addition.

The 13-cell Voronoi partition is a projection from a 4D quaternion space
to a discrete set. It preserves 83% of the computational structure but
the remaining 17% requires finer state information.

The max split factor is always 2 (binary), suggesting the information
loss is minimal at each step — but it accumulates.

## 6. Synthesis

| Formulation | Computes XOR6? | Determinism | Why |
|-------------|---------------|-------------|-----|
| Multiplicative (braid product) | NO | 81% | Cell can't distinguish parity |
| Additive (weight sum) | YES | 83% | Cell loses magnitude/angle info |

The DKC computation is:
- **Additive, not multiplicative**: Vector sums, not matrix products
- **Almost an automaton**: 83% of cell transitions are deterministic
- **Binary splitting**: Where non-determinism occurs, it's always 2-way
- **Cell 0 is the bottleneck**: Most splits originate from this cell

The 13-cell Voronoi partition is remarkably close to supporting an
automaton structure.

## 7. Finer Partition: 27 Cells Needed (Part F)

### Distinct partial sums per step

| Step | Possible (2^{k+1}) | Distinct | Cells needed |
|------|-------------------|----------|-------------|
| 0 | 2 | 2 | 2 |
| 1 | 4 | 3 | 3 |
| 2 | 8 | 6 | 6 |
| 3 | 16 | 9 | 9 |
| 4 | 32 | 18 | 18 |
| 5 | 64 | **27** | **27** |

The minimum number of cells for 100% determinism is **27** — the maximum
number of distinct partial sums at any step. With 64 possible inputs at
step 5, only 27 distinct quaternion sums arise (the ±weight structure
creates many cancellations).

### Signed partition test

| Partition | Cells | Determinism |
|-----------|-------|-------------|
| Unsigned (standard) | 14 | 82.8% |
| Signed (no antipodal ID) | 27 | 87.1% |

The 27-cell signed partition improves determinism from 82.8% to 87.1%
but does NOT achieve 100%. It uses only 17 of its 27 cells at step 5.
The remaining non-determinism comes from partial sums that have different
magnitudes but the same eigenvector direction — the cell captures the
compass heading but not the odometer reading.

## 8. Universal Determinism: 82.8% Is a Structural Constant (Part G)

**All 36 winning triples have exactly 82.8% determinism (48/58 transitions).**

| Metric | Value |
|--------|-------|
| Winners tested | 36 |
| Min determinism | 82.8% |
| Max determinism | 82.8% |
| Mean determinism | 82.8% |
| 100% deterministic | 0 |

Zero variation across all triples. 82.8% determinism (48/58 transitions)
is a **structural constant** of DKC at ζ_8, invariant across all 36
winning triples. The information lost by the Voronoi cell is the partial
sum magnitude — the cell captures direction but not distance from origin.

This universality follows from the symmetry group: all 36 winning triples
are related by the binary octahedral group acting on the 13 directions.
The combinatorial structure of cell collisions is identical across all
triples. The "almost automaton" property is intrinsic to the geometry
of the 13-direction partition, not an accident of triple selection.

## 9. Updated Synthesis

| Formulation | Computes XOR6? | Determinism | Why |
|-------------|---------------|-------------|-----|
| Multiplicative (braid product) | NO | 81% | Cell can't distinguish parity |
| Additive (weight sum) | YES | 83% | Cell loses magnitude info |
| Additive + signed partition | YES | 87% | Still loses magnitude info |

The DKC computation is:
- **Additive, not multiplicative**: Vector sums, not matrix products
- **Almost an automaton**: 82.8% determinism — a structural constant
- **Universal**: Identical across all 36 winning triples
- **Compass without odometer**: The cell captures direction but not magnitude
- **27 states at step 5**: Only 27 of 64 possible partial sums are distinct
- **Binary splitting**: All non-deterministic transitions split 2-way

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 67 | Eigenvector S² | 13 directions, 36 XOR6 solutions |
| 71 | Spectral DKC | Bandwidth l=6; 2l+1=13=directions |
| 72 | Spherical Design | NOT a t-design; optimal design gets fewer solutions |
| **73** | **Automaton** | **NOT an automaton (82.8% universal); additive not multiplicative; 27 min cells** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo73
./demo73
```

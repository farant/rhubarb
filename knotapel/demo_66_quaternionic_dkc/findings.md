# Demo 66: Quaternionic DKC — First Contact

## Summary

Instead of extracting a scalar bracket (trace) from each braid, we extract the full
quaternion from an SU(2) representation. This preserves the 3D rotational information
that the trace discards, lifting the Kauffman bracket computation from S¹ (complex plane)
to S³ (unit quaternions). The result: the **24-cell** polytope emerges as the natural
computational geometry, and its Voronoi partition dramatically outperforms arbitrary grids.

**Test count: 30/30 pass (Parts A–L)**

---

## 1. The 24-Cell Emergence (Part C)

The SU(2) braid representation with generators σ₁ → (1+i)/√2 and σ₂ → (1−k)/√2
produces exactly **24 distinct unit quaternions** (mod ±) from 87,890 enumerated braids.
These decompose as:

| Type | Count | Description |
|------|-------|-------------|
| Axis-aligned | 4 | ±1, ±i, ±j, ±k (mod ±) |
| Edge (√2/2) | 12 | Two components at ±1/√2 |
| Body (½) | 8 | All four components at ±½ |
| **Total** | **24** | |

These are the vertices of the **24-cell**, the unique self-dual regular 4-polytope.
The 48 quaternions (with negatives) form the **binary octahedral group**.

Meanwhile, the same braids produce **100 distinct Kauffman bracket values** in Z[ζ₈].
The quaternion-to-bracket map is many-to-one: up to 8 quaternions share a single bracket.

## 2. Bracket ≠ Trace (Part D)

The standard relationship between the Kauffman bracket and SU(2) representation theory
predicts bracket ∝ trace(ρ(b)) = 2·Re(q). Our data shows this is **NOT** a simple scalar
relationship:

- Average Re(bracket)/trace ratio: −0.0799 (no clean constant)
- Same quaternion maps to different brackets in 8,424 of 124,750 pairs
- The bracket includes Markov trace normalization and writhe correction that the raw
  quaternion does not

**Key insight:** The bracket and quaternion are **complementary projections** of the full
braid group representation — neither subsumes the other.

## 3. Antipodal-Only XOR6 (Part E)

Full combinatorial search over all C(24,6) = 475,020 six-tuples from the quaternionic
catalog finds **zero** XOR6 solutions at any grid resolution (3×3 through 8×8).

But restricting to **antipodal pairs** (3 pairs of (q, −q) forming 6 weights):

| Grid | Cells | Solutions |
|------|-------|-----------|
| 3×3 | 9 | 0 |
| 4×4 | 16 | 0 |
| 5×5 | 25 | 0 |
| 6×6 | 36 | **3** |
| 8×8 | 64 | **34** |

The antipodal constraint is **absolute** — it reflects the SU(2) → SO(3) double cover.
This matches the complex S¹ result from Demo 65 where only antipodal weight pairs succeed.

## 4. 24-Cell Voronoi: The Natural Activation (Part H)

Instead of arbitrary geographic grids, assign each subset sum to its nearest 24-cell vertex
(Voronoi cell on S³). This gives **25 cells** (24 Voronoi + 1 zero cell).

**Result: 35 solutions at 25 cells.**

| Activation | Cells | Solutions |
|-----------|-------|-----------|
| Geographic 6×6 | 36 | 3 |
| Geographic 8×8 | 64 | 34 |
| **24-cell Voronoi** | **25** | **35** |

The 24-cell's natural geometry with only 25 cells finds **more solutions than 64
geographic cells**. The polytope IS the computation, not just the stage.

**Connection to complex DKC:** On S¹, the minimum sector count for XOR6 is k=24 (Demo 65).
The match 24 ≈ 25 is not coincidental — the complex k=24 result is a **projection** of the
24-cell. When 24 quaternion vertices project onto S¹ via the trace, exactly 24 sectors are
needed to resolve them.

## 5. Universal 7/7/11 Structure (Parts I, L)

Every one of the 35 Voronoi XOR6 solutions has **exactly** the same dimensional structure:

- **7 cells** labeled 0 (parity-0 sums only)
- **7 cells** labeled 1 (parity-1 sums only)
- **11 cells** unused (no subset sum lands there)
- **Zero cell** always labeled 0

This is a geometric constraint: 3 antipodal pairs define 3 great circles on S³, and
the 64 subset sums only touch 14 of the 25 Voronoi cells. The remaining 11 cells are
in geometric "shadow."

**Cell usage across all 35 winners:**
- Every cell is used in at least one solution (no permanently excluded cells)
- Only the zero cell is used in all 35
- Usage frequency ranges from 11/35 to 25/35
- 24 distinct unused-cell patterns among the 35 winners

## 6. Six Fundamental Patterns Under F4 Symmetry (Part K)

The full two-sided symmetry group of the 24-cell (q → g·q·h for all group elements g,h)
has **exactly 576 elements** — the F4 Coxeter rotation subgroup.

Under this full symmetry, the 35 solutions decompose into **6 orbits**:

| Orbit | Size | Representative | Character |
|-------|------|---------------|-----------|
| 1 | 12 | [0,1,18] | Edge-dominant-1 |
| 2 | 5 | [0,1,23] | Edge-dominant-1 |
| 3 | 7 | [2,3,9] | Body-dominant-1 |
| 4 | 2 | [2,9,19] | Body-dominant-1 |
| 5 | 5 | [4,5,15] | Edge-dominant-1 |
| 6 | 4 | [6,12,20] | Body-dominant-1 |

The 6 representatives fall into **two families**:
- **Edge-dominant** (orbits 1,2,5 — 22 solutions): edge-type cells → label 1,
  axis/body → label 0
- **Body-dominant** (orbits 3,4,6 — 13 solutions): axis/body → label 1,
  edge-type → label 0

These two families are essentially **complementary labelings** of the same cell structure.

Note: orbit sizes (12,7,5,5,4,2) do not all divide 576. The 35 solutions are the
intersection of full F4 orbits with the antipodal XOR6 constraint — the full orbit of
each representative is larger, but most images fall outside the winning set.

## 7. Complementary Hash Functions (Part J)

Treating bracket and quaternion as "hash functions" for braids:

| Category | Count | Rate |
|----------|-------|------|
| Same bracket AND quaternion | 16,047 | 12.86% |
| Same bracket, different quaternion | **450** | 0.36% |
| Same quaternion, different bracket | 8,424 | 6.75% |
| Different both | 99,829 | 80.03% |

The quaternion **resolves 450 bracket collisions** — braids that produce the same bracket
value but different quaternions. Conversely, the bracket distinguishes 8,424 cases where
the quaternion agrees. Neither is strictly more informative; they capture different aspects
of the braid group representation.

## 8. Anatomy of Winning Tuples (Part G)

At 6×6 = 36 geographic cells, the 3 winning tuples include:
- 2 mixed (edge + body), non-orthogonal
- 1 all-edge, **mutually orthogonal** (forming a coordinate frame in R⁴)

At 8×8 = 64 cells, 34 winners:
- 15/34 mutually orthogonal (44%)
- Vertex type breakdown: 0 all-axis, 5 all-edge, **16 all-body**, 13 mixed
- Body-type (±½,±½,±½,±½) vertices dominate the solution landscape

## Open Questions

1. **Why exactly 6 orbits?** The orbit count 6 = 1+2+3 suggests a connection to the
   partition structure of 6 (the XOR arity). Does XOR_n on the 24-cell always have
   n*(n-1)/2 orbits?

2. **Complementary families:** The edge-dominant/body-dominant split (22/13) might be
   related to the 24-cell's self-duality. Under the duality that exchanges vertices
   with face centers, do the two families swap?

3. **Scaling to XOR7:** Demo 65 showed XOR7 requires k=91 sectors on S¹. What happens
   with the 24-cell Voronoi activation? Does XOR7 require more than 25 cells, or does
   the richer 4D geometry accommodate it?

4. **Higher quaternion groups:** The binary tetrahedral (24), octahedral (48), and
   icosahedral (120) groups form a sequence. Does moving to the 120-cell (binary
   icosahedral group at level ℓ=5) change the computational landscape?

5. **Zero-margin question:** Demo 65 showed all S¹ solutions have zero margin to sector
   boundaries. Do the Voronoi solutions also sit exactly on cell boundaries, or does
   the 4D geometry provide genuine separation?

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo66
./demo66
```

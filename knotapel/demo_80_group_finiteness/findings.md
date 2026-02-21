# Demo 80: Group Finiteness Survey

## Summary

For each root of unity ζ_N, the SU(2) representation generators at angle
2π/N generate a quaternion group via closure. The **finiteness of this
group determines the hard capacity ceiling** for Boolean computation.

**Only ζ₄ and ζ₈ generate finite groups.** Everything else — ζ₆, ζ₁₀, ζ₁₂,
ζ₁₆, ζ₂₀ — generates an infinite (or very large) group. The boundary is
NOT monotonic: ζ₆ (between the two finite cases) is infinite.

**Test count: 11/11 pass**

---

## 1. Survey Results

| Root  | Angle  | Size     | Rounds | Dirs | Angles | ADE Type            |
|-------|--------|----------|--------|------|--------|---------------------|
| ζ₄    | 90.0°  | 4        | 2      | 3    | 2      | Binary Dihedral 2D₂ |
| ζ₆    | 60.0°  | >4096    | 10     | 512  | 37     | INFINITE            |
| ζ₈    | 45.0°  | 24       | 4      | 13   | 4      | Binary Octahedral   |
| ζ₁₀   | 36.0°  | >4096    | 7      | 512  | 71     | INFINITE            |
| ζ₁₂   | 30.0°  | >4096    | 8      | 512  | 43     | INFINITE            |
| ζ₁₆   | 22.5°  | >4096    | 8      | 512  | 51     | INFINITE            |
| ζ₂₀   | 18.0°  | >4096    | 7      | 512  | 67     | INFINITE            |

Finite: 2 (ζ₄, ζ₈). Infinite: 5 (ζ₆, ζ₁₀, ζ₁₂, ζ₁₆, ζ₂₀).

## 2. ADE Classification

The finite subgroups of SU(2) are classified:
- Cyclic Z_n (any order)
- Binary dihedral 2D_n (order 4n)
- Binary tetrahedral 2T (order 24) — E₆
- Binary octahedral 2O (order 48) — E₇
- Binary icosahedral 2I (order 120) — E₈

Our results:
- **ζ₄ → Binary Dihedral 2D₂** (order 8, 4 up to ±). The quaternion
  group Q₈ = {±1, ±i, ±j, ±k}.
- **ζ₈ → Binary Octahedral 2O** (order 48, 24 up to ±). E₇ in the ADE
  classification. 13 directions, 4 half-angles (0°, 45°, 60°, 90°).

Notable: ζ₁₀ (angle π/5, Fibonacci anyons territory) is NOT binary
icosahedral — it's infinite. The golden ratio angle does not close.

## 3. Why Only ζ₄ and ζ₈ Are Finite

The generators are:
- R₁ = cos(θ) + sin(θ)·i  (rotation by 2θ around x-axis)
- R₂ = cos(θ) + sin(θ)·(-k) (rotation by 2θ around z-axis)

where θ = 2π/N is the half-angle.

For the group to be finite, products of R₁ and R₂ must eventually
repeat. This happens when cos(θ) is an element of a specific algebraic
set. The key values:

| θ       | cos(θ) | Algebraic degree | Group    |
|---------|--------|------------------|----------|
| π/2     | 0      | 1 (rational)     | Finite   |
| π/4     | √2/2   | 2 (quadratic)    | Finite   |
| π/3     | 1/2    | 1 (rational!)    | INFINITE |
| π/5     | φ/2    | 2 (quadratic)    | INFINITE |
| π/6     | √3/2   | 2 (quadratic)    | INFINITE |

Surprisingly, cos(π/3) = 1/2 is RATIONAL but generates an infinite group.
And cos(π/5) is quadratic (golden ratio) but also generates infinite.
The finiteness is NOT simply about algebraic degree of cos(θ).

The deeper reason: ζ₄ (θ = π/2) generates order-4 rotations (90° turns
around perpendicular axes), which close into the octahedral symmetry
group. ζ₈ (θ = π/4) generates order-8 rotations that ALSO close (into
the binary octahedral group). But ζ₆ (θ = π/3) generates order-6
rotations (60° turns) around perpendicular axes, and these do NOT close
— the products create an ever-growing set of new directions.

The mathematical fact: rotations by π/2 and π/4 around perpendicular
axes generate finite subgroups of SO(3) because they are elements of the
octahedral symmetry group. Rotations by π/3 around perpendicular axes
are NOT elements of any finite point group — the 60° rotation is a
symmetry of the HEXAGONAL system, but two perpendicular hexagonal axes
generate an infinite group.

## 4. Computational Implications

| Group type | Example | Capacity | Reason |
|------------|---------|----------|--------|
| Finite, small | ζ₄ (4 elts) | XOR4 only? | Too few quaternions |
| Finite, medium | ζ₈ (24 elts) | XOR8 (confirmed) | Hard ceiling at 24 |
| Infinite | ζ₆, ζ₁₀, ζ₁₂, ... | Potentially unbounded | No ceiling on catalog size |

For infinite groups, the capacity is limited only by:
1. Computational cost of searching larger catalogs
2. The Voronoi/sector resolution needed (more cells = more separation)
3. Possibly by algebraic structure (not all infinite groups are equal)

Demo 79 retest confirmed: ζ₁₂ with 4096-entry catalog gives 50+ XOR12
winners. The "wall" at XOR12 was entirely an artifact of the 256-entry
truncation.

## 5. Structure of Infinite Groups

All infinite groups show the same pattern at 4096 entries:
- **512 directions** (saturated MAX_DIR), covering all 8 S² octants
- **37-71 distinct half-angles**, with minimum gaps < 1° (approaching
  continuum)
- Directions appear to approach uniform S² coverage (dense subgroup)

The number of half-angles varies:
- ζ₆: 37 angles
- ζ₁₀: 71 angles (most for its size — Fibonacci anyons?)
- ζ₁₂: 43 angles
- ζ₁₆: 51 angles
- ζ₂₀: 67 angles

This non-monotonicity (ζ₁₀ has more distinct angles than ζ₁₂ or ζ₁₆ at
the same group size) suggests different infinite groups have different
density structures.

## 6. The Alternating Pattern

The finite/infinite pattern by N:
- N=4: FINITE
- N=6: infinite
- N=8: FINITE
- N=10: infinite
- N=12: infinite
- N=16: infinite
- N=20: infinite

The two finite cases (N=4, N=8) correspond to angles θ = π/2 and π/4,
both of which are rotational symmetries of the CUBE/OCTAHEDRON. No other
sampled angles correspond to a regular polyhedron's rotational symmetry.

Conjecture: the finite cases are EXACTLY those angles that are rotational
symmetries of one of the five Platonic solids (or equivalently, one of
the ADE exceptional groups). Testing θ = π/3 for icosahedral (ζ₁₀)
showed this is NOT the case — the icosahedron has 5-fold symmetry but
π/5 generates an infinite group when combined with a perpendicular axis.

The correct characterization may be: only angles of the form π/(2^k) for
k ≥ 1 generate finite groups (π/2 for k=1, π/4 for k=2). This would
predict ζ₃₂ (θ = π/16) might also be finite.

## 7. Quantum Dimension Connection

At each root of unity A = ζ_N, the quantum parameter is q = A² = ζ_{N/2}.
The quantum integer [2]_q = q + q⁻¹ = 2·cos(2π/N). Key values:

| Root | q       | [2]_q           | Status    |
|------|---------|-----------------|-----------|
| ζ₄   | -1      | -2              | Finite    |
| ζ₆   | ζ₃      | 1               | INFINITE  |
| ζ₈   | i       | **0**           | Finite    |
| ζ₁₀  | ζ₅      | (√5-1)/2 ≈ 0.618 | INFINITE |
| ζ₁₂  | ζ₆      | √3 ≈ 1.732     | INFINITE  |

At ζ₈, the quantum dimension [2]_q **vanishes**. This is the most
SINGULAR point of the quantum group parameter space — where the
representation theory degenerates maximally. The finite groups correspond
to these singular/lattice points.

This connects to **Kuperberg's #P-hardness result**: his proof that
evaluating the Jones polynomial is #P-hard applies specifically at
**non-lattice** roots of unity. Our finite groups (ζ₄, ζ₈) are exactly
the lattice roots — where computation is tractable and the group
structure provides a clean algebraic framework.

The pattern: lattice roots → finite groups → hard capacity ceiling but
tractable computation. Non-lattice roots → infinite groups → potentially
unbounded capacity but #P-hard in general.

## 8. Open Questions

1. **Is ζ₃₂ (π/16) finite?** The conjecture π/(2^k) predicts yes.
2. **What is the exact group theory?** Why do perpendicular rotations
   by π/2 and π/4 close but π/3 and π/6 don't?
3. **Do all infinite groups give unbounded capacity?** Or do some infinite
   groups have structural limitations?
4. **Which infinite group is "best" for computation?** ζ₁₀ has the most
   half-angles at 4096 entries (71 vs 37-67). Does more angular diversity
   mean easier XOR computation?

## Cross-Demo Connection

| Demo   | What                  | Key Result                             |
|--------|-----------------------|----------------------------------------|
| 66     | Quaternionic DKC      | 24 quaternions, 24-cell geometry       |
| 77     | Activation Zoo        | XOR8 = 6 (ζ₈, Sec8 x Vor)             |
| 78     | Recursive Scaling     | XOR10 = 0 at ζ₈. Wall confirmed.      |
| 79     | ζ₁₂ Capacity          | XOR10=124, XOR12=50+ (wall was artifact)|
| **80** | **Finiteness Survey** | **Only ζ₄ and ζ₈ finite. All else infinite.**|

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   -O2 main.c -lm -o demo80
./demo80
```

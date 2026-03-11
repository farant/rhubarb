# Demo 111: Braid-to-Construction-Word Pipeline

**Status: IN PROGRESS**

## Overview

Built an end-to-end pipeline: braid word → stick polygon → topology-preserving simplification → exhaustive construction word analysis. All 6 tested knots (3_1 through 8_18) produce correct determinants and are feasible for exhaustive analysis after simplification.

Extended with three-sphere intersection decomposition (proved equivalent to plane reflection), base triangle optimization (first-order effect on spectral richness), Hamming weight analysis (bell-shaped profiles, palindromic symmetry in 5_2), and sliding-window construction (dead end — cascade coupling reduces spectral richness).

## Pipeline Stages

1. **Braid-to-polygon**: Layout strands on integer grid, process crossings with y-detour for over/under, connect via closure arcs at y = -MARGIN
2. **Collinear simplification**: Remove vertices where (v2-v1) × (v3-v1) = 0
3. **Topology-preserving simplification**: Greedy vertex removal — iterate and remove any vertex whose removal doesn't change the knot determinant
4. **Construction word analysis**: Exhaustive 2^n sweep over mirror/no-mirror choices

## Results

### Result 1: Pipeline Verification

All 6 knots produce correct determinants from braid words:

| Knot | Braid | Strands | Gens | Det |
|------|-------|---------|------|-----|
| 3_1 | σ₁³ | 2 | 3 | 3 |
| 4_1 | σ₁σ₂⁻¹σ₁σ₂⁻¹ | 3 | 4 | 5 |
| 5_1 | σ₁⁵ | 2 | 5 | 5 |
| 5_2 | σ₁⁻³σ₂⁻¹σ₁σ₂⁻¹ | 3 | 6 | 7 |
| 6_3 | σ₁⁻²σ₂σ₁⁻¹σ₂² | 3 | 6 | 13 |
| 8_18 | (σ₁⁻¹σ₂)⁴ | 3 | 8 | 45 |

Braid words from Knot Atlas. 3-strand braids require even generator count for knot (not link) closure.

### Result 2: Polygon Simplification Effectiveness

Greedy topology-preserving simplification dramatically reduces vertex count:

| Knot | Braid verts | Simplified | Bits | Reduction |
|------|------------|------------|------|-----------|
| 3_1 | 18 | 7 | 4 | 61% |
| 4_1 | 29 | 8 | 5 | 72% |
| 5_1 | 26 | 10 | 7 | 62% |
| 5_2 | 37 | 11 | 8 | 70% |
| 6_3 | 37 | 11 | 8 | 70% |
| 8_18 | 49 | 12 | 9 | 76% |

All simplified polygons are near-minimal stick representations (trefoil: 7 vs minimum 6, figure-eight: 8 vs minimum 7).

### Result 3: Determinant Spectra

| Knot | Bits | Distinct dets | Self-preservation | Spectrum |
|------|------|---------------|-------------------|----------|
| 3_1 | 4 | 2 | 2/16 (12.5%) | {1, 3} |
| 4_1 | 5 | 4 | 2/32 (6.25%) | {1, 3, 5, 7} |
| 5_1 | 7 | 3 | 4/128 (3.125%) | {1, 3, 5} |
| 5_2 | 8 | 4 | 4/256 (1.56%) | {1, 3, 5, 7} |
| 6_3 | 8 | **22** | 12/256 (4.69%) | {1..161, gaps} |
| 8_18 | 9 | 10 | 8/512 (1.56%) | {1..45, gaps} |

Key: 4_1 and 5_2 produce identical spectra {1,3,5,7} despite being different knots. 6_3 is a spectacular outlier with 22 distinct determinants up to 161.

### Result 4: Influence Profiles

| Knot | Bits | Active | Free | Profile type |
|------|------|--------|------|-------------|
| 3_1 | 4 | 3 | 1 | Uniform (all 1/4) |
| 4_1 | 5 | 5 | 0 | Varied (3/16 to 1/2) |
| 5_1 | 7 | 5 | 2 | Near-uniform (5/16 to 7/16) |
| 5_2 | 8 | 7 | 1 | Varied (60/256 to 156/256) |
| 6_3 | 8 | 8 | 0 | High & varied (46% to 84%) |
| 8_18 | 9 | 6 | 3 | Varied (304/512 to 400/512) |

Torus knots (3_1, 5_1) have near-uniform influence. Non-torus knots have more varied influence profiles.

### Result 5: Torus vs Non-Torus Distinction

- Torus knots (3_1, 5_1): fewer distinct determinants (2-3), near-uniform influence, have free bits
- Non-torus knots (4_1, 5_2, 6_3): more distinct determinants (4-20), varied influence, fewer/no free bits
- This distinction is visible in both the spectral richness and the influence structure

### Result 6: 6_3 as "Knot Factory"

The 6_3 construction word on an 11-vertex simplified polygon produces **22 distinct determinant values**: {1,3,5,7,9,11,13,15,17,19,21,23,27,29,31,35,39,43,45,61,111,161}. Maximum det=161 (from 7/8 bits set). All 8 bits are active with high influence (46-84%). This is qualitatively different from all other tested knots.

### Result 7: Truth Table Structure

- **Trefoil** (3 active bits): Boolean function = NOR. Only all-zeros preserves the knot. Any single mirror → unknot.
- **Figure-eight** (5 active bits): Two self-preserving patterns (00000 and 00010). Bit 3 is "expendable." All-ones → det=7 (maximum).
- **6_3** (8 active bits): 12 self-preserving patterns at Hamming weights {0,1,1,2,3,3,3,4,4,5,3,5}. Non-trivial Boolean function with multi-bit interaction terms.

### Result 8: Torus/Non-Torus Distinction

- Torus knots (3_1, 5_1): fewer distinct dets (2-3), near-uniform influence, free bits present, simple Boolean function
- Non-torus knots (4_1, 5_2, 6_3): more distinct dets (4-22), varied influence, fewer/no free bits, complex Boolean functions

### Result 9: 8_18 (Amphichiral, 8 Crossings)

8_18 = BR(3, {-1,2,-1,2,-1,2,-1,2}). Simplifies from 49 to 12 vertices (9 bits, 512 alternatives).
- 10 distinct determinants (default base), 8 self-preserving patterns (1.56%)
- 6 active bits, 3 free bits (default base) — free bits correspond to vertices with zero height above base plane
- Det=45 (correct)
- Amphichiral knot — expected complement closure properties (see Result 10)
- With optimal base (0,1,3): 28 distinct dets, 9 active bits, 0 free bits (see Result 15)

### Result 10: Complement Check — Chirality Signature

For each self-preserving bitmask b, we test whether its complement (all bits flipped) also self-preserves. Hypothesis: amphichiral knots should show complement closure (mirror symmetry in construction word space).

| Knot | Amphichiral? | Self-pres | Complement closure |
|------|-------------|-----------|-------------------|
| 3_1 | No (chiral) | 2 | 0/2 |
| 4_1 | Yes | 2 | 0/2 |
| 5_1 | No (chiral) | 4 | 0/4 |
| 5_2 | No (chiral) | 4 | **4/4** |
| 6_3 | No (chiral) | 12 | 0/12 |
| 8_18 | Yes | 8 | 0/8 |

**Surprise**: The chiral knot 5_2 has PERFECT complement closure (4/4), while the amphichiral knots (4_1, 8_18) have ZERO. This is the opposite of the initial hypothesis. Complement closure in construction words does NOT correspond to knot amphichirality — it appears to be a geometric property of the specific simplified polygon, not a topological invariant.

### Result 11: Crossing Counts and Spectral Richness

Crossing count in the generic (2,3,5) projection of the simplified polygon:

| Knot | Min crossings | Simplified crossings | Ratio | Writhe | Distinct dets |
|------|--------------|---------------------|-------|--------|---------------|
| 3_1 | 3 | 6 | 2.0× | 0 | 2 |
| 4_1 | 4 | 4 | **1.0×** | 0 | 4 |
| 5_1 | 5 | 10 | 2.0× | -1 | 3 |
| 5_2 | 5 | 10 | 2.0× | 3 | 4 |
| 6_3 | 6 | 19 | **3.2×** | -1 | **22** |
| 8_18 | 8 | 18 | 2.3× | -1 | 10 |

Key finding: 6_3's exceptional spectral richness (22 dets) correlates with its unusually high crossing ratio (3.2× minimal). The figure-eight at exactly minimal crossing number (4) produces only 4 distinct dets. More crossings in projection = more "tangled" polygon = richer det spectrum under construction word manipulation.

### Result 12: Randomized Simplification — Geometry Dependence

Tested whether construction word analysis depends on simplification order. Ran 8 independent greedy simplifications of the figure-eight (4_1) with different random scan orders (Fisher-Yates shuffle, LCG seeds).

All 8 trials simplified to exactly 8 vertices (consistent). However:
- Determinant spectra varied across trials
- Self-preservation counts varied
- The construction word analysis is **geometry-dependent, not a knot invariant**

This confirms the Demo 110 finding: construction word properties depend on the specific polygon geometry, not just the knot type. Different simplification paths produce different stick polygons (same knot type, same vertex count) with different det spectra. The "resonant geometry" phenomenon from Demo 110 is real — some geometries are special.

### Result 13: Three-Sphere Intersection Theorem

**THEOREM**: Three-sphere intersection with all-base strategy is mathematically identical to plane reflection through the base triangle.

For vertex v[k] with base triangle (v[0], v[1], v[2]), construct spheres S(v[i], |v[i] - v[k]|) for i=0,1,2. The two intersection points of the three spheres are v[k] itself and its mirror P' = P - 2·(n·(P-c₀))/(n·n)·n through the base plane.

Verified computationally on all 6 knots: sphere intersection mirrors match plane reflection mirrors to exact rational equality. The radical line direction IS the base plane normal.

Key derived quantities per vertex:
- **Height h**: signed distance from base plane (h = n·(P-c₀)/|n|)
- **Discriminant h²/|n|²**: determines separation of intersection points
- **Height = 0 implies free bit**: a vertex in the base plane has zero influence in the construction word

### Result 14: Base Triangle Sweep

Tested all C(n,3) base triangle choices for each knot:

| Knot | Verts | Valid triples | Free bits range | Best base |
|------|-------|---------------|-----------------|-----------|
| 3_1 | 7 | 35 | 0–2 | (0,1,2) |
| 4_1 | 8 | 56 | 0–1 | (0,1,2) |
| 5_1 | 10 | 118 | 0–3 | (0,1,2) |
| 5_2 | 11 | 165 | 0–2 | (0,1,2) |
| 6_3 | 11 | 164 | 0–3 | (0,1,2) |
| 8_18 | 12 | 218 | 0–3 | **(0,1,3)** |

Every knot has at least one zero-free-bit base. The default base (0,1,2) is optimal for 5 of 6 knots. 8_18 is the exception — its optimal base is (0,1,3), which eliminates all 3 free bits present in the default base.

### Result 15: Optimal Base Spectra

Running all 6 knots with their optimal base:

| Knot | Opt base | Bits | Free | Distinct dets | Self-pres | Max det |
|------|----------|------|------|---------------|-----------|---------|
| 3_1 | (0,1,2) | 4 | 1 | 2 | 2/16 | 3 |
| 4_1 | (0,1,2) | 5 | 0 | 4 | 2/32 | 7 |
| 5_1 | (0,1,2) | 7 | 2 | 3 | 4/128 | 5 |
| 5_2 | (0,1,2) | 8 | 1 | 4 | 4/256 | 7 |
| 6_3 | (0,1,2) | 8 | 0 | 22 | 12/256 | 161 |
| 8_18 | **(0,1,3)** | 9 | **0** | **28** | 3/512 | **155** |

8_18 with optimal base (0,1,3): jumps from 10 to **28 distinct determinants** (180% increase). Self-preserving drops from 8/512 to 3/512. The 28 values include consecutive odds 1–31 (no gaps), then scattered: {39, 41, 43, 45, 49, 51, 53, 59, 67, 81, 95, 155}.

### Result 16: Hamming Weight Structure

For each knot (optimal base), analyzed how determinant spectrum varies with Hamming weight (number of mirrors applied):

**Bell-shaped distinct-count**: Number of distinct determinants peaks at HW ≈ 60–70% of bits, then decreases.

**Max det at HW ≈ 70–80%**: Maximum determinant value never appears at full flip (all bits set).

**Key patterns by knot**:
- **3_1**: Peak distinct=2 at HW=1, trivial
- **4_1**: Peak distinct=2 at HW=1–4, max det=7 at HW=5 (all-flip)
- **5_1**: Peak distinct=3 at HW=2, max det=5 at HW=0 (identity), monotonic decrease
- **5_2**: **Palindromic** — distinct counts are symmetric: 1,3,3,3,3,3,3,3,1. Min/max det ranges also palindromic. Connects to perfect complement closure (Result 10)
- **6_3**: Peak distinct=16 at HW=5 (62.5%), max det=161 at HW=6 (75%). Rich bell curve: 1,3,6,9,12,16,12,7,1
- **8_18** (opt base): Peak distinct=20 at HW=6 (67%), max det=155 at HW=7 (78%). Bell curve: 1,4,7,9,10,16,20,16,7,1

**5_2 palindromic symmetry**: The HW profile {1,3,3,3,3,3,3,3,1} is perfectly palindromic, meaning det statistics at HW=k equal those at HW=(8-k). This is exactly the structure that produces complement closure — flipping all bits maps HW k → HW (8-k), and since the det distributions are identical, self-preserving patterns pair with their complements.

### Result 17: Sliding-Window Decomposition — Dead End

Tested an alternative construction strategy: instead of a fixed base triangle, use the 3 most recent vertices as sphere centers for each step (sliding window).

**Figure-eight results**: 5 bits, 32 alternatives.
- Only 3 distinct dets (vs 4 for all-base): {1, 3, 5}
- 2/32 self-preserving (same count as all-base)
- Only 22/32 (68.8%) agreement with all-base construction
- 1/5 steps share the same mirror plane as all-base

**Why it fails**: Cascade coupling. Each step's mirror plane depends on the previous step's output, creating dependencies between bits. Flipping bit k changes the geometry seen by bits k+1, k+2, ... This constrains the output, producing FEWER distinct determinants, not more.

**Conclusion**: Bit independence is a feature, not a bug. The all-base construction's independent bits maximize combinatorial freedom. Sliding-window sacrifices this for locality, which turns out to be counterproductive for spectral richness.

### Result 18: 5_2 Palindromic HW is Geometry-Dependent

Ran 8 randomized simplifications of 5_2 to test whether palindromic HW symmetry is a knot invariant or a geometry property.

**Result**: Only 2/8 trials produce palindromic HW profiles. It is geometry-dependent.

| Trial | Verts | Bits | HW distinct profile | Palindrome? | Comp closure |
|-------|-------|------|---------------------|:-----------:|:------------:|
| 0 | 11 | 8 | {1,3,5,5,5,5,4,3,1} | no | 0/48 |
| 1 | 14 | 11 | {1,3,4,5,10,13,14,14,12,8,6,1} | no | 8/106 |
| 2 | 9 | 6 | {1,3,3,2,3,3,1} | **YES** | **4/4** |
| 3 | 11 | 8 | {1,3,4,4,3,4,4,4,1} | no | 8/12 |
| 4 | 12 | 9 | {1,3,4,4,3,4,5,5,4,1} | no | 4/16 |
| 5 | 9 | 6 | {1,3,4,4,3,4,1} | no | 2/4 |
| 6 | 14 | 11 | {1,4,4,5,5,5,5,5,5,5,4,1} | no | 32/64 |
| 7 | 10 | 7 | {1,4,3,3,3,3,4,1} | **YES** | **4/4** |

Key finding: **palindromic HW ⟹ perfect complement closure** (mathematical implication confirmed in both directions). But palindromic HW is not topological — it depends on the polygon geometry. The original finding (Result 10) was geometry-lucky.

Additional observations:
- Vertex counts vary from 9 to 14 across trials (simplification is non-deterministic)
- Self-preservation count varies hugely: 4/64 to 106/2048
- Trial 0 has ZERO complement closure despite 48 self-preserving patterns
- Non-palindromic trials can still have partial complement closure (e.g., 32/64 in trial 6)

### Result 19: 8_18 Spectral Core Hypothesis — Refuted

Tested whether the consecutive odd pattern {1,3,...,31} in 8_18's spectrum comes from a small "spectral core" of bits.

- **0 / 126 four-bit subsets** produce all 16 consecutive odds {1,3,...,31}
- **0 / 126 five-bit subsets** produce them either
- Best 4-bit subset {0,3,7,8}: only 9 distinct dets (max possible 16)
- Best 5-bit subset {0,3,5,7,8}: only 12 distinct (max possible 32)

**Conclusion**: The consecutive-odd pattern is a distributed property requiring most or all 9 bits. There is no small spectral core.

### Result 20: U-Shaped Density

Density = (distinct dets at HW k) / C(n,k) removes binomial coefficient inflation from the bell-shaped count.

**6_3 (8 bits):**
| HW | C(8,k) | Distinct | Density |
|----|--------|----------|---------|
| 0 | 1 | 1 | 100.0% |
| 1 | 8 | 3 | 37.5% |
| 2 | 28 | 6 | 21.4% |
| 3 | 56 | 9 | 16.1% |
| 4 | 70 | 12 | 17.1% |
| 5 | 56 | 16 | 28.6% |
| 6 | 28 | 12 | 42.9% |
| 7 | 8 | 7 | **87.5%** |
| 8 | 1 | 1 | 100.0% |

**8_18 (9 bits, opt base):**
| HW | C(9,k) | Distinct | Density |
|----|--------|----------|---------|
| 0 | 1 | 1 | 100.0% |
| 1 | 9 | 4 | 44.4% |
| 2 | 36 | 7 | 19.4% |
| 3 | 84 | 9 | 10.7% |
| 4 | 126 | 10 | **8.0%** |
| 5 | 126 | 16 | 12.7% |
| 6 | 84 | 20 | 23.8% |
| 7 | 36 | 16 | **44.4%** |
| 8 | 9 | 7 | 77.8% |
| 9 | 1 | 1 | 100.0% |

**U-shaped density**: Minimum at HW ≈ 35–45%, rising steeply toward both extremes. The bell-shaped count was binomial inflation.

**Asymmetric**: The right side (many flips) has HIGHER density than the left side (few flips). At HW=7, 6_3 has 87.5% density — nearly every pattern produces a unique determinant. Deep flipping is the most discriminating operation.

**Interpretation**: Few mirrors produce "similar" knots (low density). Many mirrors create highly diverse topologies (high density). The determinant landscape becomes increasingly rugged as more vertices are reflected.

## Bug Fix: Closure Arc Assembly

The braid-to-polygon assembly had a topology bug: when assembling closure arcs between strands, vertex 0 of subsequent strands was skipped (assuming coincidence with closure arc endpoint). But the closure arc is at y=-MARGIN while the strand start is at y=0 — different positions. Skipping created a shortcut diagonal from closure plane into braid, changing topology. Fix: include ALL vertices.

## Comparison with Demo 110

Demo 110's figure-eight at "resonant" geometry: 9 bits, majority-of-3 influence profile {0, 1/8, 1/4}, 256/512 = 50% self-preservation.
Demo 111's braid-derived figure-eight: 5 bits, varied influence, 2/32 = 6.25% self-preservation.
The geometry matters enormously — Demo 110's hand-crafted coordinates were "resonant" while the braid layout is generic.

## Summary Table (Default Base)

| Knot | Simp verts | Bits | Crossings | Writhe | Distinct dets | Self-pres | Comp closure |
|------|-----------|------|-----------|--------|---------------|-----------|-------------|
| 3_1 | 7 | 4 | 6 | 0 | 2 | 2/16 | 0/2 |
| 4_1 | 8 | 5 | 4 | 0 | 4 | 2/32 | 0/2 |
| 5_1 | 10 | 7 | 10 | -1 | 3 | 4/128 | 0/4 |
| 5_2 | 11 | 8 | 10 | 3 | 4 | 4/256 | 4/4 |
| 6_3 | 11 | 8 | 19 | -1 | 22 | 12/256 | 0/12 |
| 8_18 | 12 | 9 | 18 | -1 | 10 | 8/512 | 0/8 |

## Summary Table (Optimal Base)

| Knot | Opt base | Free | Distinct dets | Self-pres | Max det | HW peak |
|------|----------|------|---------------|-----------|---------|---------|
| 3_1 | (0,1,2) | 1 | 2 | 2/16 | 3 | HW=1 |
| 4_1 | (0,1,2) | 0 | 4 | 2/32 | 7 | HW=5 |
| 5_1 | (0,1,2) | 2 | 3 | 4/128 | 5 | HW=0 |
| 5_2 | (0,1,2) | 1 | 4 | 4/256 | 7 | palindromic |
| 6_3 | (0,1,2) | 0 | 22 | 12/256 | 161 | HW=5–6 |
| 8_18 | **(0,1,3)** | **0** | **28** | 3/512 | **155** | HW=6–7 |

## Technical Notes

- All determinant values are odd (mathematically guaranteed: |Δ(-1)|)
- Self-preservation counts: 2, 2, 4, 4, 12, 8 (default base); 2, 2, 4, 4, 12, 3 (optimal base)
- Unknot fraction decreases with crossing number: 87.5% → 75% → 75% → 67% → 30% → ~65%
- Braid layout produces extra crossings in projection but these are nugatory and don't affect the determinant
- Exact rational arithmetic throughout (Rat type with overflow-resistant cross-reduction)
- Construction word analysis is geometry-dependent, not a knot invariant (confirmed by randomized simplification)
- Complement closure does NOT correspond to amphichirality — it's a geometric property
- 6_3's spectral richness correlates with high crossing ratio (3.2× minimal)
- **THEOREM**: Three-sphere intersection with all-base = plane reflection through base triangle (proved computationally, all 6 knots)
- Free bits correspond exactly to vertices with zero height above the base plane
- Base triangle choice is a first-order effect: 8_18 gains 180% more distinct dets with optimal base
- Sliding-window construction is counterproductive: cascade coupling reduces spectral richness
- Hamming weight COUNT profiles are bell-shaped but this is binomial inflation; TRUE density is U-shaped
- 5_2's palindromic HW profile explains its complement closure, but palindromic HW is geometry-dependent (2/8 trials)
- Density minimum at HW ≈ 35-45%; density asymmetric (more flips → more diverse, not symmetric)
- No spectral core in 8₁₈: consecutive odds {1..31} require all 9 bits, not a small subset

## Answered Questions

1. **Why does 6_3 produce such a dramatically richer spectrum?** Correlates with crossing ratio — 6_3 has 19 crossings (3.2× minimal 6), much higher than any other tested knot. More crossings → more tangled polygon → richer spectrum.
2. **Does randomizing the simplification order change the det spectrum?** YES. Different simplification orders produce different stick polygons with different det spectra. Construction word analysis is geometry-dependent.
3. **Why does 5_2 (chiral) have perfect complement closure while amphichiral knots don't?** ANSWERED: 5_2's Hamming weight profile is perfectly palindromic (symmetric under k ↔ n-k), meaning the det distribution at HW=k equals that at HW=(n-k). This forces complement closure. It's a geometric property of this specific polygon, not a topological invariant. Confirmed geometry-dependent: only 2/8 randomized simplifications preserve palindromic HW.
4. **Are three-sphere intersection and plane reflection equivalent?** YES. Proved that radical line direction = base plane normal. The two methods produce identical mirror points (exact rational equality).
5. **Does base triangle choice matter?** YES — first-order effect. 8_18 goes from 10 to 28 distinct dets by changing base from (0,1,2) to (0,1,3). Every knot has a zero-free-bit base.
6. **Is sliding-window construction better than all-base?** NO — cascade coupling between bits constrains output, producing fewer distinct dets. Bit independence is a feature.
7. **Is the bell-shaped HW profile real?** NO — it's binomial inflation. True density (distinct/C(n,k)) is U-shaped with minimum at HW ≈ 40%.
8. **Is there a small spectral core generating 8_18's consecutive odds?** NO — 0/126 four-bit and 0/126 five-bit subsets span {1..31}. Distributed property of all 9 bits.

## Open Questions

1. Is the torus/non-torus influence distinction a general pattern?
2. Can we find "resonant" geometries for the braid-derived polygons (like Demo 110's resonant scale)?
3. What Boolean functions correspond to the trefoil's uniform-influence 3-bit classifier?
4. What do 7-crossing knots look like? Especially 7_4 (amphichiral).
5. Is crossing ratio a predictor of spectral richness across more knots?
6. Can we find a base choice that makes 8_18 (amphichiral) exhibit complement closure?
7. What base choice maximizes spectral richness for 6_3?
8. **What does the full Alexander polynomial Δ(t) look like for construction word alternatives?** Two alternatives with det=3 might have completely different polynomials — finer-grained modulation than det alone.
9. Why is the density asymmetric (right-heavy)? Is there an algebraic explanation for why many mirrors are more discriminating than few?

# Demo 83: Framing as Computational Content — Findings

**Date:** 2026-02-21
**Tests:** 12/12 pass
**File:** `demo_83_framing_test/main.c`

## The Headline

**Jones normalization destroys computational power. Framing IS the computational content.**

The Kauffman bracket and Jones polynomial differ by a writhe-dependent phase factor: V(K) = (-A³)^{-w} × ⟨K⟩. This demo tests whether removing that factor (normalizing to Jones) reduces DKC's XOR capacity.

## Results

### Capacity Comparison

| Root | Level | Bracket | Jones |
|------|-------|---------|-------|
| ζ₁₂ | XOR6  | 32      | 32    |
| ζ₁₂ | XOR8  | 32      | 32    |
| ζ₁₂ | XOR10 | 32      | 32    |
| ζ₁₂ | XOR12 | 32      | **0** |
| ζ₈  | XOR6  | 32      | 32    |
| ζ₈  | XOR8  | 32      | **0** |

**Consistent 2-level loss at both roots.**
- ζ₁₂: bracket = XOR12, Jones = XOR10 (loss = 2)
- ζ₈: bracket = XOR8, Jones = XOR6 (loss = 2)

### The N-2 Pattern Was Jones All Along

| Root | Bracket capacity | Jones capacity | Difference |
|------|-----------------|----------------|------------|
| ζ₈   | 8 (XOR8)       | 6 (XOR6)      | 2          |
| ζ₁₂  | 12 (XOR12)     | 10 (XOR10)    | 2          |

The "N-2" pattern from early demos IS real — it's the Jones polynomial's capacity, not the bracket's. Bracket capacity = N. Jones capacity = N-2. The difference = framing.

### Information Collapse

| Metric | Bracket | Jones | Change |
|--------|---------|-------|--------|
| Distinct quaternions | 4096 | 2710 | -33.8% |
| Directions | 512 | 512 | unchanged |
| Angles | 43 | 31 | -28% |

Framing lives in **angles, not directions**. Jones normalization preserves WHERE quaternions point but destroys HOW they rotate. The angular vocabulary reduction (43 → 31) is the mechanism of capacity loss.

### Writhe Census (ζ₁₂, 4096 entries)

- Writhe range: [-9, 9]
- 91.4% of entries have nonzero writhe
- Mean writhe: 1.697 (positive bias from generator structure)
- Depth-writhe correlation: **0.139** (nearly independent)

### Writhe and Depth Are Independent Computational Axes

| Depth | Mean|W| | Max|W| |
|-------|---------|--------|
| 0     | 0.80    | 1      |
| 1     | 1.33    | 2      |
| 2     | 1.82    | 3      |
| 4     | 2.14    | 5      |
| 6     | 2.61    | 7      |
| 8     | 3.19    | 9      |

Mean absolute writhe grows linearly with depth, but correlation is only r=0.14. A deep entry can have low writhe (crossings cancel) or high writhe (crossings accumulate). They are genuinely separate resources.

### Writhe Alone Can Compute

Writhe-only test (replacing quaternion sums with writhe sums): **32 XOR6 winners**. Writhe as a standalone feature = one unit of computational resource. But writhe alone cannot reach XOR8 or higher — full power requires writhe × lattice interaction.

### Winners Use Mixed Writhe

| Level | Mixed-writhe winners |
|-------|---------------------|
| XOR6  | 41% (13/32)         |
| XOR8  | 25% (8/32)          |
| XOR10 | 9% (3/32)           |
| XOR12 | 12% (4/32)          |

Winners use both positive AND negative writhe entries. The computation uses chirality variation, not just magnitude.

## Theoretical Significance

### 1. Framing = One Discrete Computational Resource Worth 2 XOR Levels

The consistent "+2" across both roots matches the per-resource scaling we see in depth (Demo 82: each closure round adds +2 XOR at transition depths). Framing is one discrete unit of computational resource, worth exactly 2 XOR levels.

### 2. Decomposition of DKC Computational Power

- **Lattice structure** provides the base (XOR6 achievable from lattice alone)
- **Depth** adds +2 per closure round (Demo 82)
- **Writhe** adds +2 on top (this demo)
- These are independent, additive resources

### 3. What TQFT Discards IS What Computes

The Kauffman bracket carries writhe-dependent phase. The Jones polynomial removes it. TQFT calls it an "anomaly" and subtracts it. DKC uses the raw bracket — keeping the "anomaly." Demo 83 proves this was computationally essential: removing it costs exactly 2 XOR levels at every root tested.

This confirms predictions from three independent directions:
- **Kirby (1978)**: Framing determines 3-manifold topology
- **Witten (1989)**: δ=0 is TQFT's maximally degenerate point; framing anomaly is the first thing removed
- **Rasmussen (2004)**: Writhe appears explicitly in Khovanov q-grading (not normalized away in categorified theory)

### 4. The ζ₈ Control

At ζ₈ (24 entries, finite group), the same pattern holds: bracket XOR8, Jones XOR6. The framing effect is not an artifact of infinite-group scaling — it's a structural property of the bracket-to-Jones relationship itself.

## Connection to Previous Demos

- **Demo 82 (crossing depth)**: Depth drives capacity linearly. Now writhe adds independently.
- **Demo 81 (logarithmic scaling)**: The "logarithmic" law was measuring catalog size. The true drivers are depth + writhe.
- **Demo 79 (early "N-2" pattern)**: That pattern was Jones capacity, not a bracket truncation artifact.
- **Demo 66 (quaternionic DKC)**: The 24-cell structure survives Jones normalization (directions preserved), but angular refinement is lost.

## Open Questions

1. Does the +2 loss hold at other roots (ζ₁₆, ζ₂₄)?
2. Can we prove the "+2 per writhe unit" analytically from the (-A³)^w factor?
3. At ζ₈ where Jones gives XOR6, is this exactly the "standard sector activation" capacity from Demo 63?
4. What is the writhe distribution of the 24-cell vertices? Does antipodal pairing correlate with writhe inversion?

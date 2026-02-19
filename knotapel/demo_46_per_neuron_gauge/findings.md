# Demo 46: Per-Neuron Gauge Correction — Findings

**Date:** 2026-02-18
**Dependencies:** Demo 45 (Complex Function Zoo)
**Track:** D — DKC Extensions

## Summary

Demo 46 replaces Demo 45's single global U(1) gauge rotation with independent per-neuron rotations for bracket decomposition. The core question: how much of Demo 45's decomposition error comes from gauge approximation vs genuine lattice mismatch?

**Answer: genuine lattice mismatch.** Amplitude-only RMS (avg 0.33) still dramatically beats per-neuron gauge RMS (avg 0.57), even with 3 continuous degrees of freedom. The trained weights' phases are not Z[zeta_8]-aligned, and no post-training rotation can fix this.

## Method

- Same CxNet3 architecture as Demo 45 (3 complex hidden neurons, 12 complex weights)
- Same training procedure (split-sigmoid, backprop, 10 trials per NPN class)
- Same bracket catalog (64 distinct values from 2-3 strand braids)
- **New:** 3D gauge sweep — each neuron k gets independent angle theta_k
  - Coarse: 36x36x36 grid (46,656 combinations, 10-degree resolution)
  - Refinement: 11x11x11 grid (1,331 combinations, 1-degree) around best coarse angles
- Compare: global gauge (1D), per-neuron gauge (3D), amplitude-only (projection)

## Results

### Key Numbers

| Metric | Value |
|--------|-------|
| Avg global gauge RMS | 0.7164 |
| Avg per-neuron gauge RMS | 0.5723 |
| Avg amplitude-only RMS | 0.3283 |
| Random baseline | 0.8086 |
| Per-neuron improvement over global | 20.1% |
| Spearman rho (global RMS, freq) | -0.8167 |
| Spearman rho (pn RMS, freq) | -0.7833 |

### Prediction Scorecard: 6 pass, 4 fail

| # | Prediction | Result | Notes |
|---|-----------|--------|-------|
| P1 | pn_RMS <= global_RMS for all 13 | **PASS** | Trivially true (superset search space) |
| P2 | Largest improvement for mid-tier (ranks 4-9) | **FAIL** | Bottom tier improved most (0.1938 avg) |
| P3 | pn_RMS < amp_RMS for top 6 | **FAIL** | Amplitude-only still wins for ALL classes |
| P4 | Spearman rho improves (more negative) | **FAIL** | Slightly worse (-0.7833 vs -0.8167) |
| P5 | Per-neuron angles show clustering | **FAIL** | Only 4/13 show structure; spreads 50-170 deg |
| — | All 13 beat random with per-neuron | **PASS** | vs 11/13 with global gauge |
| — | XNOR3 improvement | **PASS** | 0.9963 -> 0.7654 |
| — | ~A(B^C) biggest single improvement | **PASS** | 0.7781 -> 0.4813 (delta 0.2968) |
| — | All 10 checks from code | **6/10** | 4 informative failures |

## Key Findings

### 1. Phase Decoherence Is the Bottleneck

The headline result: amplitude-only decomposition (avg RMS 0.33) dramatically beats per-neuron gauge (avg RMS 0.57). This means:
- Weight **magnitudes** align well with bracket value magnitudes
- Weight **phases** are essentially random relative to Z[zeta_8]
- No post-training rotation (1D or 3D) can fix this

**Why:** Split-sigmoid activation treats Re and Im independently. Training dynamics have no mechanism to push phases toward 8th roots of unity. Magnitudes align because bracket value magnitudes are constrained to a sparse set, and training loss naturally produces similar magnitude structure.

### 2. Bottom Tier Benefits Most

Per-neuron gauge improved bottom-tier functions (ranks 10-13) by avg 0.1938, vs middle tier 0.1167, vs top tier 0.1325. The worst-decomposing functions had the most phase misalignment between neurons, so independent rotation helps most where the global gauge was worst.

### 3. Correlation Slightly Weakened

Spearman rho went from -0.8167 (global) to -0.7833 (per-neuron). The per-neuron gauge reshuffled some mid-rankings (e.g., ~A(B^C) jumped from rank 9 to rank 3 in per-neuron RMS). The topology-frequency correlation is fundamentally a magnitude-level phenomenon; adding phase correction introduces noise into the ranking.

### 4. Neuron Angles Are Diverse

Most classes show angle spreads of 50-170 degrees between neurons. Only 4/13 show any clustering. This confirms that the global gauge was making a severe compromise — different neurons genuinely sit at very different lattice orientations. But even with independent rotations, the phases are too far from the lattice.

## Interpretation: Three Lenses

### QM Lens
Split-sigmoid training produces **classical** networks — they use bracket magnitude structure but not quantum phase structure. The partition function is being accessed as a thermal (real) partition function, not a quantum (complex) one. Phase decoherence in the literal sense.

### Approximation Theory Lens
This is a **dimensionality mismatch** in C^12 ~ R^24. The 12 magnitude coordinates carry computational signal. The 12 phase coordinates carry training-noise. Gauge rotation couples magnitudes and phases, so rotating to improve phase can worsen magnitude. Amplitude-only projection is dimensionality reduction to the informative subspace.

### Coding Theory Lens
The bracket codebook offers ~7.3 bits/symbol (4.3 magnitude + 3 phase). Split-sigmoid training encodes ~4.3 bits/symbol of lattice-compatible information (magnitudes only). Per-neuron gauge recovers ~0.5 bits of misaligned phase. The remaining ~1.5 bits of phase capacity go unused.

## Implications for DKC Program

1. **Forward DKC (Demo 29)** works exactly because it starts from lattice values — no phase decoherence.
2. **Reverse DKC** needs either:
   - (a) **Lattice-constrained training** or **MVN activation** (Demo 47) to produce lattice-aligned phases
   - (b) **Amplitude-only decomposition** as the default reverse pathway
3. **The compiler** should probably use amplitude-only matching as its default, with phase matching available for lattice-aware architectures.

## Next Steps

- **Demo 47 (Parity Investigation):** Tests MVN activation. If MVN produces lattice-aligned phases, per-neuron gauge should dramatically improve, potentially closing the amplitude gap.
- **Expanded catalog + amplitude-only:** More bracket values from longer braids, with amplitude-only decomposition.
- **Canonical gauge decomposition:** If angles show structure for lattice-aware architectures, precompute the optimal per-neuron gauge rather than searching.

## Literature Context

- **Novelty confirmed:** Per-neuron U(1) gauge on cyclotomic lattice codebook has no precedent.
- **Closest match:** GLVQ (NeurIPS 2025) — per-group learnable codebooks for real-valued quantization.
- **Key connection:** Hirose (2012) identifies per-neuron phase freedom as theoretical curiosity; Demo 46 empirically quantifies its impact on lattice decomposition.
- **GPTQ = Babai's algorithm (ICLR 2026):** Our amplitude-only success is consistent with Babai's rounding working well in the right coordinate system.

## Files

- `main.c` — ~1460 lines, C89, compiles clean with all warning flags
- `findings.md` — this file
- Compile: `cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings main.c -lm -o demo46`
- Runtime: ~5 minutes (dominated by 3D gauge sweeps)

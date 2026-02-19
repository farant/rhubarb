# Per-Neuron Gauge Correction

**Track:** D — DKC Extensions
**Status:** Proposed
**Dependencies:** Demo 45 (complex function zoo)
**Code path:** `knotapel/demo_46_per_neuron_gauge/`

## Motivation

Demo 45 revealed that amplitude-only RMS often beats gauge-corrected complex RMS for 3-input networks. For BUF: amp_RMS = 0.40 vs cx_RMS = 0.53. The single global U(1) rotation can't satisfy all 12 complex weights simultaneously because different hidden neurons may have learned solutions at different lattice orientations.

## Hypothesis

Replacing the single global gauge angle with per-neuron gauge correction (one angle per hidden neuron's weight group) will significantly improve decomposition RMS, especially for mid-ranking functions (ranks 4-9 in Demo 45's table). The improvement should be largest where the global gauge was clearly compromising — functions where amp_RMS << cx_RMS.

## Approach

- **Architecture:** Same CxNet3 as Demo 45 (3 complex hidden neurons, 12 complex weights total)
- **Gauge sweep:** 3D optimization over (theta_1, theta_2, theta_3). Each theta_k rotates neuron k's 4 weights (3 inputs + bias) independently.
- **Search strategy:** Coarse grid (e.g., 36x36x36 = ~47k combinations at 10-degree resolution) then refine around the best. Or gradient descent from multiple starting points.
- **Comparison:** Report global-gauge RMS (Demo 45 baseline), per-neuron-gauge RMS, and amplitude-only RMS for all 13 NPN classes.

## Predictions

- P1: Per-neuron gauge RMS < global gauge RMS for all 13 classes (trivially true — it's a superset of the search space)
- P2: The improvement is largest for mid-ranking functions (ranks 4-9), modest for top-3 and bottom-3
- P3: Per-neuron gauge RMS < amplitude-only RMS for at least the top 6 functions (the global gauge's failure to beat amplitude-only was the original motivation)
- P4: The Spearman rho between topology frequency and per-neuron RMS will be stronger (more negative) than -0.8167
- P5: Optimal per-neuron angles will show structure — e.g., two neurons at similar angles, one at a different angle — reflecting the network's internal symmetry decomposition

## Notes

This is a measurement improvement, not a new experiment. The underlying question is: how much of Demo 45's decomposition error comes from the gauge approximation vs genuine mismatch with the lattice? If per-neuron gauge closes most of the gap, the bracket representation is even better than Demo 45 suggested. If it doesn't help much, the error is fundamental.

Could be combined with Demo 47 as a "Part B" — run per-neuron gauge for all classes, then switch to MVN activation for the parity investigation.

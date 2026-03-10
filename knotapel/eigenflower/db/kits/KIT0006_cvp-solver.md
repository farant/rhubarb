---
id: KIT0006
title: "CVP solver (Babai nearest plane)"
status: planned
tier: 2
input: "Lattice basis matrix, target point vector"
output: "Closest lattice point"
depends_on: [KIT0007]
effort: medium
source: ["GPTQ-as-CVP paper (Chen et al., 2025)", "NVFP4 paper (NVIDIA, 2025)"]
migrated_from: PT0060
applicable_to: [G0005]
related: [KIT0003, KIT0004]
---

## Description

Weight quantization IS the Closest Vector Problem on a lattice defined by the input Hessian. NVFP4 shows only 16 distinct values per weight are needed. Together: compile each syllogism as a CVP instance where the lattice has only 16 points per dimension (very sparse).

The Hessian from calibration data tells which lattice configurations are "close" in behavior space. This turns syllogism-to-weight compilation from a vague aspiration into a well-defined mathematical optimization problem with known algorithms and provable error bounds (1/4 * trace(D)).

Babai's nearest plane algorithm (1986): apply Gram-Schmidt to the lattice basis, then round each coordinate in the orthogonalized basis. Simple, well-understood, O(n^2) per query.

## Concrete path for syllogism compilation

1. Parse syllogism from text
2. Define target behavior vector (what the model should output given these premises)
3. Compute Hessian from calibration data (only 256 samples needed)
4. Solve CVP: find nearest lattice point = quantized weight configuration
5. Apply GPTQ-style greedy error propagation for sequential compilation

Dynamic recompilation: when new syllogism arrives, compute its effect on the Hessian (rank-1 update via KIT0005), re-quantize affected weights, propagate error.

~250 LOC. Algorithm is well-known (Babai 1986). Requires Gram-Schmidt orthogonalization.

---
id: PT0060
title: "Lattice-aware syllogism compilation via CVP"
status: promoted_to_kit
promoted_to: KIT0006
source: ["GPTQ-as-CVP paper (Chen et al., 2025)", "NVFP4 paper (NVIDIA, 2025)"]
applicable_to: []
related: [PT0009, PT0018, PT0019]
---

The most novel combined idea from the Group C synthesis. GPTQ shows weight quantization IS the Closest Vector Problem on a lattice defined by the input Hessian. NVFP4 shows only 16 distinct values per weight are needed. Together: compile each syllogism as a CVP instance where the lattice has only 16 points per dimension (a very sparse lattice).

The Hessian from the training data tells you which lattice configurations are "close" in behavior space. This turns syllogism-to-weight compilation from a vague aspiration into a well-defined mathematical optimization problem with known algorithms (Babai's nearest plane) and provable error bounds (1/4 * trace(D)).

Concrete path:
1. Parse syllogism from text
2. Define target behavior vector (what the model should output given these premises)
3. Compute Hessian from calibration data (only 256 samples needed)
4. Solve CVP: find nearest lattice point (quantized weight configuration) that produces target behavior
5. Apply GPTQ-style greedy error propagation for sequential compilation

Dynamic recompilation: when new syllogism arrives, compute its effect on the Hessian (rank-1 update), re-quantize affected weights using Babai's nearest plane, propagate error to neighbors.

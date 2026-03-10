---
id: PT0018
title: "GPTQ-as-CVP for weight quantization on standard lattices"
status: untested
source: ["GPTQ-as-CVP paper (Chen et al., 2025)"]
applicable_to: []
related: [PT0019, PT0020, PT0021]
---

GPTQ, when executed back-to-front, is mathematically identical to Babai's nearest plane algorithm for the Closest Vector Problem (CVP) on a lattice defined by the Hessian matrix of the layer's inputs. The weight vector maps to a target vector, integer quantized weights map to lattice coordinates.

This means decades of lattice algorithms (CVP, basis reduction, etc.) can be imported directly into neural network quantization. The paper derives tight, closed-form error bounds: worst-case layer error bounded by 1/4 * trace(D) where D is diagonal of LDL decomposition.

Min-pivot ordering: at each step, choose the dimension with minimum diagonal entry in Cholesky/LDL. Consistently minimizes error compared to GPTQ default act-order.

For eigenflower: if compiling discrete logical structures to continuous weights, CVP gives the exact mathematical tool. The lattice of "integer weight configurations" is searched for the closest point to the target behavior. The Hessian of input data defines which configurations are "close" in output space. This makes syllogism-to-weight compilation a well-posed optimization problem with provable error bounds.

Only 256 calibration samples needed to compute the Hessian. O(c^3) complexity in column dimension, not model size. Layer-by-layer (embarrassingly parallel).

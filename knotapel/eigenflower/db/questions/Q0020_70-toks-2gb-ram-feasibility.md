---
id: Q0020
title: "Can the target of 70 tok/s at 1-2GB RAM be achieved on M2 with a 50-100M param ternary model?"
status: open
source: "CLAUDE.md"
related: []
---

The performance targets are specific: 70 tok/s output, maximum 1-2GB RAM, running on M2 MacBook while doing other things. A 50-100M parameter model with ternary weights (BitNet b1.58) stores ~2 bits per weight, so 50M params = ~12.5MB, 100M params = ~25MB for weights alone. This is well within the 1-2GB RAM budget. The question is whether inference speed hits 70 tok/s given the additional memory needed for activations, KV cache, and the syllogism layer's graph structure. BitNet's advantage is integer-only inference (no floating point), which is fast on M2's integer units. A back-of-envelope calculation or small prototype could validate or refute the feasibility of these combined targets.

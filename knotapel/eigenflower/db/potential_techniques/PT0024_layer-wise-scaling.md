---
id: PT0024
title: "Layer-wise scaling (wider deeper layers)"
status: untested
source: ["MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0022]
---

Instead of constant width across all transformer blocks, linearly increase width (hidden dimension) from early to later layers. Deeper layers learn more complex features and benefit from more parameters. Outperforms constant-width at equal compute.

For eigenflower: allocate more parameters to deeper layers where complex reasoning representations form, fewer to early layers handling surface-level pattern matching. This is an efficient parameter allocation strategy for fitting models on M2.

Combines naturally with the monotone width hierarchy from H-Net (D^0 <= D^1 <= ... <= D^S), where inner/deeper stages get wider models since they process more semantic representations.

Implementation: straightforward -- just use different hidden dimensions for different transformer blocks. No architectural novelty beyond dimension specification.

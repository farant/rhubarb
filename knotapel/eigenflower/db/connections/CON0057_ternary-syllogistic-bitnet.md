---
id: CON0057
title: "BitNet ternary {-1, 0, +1} maps to syllogistic affirm/deny/ignore"
connects:
  - "BitNet b1.58 ternary weight quantization"
  - "Eigenflower syllogism compilation: propositions affirmed, denied, or irrelevant"
type: structural_parallel
strength: moderate
source: "notes/01-analysis.md, Part 1 (Group A backbone)"
---

BitNet ternary weights {-1, 0, +1} map almost too perfectly onto syllogistic logic: +1 = affirm the proposition, -1 = deny the proposition, 0 = proposition is irrelevant to this inference step.

Combined with TRM's recursive refinement (7M params beating billion-parameter models on reasoning), this gives a concrete architecture: a tiny ternary model that reasons by iteratively applying the same compiled syllogistic transformation.

The "Ternary Recursive Orchid" proposal (2-5M params, under 1MB) fits the eigenflower constraints: training in 30-60 minutes on M2, running at 70+ tok/s with 1-2GB RAM.

DKC's Z/4Z axis-alignment enriches this: rather than ternary on 1 axis, weights can be ternary on 4 cyclotomic axes. This gives 81 possible weight values while remaining purely integer arithmetic -- a richer "syllogistic vocabulary" (4 modes of affirmation, 4 of denial, 1 irrelevant) that still compiles to addition/subtraction.

---
id: UNK0029
title: "Why the optimal Z[zeta_8] catalog is 21 entries (not 24) and what this means for weight design"
status: open
source: "notes/01-analysis.md"
related: []
---

The analysis mentions "the optimal z8 catalog of 21 (not 24) entries" as a structural observation parallel to the 3.125 bits natural information density result — both demonstrate that "the natural resolution is smaller than naively expected, and that removing certain elements actively improves performance." But why exactly 21? Which 3 elements are removed and why? Does this have implications for eigenflower's weight design — if the natural resolution of Z[zeta_8] for computation is 21, not 24, then weight quantization should respect this. The specific elements that are removed may carry algebraic significance (e.g., they might be the elements that introduce floating-point-like precision issues in exact arithmetic).

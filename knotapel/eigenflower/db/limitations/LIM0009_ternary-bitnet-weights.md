---
id: LIM0009
title: "Ternary/BitNet weights {-1, 0, +1}"
type: chosen
source: "notes/01-analysis.md, notes/02-architecture-insight.md"
impacts: []
related: [LIM0001, LIM0002, LIM0003, LIM0010]
---

The fluency ANN uses ternary weights from BitNet b1.58: each weight is one of {-1, 0, +1}, meaning affirm, deny, or ignore.

Why chosen:
- Maps almost perfectly to syllogistic logic. A weight of +1 affirms a connection, -1 denies it, 0 ignores it. This is the natural vocabulary of logical propositions.
- Inference becomes integer-only arithmetic — no floating point multiply-accumulate. Each "multiply" is just a sign flip or zero. This is dramatically faster on CPU.
- Storage is ~1.58 bits per parameter. A 100M parameter model fits in ~20MB. This easily satisfies the 1-2GB RAM constraint (LIM0002).
- Training is faster because gradient computation with ternary weights is simpler.

Implications:
- Need to implement ternary-specific training (straight-through estimator or similar for gradient through the quantization).
- The GPTQ-as-CVP framework means "compile syllogism to weights" becomes a Closest Vector Problem with known algorithms and provable error bounds.
- Ternary weights may limit the model's representational capacity for subtle linguistic distinctions. This is acceptable because reasoning is offloaded to the syllogism layer — the fluency layer only needs to parse and generate, not reason.
- Connection to DKC: axis-aligned entries in Z[zeta_8] decompose as integer magnitude + 2-bit phase, structurally isomorphic to ternary on 4 axes.

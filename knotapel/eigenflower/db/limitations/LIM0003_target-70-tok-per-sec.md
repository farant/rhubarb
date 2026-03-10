---
id: LIM0003
title: "Target ~70 tokens/second output speed"
type: chosen
source: "CLAUDE.md, performance constraint"
impacts: []
related: [LIM0001, LIM0002, LIM0010]
---

The model must generate output at approximately 70 tokens per second on the M2 MacBook. This is a usability constraint — output needs to feel fast and responsive, not sluggish.

Implications:
- With ternary weights, inference is integer-only arithmetic (no floating point multiply-accumulate). This is much faster on CPU than standard FP16/FP32 inference.
- At 100M params with ternary weights, each forward pass involves ~100M integer additions (no multiplications needed since weights are {-1, 0, +1}). On M2 this should be well within 70 tok/s.
- The syllogism layer queries (graph traversals) add latency per token if reasoning is invoked during generation. Need to consider whether reasoning is done in a batch before generation starts, or interleaved.
- Recursive architectures (TRM-style multiple passes) trade latency for quality — more passes = better reasoning but fewer tok/s. Need to find the right number of recursion steps.
- KV cache management affects throughput — must be efficient enough to not bottleneck at 70 tok/s.

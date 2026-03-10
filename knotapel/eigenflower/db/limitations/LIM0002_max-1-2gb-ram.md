---
id: LIM0002
title: "Maximum 1-2GB RAM at runtime"
type: chosen
source: "CLAUDE.md, hardware constraint"
impacts: []
related: [LIM0001, LIM0003, LIM0010]
---

The running model must fit within 1-2GB of RAM so it can run performantly on a MacBook while the user is doing other things. This is not a training constraint but a deployment constraint — the model needs to coexist with the rest of a working development environment.

Implications:
- With ternary weights (1.58 bits per param), 100M parameters = ~20MB for weights alone. This leaves substantial headroom for the syllogism graph, KV cache, and working memory.
- The syllogism graph (compiled tensor equations, proposition nodes, edges) needs to fit in the remaining budget. The Summa has ~3,000 articles — even with generous per-article overhead, this should be manageable.
- Rules out storing large embedding tables or dense attention caches.
- Favors recursive architectures (TRM-style) where the same small set of weights is applied repeatedly, keeping the memory footprint constant regardless of reasoning depth.
- The arena allocator (piscina) from the rhubarb libraries gives precise control over memory usage.

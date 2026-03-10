---
id: UNK0019
title: "How 'decompiling' existing open source models to ANN will work in practice"
status: open
source: "CLAUDE.md"
related: []
---

The CLAUDE.md states "we might try to 'decompile' existing open source models to ANN to understand how they work." The transparency document reframes this as "reverse engineering the mechanism so you can forward-engineer it with clarity," using GPTQ-as-CVP on cyclotomic lattices. But the practical path is vague. Which models to decompile? (GPT-2, LLaMA, something smaller?) Which layers to focus on? (Attention heads, MLP layers, embeddings?) What does "interpretable structure" look like when you decompose weights onto Z[zeta_8]? Nobody has done cyclotomic decomposition of trained LLM weights before, so there is no prior art to guide expectations. This could be a goldmine or a dead end, and we won't know until someone tries.

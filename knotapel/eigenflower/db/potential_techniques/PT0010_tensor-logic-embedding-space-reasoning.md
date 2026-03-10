---
id: PT0010
title: "Tensor logic embedding space reasoning with temperature control"
status: untested
source: ["Tensor Logic paper (Domingos, 2025)"]
applicable_to: []
related: [PT0009, PT0011]
---

Embed facts and relations into dense vectors using random unit vectors. A relation R(x,y) embeds as EmbR[i,j] = R(x,y) * Emb[x,i] * Emb[y,j]. Rules embed as products of antecedent embeddings. Reasoning in this space gives approximately correct results, with error decreasing as embedding dimension increases. This is like a Bloom filter for logic.

Temperature-controlled reasoning: applying sigmoid with temperature T to each equation controls the reasoning mode. T->0 makes reasoning purely deductive (Gram matrix becomes identity). T->infinity makes reasoning analogical (similar objects borrow inferences). Different T values can apply to different rules -- mathematical truths use T=0, weak-evidence rules use high T.

This provides a spectrum from exact deductive reasoning to approximate analogical reasoning, controlled by a single parameter per rule. Inferred tensors can be extracted at any point during inference -- full transparency (unlike LLMs).

For eigenflower: could embed the Summa's propositional structure in dense vectors, then reason over it with temperature control. Deductive conclusions (demonstrations) use T=0, analogical reasoning uses higher T, arguments from authority use intermediate T weighted by source reliability.

Scaling via Tucker decomposition converts sparse tensors to dense for efficiency. Even random decomposition works with small controllable error.

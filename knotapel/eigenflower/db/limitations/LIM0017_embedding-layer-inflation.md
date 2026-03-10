---
id: LIM0017
title: "Model size estimates don't account for vocabulary — embedding layer inflation"
type: discovered
source: "notes/01-analysis.md"
impacts: []
related: [LIM0010, LIM0014, LIM0004]
---

The TRM paper achieves impressive results at 7M parameters, but it operates on grid puzzles with tiny vocabularies. Latin theological text has a much larger vocabulary, which inflates the embedding layer. A 7M-parameter model might not have enough capacity for natural language.

From 01-analysis.md: "The model size estimates don't fully account for vocabulary. TRM works at 7M for grid puzzles with tiny vocabularies. Latin theological text has a much larger vocabulary, which inflates the embedding layer."

The embedding layer size is roughly: vocabulary_size * embedding_dimension. Even with a modest vocabulary of 30,000 tokens and a 256-dimensional embedding, that's ~7.7M parameters — already more than TRM's entire model, just for the embedding table.

Implications:
- The 50-100M parameter budget (LIM0010) must allocate a significant fraction to embeddings. With a 30K vocabulary and 512-dim embeddings, the embedding layer alone is ~15M params, leaving 35-85M for the transformer layers.
- H-Net's tokenizer-free (byte-level) approach could help by eliminating the embedding table entirely, but at the cost of longer sequences (every character is a token).
- The hybrid architecture helps: the fluency layer's vocabulary can be limited to theological Latin + English from the corpus (LIM0004), which is smaller than a general-purpose vocabulary.
- Vocabulary design (BPE, character-level, byte-level, or morphological) is a critical early decision that affects the entire parameter budget.

---
id: LIM0021
title: "Multilingual corpus adds complexity (Latin, English, Greek)"
type: discovered
source: "notes/02-architecture-insight.md"
impacts: []
related: [LIM0004, LIM0010, LIM0017]
---

The corpus includes texts in three languages: Latin (Vulgate, Summa), English (Douay-Rheims, Summa translation), and Greek (Septuagint). How the system handles multilingual input is an open design question.

From 02-architecture-insight.md: "How does the system handle the multilingual corpus? Latin Summa + English Summa + Latin Bible + English Bible + Greek Septuagint. Does the fluency layer need to be multilingual, or do we train separate fluency models per language?"

Options:
1. Single multilingual fluency model — handles all three languages, with implicit alignment from parallel texts (same content in Latin and English). More efficient in parameter use but harder to train well.
2. Separate fluency models per language — simpler per-model but triples the training/deployment cost and loses cross-lingual signal.
3. Shared architecture with language-specific adapters — compromise approach, shared core with thin language-specific layers.

Implications:
- Latin is heavily inflected (word order is flexible, meaning carried by endings). English is positional (meaning carried by word order). Greek is somewhere between. These are fundamentally different parsing challenges.
- The Free Transformer's latent Z variable could encode language as one of its modes (Latin vs English vs Greek).
- Parallel texts (same verse in Latin and English) provide natural alignment signal that smaller models can leverage — each pair is a free training example for cross-lingual understanding.
- The syllogism layer is language-independent (logical structure is universal). Only the fluency layer and interface need to handle multilingual complexity.
- Vocabulary size increases with each language, compounding the embedding layer inflation problem (LIM0017).

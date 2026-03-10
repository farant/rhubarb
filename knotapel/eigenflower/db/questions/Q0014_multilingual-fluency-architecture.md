---
id: Q0014
title: "Should the fluency layer be multilingual or should there be separate models per language?"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The corpus includes Latin Summa + English Summa + Latin Vulgate + English Bible + Greek Septuagint. A single multilingual fluency model shares parameters across languages but needs more capacity. Separate per-language models are smaller but require maintaining multiple models and don't benefit from cross-lingual transfer. For theological Latin specifically, cross-lingual transfer from English might help (shared theological concepts) or hurt (very different grammar). The Free Transformer's latent Z could encode language as one of its mode variables, which would favor the single-model approach. The question is testable: train a small model on Latin-only vs English-only vs combined and compare parsing quality on theological text.

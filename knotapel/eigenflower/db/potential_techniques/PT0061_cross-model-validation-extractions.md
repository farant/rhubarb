---
id: PT0061
title: "Cross-model validation for syllogism extraction"
status: promoted_to_kit
promoted_to: KIT0015
source: ["Inverse Knowledge Search paper (Li et al., 2026)"]
applicable_to: []
related: [PT0032, PT0033]
---

Use multiple distinct LLMs from different providers to validate syllogism extractions from the Summa. If models disagree on the logical structure, the extraction is discarded. Only keep extractions where multiple models agree.

The Inverse Knowledge Search paper reports ~70% success rate for undergraduate-level derivations, dropping to ~50% for graduate-level. For eigenflower: syllogistic structure extraction from the Summa is a specialized task -- success rate would depend on the quality of structural markers and prompt design.

A distinct LLM screens each extraction for flawed assumptions before validation (filters ~5% of auto-generated questions).

For eigenflower: use Claude + Gemini + local model (gemini CLI available) for cross-validation. The prediction vs rationalization discipline from knotapel applies: ask models to predict the logical structure BEFORE showing them the text, not after.

This is a quality assurance technique for the corpus preparation phase, not a training technique.

---
id: KIT0015
title: "Cross-model syllogism validation"
status: planned
tier: 4
input: "Extracted syllogisms, list of model endpoints"
output: "Validated syllogisms (agreement across models)"
depends_on: [KIT0014]
effort: small
source: ["Inverse Knowledge Search paper (Li et al., 2026)"]
migrated_from: PT0061
applicable_to: [G0005]
related: [KIT0014, KIT0012]
---

## Description

Quality assurance layer for syllogism extraction. Use multiple distinct LLMs from different providers to validate extractions from the Summa. If models disagree on the logical structure, the extraction is discarded. Only keep extractions where multiple models agree.

The Inverse Knowledge Search paper reports ~70% success rate for undergraduate-level derivations, dropping to ~50% for graduate-level. Success rate for Summa extraction depends on structural marker quality and prompt design.

A distinct LLM screens each extraction for flawed assumptions before validation (filters ~5% of auto-generated questions).

## Implementation

Use Claude + Gemini + local model (gemini CLI available). The prediction vs rationalization discipline from knotapel applies: ask models to predict the logical structure BEFORE showing them the text, not after.

This is orchestration code -- prompt construction, API calls, agreement checking. ~200 LOC + prompts.

## API Sketch

```c
/* This is likely a script/tool rather than a C89 library,
   since it calls external APIs. Bun/TypeScript is appropriate. */
```

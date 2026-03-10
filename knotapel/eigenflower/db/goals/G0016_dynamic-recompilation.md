---
id: G0016
title: "Dynamically recompile weights for new syllogisms"
status: open
priority: high
depends_on: [G0005, G0015]
blocks: []
related: []
---

## Description

Enable the system to record new syllogisms at runtime by dynamically recompiling weights on the fly. When a new syllogism is provided (either extracted from text or manually specified):

1. Parse it with the fluency layer (text -> logical structure)
2. Compile it into the syllogism layer (new tensor equation / graph node + edges)
3. Recompute deductive closure (forward chaining picks up new consequences)

The fluency layer does NOT need retraining for this. The two layers update independently. The syllogism layer grows by accretion — each new syllogism adds structure.

## Why It Matters

This is a massive architectural advantage over monolithic LLMs where "learning something new" requires fine-tuning the entire model. In eigenflower, adding new knowledge to the reasoning layer is a compilation step, not a training step. It takes seconds, not hours. It is exact, not approximate. And it is incremental — the existing knowledge is untouched.

The fluency layer only needs retraining if the VOCABULARY changes (new terms it has not seen before), not when new logical relationships are added. This separation of concerns — fluency trained once, reasoning compiled incrementally — is the core architectural insight.

Dynamic recompilation also means the system can grow over time: as more of the Summa is parsed and compiled, or as new theological texts are ingested, the reasoning graph becomes richer without any retraining cost.

## What "Achieved" Looks Like

- New syllogism can be added at runtime without restarting the system
- The deductive closure updates correctly (new consequences are derivable)
- Existing derivations are not broken by the addition
- The process completes in seconds, not minutes
- Works for both manually specified syllogisms and automatically extracted ones
- The system can process a batch of new syllogisms efficiently

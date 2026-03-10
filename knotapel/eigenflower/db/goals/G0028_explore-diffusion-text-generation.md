---
id: G0028
title: "Explore diffusion-based text generation"
status: open
priority: low
depends_on: []
blocks: []
related: []
---

## Description

Explore diffusion models as an alternative or complement to autoregressive text generation for the fluency layer. The CLAUDE.md lists "explore diffusion text generation" as a research direction, and research/mercury-2.md contains notes on diffusion-based reasoning LLMs.

Diffusion text generation works by iteratively denoising a corrupted text sequence — starting from random noise and progressively refining toward coherent text. This is fundamentally different from autoregressive generation (one token at a time, left to right) and may offer advantages for:

- Parallel generation (all tokens refined simultaneously, potentially faster)
- Global coherence (the entire output is refined together, not committed token by token)
- Natural integration with the syllogism layer (the logical structure could constrain the denoising process globally rather than token by token)

## Why It Matters

This is an exploratory goal — it may lead nowhere, or it may reveal a better architecture for the fluency layer than autoregressive generation. The key question is whether diffusion text generation can meet the performance targets (70 tok/s, 1-2GB RAM) while producing coherent theological Latin/English.

The potential connection to the syllogism layer is intriguing: if the logical structure provides a "skeleton" that the diffusion process fills in with natural language, the interface problem (G0012, G0013) might become easier — the syllogism layer constrains the generation globally rather than through a sequential query-narration protocol.

## What "Achieved" Looks Like

- mercury-2.md notes reviewed and assessed for applicability
- At least one diffusion text generation experiment implemented
- Quality compared against autoregressive generation on the same corpus
- Performance measured against the 70 tok/s and 1-2GB targets
- Assessment: is diffusion text generation viable for eigenflower, and if so, for which component?
- Decision recorded (DEC record) with rationale for adopting, deferring, or rejecting

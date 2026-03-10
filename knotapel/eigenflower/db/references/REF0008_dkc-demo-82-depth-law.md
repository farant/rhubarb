---
id: REF0008
title: "DKC Demo 82 — Linear Depth Law"
type: dkc_demo
source_project: knotapel
demo_number: 82
summary: "max_xor approximately equals depth + 6, where depth is the number of braid generator multiplications. Deep entries (from recursive group-closure) are 2x more efficient than shallow entries with more vocabulary diversity. Algebraic coherence beats raw vocabulary."
related: []
---

## Description

Demo 82 demonstrated the linear depth law: computational capacity (measured as maximum XOR arity) scales linearly with braid crossing depth. Depth-8 entries with 476 directions reach XOR12; strided entries with 512 directions (maximum vocabulary diversity) reach only XOR10. The mechanism: deep entries share algebraic structure from common BFS ancestry -- they are "semantically chunked."

Two-phase structure:
- Stage 1 (depth 0-6): Direction explosion -- new S2 directions dominate catalog growth
- Stage 2 (depth 7+): Angle refinement -- directions saturate, angular precision dominates

## Relevance to Eigenflower

- The "algebraic coherence beats vocabulary" result is the algebraic formalization of TRM's "less is more" principle: recursive application of the same small transformation beats wider/bigger single applications
- The two-phase structure (direction explosion then angle refinement) maps to Paper 2's (Early Experience) two-stage training: world-model pre-training then supervised fine-tuning
- The linear slope of 1 (each recursion adds 1 XOR level) provides quantitative predictions for recursive model depth
- BFS depth = compression stage, directly parallel to H-Net's hierarchical compression

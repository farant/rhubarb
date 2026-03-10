---
id: UNK0027
title: "How to implement backpropagation and AdamW optimizer in C89 without dependencies"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

Phase 2 of the build order requires "training infrastructure in C89: matrix multiply (ternary for BitNet), backpropagation, optimizer (AdamW), data loading." The rhubarb project's philosophy is no dependencies — write everything in C89. Implementing backpropagation and AdamW from scratch is doable but nontrivial, especially with strict compiler flags and Latin identifiers. Key unknowns: numerical stability with ternary quantization during training (not just inference), memory layout for efficient matrix operations on M2, whether Metal/Accelerate can be used through a platform abstraction layer for GPU-accelerated training, and whether the 30-60 minute training constraint is achievable without hardware acceleration.

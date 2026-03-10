---
id: G0002
title: "Run at 70 tok/s within 1-2GB RAM"
status: open
priority: high
depends_on: [G0010]
blocks: []
related: []
---

## Description

The deployed eigenflower system must generate output at approximately 70 tokens per second while consuming no more than 1-2GB of RAM. The system must run performantly on a MacBook while the user is doing other work — it cannot monopolize system resources.

## Why It Matters

This goal ensures eigenflower is a practical tool, not a demonstration. If it requires dedicated hardware or consumes all available memory, it becomes a project you run in isolation rather than a system that augments daily work. The 70 tok/s target means responses feel real-time. The 1-2GB cap means it coexists with a browser, editor, and other tools.

BitNet ternary weights are critical here: integer-only inference eliminates floating-point overhead, and {-1, 0, +1} weights mean the model fits in roughly 2 bits per parameter. A 100M-param model at 2 bits/param is ~25MB for weights alone — well within the RAM budget even with activation buffers and the syllogism graph.

## What "Achieved" Looks Like

- Sustained output generation at 70+ tok/s on M2 MacBook
- Total memory footprint (model + syllogism graph + buffers) stays under 2GB
- System remains responsive while running alongside normal workloads
- No perceptible latency between query and first token

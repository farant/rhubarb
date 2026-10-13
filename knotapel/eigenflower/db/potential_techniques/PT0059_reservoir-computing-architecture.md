---
id: PT0059
title: "Reservoir computing architecture (fixed reservoir + trained readout)"
status: untested
source: ["Intelligence at the Edge of Chaos paper (Zhang et al., ICLR 2025)", "Knotapel crossover analysis"]
applicable_to: []
related: [PT0015, PT0017]
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Use a fixed, pretrained network as a "reservoir" that projects inputs into high-dimensional space, then train only a readout layer on top. The ECA-pretrained transformers already exhibit this property: internal representations are fixed, only I/O layers adapt.

DKC from knotapel already implements this pattern: fixed catalog (reservoir) + trained readout (activation function). The five-pillar synthesis (TL algebra + Hilbert space operators + edge of chaos + tensor logic + reservoir computing) provides theoretical justification.

For eigenflower: pretrain the core model on complex data (Bible/Summa or even ECA rules), freeze the internal layers, then train only input/output projections for specific tasks. This dramatically reduces the number of trainable parameters and speeds up training.

Dynamic recompilation could focus on the readout layer only, making syllogism compilation a lightweight operation on top of a fixed computational substrate.

The knotapel crossover identifies this as "DEEPEST" connection: DKC provides the theoretical framework for why a ternary recursive model should work as a reservoir, via the depth law and parity-lock theorem.

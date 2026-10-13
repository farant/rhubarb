---
id: PT0059
title: "Reservoir computing architecture (fixed reservoir + trained readout)"
status: untested
source: ["Intelligence at the Edge of Chaos paper (Zhang et al., ICLR 2025)", "Knotapel crossover analysis"]
applicable_to: []
related: [PT0015, PT0017]
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Use a fixed, pretrained network as a "reservoir" that projects inputs into high-dimensional space, then train only a readout layer on top. The ECA-pretrained transformers already exhibit this property: internal representations are fixed, only I/O layers adapt.

DKC from knotapel already implements this pattern: fixed catalog (reservoir) + trained readout (activation function). The five-pillar synthesis (TL algebra + Hilbert space operators + edge of chaos + tensor logic + reservoir computing) provides theoretical justification.

For eigenflower: pretrain the core model on complex data (Bible/Summa or even ECA rules), freeze the internal layers, then train only input/output projections for specific tasks. This dramatically reduces the number of trainable parameters and speeds up training.

Dynamic recompilation could focus on the readout layer only, making syllogism compilation a lightweight operation on top of a fixed computational substrate.

The knotapel crossover identifies this as "DEEPEST" connection: DKC provides the theoretical framework for why a ternary recursive model should work as a reservoir, via the depth law and parity-lock theorem.

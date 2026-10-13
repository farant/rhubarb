---
id: CON0049
title: "Paper 2 two-stage training maps to DKC depth law's direction explosion then angle refinement"
connects:
  - "Early Experience two-stage pipeline: world-model pre-training then supervised fine-tuning"
  - "DKC depth law two-phase structure: direction explosion (depth 0-6) then angle refinement (depth 7+) (D82)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 10"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The depth law reveals a natural two-stage structure:

Stage 1 (world model / depth 0-6): Direction explosion. New S^2 directions dominate catalog growth. The "world model" -- quaternion representation space geometry -- is being explored. Corresponds to Paper 2's IWM phase: learning transition dynamics.

Stage 2 (expert data / depth 7+): Angle refinement. Directions saturate at MAX_DIR=512 and angle refinement dominates. The "expert data" -- fine-grained angular relationships enabling parity -- is accumulated. Corresponds to SFT phase: fine-tuning on task structure.

D82 demonstrated this is real: shallow entries (depths 0-5) reach XOR10, but deep entries (all depth 8) reach XOR12 with strictly less vocabulary diversity. Deep entries' advantage is algebraic coherence -- structured exploration data sharing common context.

D90's spectral inversion: S^2 point cloud bandwidth DECREASES with depth while computational power INCREASES. Expert data quality > data diversity. D91's balanced exponentials (vocabulary ~2x/round, parity demand ~4x/weight) quantifies when Stage 1 saturates and Stage 2 begins.

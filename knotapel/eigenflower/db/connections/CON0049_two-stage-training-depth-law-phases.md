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

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The depth law reveals a natural two-stage structure:

Stage 1 (world model / depth 0-6): Direction explosion. New S^2 directions dominate catalog growth. The "world model" -- quaternion representation space geometry -- is being explored. Corresponds to Paper 2's IWM phase: learning transition dynamics.

Stage 2 (expert data / depth 7+): Angle refinement. Directions saturate at MAX_DIR=512 and angle refinement dominates. The "expert data" -- fine-grained angular relationships enabling parity -- is accumulated. Corresponds to SFT phase: fine-tuning on task structure.

D82 demonstrated this is real: shallow entries (depths 0-5) reach XOR10, but deep entries (all depth 8) reach XOR12 with strictly less vocabulary diversity. Deep entries' advantage is algebraic coherence -- structured exploration data sharing common context.

D90's spectral inversion: S^2 point cloud bandwidth DECREASES with depth while computational power INCREASES. Expert data quality > data diversity. D91's balanced exponentials (vocabulary ~2x/round, parity demand ~4x/weight) quantifies when Stage 1 saturates and Stage 2 begins.

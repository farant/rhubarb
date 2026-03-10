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

The depth law reveals a natural two-stage structure:

Stage 1 (world model / depth 0-6): Direction explosion. New S^2 directions dominate catalog growth. The "world model" -- quaternion representation space geometry -- is being explored. Corresponds to Paper 2's IWM phase: learning transition dynamics.

Stage 2 (expert data / depth 7+): Angle refinement. Directions saturate at MAX_DIR=512 and angle refinement dominates. The "expert data" -- fine-grained angular relationships enabling parity -- is accumulated. Corresponds to SFT phase: fine-tuning on task structure.

D82 demonstrated this is real: shallow entries (depths 0-5) reach XOR10, but deep entries (all depth 8) reach XOR12 with strictly less vocabulary diversity. Deep entries' advantage is algebraic coherence -- structured exploration data sharing common context.

D90's spectral inversion: S^2 point cloud bandwidth DECREASES with depth while computational power INCREASES. Expert data quality > data diversity. D91's balanced exponentials (vocabulary ~2x/round, parity demand ~4x/weight) quantifies when Stage 1 saturates and Stage 2 begins.

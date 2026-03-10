---
id: PT0070
title: "Heavy data augmentation for small-sample learning"
status: untested
source: ["TRM paper (Jolicoeur-Martineau, 2025)"]
applicable_to: []
related: [PT0003, PT0026]
---

Apply aggressive data augmentation for small-sample learning. TRM uses: Sudoku 1000 shuffles per example, Maze 8 dihedral transformations, ARC-AGI 1000 augmentations (color permutation, dihedral group, translations). Augmentation is critical for small-sample learning.

For eigenflower text: augmentation strategies could include:
- Paraphrase (same logical structure, different surface text)
- Term substitution (replace terms while preserving syllogistic form)
- Language switching (Latin <-> English for same content)
- Premise order permutation (same syllogism, different premise ordering)
- Equivalent reformulation (active -> passive, etc.)

The contrastive invalid syllogism generation (PT0026) is itself a form of augmentation -- expanding K=3-4x per valid example.

Combined with early experience (PT0027), augmentation amplifies the small Bible+Summa corpus significantly without adding new content, only new presentations of existing logical structure.

Caveat: TRM found MoE hurts with augmented data (too much capacity for the effective data size). Keep model small.

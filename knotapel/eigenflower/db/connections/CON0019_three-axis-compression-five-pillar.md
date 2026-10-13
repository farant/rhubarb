---
id: CON0019
title: "Group B three-axis compression (spatial/representational/structural) = DKC five-pillar synthesis"
connects:
  - "Group B combined 24x compression: H-Net spatial + CALM representational + Free Transformer structural"
  - "DKC five-pillar reservoir computing synthesis (D94, D73, D82, D83, D84)"
type: mathematical_identity
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 5"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

DKC IS a concrete, exactly-analyzable instance of three-axis compression:

1. Spatial compression (H-Net axis): BFS closure compresses infinite braid group into finite catalog. At zeta_8, 87,890 enumerated braids produce only 24 quaternionic states -- 3,662x compression (D66). Depth law (D82) shows capacity preserved linearly.

2. Representational compression (CALM axis): Each bracket value compresses a variable-length braid word into a single Z[zeta_8] integer (4 coefficients). Z/4Z axis-alignment (D107) further compresses to integer + 2-bit phase code.

3. Structural compression (Free Transformer axis): The encoding (D92) determines global structure with minimal overhead. One binary choice (encoding = +/-q or 1wpi) determines the entire accessible function space -- 1 bit of latent information controls which Boolean functions are reachable.

The reservoir computing mapping (D94) makes this precise, with each component identified:
- Fixed reservoir = Z[zeta_8] catalog (H-Net encoder)
- Linear readout = signed weight sum (CALM projection)
- Nonlinear output = k-sector activation (CALM energy head)
- Memory depth = BFS crossing depth (H-Net nesting)
- Channel independence = writhe+depth+lattice additive (Free Transformer Z)

This is not analogy -- DKC satisfies the formal definition of an Echo State Network (Jaeger 2001).

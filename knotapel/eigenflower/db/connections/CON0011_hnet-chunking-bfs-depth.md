---
id: CON0011
title: "H-Net dynamic chunking structurally mirrors DKC BFS depth rounds as chunk boundaries"
connects:
  - "H-Net learned content-aware dynamic chunking via cosine similarity routing"
  - "DKC BFS depth rounds as natural compression boundaries (D82, D90)"
type: structural_parallel
strength: moderate
source: "extractions/group-b-knotapel-relevance.md, Connection 1"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

H-Net replaces fixed tokenizers with learned boundaries that emerge where adjacent encoder outputs disagree (low cosine similarity). DKC's BFS closure produces natural "boundaries" at each depth round: the value set grows exponentially (~2x per round) but computational power grows linearly.

The depth round IS the natural chunk boundary. Entries born at the same BFS depth share intermediate algebraic products (D90: "algebraic coherence beats raw vocabulary"), exactly as H-Net's chunks share encoder hidden states.

DKC's pipeline is a concrete hierarchical compression instance: Braid word -> TL algebra -> bracket value -> Boolean function, with the numerical pivot at D10 being a literal 50x compression (Poly 808 bytes/entry replaced by Cx 16 bytes/entry).

The ratio loss in H-Net (preventing trivial compression solutions) has a DKC analog: the finite group ceiling (D79-D80). At zeta_8, the binary octahedral group's 24 elements cap the catalog. Moving to zeta_12 (infinite group) removes this ceiling -- analogous to relaxing H-Net's compression ratio constraint.

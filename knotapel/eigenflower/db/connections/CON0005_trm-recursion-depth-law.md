---
id: CON0005
title: "TRM recursive refinement and DKC linear depth law are the same mechanism"
connects:
  - "TRM recursive 2-layer network (n=6 times, 42 effective layers)"
  - "DKC linear depth law: max_xor ~ depth + 6 (D82, D89-D91)"
type: mathematical_identity
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 5"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

TRM's core insight -- recursively applying the same transformation is more powerful than applying a bigger transformation once -- is exactly what the DKC depth law says. In DKC, "depth" is the number of times braid generators are multiplied together. The linear depth law says each additional recursive application adds +1 to computational capacity (max_xor ~ depth + 6).

TRM's "42 effective layers from 2 layers applied 21 times" is the same architecture principle as DKC's "depth-8 entries from generators multiplied 8 times."

The "algebraic coherence beats raw vocabulary" result from D82 is the precise algebraic analog of TRM's "2 layers + deep recursion beats 4 layers + shallow recursion." In both cases, recursive structure creates shared intermediate representations (common BFS ancestors in DKC, latent z state in TRM) that raw width cannot replicate.

TRM's two features (y = current answer, z = reasoning trace) map onto the Dual-Channel Theorem (D108-D109): parity requires BOTH phase coherence (the z-like "how we got here" channel) AND magnitude diversity (the y-like "current state" channel). Both channels are independently necessary.

The balanced exponentials mechanism (D91: supply ~2x/depth, demand 4x/weight, giving slope ~1) could inform TRM's learning rate scheduling.

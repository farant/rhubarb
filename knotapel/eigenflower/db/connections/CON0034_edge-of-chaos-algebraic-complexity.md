---
id: CON0034
title: "DKC at the edge of chaos: ALGEBRAIC complexity predicts computation where DYNAMICAL complexity is orthogonal"
connects:
  - "Zhang et al.: GPT-2 at intermediate ECA complexity (Wolfram Class IV) develops best representations"
  - "DKC finite/infinite group transition as edge-of-chaos + D106 falsification of dynamical measures"
type: cross_pollination
strength: strong
source: "extractions/group-d-knotapel-relevance.md, Connection 3"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The five-pillars synthesis already identifies this as STRUCTURAL IDENTITY:
- zeta_8 = ordered regime: finite group (24 elements), capacity ceiling (XOR8)
- zeta_12 = edge of chaos: infinite group, capacity breaks through (XOR12+), rich but structured
- No DKC analog for Class III (chaotic) -- algebraic structure prevents true chaos

The finite/infinite group transition (D79-D80) IS the phase transition from ordered to edge-of-chaos. The transition variable is GROUP FINITENESS with ADE classification.

D106 FALSIFIES naive application of dynamical complexity measures to DKC. Topological entropy (the Lyapunov analog for braids) is completely orthogonal to computation -- both periodic and pseudo-Anosov braids compute XOR at 100% participation. The TL quotient at delta=0 erases dynamical information while preserving algebraic structure.

The correct complexity measures for algebraic reservoirs are:
- Derived series depth (D93)
- Solvability vs non-solvability (D94, Barrington's theorem)
- BFS catalog depth (D82)
- Galois orbit structure (D107)
- Product closure and v_2 connectivity (D108)

This is a genuine refinement of the edge-of-chaos thesis: for algebraic reservoirs, relevant complexity is representation-theoretic, not dynamical. PUBLISHABLE as noted in the analysis.

D82's "models learn complex solutions even when simple ones exist" has exact DKC counterpart: deep entries are 2x more efficient than shallow, developing algebraic coherence that transfers to harder problems.

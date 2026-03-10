---
id: CON0015
title: "Free Transformer split decoder maps to DKC catalog (fixed) + activation (conditioned)"
connects:
  - "Free Transformer split decoder: first L/2 context-independent, Z injected at midpoint"
  - "DKC split architecture: BFS catalog (context-free) + encoding/activation (conditioned) (D92, D94)"
type: structural_parallel
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 7"
---

The Free Transformer splits the decoder at its midpoint: first L/2 layers build context-independent representations, then Z is injected and the second half conditions on latent structure.

DKC has an exact structural parallel:
1. First half (context-independent): BFS closure builds the Z[zeta_8] catalog without knowing what function will be computed. The catalog is the same regardless of encoding, activation, or target function.
2. Z injection (encoding choice): The encoding is chosen, restricting the accessible function space (D92: parity-lock theorem).
3. Second half (conditioned generation): The activation function determines which specific Boolean function is computed from the catalog.

The catalog (first half) is REUSABLE across different encodings and activations, just as the Free Transformer's first L/2 layers produce representations usable under any Z value. This maps onto the reservoir computing decomposition (D94): fixed reservoir (first half) + readout (second half).

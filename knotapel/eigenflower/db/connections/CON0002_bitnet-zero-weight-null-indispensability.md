---
id: CON0002
title: "BitNet zero-weight feature filtering parallels DKC bracket-null indispensability"
connects:
  - "BitNet b1.58 zero-weight feature filtering"
  - "DKC bracket-null indispensability and regime transition (D84, D87, D88)"
type: structural_parallel
strength: moderate
source: "extractions/group-a-knotapel-relevance.md, Connection 2"
---

BitNet includes 0 in {-1, 0, +1}, enabling explicit feature filtering -- the network learns to ignore certain inputs. DKC's bracket-null entries (Re(q)=0) are "invisible" to the scalar bracket trace yet carry directional information essential for computation.

Both point to the same principle: computational power requires the ability to selectively suppress signal components. Removing bracket-null entries from the Z[zeta_8] catalog drops XOR capacity from XOR8 to XOR6 (D84), below even a random-removal baseline. These null entries cover 6 S^2 directions absent from all non-null entries.

The regime transition (D87) deepens this: null entries are computationally indispensable at finite groups (zeta_8, 37.5% null) but dispensable at infinite groups (zeta_12, 3% null). This predicts that BitNet's zero-weight fraction should decrease as model size increases.

The two-role direction theorem (D88) -- 4 body-diagonal directions as rigid anchors (8% XOR loss under 10-degree perturbation), 6 edge-midpoint directions as flexible scaffolding (slight GAIN under perturbation) -- suggests structured sparsity for BitNet: some zero-weight positions should be locked while others are flexible.

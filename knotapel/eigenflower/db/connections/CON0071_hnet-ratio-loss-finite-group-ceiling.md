---
id: CON0071
title: "H-Net ratio loss preventing trivial compression parallels DKC finite group ceiling"
connects:
  - "H-Net ratio loss: prevents chunking from degenerating to trivial 1-token compression"
  - "DKC finite group ceiling: binary octahedral group caps catalog at 24 elements (D79-D80)"
type: structural_parallel
strength: speculative
source: "extractions/group-b-knotapel-relevance.md, Connection 1 (ratio loss)"
---

H-Net's ratio loss prevents trivial compression (collapsing everything into one token). DKC's finite group ceiling prevents infinite catalog growth at zeta_8 (24 elements cap the catalog).

Moving to zeta_12 (infinite group) removes DKC's ceiling, analogous to relaxing H-Net's compression ratio constraint. But the infinite group introduces its own challenges: the catalog never closes, requiring truncation decisions.

The 90% at 3457 directions (D69) is the "trivial compression" failure in DKC: too many directions trivially separates everything but provides no meaningful computation. The ratio loss analog: too little compression also fails.

Both systems need a compression sweet spot: enough compression to create meaningful structure, not so much that all information is lost, not so little that the representation is trivially over-complete.

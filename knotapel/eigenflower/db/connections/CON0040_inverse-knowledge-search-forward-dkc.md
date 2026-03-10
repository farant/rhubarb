---
id: CON0040
title: "Forward DKC IS inverse knowledge search over the braid group"
connects:
  - "SciencePedia inverse knowledge search: retrieve derivation chains for a target concept"
  - "DKC forward computation: search braid-space backward from Boolean endpoints (D29, D108)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 1"
---

Paper 1's Inverse Knowledge Search -- given a target endpoint, retrieve all derivation chains leading to it -- is exactly what forward DKC does. The "endpoints" are Boolean truth tables (XOR, AND, etc.). The "derivation chains" are braid words whose bracket evaluations produce weight triples computing those truth tables.

The analogous "dark matter" in DKC is braid topology connecting a truth table to its implementing weights. The bracket value (a Z[zeta_8] integer) is the compressed fact; the braid word is the derivation chain that standard knot theory literature never records in computational context.

D108's Raqiya analysis is exactly the Brainstorm Search Engine's concept-to-provenance inversion applied to Boolean computation: given parity capability, retrieve all Z[zeta_8] values participating in it, then analyze their algebraic graph structure. The 3-way classification (41 parity / 17 poison / 42 neutral) is inverse knowledge search -- not "what does this value compute?" but "what reasoning chains pass through values that compute parity?"

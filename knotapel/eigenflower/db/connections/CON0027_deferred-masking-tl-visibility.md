---
id: CON0027
title: "MicroDiT deferred masking parallels TL visibility filter"
connects:
  - "MicroDiT deferred masking: process ALL patches through lightweight mixer, then mask 75%"
  - "DKC TL visibility filter: most braid structure invisible to trace (D106, D107)"
type: structural_parallel
strength: speculative
source: "extractions/group-c-knotapel-relevance.md, Connection 7"
---

MicroDiT's deferred masking processes all input patches through a "patch-mixer" before masking 75% for the main backbone. Unmasked patches carry global context despite most being dropped.

The TL trace is a "masking" operation on braid information: it projects the full representation matrix onto a single number, discarding most matrix structure. D106 showed this projection kills all dynamical/topological information (entropy) while preserving all computational information (DKC capacity).

D107's constructive phase formula makes this precise: the trace sums over diagonal entries, collapsing the full matrix to a single cyclotomic integer. This is structured masking preserving exactly Z/4Z phase information while discarding off-diagonal structure.

This suggests: in a DKC-inspired architecture, use a full algebraic "mixer" layer (braid representation computation) followed by a trace-based "masking" layer. The mixer processes all information expensively; the mask retains only the computationally relevant projection cheaply.

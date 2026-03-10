---
id: CON0030
title: "MicroDiT precomputed embeddings = DKC precomputed bracket catalog"
connects:
  - "MicroDiT precomputing text (CLIP) and image (VAE) embeddings once"
  - "DKC BFS bracket catalog computed once, used across 109 demos (D29, D48)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 10"
---

MicroDiT precomputes embeddings once and stores them; training operates purely on embeddings, amortizing encoding cost. The bracket catalog IS a precomputed embedding of the braid group into Z[zeta_8]. Computing it requires O(2^n) state-sum evaluations per braid, but once computed, each entry is a 4-tuple of integers manipulated in O(1).

The amortization is more dramatic than MicroDiT's: MicroDiT recomputes when changing encoder or corpus. The bracket catalog never changes -- it is a mathematical fact about the Kauffman bracket at delta=0. Once computed, it is permanent. DKC weights are literally zero-cost to deploy: precompute once, store as 16 bytes per entry, use forever.

A hybrid architecture could exploit this: precompute DKC weight catalogs for multiple cyclotomic rings (minutes to hours), store them, then use as fixed embedding layers in a trainable architecture (analogous to MicroDiT's frozen VAE/CLIP encoders).

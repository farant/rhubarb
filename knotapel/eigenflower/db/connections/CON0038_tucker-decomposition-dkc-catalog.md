---
id: CON0038
title: "Tucker decomposition of DKC catalog could reveal higher-order solution structure"
connects:
  - "Domingos tensor logic: Tucker decomposition reveals invented predicates"
  - "DKC 100-value catalog partitioned into 41 parity / 17 poison / 42 neutral (D96, D108)"
type: technique_transfer
strength: moderate
source: "extractions/group-d-knotapel-relevance.md, Connection 7"
---

Paper 1's Tucker decomposition of learned tensors reveals invented predicates: factor matrices M, M', M'' and core tensor C form a low-rank approximation. Thresholding to Boolean yields new logical relations.

The 100-value DKC catalog viewed as a 100x100x100 tensor (indexed by weight triple, Boolean XOR output) has natural Tucker structure. The 41/17/42 parity/poison/neutral partition IS a discovered "predicate" -- parity-capability is a latent property that Raqiya's graph analysis detects.

Tucker decomposition would formalize this: factor matrices separate values into parity-capable/poison/neutral along each mode, and the core tensor encodes which COMBINATIONS work. The 906 parity solutions at k=6 (D50) form a sparse Boolean tensor over the 41 parity values whose decomposition would reveal combinatorial synergy.

The D96 result (21 > 24) IS a rank-1 Tucker update: removing 3 Q8-null entries improved capacity. The optimal low-rank approximation has fewer entries than the full set -- the removed entries contributed noise without increasing effective rank.

This is a new capability beyond Raqiya's pairwise analysis: Tucker captures higher-order correlations among triples/quadruples.

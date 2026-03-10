---
id: PT0011
title: "Tucker decomposition for predicate invention from data"
status: untested
source: ["Tensor Logic paper (Domingos, 2025)"]
applicable_to: []
related: [PT0009, PT0010]
---

Use Tucker decomposition on data tensors to discover latent predicates. If the program to learn is A[i,j,k] = M[i,p] * M'[j,q] * M''[k,r] * C[p,q,r], the learned factor matrices M, M', M'' and core tensor C form a Tucker decomposition of A. Thresholding the factor matrices into Booleans yields invented predicates -- new logical relations discovered from data.

This is how you EXTRACT new syllogistic relationships not explicitly stated in the corpus. The Bible and Summa contain implicit logical relationships that Tucker decomposition could surface.

Tucker decomposition also provides scaling: converting sparse tensors to dense via Tucker is exponentially more efficient than operating on sparse tensors directly. Even random decomposition works with small controllable error.

For eigenflower: apply to the Boolean tensor representation of Bible/Summa text. The discovered factor matrices reveal latent conceptual structure. Threshold to Boolean to get new propositions. Verify by checking against known theology.

Implementation: Tucker decomposition is a well-studied numerical algorithm (ALS, HOSVD). Implementable in C89 with LAPACK support from Apple Accelerate.

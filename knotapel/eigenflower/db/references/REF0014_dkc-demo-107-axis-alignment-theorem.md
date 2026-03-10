---
id: REF0014
title: "DKC Demo 107 — Z/4Z Axis-Alignment Theorem (PROVEN)"
type: dkc_theorem
source_project: knotapel
demo_number: 107
summary: "Every braid representation matrix entry over a TL module at delta=0 is axis-aligned: each entry is exactly n * zeta_8^k (integer * root of unity). Constructive phase formula: Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}. Nesting parity Z/2Z grading also proven. NOVEL."
related: []
---

## Description

Demo 107 proved the Z/4Z Axis-Alignment Theorem universally with a constructive phase formula. Key results:
1. Every braid matrix entry at delta=0 is axis-aligned: exactly one nonzero cyclotomic component (1-sparse in the {1, zeta_8, zeta_8^2, zeta_8^3} basis)
2. Constructive formula: Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}
3. Three binary variables (writhe parity, nesting of source, nesting of target) determine the cyclotomic phase -- a 3-bit latent code
4. Nesting parity Z/2Z grading proven (NOVEL)
5. q-Catalan identity confirmed
6. Orthogonality theorem: multiplicative quotient graph = K_5 (Z/4Z rotation), additive = star tree

## Relevance to Eigenflower

- Axiality = extreme compressed sensing: each Z[zeta_8] value is 1-sparse, reducible to integer magnitude + 2-bit phase index
- Structurally isomorphic to BitNet's ternary constraint: weight = integer coefficient on one of 4 fixed basis vectors
- The constructive phase formula IS a 3-bit binary mapper (Free Transformer's Z): writhe parity + 2 nesting bits -> Z/4Z phase
- The orthogonality theorem decomposes DKC's "parity operator" into phase channel (Z/4Z rotation) and magnitude channel (axis-preserving addition)
- Enables extremely efficient inference: kernel matrix becomes block-diagonal with at most 4 blocks
- Axiality is the mathematical foundation for "4-ary BitNet" where weights are integer on each of 4 axes

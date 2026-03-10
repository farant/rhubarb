---
id: CON0060
title: "Free Transformer binary mapper IS D107's Z/4Z phase formula"
connects:
  - "Free Transformer binary mapper: H independent Bernoulli bits -> 2^H latent values"
  - "DKC constructive phase formula: 3 binary variables -> Z/4Z phase (D107)"
type: mathematical_identity
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 4 (detail)"
---

D107's constructive formula Entry(r,c) = (integer path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4} IS a natural binary mapper.

Three binary variables determine the cyclotomic phase:
1. Writhe parity (w mod 2)
2. Nesting of source state (nest(r) mod 2)
3. Nesting of target state (nest(c) mod 2)

This is a 3-bit latent code (2^3 = 8 possible phases, but only 4 realized because of mod 4 reduction). It IS the Free Transformer's binary mapper with H=3 and a Z/4Z output constraint.

An encoder that infers (writhe_parity, nest_source, nest_target) from a braid word implements the Free Transformer's encoder using DKC's algebraic structure. The decoder uses these 3 bits to determine the phase and generate matrix entries.

This is one of the tightest mathematical correspondences in the entire crossover: the formulas are literally the same computation expressed in different notation.

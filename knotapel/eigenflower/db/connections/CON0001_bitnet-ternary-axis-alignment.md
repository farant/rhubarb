---
id: CON0001
title: "BitNet ternary weights structurally isomorphic to Z/4Z axis-aligned DKC weights"
connects:
  - "BitNet b1.58 ternary weight quantization {-1, 0, +1}"
  - "DKC Z/4Z Axis-Alignment Theorem (D107)"
type: mathematical_identity
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 1"
---

DKC's Z/4Z Axis-Alignment Theorem (D107, proven) shows that every braid representation matrix entry over a TL module at delta=0 is axis-aligned: each entry is exactly `n * zeta_8^k` where n is an integer and k in {0,1,2,3}. The weight decomposes into an integer magnitude (the path count) and a 2-bit phase index (the axis selector).

This is structurally isomorphic to BitNet's ternary constraint. BitNet uses {-1, 0, +1} on 1 axis; DKC uses {n} on 4 axes. The constructive phase formula `Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}` means the weight is determined by an integer coefficient plus a 2-bit axis selector -- exactly the same decomposition as BitNet's sign + magnitude.

The `Cyc8` exact arithmetic library already implements weight operations as pure integer arithmetic (16 integer muls per product, zero floating-point), achieving the same efficiency win BitNet claims through algebraic structure rather than quantization.

This suggests a "4-ary BitNet" where weights are {-1, 0, +1} on each of 4 cyclotomic axes, giving up to 81 possible weight values per parameter while remaining integer-only.

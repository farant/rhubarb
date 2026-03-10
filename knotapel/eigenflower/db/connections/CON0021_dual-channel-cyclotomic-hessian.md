---
id: CON0021
title: "DKC Dual-Channel Theorem as 'cyclotomic Hessian' for quantization geometry"
connects:
  - "GPTQ Hessian lattice geometry determining quantization error"
  - "DKC Dual-Channel Theorem: phase coherence + magnitude diversity (D108-D109)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 2"
---

GPTQ's Hessian determines which weight configurations are "close" in output space -- weights that differ numerically may produce similar outputs if the Hessian has low eigenvalues along that direction.

The Dual-Channel Theorem shows an analogous phenomenon: Z[zeta_8] values that are algebraically close (same Galois orbit, same norm class) may be computationally very different (one parity-capable, one poison), while algebraically distant values may be computationally equivalent.

D108 data: parity and poison vocabularies are maximally contrasted on product closure (17 edges vs 0) but overlap on same_axis, additive_closure, norm, and v_2 metrics. The "importance weighting" in DKC is not defined by an input Hessian but by the algebraic graph structure of the weight lattice itself.

The Raqiya analysis framework already computes the algebraic graph metrics that would define a "cyclotomic Hessian." The partition hierarchy (root orbits -> Galois orbits -> norm classes -> v_2 classes) is a refinement chain that could replace LDL decomposition for quantization decisions on non-standard lattices.

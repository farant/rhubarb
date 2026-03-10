---
id: CON0032
title: "Forward DKC IS tensor logic over cyclotomic Boolean tensors"
connects:
  - "Domingos tensor logic: syllogisms as einsum over Boolean tensors with step function"
  - "DKC forward computation: Output[mask] = Activation(Weight[i] * Input[mask, i]) (D29, D45-D50)"
type: mathematical_identity
strength: strong
source: "extractions/group-d-knotapel-relevance.md, Connection 1"
---

Domingos proposes that a Datalog rule like `Aunt(x,z) <- Sister(x,y), Parent(y,z)` is an einsum over Boolean tensors: `A_xz = H(S_xy * P_yz)`. Syllogisms compile to matrix multiplications + nonlinearity. Forward chaining reaches deductive closure.

Forward DKC (D29) IS this. The computation:
1. Bracket catalog = finite set of Z[zeta_8] values (100 entries from TL_3 braids)
2. Weight assignment = select weights from catalog
3. Weighted sum = einsum over input index: S = w1*x1 + w2*x2 + w3*x3
4. Activation = k-sector MVN or phase_cell
5. Output = Boolean function value

DKC's tensors are cyclotomic (Z[zeta_8]), not just {0,1}. The tensor entries come from topology (Habiro 2002), not learning. DKC's "forward chaining" IS BFS catalog closure (D82, D103): start with generators, apply all compositions, accumulate until closure.

The parity-lock theorem (D92) IS a tensor logic impossibility result: under +/-q encoding, weight sums for masks 00...0 and 11...1 are both zero, so any function where f(0...0) != f(1...1) is structurally impossible. The circuit complexity hierarchy (D93: AND/XOR ratio 1.01 at N=3 to infinity at N=8) quantifies the "hardness landscape" that tensor logic would predict should exist but cannot yet compute.

DKC provides 20,000+ lines of working C89 code implementing exact cyclotomic tensor arithmetic -- an implementation Domingos's framework currently lacks.

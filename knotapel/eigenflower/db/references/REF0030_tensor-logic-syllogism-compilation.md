---
id: REF0030
title: "Tensor Logic — Syllogism-to-ANN Compilation (Domingos)"
type: other
source_project: external
summary: "Domingos's tensor logic: a Datalog rule like Aunt(x,z) <- Sister(x,y), Parent(y,z) compiles to an einsum over Boolean tensors with step function: A_xz = H(S_xy * P_yz). Barbara = matrix multiply + threshold. No gap between syllogism and ANN weight -- they are the same thing in two notations."
related: []
---

## Description

The tensor logic framework (Domingos, Group D Paper 1) establishes that syllogistic reasoning compiles directly to tensor equations:

- SP[s,p] = H(SM[s,m] * MP[m,p]) for Barbara
- Forward chaining reaches the deductive closure
- Tucker decomposition reveals invented predicates (latent relations)
- Temperature T controls deductive (T=0) vs analogical (T=inf) reasoning

Key result: there is NO gap to bridge between "syllogism" and "ANN weight." A syllogism IS a matrix multiplication plus a step function.

## Relevance to Eigenflower

- This is the theoretical foundation for Eigenflower's syllogism layer
- DKC's exact Z[zeta_8] arithmetic IS a working implementation of tensor logic over algebraic integer tensors
- The forward DKC computation `Output[mask] = Activation(Weight[i] * Input[mask, i])` is Domingos's einsum
- The BFS catalog closure IS forward chaining
- Temperature = sector count (DKC's k) mapping is non-trivial due to non-monotonic behavior
- Paper has no implementation; DKC provides one with 20,000+ lines of C89 code
- Build priority #1 for Eigenflower: tensor logic interpreter

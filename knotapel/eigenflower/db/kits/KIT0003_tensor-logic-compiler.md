---
id: KIT0003
title: "Tensor logic compilation of syllogisms"
status: planned
tier: 3
input: "Syllogism in logical form (premises, conclusion, figure, mood)"
output: "Tensor equation (matrix operations + Heaviside step function)"
depends_on: [KIT0001, KIT0007]
effort: medium
source: ["Tensor Logic paper (Domingos, 2025)"]
migrated_from: PT0009
applicable_to: [G0005, G0004]
related: [KIT0004, KIT0010, KIT0012]
---

## Description

Compile syllogisms directly as tensor equations over Boolean tensors. A Datalog rule is an einsum over Boolean tensors with a step function.

Barbara: "All M are P, All S are M, therefore All S are P" compiles to SP[s,p] = H(SM[s,m] * MP[m,p]) -- one matrix multiplication plus a step function. There is no gap between "syllogism" and "ANN weight" -- they are the same thing in two notations.

Forward chaining = execute tensor equations as linear code until fixpoint = deductive closure. Backward chaining = treat each equation as a function, recurse = query answering.

The derivative of a tensor equation w.r.t. any RHS tensor is the product of all other RHS tensors. The entire gradient of a tensor logic program is itself a tensor logic program. Tucker decomposition on data tensors discovers latent predicates (predicate invention).

Relations as sparse Boolean tensors are memory-efficient for small corpora. The Bible as a sparse Boolean matrix where M[i,j] = 1 iff position i contains word j.

A complete Transformer can be specified in ~12 tensor equations (Table 2 of paper). RNN in tensor logic is Turing-complete.

No code released by the paper authors -- this is a language specification, not an implementation. We build the implementation.

## Key Insight from DKC

Forward DKC (Demo 29, confirmed Demos 45-50) already proved this works: exact Z[zeta_8] bracket values compute XOR and all 13 NPN classes without training. The tensor logic framework is a different notation for the same algebraic truth.

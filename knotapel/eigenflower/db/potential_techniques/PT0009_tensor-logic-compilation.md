---
id: PT0009
title: "Tensor logic compilation of syllogisms to ANN weights"
status: promoted_to_kit
promoted_to: KIT0003
source: ["Tensor Logic paper (Domingos, 2025)"]
applicable_to: []
related: [PT0010, PT0011, PT0012, PT0013]
---

Compile syllogisms directly as tensor equations over Boolean tensors. A Datalog rule is an einsum over Boolean tensors with a step function: the rule "Aunt(x,z) <- Sister(x,y), Parent(y,z)" becomes A_xz = H(S_xy * P_yz). This is THE direct bridge between syllogisms and matrix operations.

Barbara: "All M are P, All S are M, therefore All S are P" compiles to SP[s,p] = H(SM[s,m] * MP[m,p]) -- one matrix multiplication plus a step function. There is no gap between "syllogism" and "ANN weight" -- they are the same thing in two notations.

Forward chaining = execute tensor equations as linear code until fixpoint = deductive closure. Backward chaining = treat each equation as a function, recurse = query answering.

Learning: the derivative of a tensor equation w.r.t. any RHS tensor is the product of all other RHS tensors. The entire gradient of a tensor logic program is itself a tensor logic program. Tucker decomposition on data tensors discovers latent predicates (predicate invention) -- this is how you EXTRACT new logical relations from data.

Relations as sparse Boolean tensors are memory-efficient for small corpora. Text becomes a Boolean matrix where M[i,j] = 1 iff position i contains word j. The Bible as a sparse Boolean matrix.

A complete Transformer can be specified in ~12 tensor equations (Table 2 of paper). RNN in tensor logic is Turing-complete.

No code released -- the paper is a language specification, not an implementation.

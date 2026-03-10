---
id: P0010
title: "Tensor Logic: The Language of AI"
authors: [Pedro Domingos]
year: 2025
arxiv_id: "2510.12269"
extraction_group: D
relevance: high
key_techniques: [tensor_equations, boolean_tensors, datalog_to_einsum, forward_chaining, backward_chaining, tucker_decomposition, predicate_invention, temperature_controlled_reasoning]
summary: "Unifies symbolic AI and neural AI: a Datalog rule IS an einsum over Boolean tensors with step function. Syllogism 'All M are P, All S are M, therefore All S are P' compiles directly to SP[s,p] = H(SM[s,m] * MP[m,p]). Complete Transformer in ~12 tensor equations."
related: []
---

## Key Contributions

Tensor logic proposes a unified programming language for AI where the sole construct is the tensor equation. The key observation: logical rules (Datalog) and Einstein summation (einsum) are fundamentally the same operation. A Datalog rule is an einsum over Boolean tensors with a step function applied elementwise. A relation is a sparse Boolean tensor. Therefore ALL of symbolic AI (logic programming) and ALL of neural AI (tensor algebra) can be expressed in a single formalism. The gradient of a tensor logic program is itself a tensor logic program.

## Key Techniques

- **Datalog = einsums over Boolean tensors**: `Aunt(x,z) <- Sister(x,y), Parent(y,z)` becomes `A_xz = H(S_xy * P_yz)` where H is Heaviside step. A syllogism IS a tensor equation.
- **Relations as sparse Boolean tensors**: Any n-ary relation is a rank-n Boolean tensor. Exponentially more efficient when sparse.
- **Forward chaining = deductive closure**: Execute tensor equations as linear code until fixpoint. Load syllogisms, run, get all possible inferences.
- **Backward chaining = function calls**: Each equation treated as a function, recurse. Inference engine for answering questions.
- **Tucker decomposition as predicate invention**: Learned factor matrices and core tensor form Tucker decomposition. Thresholding into Booleans yields INVENTED predicates. Discovers new logical relations from data.
- **Reasoning in embedding space**: Embed facts/relations into dense vectors. Rules embed as products of antecedent embeddings. Approximate reasoning with error decreasing as embedding dimension increases (like Bloom filter for logic).
- **Temperature-controlled reasoning**: T->0 = purely deductive (Gram matrix -> identity). T->infinity = analogical (similar objects borrow inferences). Different T per rule possible.
- **Transparency**: Unlike LLMs, reasoning chain is fully transparent and at low T immune to hallucination.

## Specific Relevance to Eigenflower

- **DIRECT HIT for syllogism compilation**: A syllogism literally compiles to a matrix multiplication + step function. "All men are mortal, Socrates is a man, therefore Socrates is mortal" becomes `Mortal[x] = H(Man[x,y] * Mortal_class[y])`.
- **DIRECT HIT for syllogism extraction**: Tucker decomposition on data tensors discovers latent predicates. Text -> Boolean matrix -> learn tensor equations -> threshold -> extract rules. The learned factor matrices ARE extracted syllogisms.
- **Dynamic recompilation**: New syllogism = new tensor equation. Just add the equation and re-run inference. No retraining needed.
- **Text as Boolean matrix**: Bible becomes a sparse Boolean matrix where M[i,j] = 1 iff position i contains word j.
- **Turing-complete**: RNN in tensor logic is Turing-complete via Siegelmann and Sontag (1995).

## Key Equations

- Syllogism as tensor equation: `SP[s,p] = H(SM[s,m] * MP[m,p])`
- Perceptron: `Y = step(W[i] X[i])`
- MLP: `X[i,j] = sig(W[i,j,k] X[i-1,k])`
- Embedding a rule: `EmbCons[...] = EmbAnt_1[...] ... EmbAnt_n[...]`
- Gradient: derivative of tensor equation w.r.t. any RHS tensor = product of all OTHER RHS tensors

## Key Structures

- Complete Transformer specified in ~12 tensor equations (Table 2)
- Full GNN in 7 equations (Table 1)
- Kernel machines, graphical models all expressible
- Datalog programs are valid tensor logic programs

## Implementation Notes

- No code released -- paper is a language specification
- For C89: tensor equations map cleanly to nested loops with array indexing
- Sparse tensor representation (relation tuples) is memory-efficient for small corpora
- Dense operations via Accelerate framework on macOS
- Key next step: tensor logic interpreter in C89 -- parse equations, allocate tensors, execute forward/backward chaining

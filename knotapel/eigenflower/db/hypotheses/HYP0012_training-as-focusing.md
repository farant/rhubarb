---
id: HYP0012
title: "Training-as-Focusing: training does not create computational structure, it locates pre-existing solutions on the algebraic lattice"
status: proposed
source: "knotapel/atlas/inventory/theorems.md (Forward DKC Theorem)"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

Neural network training does not CREATE the computational structure needed for a function — it LOCATES pre-existing solutions that already exist in the algebraic structure of the weight space. The solutions are there before training begins; gradient descent is a search procedure that finds them, not a constructive procedure that builds them.

This is the "Training-as-Focusing thesis" from the knotapel project, named for the Forward DKC theorem (D29): exact Z[zeta_8] bracket values compute XOR without any neural network training. The 100+ valid XOR triples were not learned — they were discovered as pre-existing features of the Z[zeta_8] lattice. Training is like focusing a camera: the image (solution) exists in the light field; the lens (training) merely brings it into resolution.

## Evidence For

- **Forward DKC theorem (D29):** LANDMARK result. Exact bracket values compute XOR with RMS = 0.000 — no training involved. The computational solutions pre-exist in the algebraic structure.
- **100+ valid XOR triples found from exact bracket values with gauge rotation:** These are not learned solutions — they are structural features of Z[zeta_8] that happen to compute Boolean functions.
- **D45-47:** All 13 NPN classes are computable from forward DKC, extending the pre-existence result beyond XOR to the full space of 3-input Boolean functions.
- **D50:** Parity reachable at k=6 sectors. The solutions exist; the activation (readout mechanism) determines which ones are accessible.
- **Philosophical grounding:** This is consistent with a realist (as opposed to nominalist) metaphysics — mathematical structures exist independently and computation discovers them rather than constructing them. This fits the project's broader intellectual framework.

## Evidence Against

- The Forward DKC theorem works on a very specific algebraic structure (Z[zeta_8] bracket values at a specific parameter). It is not clear that arbitrary weight spaces have the same "pre-existing solution" property.
- Standard neural networks with random initialization and gradient descent may be doing something genuinely constructive, not merely locating. The distinction between "locating" and "creating" may be a matter of interpretation rather than a falsifiable claim.
- For large models with billions of parameters, the "weight space" is so high-dimensional that the distinction between "solutions pre-exist" and "solutions are trivially constructible" may be meaningless.
- The thesis has been validated only for Boolean functions on 3-5 inputs. Whether it extends to the complexity of language model training is unknown.

## What Would Confirm It

- Showing that multiple independent training runs on the same architecture converge to the same algebraic weight configurations (up to symmetry), suggesting they are finding the same pre-existing solutions rather than constructing different ones.
- Demonstrating that the solutions found by training correspond to known algebraic features of the weight space (e.g., lattice points, algebraic integers, roots of unity).
- Extending the Forward DKC result to more complex functions and larger algebraic structures.
- Using GPTQ-as-CVP to decompose trained weights and finding that they lie on (or near) algebraic lattice points.

## What Would Refute It

- Showing that trained weights are generic (irrational, no algebraic structure) and that the solutions are genuinely constructed by the training process rather than located.
- Demonstrating that the Forward DKC pre-existence result is specific to Z[zeta_8] and does not generalize to other algebraic substrates.
- Finding that the "solutions pre-exist" framing makes no empirically testable predictions that differ from the standard "training constructs solutions" framing.

## Implications If True

- The eigenflower syllogism layer can be built by COMPILATION (locating the right lattice points) rather than TRAINING. This is exactly the project's approach.
- The algebraic structure of the weight space is not an implementation detail — it is the source of computational capability. Choosing the right algebraic substrate is the primary design decision.
- Training infrastructure becomes a search/optimization tool rather than a construction tool. This changes the design of optimizers, loss functions, and training procedures.
- There is a deep resonance with mathematical realism and Thomistic metaphysics: mathematical truths are discovered, not invented. Computation, as a mathematical process, discovers pre-existing structure.

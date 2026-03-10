---
id: HYP0004
title: "DKC IS tensor logic over Boolean tensors — a mathematical identity, not an analogy"
status: proposed
source: "notes/01-analysis.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

The forward DKC computation `Output[mask] = Activation(Weight[i] * Input[mask, i])` is literally a tensor equation in Domingos's tensor logic formalism. This is not a metaphorical parallel — it is a mathematical identity. DKC implements tensor logic over Boolean tensors with exact cyclotomic arithmetic.

More specifically:
- DKC weights are predicate-to-predicate relationship matrices (the SP, SM, MP matrices of syllogistic logic).
- DKC activation functions are Heaviside-like threshold operators (the H in Domingos's SP[s,p] = H(SM[s,m] * MP[m,p])).
- DKC's Boolean function computation IS syllogistic inference instantiated on 3-5 input variables.

This means DKC provides a concrete, exactly-analyzable implementation of Domingos's theoretical framework — an implementation that Domingos does not have (his paper is purely theoretical).

## Evidence For

- **Forward DKC theorem (D29):** Exact Z[zeta_8] bracket values compute XOR without training. This is tensor logic in action — a matrix multiply + activation computes a logical function.
- **D45-47:** All 13 NPN classes computed via forward DKC, demonstrating that tensor logic over Z[zeta_8] covers the full Boolean function space at 3 inputs.
- **D50:** Parity reachable at k=6 sectors — the activation IS the Heaviside/threshold operator in tensor logic.
- **D107 axis-alignment theorem:** DKC's axis-aligned entries decompose as integer magnitude + 2-bit phase. This is the structural basis for why the tensor equation works in exact arithmetic.
- **D108-109 dual-channel theorem:** The two computational channels correspond to two ways of decomposing the tensor equation.
- **20,000+ lines of working C89 code** implementing exactly what Domingos describes theoretically.

## Evidence Against

- The identification has not been formally written up or verified by someone outside the project.
- DKC operates on Boolean functions with 3-5 inputs. Domingos's tensor logic is defined for arbitrary predicates over arbitrary domains. The identification is proven only at the scale of small Boolean functions.
- It is possible that the structural similarity breaks down when scaling to the propositional complexity of real syllogistic reasoning (10,000+ terms vs 3-5 variables).

## What Would Confirm It

- Formally mapping every element of Domingos's tensor logic framework onto its DKC counterpart and verifying the equations are identical.
- Extending DKC to compute syllogisms (not just Boolean functions) and showing that the tensor logic equations produce correct conclusions.
- A publication demonstrating the identity, ideally reviewed by someone familiar with Domingos's work.

## What Would Refute It

- Finding a structural element of Domingos's formalism that has no DKC counterpart, or vice versa.
- Showing that the identification only works for the simplest cases and breaks down when tensor logic features beyond basic syllogism forms (e.g., multi-predicate, nested quantification) are needed.

## Implications If True

- DKC is not merely "relevant to" eigenflower — it IS the proof environment for the core computational mechanism.
- Domingos's framework gains its first concrete implementation with exact arithmetic.
- The entire knotapel demo catalog (109 demos) becomes experimental evidence for tensor logic.
- The bridge from DKC's Boolean-function world to eigenflower's syllogistic reasoning is narrower than it appears — it is a scaling problem, not a bridging problem.
- This is identified as a publishable result in the analysis.

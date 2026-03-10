---
id: HYP0001
title: "Circuit sharing of abstract algebraic patterns is the mechanism behind LLM generalization"
status: proposed
source: "notes/03-transparency-and-wisdom-circuits.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

LLMs generalize because they develop internal circuits that match abstract algebraic patterns of relationships between objects in reality. Once a model has a circuit for "asymmetric dependency," inputs from different domains (family trees, causal reasoning, logical inference) start sharing that circuit. The circuit is domain-independent; the inputs are domain-specific. Generalization happens when domain-specific inputs route through domain-independent circuits.

This is NOT an analogy — it is a claim about the actual mechanism. The abstract relational pattern IS the shared computational structure, and the reason scaling works is that more data provides more instances of the same abstract patterns, strengthening the shared circuits until they "click" and work for all instances (emergence).

## Evidence For

- **DKC depth law (D82):** Recursive application of the same transformation builds computational power. This is structurally identical to the claim that wisdom circuits gain power through recursive application of the same abstract pattern at multiple levels of a problem. The DKC result provides a concrete mathematical example.
- **DKC algebraic coherence beats vocabulary (D82):** Shared intermediate structure matters more than raw diversity — directly supporting the claim that circuits are powerful because they are SHARED, not because they are diverse.
- **Mechanistic interpretability literature:** Anthropic's published work on superposition and circuits in transformers shows that internal features are reused across contexts, consistent with the circuit sharing thesis.
- **The proportionality argument:** "parent:child :: cause:effect :: premise:conclusion" — all instances of asymmetric dependency — and humans naturally recognize this. If human cognition shares circuits this way, ANNs trained on human-generated text would be under selection pressure to develop the same sharing.

## Evidence Against

- No direct experimental evidence from eigenflower yet.
- The "circuit" metaphor may be imprecise — transformer computation may not decompose into discrete reusable circuits in the clean way this thesis imagines.
- Scaling could work for other reasons (memorization + interpolation, for example) that don't require abstract pattern circuits.

## What Would Confirm It

- Decompiling a trained model's weights and identifying specific weight configurations that correspond to known abstract patterns (asymmetric dependency, part-whole, cyclic return, etc.).
- Showing that the same weight configuration activates for inputs from different domains that share the same abstract structure.
- Building a model with explicit wisdom circuits and showing it generalizes to new domains without retraining, purely because the new domain maps onto existing circuits.

## What Would Refute It

- Finding that trained models use entirely different circuits for structurally identical problems in different domains (e.g., separate circuits for "parent-child" and "premise-conclusion" despite both being asymmetric dependency).
- Showing that generalization is fully explained by surface-level statistical patterns without any need for abstract structural matching.

## Implications If True

- The eigenflower architecture should be designed around explicit, transparent wisdom circuits rather than hoping they emerge from training.
- The GPTQ-as-CVP framework on cyclotomic lattices becomes a concrete tool for identifying these circuits in existing models.
- The number of distinct abstract patterns needed for general intelligence may be small (dozens to hundreds), making the project tractable.
- This would validate the Thomistic hierarchy: scientia (particular facts) < intellectus (principles) < sapientia (abstract patterns organizing all knowledge).

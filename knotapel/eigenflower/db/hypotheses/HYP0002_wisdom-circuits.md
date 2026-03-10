---
id: HYP0002
title: "Domain-independent 'wisdom circuits' handle abstract relational patterns and are the source of generalization in neural networks"
status: proposed
source: "notes/03-transparency-and-wisdom-circuits.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

There exist specific computational circuits within neural networks that are:
1. **Domain-independent:** The same circuit handles asymmetric dependency whether the domain is family trees, causal chains, or syllogistic inference.
2. **Reusable:** Many domain-specific inputs route through the same wisdom circuit.
3. **Hierarchical:** Some patterns are more abstract than others (asymmetric dependency > parent-child > "Abraham begat Isaac").
4. **The source of generalization:** A new domain generalizes immediately if its structure maps onto existing wisdom circuits.

These circuits correspond to what Aquinas calls "sapientia" — not knowledge of everything, but understanding of the principles by which everything is organized.

In the eigenflower architecture, wisdom circuits would live in three locations: (1) the syllogism layer (tensor equations like Barbara are literally reusable circuits for syllogistic forms), (2) the fluency layer (learned grammatical patterns like ablative absolute = simultaneous background condition), and (3) the interface between them (structural mappings between linguistic form and logical content).

## Evidence For

- **Syllogistic forms as literal wisdom circuits:** Barbara (All M are P, All S are M, therefore All S are P) is domain-independent by definition — it works for any domain. When compiled as a tensor equation SP[s,p] = H(SM[s,m] * MP[m,p]), it IS a reusable circuit.
- **DKC parity-lock theorem (D92):** Encoding determines which functions are accessible. This is the "channeling in" problem — routing inputs to the right circuit.
- **DKC activation determines capability (D50):** The readout mechanism controls what the circuit can express. This is the "channeling out" problem.
- **Thomistic framework provides independent theoretical support:** The scientia/intellectus/sapientia hierarchy predates neural networks by 800 years and identifies the same structural insight — that abstract organizing principles are primary.

## Evidence Against

- No empirical demonstration in eigenflower yet.
- The concept of "wisdom circuits" may be too idealized — real neural computation may be messier, with circuits that are partially domain-specific and partially shared, rather than cleanly separated.
- It is unclear how many distinct wisdom circuits would be needed and whether the count is tractable.

## What Would Confirm It

- Building explicit wisdom circuits (starting with syllogistic forms) and demonstrating that they handle inputs from domains never seen during construction.
- Decompiling a trained model and finding weight configurations that activate for structurally similar but domain-different inputs.
- Demonstrating that adding a new wisdom circuit (e.g., for proportional analogy A:B::C:D) enables the model to handle ALL instances of that pattern across ALL domains without domain-specific training.

## What Would Refute It

- Finding that explicit wisdom circuits fail to generalize — that domain-specific fine-tuning is always needed even when the abstract pattern is the same.
- Showing that trained models achieve generalization through a mechanism that has no correspondence to discrete reusable circuits.

## Implications If True

- The eigenflower architecture is on the right track: build transparent wisdom circuits rather than relying on opaque emergence.
- The number of wisdom circuits needed for "general intelligence" in a specific domain (theological reasoning) may be quite small.
- This provides a concrete implementation path for the Thomistic hierarchy of knowledge.
- Wisdom circuits could be catalogued, composed, and shared — creating a kind of "standard library of abstract reasoning patterns."

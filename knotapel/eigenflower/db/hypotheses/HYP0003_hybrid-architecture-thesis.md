---
id: HYP0003
title: "Splitting fluency from reasoning into two coupled systems solves both the hallucination problem and the extraction problem"
status: proposed
source: "notes/02-architecture-insight.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

The hybrid architecture — a fluency ANN (small language model for parsing/generation) coupled to a syllogism ANN (compiled tensor logic graph for exact reasoning) — solves two problems simultaneously:

1. **The hallucination problem:** Normal LLMs try to do fluency AND reasoning with one system, so reasoning is unreliable because it is compressed into weights trained on token prediction. Splitting reasoning into an exact algebraic layer eliminates hallucination for deductive reasoning.

2. **The extraction problem:** Parsing syllogistic structure from natural language is a hard problem. A trained fluency layer handles the fuzzy, statistical work of language understanding, while the syllogism layer handles the exact, structural work of reasoning.

The key insight is that fluency is a pattern matching problem (tolerates approximation) while reasoning is a structural problem (requires exactness). ANNs are good at the former; exact algebra is good at the latter. The split happens at exactly the right seam.

## Evidence For

- **DKC provides the exact reasoning substrate:** 109 demos proving that algebraic structure encodes computation with zero floating point. The syllogism layer's compiled operators could live on cyclotomic lattices with DKC's exactness guarantees.
- **Domingos tensor logic:** SP[s,p] = H(SM[s,m] * MP[m,p]) — syllogisms compile to matrix multiply + step function. No approximation needed.
- **Hilbert space kernel operators:** Closed-form solution alpha = (K + lambda*I)^{-1} y — reasoning operators computed without iterative training.
- **The Summa's structure helps the fluency layer:** "Videtur quod," "Sed contra," "Respondeo dicendum quod," "Ad primum" — consistent structural markers across 3,000+ articles give the parser a huge head start.
- **Dynamic recompilation makes sense in this framing:** Adding new syllogisms only requires updating the syllogism graph, not retraining the fluency layer. The two layers update independently.

## Evidence Against

- No working implementation of either layer yet, let alone their coupling.
- The interface between the two layers is identified as "probably the most novel and hardest design problem." If the interface doesn't work, the whole architecture fails.
- It is unknown what fraction of the Summa's reasoning is strictly syllogistic vs analogical/authoritative/other. If only 30% is formal syllogisms, the syllogism layer covers a minority of cases.
- Existing retrieval-augmented generation (RAG) systems attempt a similar split and have mixed results — the coupling problem is hard.
- The fluency layer still needs to be "smart enough" to correctly parse logical structure from Latin prose, which is itself a significant capability.

## What Would Confirm It

- Phase 1 validation: building a tensor logic interpreter, manually compiling 50-100 syllogisms from the Summa, and verifying that forward chaining produces correct conclusions.
- Phase 2 validation: training a small fluency model on Bible + Summa that can parse the Summa's structural markers into proposition structures.
- Phase 3 validation: end-to-end "the walk" — text in, reasoned text out, where the fluency layer parses, the syllogism layer reasons, and the fluency layer narrates.

## What Would Refute It

- Finding that the interface between fluency and reasoning cannot be made reliable — that the mapping between natural language and logical structure is too ambiguous for a small model.
- Discovering that the fraction of the Summa's reasoning that is formally syllogistic is too small for the syllogism layer to be useful.
- Showing that a monolithic model of the same total parameter count outperforms the hybrid on theological reasoning tasks.

## Implications If True

- The eigenflower project has a viable architecture that is fundamentally different from mainstream LLM approaches.
- Hallucination-free deductive reasoning is achievable at small model scale.
- The system can learn incrementally (add syllogisms) without retraining — a massive advantage over monolithic LLMs.
- The architecture maps naturally onto Aquinas's own distinction between modes of reasoning (demonstration, analogy, authority, reductio, distinguo), with potential for multiple transparent reasoning layers rather than one opaque one.

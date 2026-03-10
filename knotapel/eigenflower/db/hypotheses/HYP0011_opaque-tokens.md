---
id: HYP0011
title: "Opaque tokens can break the finite-vocabulary limitation by allowing the net to reason about logical structure without knowing content"
status: proposed
source: "CLAUDE.md, knotapel/atlas/planning/vision.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

Because eigenflower compiles its net and has granular transparency of its execution, it can introduce a fundamentally new kind of token — "opaque tokens" (also: passthrough tokens, argument tokens).

A syllogism does not need to know the VALUE of its arguments — only their logical nature (term, predicate, subject, etc.). "All X are Y, all Y are Z, therefore all X are Z" works regardless of what X, Y, Z are.

**Mechanism:** Opaque tokens flow through the compiled portion of the net. The net operates on logical structure, not content. Execution tracks which input positions flow to which output positions. At output, opaque tokens are replaced with the original input values.

This is essentially type-level computation — the net reasons about relationships between slots, not about what fills the slots.

**Implications claimed:**
1. Breaks the finite-vocabulary limitation. New proper nouns, technical terms, any string can be an opaque token. The net doesn't need to have seen "bears" in training to reason about bears.
2. Enables graceful ignorance: if the net lacks a compiled subnet for a particular entity, it can recognize that it lacks the relevant syllogism and express "I don't know" rather than hallucinating.
3. The compiled portion becomes a logical skeleton that the statistical portion fills with content.

## Evidence For

- **Logical validity is form-independent:** This is a 2,300-year-old observation (Aristotle). Barbara works for any terms. The opaque token mechanism is the computational realization of this philosophical truth.
- **DKC's exact arithmetic provides the transparency needed:** DKC knows what every weight does (axis-aligned cyclotomic integer), what the activation does (k-sector partition), and why certain combinations compute certain functions. This level of transparency is necessary for tracking opaque token flow.
- **The hybrid architecture enables it:** In a monolithic LLM, you can't have opaque tokens because every token must interact with every other token through attention. In the hybrid architecture, the syllogism layer processes logical structure while the fluency layer handles content — a natural separation.
- **Type-level computation has precedent:** This is essentially what parametric polymorphism does in type theory. The mechanism is well-understood in programming language theory.

## Evidence Against

- **Status: CONCEPTUAL.** No implementation exists. The vision document explicitly marks this as requiring: (1) the compiled net to actually exist, (2) an execution model that tracks token flow through compiled weights, (3) the substitution mechanism at output.
- It is unclear how opaque tokens interact with attention mechanisms (if the fluency layer uses attention).
- Morphological variation is a practical challenge: "bear" -> "bears" -> "bear's" -> Latin declensions. The substitution mechanism must handle this.
- The claim that this "breaks the finite-vocabulary limitation" may overstate the case — the fluency layer still needs vocabulary for parsing and generation.

## What Would Confirm It

- Building a compiled syllogism net, feeding it input with opaque tokens (e.g., subject=OPAQUE_1, predicate=OPAQUE_2), and verifying that the net produces correct logical output with opaque tokens in the right positions.
- Demonstrating that after substitution, the natural language answer is correct for terms the model has never seen.
- Showing that the system correctly reports "I don't know" when it lacks a compiled subnet for the query.

## What Would Refute It

- Finding that opaque token flow through compiled weights is not reliably trackable — that the positions get scrambled or lost.
- Showing that the substitution mechanism at output produces incoherent results due to morphological or syntactic complications.
- Demonstrating that the overhead of tracking opaque token flow eliminates the performance benefit of compilation.

## Implications If True

- Eigenflower can reason about entities it has never been trained on — a capability no current LLM has.
- Graceful ignorance replaces hallucination — the system knows what it knows and what it doesn't.
- The compiled syllogism layer becomes a genuinely general reasoning engine, not limited to the vocabulary seen during training.
- This is a novel architectural concept with no published precedent in the AI literature (though parametric polymorphism in type theory is the closest analogue).

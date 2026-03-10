---
id: HYP0006
title: "The Summa Theologica is an 'uncompressed reasoning corpus' — it exposes its derivations rather than hiding them, making it uniquely suitable for training a reasoning model"
status: proposed
source: "notes/01-analysis.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

Most text hides its reasoning — it states conclusions without showing the derivation steps. The Summa Theologica is different: it systematically exposes the full argumentative structure in every article. Each article follows a fixed pattern:

1. "Videtur quod..." (It seems that...) — objections (premises to be refuted)
2. "Sed contra..." (On the contrary...) — counter-authority
3. "Respondeo dicendum quod..." (I answer that...) — the actual argument with derivation
4. "Ad primum/secundum/tertium..." — replies to objections, showing why the counter-premises fail

This structure means the Summa is an "uncompressed reasoning corpus" in the sense of the Inverse Knowledge Search paper. It doesn't just contain conclusions — it contains the full chain of reasoning from premises to conclusions, including the wrong arguments and why they're wrong.

This makes the Summa uniquely suitable for training a reasoning model because:
- The model can learn the STRUCTURE of arguments, not just their conclusions
- Invalid syllogisms (objections) provide natural contrastive examples
- The consistent article format provides reliable structural markers for parsing

## Evidence For

- **The Summa's structure is empirically consistent:** 3,000+ articles follow the same pattern across all parts. The structural markers are reliable and parseable.
- **Inverse Knowledge Search paper (Group E extraction):** Identifies the principle that uncompressed reasoning corpora are more valuable for training than compressed ones (which hide derivation steps).
- **Early Experience paper (Group E extraction):** Suggests generating invalid syllogism variants as contrastive examples — but the Summa already CONTAINS them as objections. The training data provides its own contrastive examples naturally.
- **1/8 data efficiency result (Group E):** When the corpus exposes reasoning structure, models learn with significantly less data.

## Evidence Against

- The actual logical structure within each article is still implicit — the structural markers identify SECTIONS but the syllogistic form within each section must still be parsed. "Respondeo dicendum quod" introduces the argument but doesn't label its premises, middle terms, and conclusions.
- It is unknown what fraction of the Summa's reasoning is formally syllogistic vs analogical, authoritative, or other modes. The claim that the Summa is an "uncompressed reasoning corpus" may apply fully only to the syllogistic portions.
- The Summa is in medieval Latin, which adds a significant parsing challenge regardless of structural markers.
- The corpus is relatively small by modern standards (~15-20MB of text). Whether this is sufficient for training even a small model is an empirical question.

## What Would Confirm It

- Building a parser that exploits the Summa's structural markers and successfully extracting syllogistic structure from a significant fraction of articles.
- Comparing training efficiency on the Summa vs an equivalent-size corpus of modern text (which hides its reasoning) and showing better reasoning performance from the Summa-trained model.
- Demonstrating that the objections (invalid arguments) serve as effective contrastive examples during training.

## What Would Refute It

- Finding that the Summa's structural markers do not reliably predict the logical structure within sections.
- Showing that the fraction of genuinely syllogistic reasoning in the Summa is too small (e.g., <20%) for the "uncompressed reasoning corpus" characterization to matter.
- Demonstrating that a model trained on a generic corpus reasons equally well, undermining the claim that the Summa's structure provides an advantage.

## Implications If True

- The eigenflower project has a significant training data advantage that most AI projects do not.
- The corpus selection is not just a constraint (small, domain-specific) but an active feature — the data's structure accelerates learning of reasoning patterns.
- Two-stage training (logical dynamics first, then theological content) becomes a natural approach exploiting the Summa's structure.
- Other similarly structured texts (e.g., scholastic disputations, Euclid's Elements) could augment the corpus with more uncompressed reasoning.

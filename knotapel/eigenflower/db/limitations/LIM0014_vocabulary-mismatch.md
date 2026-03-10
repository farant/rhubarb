---
id: LIM0014
title: "Vocabulary mismatch: DKC ~21-100 cyclotomic integers vs natural language 10,000+ words"
type: discovered
source: "notes/01-analysis.md"
impacts: []
related: [LIM0013, LIM0017, LIM0024]
---

DKC's "vocabulary" is 21-100 cyclotomic integers (the optimal z8 catalog). Natural language vocabulary is 10,000+ words. These are different by two orders of magnitude, and more importantly, they are different in KIND — cyclotomic integers have algebraic structure (multiplication, conjugation, norm) that words do not inherently possess.

From 01-analysis.md: "DKC's 'vocabulary' is 21-100 cyclotomic integers. Natural language vocabulary is 10,000+ words."

This is not just a size problem. The DKC mechanism works BECAUSE the cyclotomic integers have rich algebraic relationships — axis alignment, Galois orbits, norm structure. Words have semantic relationships, but those relationships are statistical and fuzzy, not algebraic and exact.

Implications:
- The fluency layer must handle the full natural language vocabulary. The syllogism layer can potentially work with a much smaller vocabulary of logical primitives (proposition types, quantifiers, connectives).
- The interface between the two layers is where the vocabulary compression happens — the fluency layer maps 10,000+ words onto a structured logical vocabulary.
- Word embeddings in the fluency layer are the bridge. The question is whether those embeddings can be given algebraic structure that connects to the syllogism layer's exact arithmetic.
- The "opaque tokens" proposal (from knotapel vision.md) attempts to solve this: tokens flow through compiled weights, the net reasons about logical structure not content, and substitution happens at output.

---
id: CON0063
title: "The bridge gap: DKC Boolean functions to natural language theological reasoning"
connects:
  - "DKC: Boolean computation over Z[zeta_8] with 3-5 inputs"
  - "Eigenflower goal: theological reasoning over Latin text with complex propositions"
type: cross_pollination
strength: speculative
source: "notes/01-analysis.md, Part 3 (The Gap)"
---

The central challenge for eigenflower is bridging DKC's Boolean-function world to natural language processing on theological text:

- DKC operates on Boolean functions with 3-5 inputs. Theological reasoning operates on propositions with complex internal structure.
- DKC's "vocabulary" is 21-100 cyclotomic integers. Natural language vocabulary is 10,000+ words.
- DKC's "activation" selects a Boolean output. Theological reasoning selects from a space of possible conclusions.
- DKC's proofs work because Z[zeta_8] has finite, exact, enumerable structure. Natural language does not.

The tensor logic framework (Domingos) is the most promising bridge: it treats propositions as Boolean tensors and syllogisms as einsum operations -- the same formalism DKC implements. But scaling from 3-input Boolean functions to propositional complexity of the Summa is nontrivial.

The Summa's advantage (noted in analysis): it is already an "uncompressed reasoning corpus" -- it doesn't hide its derivations like most text. The sed contra / respondeo structure provides natural parse boundaries for syllogism extraction.

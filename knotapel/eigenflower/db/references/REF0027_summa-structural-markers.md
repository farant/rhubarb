---
id: REF0027
title: "Summa Theologica Structural Markers"
type: corpus
source_project: external
summary: "The Summa's consistent structural markers (videtur quod, sed contra, respondeo dicendum quod, ad primum/secundum/tertium) across 3,000+ articles provide a massive head start for semantic parsing. Each marker signals a specific logical function in the argument structure."
related: []
---

## Description

The Summa Theologica has remarkably consistent structural markers across its 3,000+ articles:

- **"Videtur quod..."** (It seems that...) = objection = premise to be refuted
- **"Sed contra..."** (On the contrary...) = counter-authority
- **"Respondeo dicendum quod..."** (I answer that...) = the actual argument / thesis
- **"Ad primum/secundum/tertium..."** = replies to specific objections

These markers are consistent throughout the entire work. A parser that exploits them gets a massive structural head start -- the logical function of each section is explicitly marked by the text itself.

## Relevance to Eigenflower

- These markers are the interface protocol between the fluency layer and the syllogism layer
- The fluency layer can use these markers to segment articles into logical components
- Objections are deliberately WRONG syllogisms -- the graph could store them as such with error-type annotations
- The consistent structure means semi-automated syllogism extraction is feasible: fluency layer proposes, human verifies
- The Summa IS an "uncompressed reasoning corpus" (per Paper 1's SciencePedia insight) -- it doesn't hide its derivations

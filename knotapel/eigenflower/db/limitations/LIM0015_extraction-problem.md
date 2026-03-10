---
id: LIM0015
title: "The extraction problem — parsing syllogisms FROM Latin prose is hard"
type: discovered
source: "notes/01-analysis.md"
impacts: []
related: [LIM0004, LIM0013]
---

Everyone in the paper analysis talks about compiling syllogisms TO weights, but actually parsing syllogisms FROM the Summa's Latin prose is its own hard problem. The Summa is structured (sed contra, respondeo, etc.) but the actual logical structure within each article is implicit.

From 01-analysis.md: "No one addressed the extraction problem concretely. Everyone talks about compiling syllogisms TO weights, but actually parsing syllogisms FROM the Summa's Latin prose is its own hard problem."

The Summa provides structural markers that help:
- "Videtur quod..." (It seems that...) = objection = premise to be refuted
- "Sed contra..." (On the contrary...) = counter-authority
- "Respondeo dicendum quod..." (I answer that...) = the actual argument
- "Ad primum/secundum/tertium..." = replies to objections

These markers are consistent across ~3,000 articles. But within each section, the actual syllogistic structure is embedded in natural language prose. Identifying the major premise, minor premise, middle term, and conclusion requires deep parsing of Latin sentence structure.

Implications:
- Phase 2 of the build order (fluency layer + structural parser) is where this gets addressed.
- The extraction problem is a major motivation for the hybrid architecture — the fluency layer's primary job is solving extraction.
- Semi-automated extraction (fluency layer proposes, human verifies) is the realistic near-term approach.
- Manual compilation of a starter set (~50-100 clearly formal syllogisms) is needed to bootstrap the system before automated extraction is viable.
- The rhubarb project's existing parser infrastructure (arbor2 GLR parser, lapifex parser generator) could potentially be adapted for Latin structural parsing.

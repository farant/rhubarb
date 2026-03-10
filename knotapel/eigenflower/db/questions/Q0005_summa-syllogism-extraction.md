---
id: Q0005
title: "Can syllogisms be parsed automatically from the Summa's Latin prose?"
status: open
source: "notes/01-analysis.md"
related: []
---

Everyone talks about compiling syllogisms TO weights, but extracting syllogisms FROM the Summa's Latin prose is its own hard problem. The Summa is structured (videtur quod, sed contra, respondeo dicendum quod, ad primum/secundum/tertium) and these markers are consistent across 3,000+ articles. But the actual logical structure within each article is implicit — the syllogistic form is embedded in natural language, not stated formally. The question is: what fraction of articles can have their syllogistic structure extracted by a parser that exploits these structural markers? Is it 80% with high confidence, or 30% with many ambiguities? This determines how much human curation the initial syllogism graph needs.

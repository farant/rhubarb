---
id: G0011
title: "Build structural parser for the Summa"
status: open
priority: medium
depends_on: [G0010]
blocks: [G0015]
related: []
---

## Description

Build a parser that exploits the Summa Theologica's highly consistent structural markers to extract proposition structure from articles. The Summa uses regular patterns across 3000+ articles:

- "Videtur quod..." (It seems that...) = objection = premise to be refuted
- "Sed contra..." (On the contrary...) = counter-authority
- "Respondeo dicendum quod..." (I answer that...) = the main argument
- "Ad primum/secundum/tertium..." = replies to specific objections

The parser combines the fluency layer's language understanding with deterministic structural pattern matching to achieve semi-automated syllogism extraction: the system proposes logical structure, a human verifies.

## Why It Matters

The 01-analysis.md document identifies a critical gap: "No one addressed the extraction problem concretely. Everyone talks about compiling syllogisms TO weights, but actually parsing syllogisms FROM the Summa's Latin prose is its own hard problem." This goal addresses that gap.

The Summa's consistent structure is a massive advantage — these markers are essentially an API for the text's logical organization. A parser that exploits them gets a head start that would not be available for less structured texts. But the actual logical structure WITHIN each article section is still implicit and requires language understanding to extract.

This is the bridge between the fluency layer and the syllogism layer: it turns text into logical form that can be compiled.

## What "Achieved" Looks Like

- Parser correctly identifies structural sections (objections, sed contra, respondeo, replies) across Summa articles
- Within each section, extracts candidate propositions and their logical relationships
- Semi-automated workflow: system proposes, human reviews and corrects
- Accuracy measured against the manually compiled syllogisms (G0008)
- Coverage analysis: what fraction of articles can be parsed into logical structure?
- Edge cases documented (analogical arguments, arguments from authority, etc.)

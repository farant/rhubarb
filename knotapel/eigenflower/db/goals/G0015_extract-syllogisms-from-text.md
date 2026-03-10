---
id: G0015
title: "Extract syllogisms from natural language"
status: open
priority: high
depends_on: [G0010, G0011]
blocks: [G0016]
related: []
---

## Description

Build the automated pipeline for extracting syllogistic structure from natural language text — primarily from the Summa Theologica, but designed to work on any sufficiently structured theological/philosophical Latin or English text.

This is the reverse of compilation (G0005): instead of logic -> weights, this is text -> logic. The fluency layer must:
- Parse Latin prose into propositions
- Identify logical relationships between propositions
- Recognize syllogistic patterns (which figure, which mood)
- Handle the Summa's structural markers as scaffolding
- Distinguish syllogistic reasoning from analogical, authoritative, and other argument forms

## Why It Matters

The 01-analysis.md identifies this as a critical gap: "actually parsing syllogisms FROM the Summa's Latin prose is its own hard problem. The Summa is structured (sed contra, respondeo, etc.) but the actual logical structure within each article is implicit."

Without automated extraction, the syllogism graph must be built entirely by hand (G0008). Manual compilation is necessary for validation but does not scale to the full Summa (~3000 articles). Automated extraction, even semi-automated with human verification, is required to build a comprehensive reasoning graph.

This goal also enables dynamic learning: the system can ingest new theological text and extend its reasoning graph without manual intervention (feeding into G0016).

## What "Achieved" Looks Like

- Pipeline takes Summa article text as input, outputs candidate syllogisms in logical form
- Correctly extracts syllogisms that match the manually compiled ground truth (G0008)
- Precision and recall measured against the manual compilation
- Semi-automated workflow: system extracts, human reviews, corrections feed back
- Handles at least the clear syllogistic cases; explicitly flags non-syllogistic arguments
- Works on both Latin and English Summa text

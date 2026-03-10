---
id: G0003
title: "Curate small, high-complexity training corpus"
status: in_progress
priority: high
depends_on: [G0026, G0027]
blocks: [G0010]
related: []
---

## Description

Build and validate the training corpus: Bible in multiple languages (Douay-Rheims English, Latin Vulgate, Greek Septuagint) plus Summa Theologica in Latin and English. The corpus is deliberately small (~20-30MB total) but high in structural and linguistic complexity.

## Why It Matters

The Edge of Chaos paper's key insight applies here: data COMPLEXITY matters more than data QUANTITY. The Bible and Summa are among the most structurally rich texts in existence — multilingual, highly cross-referenced, with dense logical argumentation (Summa) and rich narrative/poetic/prophetic variety (Bible). This is the opposite of scraping the internet for volume.

The Inverse Knowledge Search paper adds that the Summa is already an "uncompressed reasoning corpus" — it does not hide its derivations. Every article explicitly states objections, counter-arguments, the main argument, and replies. This makes it exceptionally high-quality training data for a reasoning system.

The corpus also defines the vocabulary and linguistic scope of the fluency layer. Everything the system can parse, understand, and generate is bounded by what it has seen in training.

## What "Achieved" Looks Like

- Complete Bible texts in English, Latin, and Greek — cleaned and validated
- Complete Summa Theologica in English — all parts present and audited for completeness
- Complete Summa Theologica in Latin — acquired and validated (currently missing)
- Secunda Secundae audited for completeness (currently flagged as potentially truncated at 708KB)
- Corpus organized for efficient data loading during training
- Total size documented and verified against training time budget

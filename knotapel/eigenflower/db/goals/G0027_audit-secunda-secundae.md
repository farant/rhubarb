---
id: G0027
title: "Audit Secunda Secundae corpus for completeness"
status: open
priority: low
depends_on: []
blocks: [G0003]
related: []
---

## Description

The CLAUDE.md flags a potential issue: "Secunda Secundae audit (708KB seems small)." The Secunda Secundae is the longest part of the Summa Theologica (189 questions), so 708KB may indicate the file is truncated or incomplete. This needs to be verified by checking article counts and comparing against a known-complete source.

## Why It Matters

An incomplete corpus produces a model with gaps. If the Secunda Secundae is truncated, the fluency layer will have blind spots in its coverage of moral theology — the very area where the Summa is most detailed and most practically relevant. The syllogism graph will also be missing potentially hundreds of extractable arguments.

This is a quick audit task but has outsized impact if the file is indeed incomplete — better to catch it now than to train a model and discover gaps later.

## What "Achieved" Looks Like

- Article count in current file verified against known-complete Secunda Secundae (189 questions, 918 articles)
- If truncated: complete text obtained and file replaced
- If complete: documented as verified, anomalous file size explained (perhaps different formatting)
- Corpus inventory updated with verified sizes

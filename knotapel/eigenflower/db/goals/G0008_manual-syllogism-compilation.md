---
id: G0008
title: "Manually compile 50-100 syllogisms from the Summa"
status: open
priority: medium
depends_on: [G0004, G0005]
blocks: [G0006, G0007]
related: []
---

## Description

Select 50-100 clearly formal syllogisms from the Summa Theologica, manually extract their logical structure (premises, middle terms, conclusions, syllogistic figure), and compile them into tensor equations using the tensor logic interpreter. Verify correctness: does forward chaining from the stated premises produce the stated conclusions?

This is Phase 1 Step 2 from the architecture document: start with hand-compiled syllogisms before attempting automated extraction.

## Why It Matters

This is the ground-truth validation step. Before building automated extraction (G0015) or kernel regression (G0006), you need a verified dataset of correctly compiled syllogisms. This dataset serves multiple purposes:

- Validates the tensor logic interpreter on real theological content (not toy examples)
- Provides training data for the kernel ridge regression operators
- Reveals the gap between the Summa's actual argumentation and the idealized syllogistic forms
- Seeds the syllogism graph with enough structure for meaningful forward/backward chaining tests
- Creates the test cases that all downstream components must pass

The manual compilation process will also surface practical questions: how much of the Summa is actually syllogistic? How complex are the real logical structures compared to textbook Barbara?

## What "Achieved" Looks Like

- 50-100 syllogisms from across the Summa (Prima Pars, Prima Secundae, etc.) extracted
- Each syllogism documented: source article, Latin text, logical form, syllogistic figure
- All compile successfully through the tensor logic interpreter
- Forward chaining produces the correct conclusions
- Coverage analysis: what percentage of Summa articles contain extractable syllogisms?
- Edge cases and difficulties documented for informing the automated extraction design

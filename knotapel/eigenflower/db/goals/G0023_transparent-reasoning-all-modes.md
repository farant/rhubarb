---
id: G0023
title: "Transparent reasoning across all modes"
status: open
priority: medium
depends_on: [G0019, G0020, G0021, G0022]
blocks: []
related: []
---

## Description

The capstone goal: transparent reasoning across ALL modes that Aquinas uses — deduction, analogy, argument from authority, reductio ad absurdum, and distinguo. Each mode has its own architectural component with its own transparency guarantees, and the system can seamlessly combine them when narrating a complex argument.

This is the full realization of the vision stated in CLAUDE.md: "Eventually: transparent reasoning across all modes (deduction, analogy, authority, reductio, distinguo)."

## Why It Matters

The Summa Theologica does not reason in a single mode. A typical article uses multiple modes:
- Objections (often syllogistic, sometimes authoritative)
- Sed contra (authoritative)
- Respondeo (mixed: syllogistic core, analogical illustrations, distinguo for precision)
- Replies (reductio, distinguo, counter-authority)

A system that handles only deduction covers a fraction of the Summa's argumentation. Full coverage requires all five modes working together, with clear boundaries between them and explicit transparency about which mode is in use at each reasoning step.

This goal represents the long-term vision — the system that can engage with the full depth and breadth of Thomistic theology, not just its formal deductive core.

## What "Achieved" Looks Like

- All five reasoning modes operational and integrated
- Complex arguments using multiple modes in sequence are handled correctly
- Derivation narration identifies which mode is used at each step
- Transparency maintained across mode transitions (no black-box gaps between modes)
- The system can process a complete Summa article (objections, sed contra, respondeo, replies) end to end
- Correctness validated against expert theological interpretation of selected articles
- The boundaries of each mode's transparency are documented (deduction = fully transparent, analogy = bounded opacity, etc.)

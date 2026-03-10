---
id: G0022
title: "Build transparent distinguo (sense disambiguation)"
status: open
priority: low
depends_on: [G0018, G0007]
blocks: [G0023]
related: []
---

## Description

Design and implement the "distinguo" reasoning mode — distinguishing senses of a term. "X is true in sense A, false in sense B." This is namespace resolution: the same word maps to different proposition nodes depending on sense.

Aquinas uses distinguo constantly. Many theological disputes dissolve when terms are carefully distinguished: "being" in the sense of essence vs existence, "good" in the sense of moral vs ontological, "cause" in the sense of efficient vs final.

The mechanism: proposition nodes are qualified by sense. The same surface term "X" can have multiple proposition nodes (X-in-sense-A, X-in-sense-B). The syllogism graph tracks which sense is in play at each step.

## Why It Matters

Distinguo is critical for handling the Summa's argumentation correctly. Many of Aquinas's objections work by equivocating on a term (using it in two senses), and the reply works by distinguishing the senses. A system that cannot distinguish senses will produce incorrect derivations from equivocal premises.

This reasoning mode is also directly relevant to natural language processing — polysemy (multiple senses of a word) is a fundamental challenge. The fluency layer must identify which sense is in play from context, and the syllogism layer must maintain separate proposition nodes for distinct senses.

The transparency is high: the sense distinction is explicit in the graph structure, and derivation narration can report exactly which sense of a term was used at each step.

## What "Achieved" Looks Like

- Proposition nodes qualified by sense (term + sense = unique node)
- Fluency layer can identify sense from context (at least for common theological distinctions)
- Syllogism graph correctly prevents cross-sense reasoning (no equivocation)
- Test cases from Summa articles where distinguo resolves apparent contradictions
- Derivation narration explicitly names the sense used at each step
- Common Thomistic distinctions catalogued (being qua essence/existence, good qua moral/ontological, etc.)

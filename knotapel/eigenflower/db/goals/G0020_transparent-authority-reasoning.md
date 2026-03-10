---
id: G0020
title: "Build transparent argument-from-authority reasoning"
status: open
priority: low
depends_on: [G0018]
blocks: [G0023]
related: []
---

## Description

Design and implement an architectural component for argument from authority — a reasoning mode Aquinas uses extensively and considers legitimate with specific weight depending on the source.

Aquinas's authority hierarchy:
- Scripture (highest weight)
- Church Fathers
- Philosophers (Aristotle especially)
- Other authorities

This is essentially a weighted citation graph. The logical structure: "Authority X says Y, therefore Y has weight W" — where W depends on the authority's position in the hierarchy.

The "sed contra" section of every Summa article is typically an argument from authority, so this mode of reasoning is structurally marked and frequent.

## Why It Matters

Argument from authority is one of Aquinas's five reasoning modes identified in the 03-transparency document. It has a naturally transparent structure — the citation graph can be made explicit, the weights can be assigned according to Aquinas's own hierarchy, and the reasoning is traceable (you can always see which authority is being cited and what weight it carries).

This reasoning mode is also interesting because it is explicitly NON-deductive — an argument from authority does not prove its conclusion, it provides evidence proportional to the authority's reliability. The system needs to represent this distinction clearly.

## What "Achieved" Looks Like

- Citation graph with authority nodes and weight assignments
- Authority hierarchy implemented per Aquinas's own categories
- "Sed contra" sections parsed and authority arguments extracted
- Clear distinction between authoritative evidence and deductive proof in system output
- Integration with the syllogism graph (authority edges as a distinct type)
- Weight propagation: how does authority evidence combine with deductive reasoning?

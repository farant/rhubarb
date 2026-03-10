---
id: REF0022
title: "DKC Demo 102 — Barrington-Radical Principle (PROVEN)"
type: dkc_theorem
source_project: knotapel
demo_number: 102
summary: "The radical (nilpotent component) carries only abelian writhe character data, provably useless for parity by Barrington's theorem. The radical-as-resource hypothesis was REFUTED. Only the semisimple quotient contributes to DKC computation."
related: []
---

## Description

Demo 102 proved the Barrington-Radical Principle: the radical of the TL algebra at delta=0 carries only abelian (writhe) information and is provably useless for parity computation. This refuted the radical-as-resource hypothesis and established that only the semisimple quotient contributes to Boolean computation.

The connection to Barrington's theorem: solvable groups cannot compute parity. The radical generates solvable (nilpotent) structure, therefore it cannot contribute to parity -- regardless of depth or catalog size.

## Relevance to Eigenflower

- The Barrington-Radical Principle constrains the Eigenflower Stack: only {-1, +1} entries (not zeros) can contribute to parity-like computation
- Predicts that DEQ models (which converge to fixed points, i.e. solvable dynamics) will fail on hard reasoning tasks
- The nilpotent/semisimple decomposition may be relevant to understanding which components of a trained network carry reasoning capability vs noise
- For the syllogism layer: the "radical" (noisy, approximate) components of the representation are useless for exact logical inference

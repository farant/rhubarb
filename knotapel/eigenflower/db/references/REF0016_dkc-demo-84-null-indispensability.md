---
id: REF0016
title: "DKC Demo 84 — Null State Indispensability"
type: dkc_demo
source_project: knotapel
demo_number: 84
summary: "Bracket-null entries (37.5% of z8 catalog, Re(q)=0) are computationally indispensable at finite groups. Removing them drops XOR capacity from XOR8 to XOR6. They cover 6 S2 directions absent from all non-null entries."
related: []
---

## Description

Demo 84 established that bracket-null entries (those with vanishing scalar trace, Re(q)=0) constitute 37.5% of the z8 catalog and are computationally indispensable. Removing them drops XOR capacity below even a random-removal baseline. These null entries are "invisible" to the scalar bracket but carry directional information (specific eigenvector directions on S2) essential for parity computation. They cover 6 S2 directions absent from all non-null entries.

## Relevance to Eigenflower

- Analogous to BitNet's zero-weight feature filtering: trace-invisible states carry structural information essential for computation
- The regime transition (nulls critical at finite groups, dispensable at infinite groups per D87) parallels BitNet's finding that zero-weight fraction should decrease as model size increases
- Informs weight initialization strategy: seed zero weights at positions providing maximal "directional coverage" rather than random
- The two-role direction theorem (D88: 4 rigid anchors + 6 flexible scaffolding) extends this insight

---
id: REF0023
title: "DKC Demo 103 — BFS Growth as Braid Group Invariant"
type: dkc_demo
source_project: knotapel
demo_number: 103
summary: "BFS catalog growth rate ~(n-1)x per round is an invariant of the braid group B_n, independent of module dimension. Confirmed across 4 data points (2-strand through 5-strand)."
related: []
---

## Description

Demos 99-103 confirmed that the BFS closure growth rate is a braid group invariant: approximately (n-1)x per round where n is the strand count, regardless of the TL module dimension being used. This was verified across 2-strand (growth ~1x/round, finite), 3-strand (~2x/round), 4-strand (~3x/round), and 5-strand (~4x/round) braids.

## Relevance to Eigenflower

- The growth rate determines how quickly the catalog (reservoir) expands with depth -- relevant to storage and computation budgets
- The invariant nature (independent of module dimension) means catalog size is predictable from strand count alone
- Parallels Paper 2's (Early Experience) branching factor K for data amplification
- The growth rate ~(n-1)x is a Krylov-like growth rate for the operator algebra -- potentially the right complexity measure for algebraic reservoirs

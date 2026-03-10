---
id: REF0010
title: "DKC Demo 93 — Circuit Complexity Hierarchy (LANDMARK)"
type: dkc_demo
source_project: knotapel
demo_number: 93
summary: "AND/XOR ratio goes from 1.01 at N=3 to 2762 at N=7 to infinity at N>=8. Complement-blindness theorem: combined_cell maps m<->~m to same cell. Phase_cell recovers all 13 NPN classes. Hierarchy is universal across group structures."
related: []
---

## Description

Demo 93 established the circuit complexity hierarchy for DKC under 1-weight-per-input encoding. Key results:
- AND/XOR ratio: 1.01 (N=3) -> 2762 (N=7) -> infinity (N>=8) -- easy functions become combinatorially overwhelming relative to hard functions as input size grows
- Complement-blindness theorem (PROVED): combined_cell maps complementary masks m and ~m to the same cell, structurally preventing discrimination between them
- Phase_cell activation recovers all 13 NPN classes by breaking the complement symmetry
- The hierarchy is universal across z8, 2I, and z12 group structures -- it is a property of the readout mechanism, not the weight algebra

## Relevance to Eigenflower

- The NPN classification forms a natural curriculum with intrinsic difficulty ordering -- AC^0 functions (AND, OR) are "introductory," TC^0 (MAJ) is "intermediate," functions outside AC^0 (parity/XOR) are "advanced"
- Provides the quantitative "difficulty metric" for reasoning tasks
- Without encoding constraints, a network would be drawn to exponentially more abundant easy solutions -- relevant to Eigenflower's architecture choices for avoiding shallow pattern-matching
- The complement-blindness theorem constrains what activation functions can express -- relevant to readout/narration layer design

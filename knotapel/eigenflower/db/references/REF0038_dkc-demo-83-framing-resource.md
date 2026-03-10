---
id: REF0038
title: "DKC Demo 83 — Framing = +2 XOR Levels"
type: dkc_demo
source_project: knotapel
demo_number: 83
summary: "Jones normalization (framing / writhe correction) destroys exactly 2 XOR levels at every root of unity tested. Writhe, depth, and lattice are additive independent channels."
related: []
---

## Description

Demo 83 demonstrated that Jones normalization (the writhe correction that makes the bracket a true knot invariant rather than just a link invariant) costs exactly 2 XOR levels at every root of unity tested. This is a precise quantitative cost: framing information is a computational resource worth exactly 2 XOR levels.

The finding also established that writhe, depth, and lattice are additive independent channels -- they contribute independently to computational capacity without interaction.

## Relevance to Eigenflower

- The channel independence supports the dual-channel framework (D108-109): independent channels can be analyzed and optimized separately
- Relevant to the reservoir computing interpretation: independent reservoir dimensions correspond to independent computational resources
- The precise "+2" quantification demonstrates the kind of exact, testable predictions DKC enables

---
id: REF0037
title: "DKC Demos 79-80 — Finite/Infinite Group Boundary"
type: dkc_demo
source_project: knotapel
demo_number: 80
summary: "The finite/infinite group transition is the DKC edge-of-chaos phase boundary. zeta_8 (finite group, 24 elements, E7) caps at XOR8. zeta_12 (infinite group) breaks through to XOR12+. The binary octahedral group (E7) is the last finite subgroup before the transition. ADE classification governs."
related: []
---

## Description

Demos 79-80 identified the finite/infinite group boundary as a sharp phase transition in DKC computational capacity. At zeta_8, the binary octahedral group (order 48, ADE type E7) caps the catalog at 24 quaternion states, limiting XOR capacity. At zeta_12, the group becomes infinite, breaking through to XOR12+ with no hard ceiling.

This maps exactly to the Edge-of-Chaos trichotomy:
- Ordered (Class I/II): zeta_4 -- binary dihedral, 8 elements, trivial XOR
- Edge of chaos (Class IV): zeta_8/zeta_12 -- structured but growing
- No DKC analog for chaotic (Class III) -- algebraic structure prevents true chaos

## Relevance to Eigenflower

- The phase transition identifies the "sweet spot" for reservoir capacity in DKC-inspired architectures
- The ADE classification provides a principled hierarchy of algebraic reservoirs
- Relevant to choosing the cyclotomic ring for Eigenflower's reasoning layer: richer rings provide more capacity but require more resources
- The ordered/edge-of-chaos correspondence validates DKC's reservoir computing interpretation

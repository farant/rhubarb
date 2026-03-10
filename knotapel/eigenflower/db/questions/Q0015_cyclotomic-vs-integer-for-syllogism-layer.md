---
id: Q0015
title: "Is the DKC cyclotomic lattice the right substrate for the syllogism layer, or is plain integer arithmetic sufficient?"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The DKC connection is mathematically beautiful, and cyclotomic arithmetic gives exact algebraic structure with proven computational properties. But propositional logic may not need Z[zeta_8]'s algebraic richness — classical syllogistic logic operates on four categorical propositions (A, E, I, O) with a small number of valid forms. Plain integer arithmetic over truth values might be entirely sufficient for the syllogism layer, and would be simpler to implement, debug, and reason about. The question is: does Z[zeta_8] provide concrete advantages for the syllogism layer (e.g., encoding certainty grades, handling analogy, representing more than Boolean propositions), or is it over-engineering for what is fundamentally propositional logic?

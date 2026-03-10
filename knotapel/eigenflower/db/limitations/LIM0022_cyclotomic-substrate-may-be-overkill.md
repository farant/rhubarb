---
id: LIM0022
title: "Cyclotomic substrate may be over-engineering for propositional logic"
type: discovered
source: "notes/02-architecture-insight.md"
impacts: []
related: [LIM0013, LIM0014]
---

DKC uses Z[zeta_8] (cyclotomic integers) because the algebraic richness — axis alignment, Galois orbits, norm structure — is essential for encoding Boolean functions via topology. But propositional logic may not need this richness. Plain integer arithmetic might be sufficient for the syllogism layer.

From 02-architecture-insight.md: "Is the DKC cyclotomic lattice the right substrate for the syllogism layer, or is plain integer arithmetic sufficient? The DKC connection is beautiful but may be over-engineering for propositional logic that doesn't need Z[zeta_8]'s algebraic richness."

The tensor logic equation SP[s,p] = H(SM[s,m] * MP[m,p]) works with ordinary matrices over any field. You don't need cyclotomic integers for matrix multiplication and step functions. The DKC machinery is powerful but possibly unnecessary overhead.

Arguments for cyclotomic substrate:
- Unifies the syllogism layer with DKC's existing codebase and proofs.
- May enable deeper connections (e.g., Galois orbits corresponding to logical symmetries).
- Exact arithmetic is guaranteed (no floating point issues).
- The GPTQ-as-CVP framework on cyclotomic lattices could enable weight compilation.

Arguments against:
- Simpler is better if it works. Integer matrices are faster and easier to implement.
- The algebraic richness of Z[zeta_8] may never be exercised by propositional logic operations.
- Over-engineering early creates complexity debt that slows iteration.

Implications:
- This is a design decision (DEC) to be made early, not a hard constraint.
- Could start with integer arithmetic for the syllogism layer and migrate to cyclotomic if the richer structure proves necessary.
- The decision should be informed by experiments: does cyclotomic arithmetic give better results than integer arithmetic for the same logical operations?

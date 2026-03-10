---
id: HYP0005
title: "Algebraic complexity (derived series depth, solvability) predicts neural computation better than dynamical complexity (topological entropy)"
status: proposed
source: "notes/01-analysis.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

For algebraic reservoirs (DKC-type systems where the computational substrate is an algebraic structure), the correct measure of computational capacity is algebraic complexity — specifically, derived series depth and group solvability — NOT dynamical complexity (topological entropy, Lyapunov exponents, edge-of-chaos measures).

D106 falsifies the naive application of the "Edge of Chaos" thesis to algebraic reservoirs: topological entropy is completely orthogonal to computational capability. A group with high topological entropy may have zero computational utility if it is solvable (Barrington's theorem: solvable groups cannot compute parity). Conversely, a group with low topological entropy may have high computational power if it has deep derived series.

This refines (not refutes) the Edge of Chaos thesis: the thesis is correct for continuous dynamical systems where "complexity" correlates with computational capacity, but the relevant notion of "complexity" in algebraic systems is algebraic (derived series), not dynamical (entropy).

## Evidence For

- **D106:** Directly demonstrates that topological entropy fails as a predictor for algebraic reservoirs. Solvable groups with high entropy cannot compute parity; non-solvable groups with modest entropy can.
- **Barrington's theorem:** Provides the theoretical foundation — the algebraic property of non-solvability (equivalently, derived series depth >= some threshold) is what separates groups that can compute all Boolean functions from those that cannot.
- **DKC depth law (D82):** max_xor ~ depth + 6. Depth (algebraic complexity) IS the predictor.
- **D50 parity reachability at k=6 sectors:** The activation (which partitions the algebraic space) determines capability, not the dynamical properties of the system.

## Evidence Against

- This has been demonstrated only within the DKC framework. It may not generalize to other types of algebraic reservoirs.
- The Edge of Chaos thesis is well-established in the reservoir computing literature. Claiming it is wrong (even in a refined sense) requires strong evidence beyond one project's results.
- No peer review or external validation of this claim yet.

## What Would Confirm It

- Testing additional algebraic groups (beyond Z[zeta_8]) and showing that derived series depth predicts computational capacity while topological entropy does not.
- A formal proof connecting Barrington's theorem to reservoir computing capacity in a way that shows algebraic complexity is the natural measure.
- Reproduction by an independent group.
- This is identified as a publishable result in the analysis ("Algebraic vs dynamical complexity for reservoir computing").

## What Would Refute It

- Finding an algebraic reservoir where topological entropy IS a good predictor of computational capacity.
- Showing that derived series depth fails to predict capacity for a class of algebraic groups where entropy succeeds.
- Demonstrating that the DKC results are an artifact of the specific Z[zeta_8] lattice and don't generalize.

## Implications If True

- The Edge of Chaos literature needs revision for algebraic systems.
- The correct measure for eigenflower's reasoning capacity is the algebraic structure of whatever substrate it uses, not dynamical properties.
- This provides a design principle: choose algebraic substrates with deep derived series for maximum computational power.
- Potentially publishable as an independent result refining the Edge of Chaos thesis.

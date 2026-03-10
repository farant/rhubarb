---
id: REF0025
title: "DKC Five-Pillar Synthesis — Discrete Algebraic Reservoir Computer"
type: dkc_proof
source_project: knotapel
summary: "DKC formalized as a reservoir computer connecting five published pillars: Habiro integrality (exact arithmetic), Nazer-Gastpar lattice codes (lattice structure), Abramsky categorical computation (composition), Aizenberg MVN activation (nonlinear readout), and TL non-semisimplicity (algebraic structure). Nobody in any pillar field cites the other four."
related: []
---

## Description

The five-pillar synthesis connects DKC to five independent published research traditions:

1. **Habiro integrality** -- cyclotomic integers are exact, not approximations
2. **Nazer-Gastpar lattice codes** -- Z[zeta_8] is a lattice code with algebraic structure
3. **Abramsky categorical computation** -- braid group representations compose categorically
4. **Aizenberg MVN activation** -- multi-valued neuron activation provides nonlinear readout
5. **TL non-semisimplicity** -- the radical/semisimple decomposition determines what the algebra can compute

The synthesis identifies DKC as a "discrete algebraic reservoir computer" where:
- Fixed reservoir = Z[zeta_8] bracket catalog
- Reservoir connectivity = Cayley graph of the binary octahedral group
- Linear readout = signed weight sum
- Nonlinear output = k-sector / Voronoi activation
- Memory depth = BFS crossing depth

The four-community intersection (no paper in any pillar field cites the other three) is the basis for DKC's novelty claim.

## Relevance to Eigenflower

- Provides the theoretical framework for WHY an algebraic reasoning layer should work
- Each pillar maps to a Group B paper's compression axis (spatial, representational, structural)
- The reservoir computing interpretation directly informs the hybrid architecture: exact algebraic reservoir (syllogism layer) + trained readout (fluency layer)
- The five pillars provide testable predictions for Eigenflower's stack

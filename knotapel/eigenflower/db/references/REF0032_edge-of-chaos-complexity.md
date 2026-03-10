---
id: REF0032
title: "Edge-of-Chaos Complexity for Representations (Zhang et al.)"
type: other
source_project: external
summary: "GPT-2 models trained on Elementary Cellular Automata data at intermediate complexity (Wolfram Class IV, edge of chaos) develop best general-purpose representations. Data complexity > data quantity. Models learn complex solutions even when simple ones exist. Works even at 67k parameters."
related: []
---

## Description

Zhang et al. (Group D Paper 3) show that models trained on intermediate-complexity data (edge of chaos) develop superior general-purpose representations. Key findings:

- Wolfram Class IV rules (edge of chaos, Turing-complete) produce the best pretrained representations
- Data complexity matters more than data quantity
- Models learn complex solutions even when simple ones exist (they "over-learn")
- The effect holds even at 67k parameters
- Reservoir computing connection: ECA-pretrained transformers resemble reservoir computers

## Relevance to Eigenflower

- DKC occupies the edge of chaos quantitatively: zeta_8 (finite group = ordered) -> zeta_12 (infinite group = edge of chaos)
- D106 FALSIFIES naive dynamical complexity measures for algebraic systems -- topological entropy is orthogonal to DKC computation
- The correct complexity is ALGEBRAIC (derived series depth, solvability, BFS depth) not DYNAMICAL (Lyapunov, entropy)
- This is a genuine refinement of the edge-of-chaos thesis, potentially publishable
- DKC goes further than 67k params: ZERO parameters suffice when algebraic structure is rich enough
- Validates DKC's reservoir computing approach: fixed algebraic reservoir + trained readout
- Krylov complexity (operator growth) may be the right measure for algebraic reservoirs where topological entropy fails

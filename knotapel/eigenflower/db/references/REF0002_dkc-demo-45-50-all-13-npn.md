---
id: REF0002
title: "DKC Demos 45-50 — All 13 NPN Classes Computable"
type: dkc_demo
source_project: knotapel
demo_number: 50
summary: "All 13 NPN equivalence classes of 3-input Boolean functions are computable via MVN activation. Parity reachable at k=6 sectors (906 solutions). Four-tier hierarchy: k=2(5), k=3(+1), k=4(+6), k=6(+1=all 13)."
related: []
---

## Description

Demos 45-50 established the completeness of DKC for Boolean computation. Demo 45 introduced the complex function zoo with MVN activation. Demo 48 ran the 100M-quartet exhaustive search proving XNOR3 has ZERO solutions under split-sigmoid. Demo 50 discovered that the parity wall was in the activation function, not the lattice -- switching to k=6 MVN sectors unlocked parity (906 solutions).

Key findings:
- Split-sigmoid: 0 parity solutions out of 100M quartets (D48)
- k-sector MVN at k=6: 906 parity solutions (D50)
- Non-monotonic: 906@k=6 > 756@k=7 > 96@k=8 (incommensurability with pi/4 lattice)
- 41/100 catalog values participate in parity solutions, restricted to octants {2,4,5,7}

## Relevance to Eigenflower

- The four-tier NPN hierarchy provides a natural "curriculum scaffolding" for computational capability -- analogous to the circuit complexity hierarchy's role in reasoning difficulty
- The sector count k is a scaling axis analogous to the Free Transformer's kappa parameter and CALM's K parameter -- controlling information rate through the activation channel
- The discovery that activation determines computability maps directly to Eigenflower's insight that narration strategy over pre-compiled knowledge determines output quality
- Temperature = sector count mapping: Paper 1 (tensor logic) temperature T maps to DKC's k

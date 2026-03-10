---
id: Q0001
title: "Does Barbara compile to a working tensor equation in exact Z[zeta_8] arithmetic?"
status: open
source: "notes/01-analysis.md"
related: []
---

The Domingos tensor logic paper gives Barbara as SP[s,p] = H(SM[s,m] * MP[m,p]) — a matrix multiply plus step function. This is the foundational claim that syllogisms compile directly to ANN operations. The question is whether this works when instantiated with exact cyclotomic arithmetic (Z[zeta_8]) rather than floating point. DKC already does exact tensor computations over Z[zeta_8], so the substrate exists. But syllogistic propositions (universal affirmative, particular negative, etc.) have different truth-value structure than Boolean functions — the tensor dimensions and the step function H may need adaptation. Answering this would validate the entire build order's Phase 1 starting point.

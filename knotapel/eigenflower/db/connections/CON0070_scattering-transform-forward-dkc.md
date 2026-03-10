---
id: CON0070
title: "Hilbert space scattering transforms requiring no training = forward DKC requiring no training"
connects:
  - "Kiruluta et al.: cascading wavelet transforms with modulus nonlinearities, no learned parameters"
  - "DKC forward computation: bracket catalog from topology with no training (D29)"
type: mathematical_identity
strength: strong
source: "extractions/group-d-knotapel-relevance.md, Connection 2 (scattering)"
---

Paper 2's scattering transforms cascade wavelets with modulus nonlinearities to produce stable features without learned parameters. Forward DKC is exactly this:

- The "wavelet" is the Yang-Baxter R-matrix (braid crossing operator)
- The "cascading" is sequential application of crossing generators
- The "modulus nonlinearity" is the bracket trace closure
- The output features (Z[zeta_8] values) are translation-invariant (under Reidemeister moves, D09) and deformation-stable (robustness plateau at ~50 degrees, D97)

Both systems produce rich computational features from fixed algebraic operations without any learned parameters. The stability properties match: scattering transforms are Lipschitz-continuous to deformations; DKC features have a measured robustness plateau.

The 82.8% universal automaton determinism (D73) quantifies how much information the "scattering" (braid -> bracket) retains: it preserves 82.8% of the full braid information through the S^2 Voronoi projection.

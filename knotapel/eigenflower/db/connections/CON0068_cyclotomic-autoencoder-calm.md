---
id: CON0068
title: "Cyclotomic autoencoder: CALM architecture with braid generators as input, Z[zeta_8] as latent"
connects:
  - "CALM VAE autoencoder: K tokens compressed to continuous vector z"
  - "DKC braid word -> Z[zeta_8] bracket value as exact autoencoding"
type: technique_transfer
strength: moderate
source: "extractions/group-b-knotapel-relevance.md, Highest-Value Opportunities #2"
---

Train a CALM-style autoencoder where input is K braid generators and latent vector is a Z[zeta_8] value. The decoder reconstructs generators from the cyclotomic integer. Use exact bracket computation as ground-truth training signal.

The cyclotomic ring arithmetic (cyc8_add, cyc8_mul, cyc8_to_cx) IS already a complete autoencoder: it encodes braid words into 4-integer vectors and decodes back to complex numbers for computation.

The variational regularization would discover the same algebraic structure DKC already characterized -- providing a neural validation that the exact algebraic structure emerges naturally from the data.

CALM's context-free chunk encoding maps directly: train a small network mapping sequences of K braid generators to a single Z[zeta_8] value using exact bracket as ground truth. This "braid autoencoder" would compress generator sequences into cyclotomic integers with smooth interpolation via VAE regularization.

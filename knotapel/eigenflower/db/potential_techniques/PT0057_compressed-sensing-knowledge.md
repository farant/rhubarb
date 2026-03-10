---
id: PT0057
title: "Compressed sensing for compact knowledge representation"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0013]
---

If the knowledge representation is sparse in some basis, it can be exactly recovered from far fewer measurements than Nyquist requires. Solve via L1 minimization: min ||alpha||_1 subject to y = Phi * Psi * alpha.

For eigenflower: theological knowledge in the Summa is highly structured and sparse in the right basis (most propositions relate to only a few other propositions). Compressed sensing provides a principled framework for representing this knowledge compactly.

The key question: what is the right sparsifying basis for theological propositions? Candidates include:
- Fourier basis (spectral decomposition of the proposition graph)
- Wavelet basis (multi-scale structure of the knowledge hierarchy)
- Learned dictionary (data-driven basis from the Summa itself)

If the knowledge IS sparse in some basis, then you need dramatically fewer parameters to represent it faithfully. This connects to the finding that ternary weights ({-1, 0, 1}) -- which are maximally sparse -- can still capture model behavior.

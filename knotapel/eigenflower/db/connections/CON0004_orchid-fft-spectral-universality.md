---
id: CON0004
title: "Orchid FFT spectral domain shares mathematical basis with DKC spectral universality"
connects:
  - "Orchid FFT-based global convolution in frequency domain"
  - "DKC spectral universality on S^2 (D71, D107)"
type: structural_parallel
strength: moderate
source: "extractions/group-a-knotapel-relevance.md, Connection 4"
---

Orchid operates in spectral space via FFT (DCT outperforms DFT). DKC's computation lives on S^2 and has a natural spectral decomposition in spherical harmonics. The spectral universality result (D71) means every DKC XOR solution shares the same spectral fingerprint: l=6 dominant (~80%), l=4 secondary (~6%), l=12 tertiary (~6-7%).

This is not just that DKC CAN be described in frequency space -- the frequency representation is canonical and universal across all 36 XOR6 winning triples. This suggests that Orchid kernels for Boolean logic tasks might have similarly universal spectral signatures.

The super-Nyquist compression (14 Voronoi cells suffice where Nyquist predicts 49, a 3.5x compression) is directly relevant to Orchid efficiency: if the task has known sparsity structure, the convolution kernel can be much shorter than the generic bound.

DCT outperforming DFT in Orchid parallels DKC's axis-alignment: all values are real multiples of fixed basis vectors -- an inherently "DCT-compatible" structure since DCT operates on real-valued, even-symmetric signals.

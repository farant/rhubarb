---
id: PT0073
title: "Data-driven basis adaptation for theological text"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0013, PT0014]
---

Fixed bases (Fourier, wavelet) may not align with the structure of theological text. Learn orthonormal or frame-based dictionaries adapted to the data distribution. Approaches include sparse coding, dictionary learning in RKHS, and graph Laplacians.

For eigenflower: the spectral basis should reflect the structure of Latin theological text, not generic text. A learned dictionary adapted to the Summa/Bible corpus could provide more compact and interpretable representations.

Graph Laplacian approach: construct a graph from the Summa's logical dependency structure, compute its Laplacian, use the eigenvectors as the spectral basis. This would give a basis where the "frequencies" correspond to structural features of the theological knowledge graph rather than temporal features of the text.

The Hilbert Space paper identifies this as a key future direction. For eigenflower's small, specialized corpus, data-driven adaptation is likely more important than for large general corpora, since the fixed bases (designed for generic signals) may poorly match theological text structure.
